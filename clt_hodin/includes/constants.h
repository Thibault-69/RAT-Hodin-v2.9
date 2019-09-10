#ifndef CONSTANTES_H_INCLUDED
#define CONSTANTES_H_INCLUDED

#include <errno.h>  //errno
#include <string.h> //strerror()
#include <stdlib.h> //printf()

#define error(expression, function)     printf("ERROR : %s\nFunction : %s\nError Number : %d\nError Message : %s\n", expression, function, errno, strerror(errno));

#define PORT    4444
#define MAXDATASIZE  200000

#define INVALID_SOCKET -1
#define SOCKET_ERROR -1
#define closesocket(s) close (s)

typedef int SOCKET;
typedef struct sockaddr_in SOCKADDR_IN;
typedef struct sockaddr SOCKADDR;

void fill_window(char *argv[]);


#endif


