#!/usr/bin/python
# -*- coding: utf-8 -*-
# @Time    : 18-5-5 下午4:20
# @Author  : Yutong
# @Email   : 416178264@qq.com
# @Site    : 
# @File    : SerialControl_Server.py
# @Software: PyCharm
# @Desc    :

import serial
import math
import time
from array import *
import rospy
import numpy as np
from nav_msgs.msg import Odometry
from geometry_msgs.msg import PoseStamped, Twist, PoseWithCovarianceStamped
import tf
from move_base_msgs.msg import MoveBaseActionResult
from actionlib_msgs.msg import GoalID
import traceback
from lsrobot.srv import cmd, cmdRequest

d2r = (2 * math.pi) / 360.0
r2d = 360.0 / (2 * math.pi)
pack_len = 8


class SerialControl:
    def __init__(self):

        serial_baudrate = rospy.get_param('~/baudrate', 115200)
        serial_port = rospy.get_param('~/port', '/dev/ttyAMA4')
        load_map_file = rospy.get_param('~/map_file', 'serial_map.txt')
        self.header = [0xAA, 0x55]
        self.content_size = 22
        self.ID = 0
        self.content = [0] * self.content_size
        self.sendSize = 1 + len(self.content)
        cmd_serial = [self.sendSize] + [self.ID] + self.content
        cs = checksum([self.ID] + self.content)
        cs_high = cs / 256
        cs_low = cs % 256
        cmdstr_start = array("B", self.header + cmd_serial + [cs_high, cs_low]).tostring()
        self.vx = 0
        self.wz = 0
        self.dir = 1
        self.posx = 0
        self.posy = 0
        self.posz = 0
        self.moveStatus = 0
        self.cmd_srv = 0
        self.arg_srv = 0
        self.active = False
        self.pose_odom = PoseStamped()
        self.pose_map = PoseStamped()
        self.newRes = False
        self.vel_vx = 0
        self.vel_wz = 0
        self.odom_x, self.odom_y, self.odom_z = 0, 0, 0


        try:
            self.device = serial.Serial(serial_port, serial_baudrate, timeout=0.1)
            print("port opened successfully")
        except:
            print("Port failed open.\n"
                  "Are you sure device is connected correctly?\n"
                  "Please press (ctrl+c) and restart it after checked!"
                  )
            return

        time.sleep(0.2)
        self.device.write(cmdstr_start)


        # Make sure there is tf transform between map and odom
        self.listener = tf.TransformListener()
        # avaliable = self.listener.canTransform("/map", "/odom", rospy.Time())
        # rospy.logfatal("TF frame is avaliable %s" %avaliable)
        # while not avaliable:
        #     time.sleep(1)
        #     avaliable = self.listener.canTransform("/map", "/odom", rospy.Time())
        #     rospy.logfatal("TF frame is avaliable %s" % avaliable)
        # self.listener.waitForTransform("/map", "/odom", rospy.Time(), rospy.Duration(9999))
        rospy.loginfo("can get tf info from buffer now")

        # Subscriber: use callback to send data to serial port
        self.odomSub = rospy.Subscriber("/odom", Odometry, callback=self.readOdom)
        self.velSub = rospy.Subscriber("/mobile_base/commands/velocity", Twist, callback=self.readVel)
        self.moveResultSub = rospy.Subscriber('/move_base/result', MoveBaseActionResult, callback=self.resolveResult)

        # Publisher: read data from serial port and send it to ROS
        self.velPub = rospy.Publisher("/mobile_base/commands/velocity", Twist, queue_size=3)
        self.moveGoalPub = rospy.Publisher("/move_base_simple/goal", PoseStamped, queue_size=1)
        self.cancelPub = rospy.Publisher("/move_base/cancel", GoalID, queue_size=3)
        self.initialPub = rospy.Publisher("/initialpose", PoseWithCovarianceStamped, queue_size=1)

        # Service: using service to manipulate gmapping, map server, amcl
        # block until all needed services are ready
        rospy.wait_for_service('/slam_gmapping')
        rospy.wait_for_service('/saver_control')
        rospy.wait_for_service('/server_control')
        rospy.wait_for_service('/amcl_control')

        self.gmapService = rospy.ServiceProxy('/slam_gmapping', cmd)
        self.saverService = rospy.ServiceProxy('/saver_control', cmd)
        self.serverService = rospy.ServiceProxy('/server_control', cmd)
        self.amclService = rospy.ServiceProxy('/amcl_control', cmd)

        rospy.logwarn("everything is ready!")

    def onStart(self):
        try:
            while not rospy.is_shutdown():
                self.resolveCMD()
                time.sleep(0.01)
                self.pubCMD()
                # rospy.sleep(0.01)
            pass
        except Exception as e:
            print e
            # traceback.print_exc()
            raise KeyboardInterrupt

    def resolveResult(self, moveResultMsg):
        GoalStatus = moveResultMsg.status.status
        reason_failed = moveResultMsg.status.text
        if GoalStatus == 3:
            print('DWA arrived')
            self.moveStatus = 2
        else:
            print('something wrong-case %d! Reason is %s.Use A* instead' % (GoalStatus, reason_failed))
            self.moveStatus = GoalStatus

    def readVel(self, vel_msg):
        self.vel_vx = vel_msg.linear.x
        # y = vel_msg.linear.y
        self.vel_wz = vel_msg.angular.z


    def readOdom(self, odom_msg):
        avaliable = self.listener.canTransform("/map", "/odom", rospy.Time())
        if not avaliable:
            rospy.logfatal_throttle(10, "TF frame is avaliable %s" % avaliable)
            return

        self.listener.waitForTransform("/map", "/odom", rospy.Time(), rospy.Duration(9999))
        self.pose_odom.pose = odom_msg.pose.pose
        self.pose_odom.header.stamp = rospy.Time()
        self.pose_odom.header.frame_id = '/odom'
        # transform the odom expression under map frame
        # t1 = time.time()
        self.pose_map = self.listener.transformPose("/map", self.pose_odom)
        # t2 = time.time()
        x = self.pose_map.pose.position.x
        y = self.pose_map.pose.position.y
        angle = quat2eul([self.pose_map.pose.orientation.w, self.pose_map.pose.orientation.x, self.pose_map.pose.orientation.y, self.pose_map.pose.orientation.z])
        theta = angle[0]
        self.odom_x = x
        self.odom_y = y
        self.odom_z = theta
        # odx, ody = odom_msg.pose.pose.position.x, odom_msg.pose.pose.position.y


        # send data back to serial port
        # TODO read all self variable here in order to avoid sudden change
        vx, wz = self.vel_vx, self.vel_wz
        ID = self.ID
        px, py, pz = self.posx, self.posy, self.posz
        status = self.moveStatus
        cmd_srv = self.cmd_srv
        arg_srv = self.arg_srv

        # TODO Sending velocity part
        if vx < 0:
            self.content[0] = 255 + int(vx)
            self.content[1] = int((1 - (vx % 1)) * 100) % 100
        else:
            self.content[0] = int(vx)
            self.content[1] = int((vx % 1) * 100)
        if wz < 0:
            self.content[2] = 255 + int(wz)
            self.content[3] = int((1 - (wz % 1)) * 100) % 100
        else:
            self.content[2] = int(wz)
            self.content[3] = int((wz % 1) * 100)
        self.content[4] = 0
        self.content[5] = 2 if vx > 0 else 0

        # TODO Sending odom part
        if x < 0:
            self.content[6] = 255 + int(x)
            self.content[7] = int((1 - (x % 1)) * 100) % 100
        else:
            self.content[6] = int(x)
            self.content[7] = int((x % 1) * 100)

        if y < 0:
            self.content[8] = 255 + int(y)
            self.content[9] = int((1 - (y % 1)) * 100) % 100
        else:
            self.content[8] = int(y)
            self.content[9] = int((y % 1) * 100)

        if theta < 0:
            ang = (2 * np.pi + theta) * 10000
            self.content[10] = int(ang / 256)
            self.content[11] = int(ang % 256)
        else:
            ang = theta * 10000
            self.content[10] = int(ang / 256)
            self.content[11] = int(ang % 256)

        # TODO sending goal/initial pose part
        if px < 0:
            self.content[12] = 255 + int(px)
            self.content[13] = int((1 - (px % 1)) * 100) % 100
        else:
            self.content[12] = int(px)
            self.content[13] = int((px % 1) * 100)

        if py < 0:
            self.content[14] = 255 + int(py)
            self.content[15] = int((1 - (py % 1)) * 100) % 100
        else:
            self.content[14] = int(py)
            self.content[15] = int((py % 1) * 100)

        if pz < 0:
            ang = (2 * np.pi + pz) * 10000
            self.content[16] = int(ang / 256)
            self.content[17] = int(ang % 256)
        else:
            ang = pz * 10000
            self.content[16] = int(ang / 256)
            self.content[17] = int(ang % 256)

        # TODO commands part
        self.content[18] = cmd_srv
        self.content[19] = arg_srv / 256
        self.content[20] = arg_srv % 256

        # TODO status part
        self.content[21] = status

        # cmd_serial = [self.sendSize] + [ID] + self.content
        # odx, ody = odom_msg.pose.pose.position.x, odom_msg.pose.pose.position.y

        # print("%3.4f,%3.4f, %s, x=%2.3f, y=%2.3f, th=%2.3f, xo=%2.3f, yo=%2.3f" %((t2-t1), (t3-t2),[ID] + self.content, x, y, theta, odx, ody))
        # cs = checksum([ID] + self.content)
        # cs_high = cs / 256
        # cs_low = cs % 256
        # cmdstr_start = array("B", self.header + cmd_serial + [cs_high, cs_low]).tostring()

        # self.device.write(cmdstr_start)


        return


    def pubCMD(self):
        cmd_serial = [self.sendSize] + [self.ID] + self.content
        cs = checksum([self.ID] + self.content)
        cs_high = cs / 256
        cs_low = cs % 256
        cmdstr_start = array("B", self.header + cmd_serial + [cs_high, cs_low]).tostring()
        self.device.write(cmdstr_start)


    def resolveCMD(self):
        buffer_len = self.device.inWaiting()
        # if self.active:
        #     self.ID = 0
        #     self.content = [0] * 20

        if buffer_len >= (pack_len * 3):
            print("need to empty buffer")
            self.device.flushInput()
        elif buffer_len == 0:
            # print("time out occur!")
            # self.ID = [0]
            # self.content[-1] = 1
            # cmd_serial = [9] + self.ID + self.content
            # cs = checksum(self.header + [9] + self.ID + self.content)
            # cmdstr_start = array("B", self.header + cmd_serial + cs).tostring()
            # time.sleep(0.2)
            # self.device.write(cmdstr_start)
            return

        # read data from serial port
        header1 = ord(self.device.read(1))

        if header1 == self.header[0]:
            header2 = ord(self.device.read(1))
            if header2 == self.header[1]:
                data_len = ord(self.device.read(1))
                data = [ord(b) for b in self.device.read(data_len)]
                cs_read_high = ord(self.device.read(1))
                cs_read_low = ord(self.device.read(1))
                cs_read = cs_read_high * 256 + cs_read_low
                cs_check = checksum(data)
                print(data_len, cs_read, cs_check)
                if cs_check == cs_read:
                    print("checksum pass")

                else:
                    print("checksum wrong!")
                    self.ID = 0
                    self.content[0:data_len] = data
                    self.content[7] = 3
                    cmd_serial = [self.sendSize] + [self.ID] + self.content
                    cs = checksum([self.ID] + self.content)
                    cmdstr_start = array("B", self.header + cmd_serial + [cs]).tostring()
                    time.sleep(0.2)
                    self.device.write(cmdstr_start)
                    return
            else:
                print("header 2 wrong!")
                self.ID = 0
                self.content[7] = 2
                cmd_serial = [self.sendSize] + [self.ID] + self.content
                cs = checksum([self.ID] + self.content)
                cmdstr_start = array("B", self.header + cmd_serial + [cs]).tostring()
                time.sleep(0.2)
                self.device.write(cmdstr_start)
                return
        else:
            print("header 1 wrong!")
            self.ID = 0
            self.content[7] = 2
            cmd_serial = [self.sendSize] + [self.ID] + self.content
            cs = checksum([self.ID] + self.content)
            cmdstr_start = array("B", self.header + cmd_serial + [cs]).tostring()
            time.sleep(0.2)
            self.device.write(cmdstr_start)
            return

        # resolve data after get one full packet
        self.ID = data[0]

        if self.ID == 1:
            # TODO velocity command
            self.vx = data[1] + data[2] / 100.0
            if data[3] > 127:
                self.wz = (data[3] - 255) - (data[4] / 100.0)
            else:
                self.wz = data[3] + data[4] / 100.0
            self.dir = data[6]
            print("vx = %s, wz=%s, dir=%s" %(self.vx, self.wz, self.dir))
            rospy.loginfo("sending velocity commands")
            vel_msg = Twist()
            vx = (self.dir - 1) * self.vx
            wz = self.wz
            vel_msg.linear.x = vx
            vel_msg.angular.z = wz
            t0 = time.time()
            while (time.time() - t0) < 0.2:
                self.velPub.publish(vel_msg)
                time.sleep(0.01)
            vel_msg = Twist()
            self.velPub.publish(vel_msg)

        elif self.ID == 2:
            # TODO move goal command, stop current move base action no matter what
            self.moveStatus = 0
            goal = GoalID()
            goal.stamp = rospy.Time.now()
            goal.id = ""
            self.cancelPub.publish(goal)

            if data[1] > 127:
                self.posx = (data[1] - 255) - (data[2] / 100.0)
            else:
                self.posx = data[1] + (data[2] / 100.0)

            if data[3] > 127:
                self.posy = (data[3] - 255) - (data[4] / 100.0)
            else:
                self.posy = data[3] + (data[4] / 100.0)

            self.posz = (data[5] * 256 + data[6]) / 10000.0
            print("goal pose is %s, %s, %s" %(self.posx, self.posy, self.posz))

            print("sending move goal command")
            goal_msg = PoseStamped()
            goal_msg.header.frame_id = "/map"
            goal_msg.header.stamp = rospy.Time.now()
            px, py, pz = self.posx, self.posy, self.posz
            goal_msg.pose.position.x = px
            goal_msg.pose.position.y = py
            [qw, qx, qy, qz] = eul2quat([pz, 0, 0])
            goal_msg.pose.orientation.w = qw
            goal_msg.pose.orientation.x = qx
            goal_msg.pose.orientation.y = qy
            goal_msg.pose.orientation.z = qz
            self.moveGoalPub.publish(goal_msg)


        elif self.ID == 3:
            # TODO initial pose command, should check if amcl is turned on
            if data[1] > 127:
                self.posx = (data[1] - 255) - (data[2] / 100.0)
            else:
                self.posx = data[1] + (data[2] / 100.0)

            if data[3] > 127:
                self.posy = (data[3] - 255) - (data[4] / 100.0)
            else:
                self.posy = data[3] + (data[4] / 100.0)

            self.posz = (data[5] * 256 + data[6]) / 10000.0
            print("initial pose is %s, %s, %s" %(self.posx, self.posy, self.posz))
            init_msg = PoseWithCovarianceStamped()
            init_msg.header.stamp = rospy.Time.now()
            init_msg.header.frame_id = '/map'
            px, py, pz = self.posx, self.posy, self.posz
            init_msg.pose.pose.position.x = px
            init_msg.pose.pose.position.y = py
            [qw, qx, qy, qz] = eul2quat([pz, 0, 0])
            init_msg.pose.pose.orientation.w = qw
            init_msg.pose.pose.orientation.x = qx
            init_msg.pose.pose.orientation.y = qy
            init_msg.pose.pose.orientation.z = qz
            self.initialPub.publish(init_msg)


        elif self.ID == 4:
            # TODO gmapping related commands
            cmd_srv = data[7]
            arg_srv = data[8] * 256 + data[9]
            req = cmdRequest()
            req.cmd = cmd_srv
            if cmd_srv == 1:
                rospy.loginfo("start gmapping")
            elif cmd_srv == 2:
                rospy.loginfo("stop gmapping")
            elif cmd_srv == 3:
                rospy.loginfo("pause gmapping")
            elif cmd_srv == 4:
                rospy.loginfo("resume gmapping")
            else:
                rospy.logwarn("invalid command")
                return
            response = self.gmapService(req)
            self.moveStatus = response.result
            self.cmd_srv = cmd_srv
            self.arg_srv = arg_srv

        elif self.ID == 5:
            # TODO map saver related commands, map filename should base on argument value
            cmd_srv = data[7]
            arg_srv = data[8] * 256 + data[9]
            req = cmdRequest()
            req.cmd = cmd_srv
            req.str = '/home/tongsky/test'
            if cmd_srv == 1:
                rospy.loginfo("save map")
            elif cmd_srv == 2:
                rospy.loginfo("stop save map")
            else:
                rospy.logwarn("invalid command")
                return

            response = self.saverService(req)
            self.moveStatus = response.result
            self.cmd_srv = cmd_srv
            self.arg_srv = arg_srv

        elif self.ID == 6:
            # TODO map server related commands, map filename should base on argument value
            cmd_srv = data[7]
            arg_srv = data[8] * 256 + data[9]
            req = cmdRequest()
            req.cmd = cmd_srv
            req.str = '/home/tongsky/test.yaml'
            if cmd_srv == 1:
                rospy.loginfo("import map")
            else:
                rospy.logwarn("invalid command")
                return
            response = self.serverService(req)
            self.moveStatus = response.result
            self.cmd_srv = cmd_srv
            self.arg_srv = arg_srv

        elif self.ID == 7:
            # TODO map server related commands, start amcl should check map imported/ gmapping stop?
            cmd_srv = data[7]
            arg_srv = data[8] * 256 + data[9]
            req = cmdRequest()
            req.cmd = cmd_srv
            if cmd_srv == 1:
                rospy.loginfo("start amcl")
            elif cmd_srv == 2:
                rospy.loginfo("stop amcl")
            else:
                rospy.logwarn("invalid command")
                return
            response = self.amclService(req)
            self.moveStatus = response.result
            self.cmd_srv = cmd_srv
            self.arg_srv = arg_srv

        self.active = True

    def shutdown(self):
        self.device.close()

