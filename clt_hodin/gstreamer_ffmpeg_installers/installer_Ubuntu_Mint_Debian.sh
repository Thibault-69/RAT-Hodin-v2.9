#!/bin/bash

sudo apt-get update -qq -y 2>/dev/null

sudo apt-get install -qq -y ffmpeg 2>/dev/null

sudo apt-get install -qq -y libgstreamer1.0-0 gstreamer1.0-plugins-base gstreamer1.0-plugins-good gstreamer1.0-plugins-bad gstreamer1.0-plugins-ugly gstreamer1.0-libav gstreamer1.0-doc gstreamer1.0-tools 2>/dev/null

sudo apt-get install gtk2.0 -qq -y 2>/dev/null

sudo apt-get install build-essential libgtk2.0-dev -qq -y 2>/dev/null
