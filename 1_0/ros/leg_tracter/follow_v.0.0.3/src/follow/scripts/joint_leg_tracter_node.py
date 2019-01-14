#!/usr/bin/python

import rospy
import kalman_mutil_tracter

if __name__ == '__main__':
    try:
        rospy.init_node("mutil_person_tracter", anonymous=True)
        kmt = kalman_mutil_tracter.KalmanMutilTracter()
    except rospy.ROSInterruptException:
        pass