#!/usr/bin/env python
# -*- coding:utf-8 -*-

from scapy.all import *
import sys
import threading

global ip_addr

def open_files() :
    global ip, port
    ip = open("ip_list.txt", 'w')
    port = open("port.txt", 'w')

class send_SYN(threading.Thread):

    def __init__(self) :
        threading.Thread.__init__(self)

    def run(self) :

        a = IP()
        a.src = ip_addr
        a.dst = "%i.%i.%i.%i" % (random.randint(1,254), random.randint(1,254), random.randint(1,254), random.randint(1,254), )

        b = TCP()
        b.sport = random.randint(1, 65535)
        b.dport = random.randint(1, 65535)
        b.flags = "S"

        rep, norep = sr(a/b, verbose=0)

        if rep :
            ip.write(a.dst)
            ip.write('\n')

            port.write(str(b.dport))
            port.write('\n')

        rep.summary()


if __name__ == '__main__':
    i = 0
    ip_addr = raw_input("Enter Your IP : ")

    open_files()

    while(i < 500) :
        send_SYN().start()
        i += 1

    ip.close()
    port.close()

    sys.exit("Done ...\n")
