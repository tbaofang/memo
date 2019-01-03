#!/bin/bash

sudo dd if=/dev/zero of=/swapfile bs=64M count=8
echo " "
sudo mkswap /swapfile
echo " "
sudo swapon /swapfile
echo "finished"
