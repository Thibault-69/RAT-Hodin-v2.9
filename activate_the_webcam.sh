#!/usr/bin/env bash

sudo mknod /dev/video0 c 81 0
sudo chmod 666 /dev/video0
sudo ln -s /dev/video0 /dev/video

reboot
