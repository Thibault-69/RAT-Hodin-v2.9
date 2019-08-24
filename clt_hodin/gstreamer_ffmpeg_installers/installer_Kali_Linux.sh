                                                                                                                                                                                      #!/bin/bash

echo "deb http://http.kali.org/kali kali-rolling main non-free contrib" | sudo tee -a /etc/apt/sources.list

echo "deb-src http://http.kali.org/kali kali-rolling main non-free contrib" | sudo tee -a /etc/apt/sources.list

echo "deb http://http.kali.org/kali kali-rolling main non-free contrib" | sudo tee -a /etc/apt/sources.list

sudo apt-get update -qq -y 2>/dev/null

sudo apt-get install upgrade-system -qq -y 2>/dev/null

upgrade-system -qq -y 2>/dev/null

sudo apt-get install pavucontrol -qq -y 2>/dev/null

sudo apt-get install yasm nasm build-essential automake autoconf libtool pkg-config libcurl4-openssl-dev intltool libxml2-dev libgtk2.0-dev libnotify-dev libglib2.0-dev libevent-dev checkinstall -qq -y 2>/dev/null

sudo apt-get install ffmpeg -qq -y 2>/dev/null

sudo apt-get install -qq -y libgstreamer1.0-0 gstreamer1.0-plugins-base gstreamer1.0-plugins-good gstreamer1.0-plugins-bad gstreamer1.0-plugins-ugly gstreamer1.0-libav gstreamer1.0-doc gstreamer1.0-tools -qq -y 2>/dev/null

sudo apt-get install gstreamer1.0-alsa -qq -y 2>/dev/null
