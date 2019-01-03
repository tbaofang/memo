#!/usr/bin/env bash

path_prefix=/home/fa
rm -rf $path_prefix/AP
rm $path_prefix/config
rm -rf $path_prefix/projects/ban_ka/src
rm /etc/init.d/app*
chown -R fa:root /home/fa
