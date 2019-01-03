#!/usr/bin/env bash

path_prefix=/home/fa/rootfs

cp $path_prefix/etc/* /etc/ -rf
cp $path_prefix/lib/* /lib/ -rf
cp $path_prefix/sbin/* /sbin/ -rf

chmod 755 /sbin/dhclient /sbin/hostapd /sbin/dhcpd

mkdir /var/db

touch /var/db/dhcpd.leases

chmod 777 /home/fa/ap_wifi_switch/script/*

