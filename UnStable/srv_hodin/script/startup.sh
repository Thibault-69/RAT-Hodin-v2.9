#!/bin/bash

### BEGIN INIT INFO
# Provides:          srv_hodin
# Required-Start:    $remote_fs $syslog
# Required-Stop:     $remote_fs $syslog
# Default-Start:     2 3 4 5
# Default-Stop:      0 1 6
# Short-Description: Hodin server
# Description:       Main Hodin server
### END INIT INFO

DESC="Programme:"
DAEMON=/usr/bin/srv_hodin

update-rc.d hodin_daemon.sh start 30 2 3 4 5 . stop 10 0 1 6 .

echo -e "\nsrv_hodin added to startup routine.\n"

