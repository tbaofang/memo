#!/usr/bin/python
# -*- coding: utf-8 -*-
# @Time    : 18-5-8 上午10:51
# @Author  : Yutong
# @Email   : 416178264@qq.com
# @Site    : 
# @File    : SerialControl_Client.py
# @Software: PyCharm
# @Desc    :

import serial
import math
import time
from array import *
import rospy
import numpy as np
from nav_msgs.msg import Odometry
from geometry_msgs.msg import PoseStamped, Twist
import tf
from move_base_msgs.msg import MoveBaseActionResult
import traceback

d2r = (2 * math.pi) / 360.0
r2d = 360.0 / (2 * math.pi)
pack_len = 8


def checksum(data):
    cs = data[0]
    for i in range(1, len(data), 1):
        cs = cs ^ data[i]
    return cs


serial_baudrate = 115200
serial_port = '/dev/ttyUSB1'

header = [0xAA, 0x55]
ID = 7
content = [0] * 9
sendSize = 1 + len(content)

vx = 0.2
wz = 0.5
dir = 2

posx = 0.05
posy = 0.05
posz = 180 * d2r

init_x = 0
init_y = 0
init_z = 0 * d2r

commands = 2
argument = 0


if ID == 1:
    content[0] = int(vx)
    content[1] = int((vx % 1) * 100)
    if wz < 0:
        content[2] = 255 + int(wz)
        content[3] = int((1 - (wz%1))*100) % 100
    else:
        content[2] = int(wz)
        content[3] = int((wz % 1) * 100)
    content[4] = 0
    content[5] = dir

elif ID == 2:
    if posx < 0:
        content[0] = 255 + int(posx)
        content[1] = int((1 - (posx%1))*100) % 100
    else:
        content[0] = int(posx)
        content[1] = int((posx % 1) * 100)

    if posy < 0:
        content[2] = 255 + int(posy)
        content[3] = int((1 - (posy%1))*100) % 100
    else:
        content[2] = int(posy)
        content[3] = int((posy % 1) * 100)

    if posz < 0:
        ang = (2 * np.pi + posz) * 10000
        content[4] = int(ang / 256)
        content[5] = int(ang % 256)
    else:
        ang = posz * 10000
        content[4] = int(ang / 256)
        content[5] = int(ang % 256)

elif ID == 3:
    if init_x < 0:
        content[0] = 255 + int(init_x)
        content[1] = int((1 - (init_x%1))*100) % 100
    else:
        content[0] = int(init_x)
        content[1] = int((init_x % 1) * 100)

    if init_y < 0:
        content[2] = 255 + int(init_y)
        content[3] = int((1 - (init_y%1))*100) % 100
    else:
        content[2] = int(init_y)
        content[3] = int((init_y % 1) * 100)

    if init_z < 0:
        ang = (2 * np.pi + init_z) * 10000
        content[4] = int(ang / 256)
        content[5] = int(ang % 256)
    else:
        ang = init_z * 10000
        content[4] = int(ang / 256)
        content[5] = int(ang % 256)

elif ID == 4:
    print("gmapping related commands")
    content[6] = commands
    content[7] = argument / 256
    content[8] = argument % 256
elif ID == 5:
    print("map saver related commands")
    content[6] = commands
    content[7] = argument / 256
    content[8] = argument % 256
elif ID == 6:
    print("map server related commands")
    content[6] = commands
    content[7] = argument / 256
    content[8] = argument % 256
elif ID == 7:
    print("amcl related commands")
    content[6] = commands
    content[7] = argument / 256
    content[8] = argument % 256

try:
    device = serial.Serial(serial_port, serial_baudrate, timeout=0.1)
    print("port opened successfully")
except:
    print("Port failed open.\n"
          "Are you sure device is connected correctly?\n"
          "Please press (ctrl+c) and restart it after checked!"
          )



cmd_serial = [sendSize] + [ID] + content
cs = checksum([ID] + content)
cs_high = cs/256
cs_low = cs%256
print(header + cmd_serial + [cs_high, cs_low])
cmdstr_start = array("B", header + cmd_serial + [cs_high, cs_low]).tostring()

time.sleep(0.2)
device.write(cmdstr_start)


while 1:
    buffer_len = device.inWaiting()

    if buffer_len >= (pack_len * 5):
        # print("need to empty buffer")
        device.flushInput()
    elif buffer_len == 0:
        continue

    # read data from serial port
    header1 = ord(device.read(1))
    if header1 == header[0]:
        header2 = ord(device.read(1))
        if header2 == header[1]:
            data_len = ord(device.read(1))
            data = [ord(b) for b in device.read(data_len)]
            cs_read_high = ord(device.read(1))
            cs_read_low = ord(device.read(1))
            cs_read = cs_read_high * 256 + cs_read_low
            cs_check = checksum(data)
            print(data_len, cs_read, cs_check)
            if cs_check == cs_read:
                # print("checksum pass")
                pass

            else:
                # print("checksum wrong!")
                ID = 0
                content[0:data_len] = data
                # content[-1] = 3
                cmd_serial = [sendSize] + [ID] + content
                cs = checksum([ID] + content)
                cmdstr_start = array("B", header + cmd_serial + [cs]).tostring()
                time.sleep(0.2)
                device.write(cmdstr_start)
                continue
        else:
            # print("header 2 wrong!")
            ID = 0
            content[-1] = 2
            cmd_serial = [sendSize] + [ID] + content
            cs = checksum([ID] + content)
            cmdstr_start = array("B", header + cmd_serial + [cs]).tostring()
            time.sleep(0.2)
            device.write(cmdstr_start)
            continue
    else:
        # print("header 1 wrong!")
        ID = 0
        content[-1] = 2
        cmd_serial = [sendSize] + [ID] + content
        cs = checksum([ID] + content)
        cmdstr_start = array("B", header + cmd_serial + [cs]).tostring()
        time.sleep(0.2)
        device.write(cmdstr_start)
        continue

    # resolve data after get one full packet
    ID = data[0]


    # TODO velocity feedback
    vx = data[1] + data[2] / 100.0
    if data[3] > 127:
        wz = (data[3] - 255) - (data[4] / 100.0)
    else:
        wz = data[3] + data[4] / 100.0
    dir = data[6]

    # print("vx = %s, wz=%s, dir=%s" % (vx, wz, dir))


    # TODO odom feedback

    if data[7] > 127:
        posx = (data[7] - 255) - (data[8] / 100.0)
    else:
        posx = data[7] + (data[8] / 100.0)

    if data[9] > 127:
        posy = (data[9] - 255) - (data[10] / 100.0)
    else:
        posy = data[9] + (data[10] / 100.0)

    posz = (data[11] * 256 + data[12]) / 10000.0

    # print("odom pose is %s, %s, %s" % (posx, posy, posz))

    # TODO goal/initial pose feedback
    if data[13] > 127:
        goalx = (data[13] - 255) - (data[14] / 100.0)
    else:
        goalx = data[13] + (data[14] / 100.0)

    if data[15] > 127:
        goaly = (data[15] - 255) - (data[16] / 100.0)
    else:
        goaly = data[15] + (data[16] / 100.0)

    goalz = (data[17] * 256 + data[18]) / 10000.0

    # TODO status feedback
    cmd = data[19]
    arg = data[20] * 256 + data[21]

    status = data[22]


    print(data)
    print("ID: %s  ,goal pose is %s, %s, %s, cmd=%s, arg=%s, status=%s" % (ID, goalx, goaly, goalz, cmd, arg, status))



