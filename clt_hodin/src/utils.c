#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>  //strlen()

#include <sys/types.h>  // send()
#include <sys/socket.h>  // send()
#include <netinet/in.h>  // SOCKADDR_IN
#include <arpa/inet.h>  // inet_addr()
#include <netdb.h>  //gethostbyaddr()

#include <gtk-2.0/gtk/gtk.h>

#include "../includes/utils.h"
#include "../includes/constants.h"

GtkWidget *upload_entry;
const gchar *chemin;


void wait_time_end(float total_time)
{
    clock_t end_of_record = clock() + ((clock_t)total_time * CLOCKS_PER_SEC);

    while(clock() < end_of_record);

    return;
}

void clean_buffer(char *buffer)
{
    memset(buffer, 0, strlen(buffer) + 1);

    return;
}

void nEraser(char *chaine)
{
    size_t i = 0;

    for(i = 0; i < strlen(chaine); i++)
    {
        if(chaine[i] == '\n')
            chaine[i] = '\0';
    }
}

void get_file_name()
{
    char invertFileName[256] = "";
    char fileName[256] = "";
    size_t i = 0, j = 0, k = 0, l = 0, m = 0;
    char temp = 0;

    sprintf(invertFileName, "%s", chemin);

    /* Inversion du path */
    j = strlen(invertFileName) - 1;
    i = 0;
    while(i < j)
    {
        temp = invertFileName[i];
        invertFileName[i] = invertFileName[j];
        invertFileName[j] = temp;
        i++;
        j--;
    }

    /* On isole le nom du fichier qui sera a l'envers */
    while(invertFileName[k] != '/')
    {
        fileName[k] = invertFileName[k];
        k++;
    }

    /* On remet le nom a l'endroit */
    m = strlen(fileName) - 1;
    while(l < m)
    {
        temp = fileName[l];
        fileName[l] = fileName[m];
        fileName[m] = temp;
        l++;
        m--;
    }

    printf("filename = %s\n", fileName);

    gtk_entry_set_text(GTK_ENTRY(upload_entry), fileName);

    return;
}

