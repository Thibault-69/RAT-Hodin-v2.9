#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <time.h>
#include <string.h>

#include <unistd.h>
#include <sys/types.h>  // getuid()

#include "../includes/utils.h"
#include "../includes/constants.h"



char *split_resolution_cmds(const char *cmd)
{
    size_t i = 0;
    int count  = 0;

    size_t len_cmd = 0;

    char *resolution = NULL;

    len_cmd = strlen(cmd) + 1;

    resolution = malloc(len_cmd * sizeof(char));
    if(resolution == NULL)
    {
        error("malloc() resolution", "split_resolution_cmds()");
        exit(-1);
    }

    spaceEraser(resolution);

    for(i = 0; i < len_cmd; i++)
    {
        if(cmd[i] == ' ')
            count++;

        if(count == 3)
        {
            resolution[i] = '\0';
            break;
        }

        resolution[i] = cmd[i];
    }

    return resolution;
}

void clean_buffer(char *buffer)
{
    memset(buffer, 0, strlen(buffer) + 1);

    return;
}


void spaceEraser(char *chaine)
{
    size_t i = 0;
    size_t count = 0;
    size_t len = 0;

    len = strlen(chaine);

    for(i = 0; i < len; i++)
    {
        if(chaine[i] == ' ')
            count++;

        if(count == 3)
            chaine[i--] = '\0';
    }
}
