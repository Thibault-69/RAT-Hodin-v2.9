#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <ctype.h>

#include <sys/resource.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <netdb.h>
#include <netinet/in.h>


#include <gtk-2.0/gtk/gtk.h>
#include <pthread.h>

#include "../includes/callbacks_rudy_ddos.h"
#include "../includes/callbacks_arme_ddos.h"
#include "../includes/utils.h"



GtkWidget *ddos_text_view;
GtkWidget *main_win;

int parseURL(char *url, struct urlparts **returnpart);


void cb_arme_ddos(void)
{

}
