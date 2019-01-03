#!/usr/bin/python
# -*- coding: utf-8 -*-
# @Time    : 18-7-18 下午1:51
# @Author  : Yutong
# @Site    : 416178264@qq.com
# @File    : move_base_class.py
# @Software: PyCharm
# @Desc    :

import rospy
import time
import math
import numpy as np
from geometry_msgs.msg import PoseStamped, Twist, Pose
from std_srvs.srv import Empty as EmptySrv
from std_srvs.srv import EmptyRequest, EmptyResponse
from nav_msgs.msg import Odometry, Path
from sensor_msgs.msg import Imu
import tf
from actionlib_msgs.msg import GoalID
from MyMatlab import eul2quat, quat2eul, deg2rad, rad2deg
from MyRobotics import angdiff, PIDcontroller, VelLim
from nav_msgs.srv import GetPlan, GetPlanRequest, GetPlanResponse

def getYaw(quat):
    """
    Get Yaw angle value from quaternion

    :param quat: quaternion with geometry_msg/Quaternion message type
    :return: yaw: the yaw angle value expressed by right hand rule
    """
    angle = quat2eul([quat.w, quat.x, quat.y, quat.z])
    yaw = angle[0]
    return yaw

class MoveBaseAssistance:
    def __init__(self):
        self.pose_odom = PoseStamped()
        self.pose_map = PoseStamped()
        self.imu = 0.0
        self.is_working = False
        self.listener = tf.TransformListener()
        self.planner = rospy.ServiceProxy('/move_base/make_plan', GetPlan)
        self.clearCM = rospy.ServiceProxy('/move_base/clear_costmaps', EmptySrv)
        # self.odomSub = rospy.Subscriber('/odom', Odometry, callback=self.odomCB)
        self.goalSub = rospy.Subscriber('/move_base_simple/goal', PoseStamped, callback=self.goalCB)
        self.cancelPub = rospy.Publisher('/move_base/cancel', GoalID, queue_size=1)
        self.planPub = rospy.Publisher('/PlannedPath', Path, queue_size=1)
        self.velPub = rospy.Publisher("/mobile_base/commands/velocity", Twist, queue_size=3)
        self.imuSub = rospy.Subscriber("/mobile_base/sensors/imu_data", Imu, callback=self.readIMU)
        self.goalPub = rospy.Publisher('/move_base_simple/goal1', PoseStamped, queue_size=1)
        self.poseSub = rospy.Subscriber('/lspose', Pose, callback=self.poseCB)

    def poseCB(self, pose_msg):
        self.pose_map = PoseStamped()
        self.pose_map.pose = pose_msg
        self.pose_map.header.frame_id = 'map'
        self.pose_map.header.stamp = rospy.Time.now()


    def odomCB(self, odom_msg):
        self.pose_odom.pose = odom_msg.pose.pose
        self.pose_odom.header.stamp = rospy.Time(0)
        self.pose_odom.header.frame_id = '/odom'
        # transform the odom expression under map frame
        try:
            self.listener.waitForTransform('/map', '/odom', rospy.Time.now(), rospy.Duration(4))
            # self.pose_map = self.listener.transformPose("/map", self.pose_odom)
            self.is_working = True
            rospy.loginfo_throttle(0.2, "get transformation between odom and map frame")
        except:
            rospy.logwarn_throttle(0.2, "cannot get transformation between odom and map frame")
            self.is_working = False
            return

    def readIMU(self, imu_msg):
        quat = imu_msg.orientation
        angles = quat2eul([quat.w, quat.x, quat.y, quat.z])
        theta = angles[0]
        self.imu = theta
        rospy.loginfo_throttle(0.5, "current heading is %s" %self.imu)

    def goalCB(self, pose_msg):
        # if not self.is_working:
        #     rospy.logwarn("can not receive tf between odom and map, navi function is disabled!")

        rospy.loginfo("receive goal pose, pre-process it")
        # cancel current goal
        self.cancelPub.publish(GoalID())
        time.sleep(0.5)

        # clear current costmap
        self.clearCM.call(EmptyRequest())
        time.sleep(0.5)

        # get robot pose in map frame
        robot_pose = self.pose_map

        # get the planned path before actual remap
        plan_req = GetPlanRequest()
        plan_req.start.header.frame_id = 'map'
        plan_req.start.header.stamp = rospy.Time.now()
        plan_req.start.pose = robot_pose.pose
        plan_goal = pose_msg
        # plan_goal = rospy.wait_for_message('/clicked_point', PoseStamped)
        plan_req.goal = plan_goal
        plan_req.tolerance = 0.1
        try:
            plan_resp = self.planner.call(plan_req)
        except:
            rospy.logwarn("failed to make plan, remap goal")
            self.goalPub.publish(pose_msg)
        plan_path = plan_resp.plan
        plan_path.header.stamp = rospy.Time(0)
        plan_path.header.frame_id = '/map'
        self.planPub.publish(plan_path)

        # plan_path = GetPlanResponse()
        path_pose = plan_path.poses

        if len(path_pose) == 0:
            rospy.logwarn("failed to find a possible path, remap it only cause the same result!")
            self.goalPub.publish(pose_msg)
            return

        dis_array = np.zeros(len(path_pose))
        theta_array = np.zeros(len(path_pose))
        for i in range(len(path_pose)):
            current_pose = path_pose[i]
            dis_array[i] = np.hypot(current_pose.pose.position.x - robot_pose.pose.position.x,
                                    current_pose.pose.position.y - robot_pose.pose.position.y)

            theta_array[i] = math.atan2(current_pose.pose.position.y - robot_pose.pose.position.y,
                                        current_pose.pose.position.x - robot_pose.pose.position.x)

        robot_heading = getYaw(robot_pose.pose.orientation)
        rospy.loginfo("robot pose-> x:%s, y:%s, theta :%s" %(robot_pose.pose.position.x , robot_pose.pose.position.y, rad2deg(robot_heading)))
        idx_cond_pre = np.where(dis_array < 0.5)[0]
        if len(idx_cond_pre) < 5:
            rospy.logwarn("probably too short to get path, remap it only!")
            self.goalPub.publish(pose_msg)
            return
        idx_cond = idx_cond_pre[-1]
        theta_sel = theta_array[2:idx_cond+1]
        theta_average = np.mean(theta_sel)
        ang_diff = angdiff(theta_average, robot_heading)
        if np.abs(ang_diff) > 0.52:
            rospy.logwarn("planned path at the back of robot, rotate it with angle diff %s" %rad2deg(ang_diff))
            kp, ki, kd = [3.5, 1.0, 0.5]
            previous_error = 0
            previous_integral = 0
            theta_init = self.imu
            rat = rospy.Rate(10)
            t0 = rospy.get_time()
            ts = rospy.get_time()
            vel_msg = Twist()
            while 1:
                theta_current = self.imu
                error = angdiff(theta_init + ang_diff, theta_current)
                dt = rospy.get_time() - t0
                if dt <= 0.0:
                    dt = 0.01
                if math.fabs(error) < 0.035:
                    vel_msg.angular.z = 0.0
                    self.velPub.publish(vel_msg)
                    break
                elif (rospy.get_time() - ts) > 15:
                    vel_msg.angular.z = 0.0
                    self.velPub.publish(vel_msg)
                    break
                wz, error, integral = PIDcontroller(kp, ki, kd, dt, 'PID', previous_error, error, previous_integral)
                wz = VelLim(wz, 0.05, 0.8)
                previous_error = error
                previous_integral = integral
                vel_msg.angular.z = wz
                self.velPub.publish(vel_msg)
                t0 = rospy.get_time()
                rat.sleep()
        else:
            rospy.logwarn("planned path right in front of robot, no need to rotate")

        self.goalPub.publish(pose_msg)
        rospy.loginfo("pre-process finished! republish it to its actual topic")
