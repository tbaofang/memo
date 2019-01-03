#!/usr/bin/python
# -*- coding: utf-8 -*-
# @Time    : 18-7-5 下午6:27
# @Author  : Yutong
# @Email   : 416178264@qq.com
# @Site    : 
# @File    : move_base_assistance.py
# @Software: PyCharm
# @Desc    :


import rospy
import time
from move_base_class import MoveBaseAssistance


if __name__ == '__main__':
    rospy.init_node("move_base_assistance")
    time.sleep(1)

    #rospy.wait_for_service("/move_base/make_plan")
    #time.sleep(0.5)
    #rospy.wait_for_service("/move_base/clear_costmaps")
    #time.sleep(0.5)

    rospy.loginfo('move base make plan service is ready')

    try:
        assistance = MoveBaseAssistance()
        rospy.spin()
        # req.start.pose.position.x =
    except rospy.ServiceException as exc:
        print("Service did not process request: " + str(exc))
