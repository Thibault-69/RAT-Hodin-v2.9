#ifndef CALLBACKS_H_INCLUDED
#define CALLBACKS_H_INCLUDED


void cb_get_server_ip(GtkButton *button, gpointer user_data);
void cb_get_server_port(GtkButton *button, gpointer user_data);

void cb_open_file(GtkButton *button, gpointer user_data);
void cb_get_path(GtkWidget *button, GtkWidget *file_selection);

void cb_binder_open_file(GtkButton *button, gpointer user_data);
void cb_binder_get_path(GtkWidget *button, GtkWidget *file_selection);

void cb_bind_file(GtkButton *button, gpointer user_data);

void cb_files_downloader(GtkButton *button, gpointer user_data);
void cb_binaries_downloader(GtkButton *button, gpointer user_data);

void cb_disconnect_server(GtkButton *button, gpointer user_data);

void cb_hosts_downloader(GtkButton *button, gpointer user_data);
void cb_open_hosts(GtkButton *button, gpointer user_data);
void cb_hosts_get_path(GtkWidget *button, GtkWidget *file_selection);

void cb_files_uploader(GtkButton *button, gpointer user_data);

/** Menu options of hosts file window text editor **/
void cb_OnQuitter(GtkButton *button, gpointer user_data);

void cb_OnOpen(GtkButton *button, gpointer user_data);
void cb_get_file_path(GtkWidget *button, GtkWidget *file_selection);

void cb_OnSave(GtkButton *button, gpointer user_data);
void cb_get_file_path_save(GtkWidget *button, GtkWidget *file_selection);

void text_view_editor(const gchar *chemin, GtkWidget *fileSelect);
void open_hosts_file(GtkWidget *text_view, const gchar *chemin, GtkWidget *fileSelect);


#endif

