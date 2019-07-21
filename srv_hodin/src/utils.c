#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <time.h>
#include <string.h>
#include <dirent.h>

#include <unistd.h>
#include <sys/types.h>

#include "../includes/utils.h"
#include "../includes/constants.h"


void clean_buffer(char *buffer)
{
    memset(buffer, 0, strlen(buffer) + 1);

    return;
}


void wait_time_end(float total_time)
{
    clock_t end_of_record = clock() + ((clock_t)total_time * CLOCKS_PER_SEC);

    while(clock() < end_of_record);

    return;
}

