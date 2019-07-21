#!/bin/bash

### BEGIN INIT INFO
# Provides:          srv_hodin
# Required-Start:    $remote_fs $syslog
# Required-Stop:     $remote_fs $syslog
# Default-Start:     2 3 4 5
# Default-Stop:      0 1 6
# Short-Description: cleaner
# Description:       cleaner
### END INIT INFO


cd /usr/bin/

sudo rm -rf srv_hodin

cd /usr/sbin/

sudo rm -rf srv_hodin

cd /bin/

sudo rm -rf srv_hodin

cd /etc/init.d/

sudo rm -rf hodin_daemon.sh

cd /etc/rc2.d/

sudo rm -rf 'S02hodin_daemon.sh'

sudo rm -rf 'S03hodin_daemon.sh'

sudo rm -rf 'S88hodin_daemon.sh'

cd /etc/rc3.d/

sudo rm -rf 'S02hodin_daemon.sh'

sudo rm -rf 'S03hodin_daemon.sh'

cd /etc/rc4.d/

sudo rm -rf 'S02hodin_daemon.sh'

sudo rm -rf 'S03hodin_daemon.sh'

cd /etc/rc5.d/

sudo rm -rf 'S02hodin_daemon.sh'

sudo rm -rf 'S03hodin_daemon.sh'

echo "Hodin have been removed from startup routine."
