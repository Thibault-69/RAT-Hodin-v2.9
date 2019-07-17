#!/bin/bash

### BEGIN INIT INFO
# Provides:          srv_hodin
# Required-Start:    $remote_fs $syslog
# Required-Stop:     $remote_fs $syslog
# Default-Start:     2 3 4 5
# Default-Stop:      0 1 6
# Short-Description: Hodin server
# Description:       Hodin server
### END INIT INFO

DESC="Programme:"
DAEMON=/usr/sbin/srv_hodin

PATH=/bin:/usr/bin:/sbin:/usr/sbin

NAME=srv_hodin

DAEMON=/usr/bin/srv_hodin

test -x $DAEMON || exit 0

case "$1" in

    start)

        if [ -z "$(ps -A | grep $NAME)" ]

then
            $NAME & >& /dev/null

        fi

;;

esac

exit 0
