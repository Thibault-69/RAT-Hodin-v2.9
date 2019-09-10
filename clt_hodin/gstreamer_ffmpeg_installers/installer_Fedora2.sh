#!/bin/bash

sudo dnf update -qq -y 2>/dev/null

sudo dnf -qq -y install https://download1.rpmfusion.org/free/fedora/rpmfusion-free-release-$(rpm -E %fedora).noarch.rpm 2>/dev/null
sudo dnf -qq -y install https://download1.rpmfusion.org/nonfree/fedora/rpmfusion-nonfree-release-$(rpm -E %fedora).noarch.rpm 2>/dev/null

sudo dnf -qq -y install ffmpeg 2>/dev/null

sudo dnf -qq -y install ffmpeg-devel 2>/dev/null

sudo dnf install gstreamer1-devel gstreamer1-plugins-base-tools gstreamer1-devel-docs gstreamer1-plugins-base-devel gstreamer1-plugins-base-devel-docs gstreamer1-plugins-good gstreamer1-plugins-good-extras gstreamer1-plugins-ugly gstreamer1-plugins-ugly-devel-docs  gstreamer1-plugins-bad-free gstreamer1-plugins-bad-free-devel gstreamer1-plugins-bad-free-extras -q -y 2>/dev/null

sudo apt-get install pavucontrol -qq -y 2>/dev/null
