#!/bin/bash

### BEGIN INIT INFO
# Provides:          srv_hodin
# Required-Start:    $remote_fs $syslog
# Required-Stop:     $remote_fs $syslog
# Default-Start:     2 3 4 5
# Default-Stop:      0 1 6
# Short-Description: hodin main server
# Description:       Application main server
### END INIT INFO

DESC="Programme:"
DAEMON=/usr/sbin/srv_hodin

PATH=/bin:/usr/bin:/sbin:/usr/sbin

# Optionnel : définit le nom qui s'affichera

# lors du démarrage ou de l'arrêt du du démon

NAME=srv_hodin

# Emplacement du binaire du démon

DAEMON=/usr/bin/srv_hodin

# On vérifie que le programme du démon

# est exécutable, sinon fin du script.

test -x $DAEMON || exit 0

case "$1" in

    start)

# On vérifie si le démon

        # n'est pas déjà lancé.

        if [ -z "$(ps -A | grep $NAME)" ]

then

            #echo "$NAME : lancement du démon."

            $NAME & >& /dev/null

        fi

;;

esac

exit 0