def quat2eul(data):
    qw = data[0]
    qx = data[1]
    qy = data[2]
    qz = data[3]
    roll = math.atan2(2 * (qy * qz + qw * qx), 1 - 2 * ((qx ** 2) + (qy ** 2)))
    pitch = math.asin(2 * (qw * qy - qz * qx))
    yaw = math.atan2(2 * (qw * qz + qx * qy), 1 - 2 * ((qy ** 2) + (qz ** 2)))
    euler = [yaw, pitch, roll]
    return euler


def eul2quat(data):
    yaw = data[0]
    pitch = data[1]
    roll = data[2]
    qw = math.cos(roll / 2) * math.cos(pitch / 2) * math.cos(yaw / 2) + math.sin(roll / 2) * math.sin(pitch / 2) * math.sin(yaw / 2)
    qx = math.sin(roll / 2) * math.cos(pitch / 2) * math.cos(yaw / 2) - math.cos(roll / 2) * math.sin(pitch / 2) * math.sin(yaw / 2)
    qy = math.cos(roll / 2) * math.sin(pitch / 2) * math.cos(yaw / 2) + math.sin(roll / 2) * math.cos(pitch / 2) * math.sin(yaw / 2)
    qz = math.cos(roll / 2) * math.cos(pitch / 2) * math.sin(yaw / 2) - math.sin(roll / 2) * math.sin(pitch / 2) * math.cos(yaw / 2)
    quat = [qw, qx, qy, qz]
    return quat

def checksum(data):
    cs = data[0]
    for i in range(1, len(data), 1):
        cs = cs ^ data[i]
    return cs


if __name__ == '__main__':
    try:
        rospy.init_node('serial_control')
        rospy.loginfo("serial control node is initialized!")
        print("Initialize node %s with Node_URI %s under namespace: %s"
              % (rospy.get_name(), rospy.get_node_uri(), rospy.get_namespace()))
        if rospy.get_namespace() != '/':
            rospy.logwarn("This node is contained under namespace %s, be careful" %rospy.get_namespace())

        try:
            serial_control = SerialControl()
            print("ready")
            serial_control.onStart()
            rospy.spin()
            # pass
        except:
            try:
                serial_control.shutdown()
            except:
                pass
            #pass
        # rospy.logwarn("keyboard interrupt---> rosnode shutdown is requested!")

    except rospy.ROSInterruptException:
        rospy.logerr("Leishen node initialization failed!")
