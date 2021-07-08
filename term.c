#include <vte/vte.h>
#include <string.h>

static void
set_font_size(VteTerminal *terminal, gint delta)
{
 PangoFontDescription *descr = pango_font_description_new();
  //if ((descr = pango_font_description_copy(vte_terminal_get_font(terminal))) == NULL)
  //return;

 //gint current = pango_font_description_get_size(descr);
pango_font_description_set_family(descr,"kakwa,1");
 pango_font_description_set_variations(descr,"AXIS2=4");

 vte_terminal_set_font(terminal, descr);
 vte_terminal_set_cjk_ambiguous_width(terminal,1);
 pango_font_description_free(descr);
 
}


static void
child_ready(VteTerminal *terminal, GPid pid, GError *error, gpointer user_data)
{
    if (!terminal) return;
    if (pid == -1) gtk_main_quit();
}


static gboolean
on_key_press(GtkWidget *terminal, GdkEventKey *event, gpointer user_data)
{
 switch (event->state & (GDK_CONTROL_MASK | GDK_SHIFT_MASK | GDK_MOD1_MASK)) {
 case GDK_CONTROL_MASK | GDK_SHIFT_MASK:
  switch (event->keyval) {
  case GDK_KEY_V:
   vte_terminal_paste_clipboard(VTE_TERMINAL(terminal));
   return TRUE;
 case GDK_KEY_C:
   vte_terminal_copy_clipboard(VTE_TERMINAL(terminal));
   return TRUE;
  }
  /* fallthrough */
 case GDK_CONTROL_MASK:
  switch (event->keyval) {
  case GDK_KEY_plus:
   set_font_size(VTE_TERMINAL(terminal), 1);
   return TRUE;
  case GDK_KEY_minus:
   set_font_size(VTE_TERMINAL(terminal), -1);
   return TRUE;
  }
  break;

}
return FALSE;
}


int
main(int argc, char *argv[])
{
    GtkWidget *window, *terminal;

    /* Initialise GTK, the window and the terminal */
    gtk_init(&argc, &argv);
    terminal = vte_terminal_new();
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "YATE");

    /* Start a new shell */
    gchar **envp = g_get_environ();
    gchar **command = (gchar *[]){g_strdup(g_environ_getenv(envp, "SHELL")), NULL };
    g_strfreev(envp);
    vte_terminal_spawn_async(VTE_TERMINAL(terminal),
        VTE_PTY_DEFAULT,
        NULL,         /* working directory  */
        command,      /* command */
        NULL,         /* environment */
        0,            /* spawn flags */
        NULL, NULL,   /* child setup */
        NULL,         /* child pid */
        -1,           /* timeout */
        NULL,         /* cancellable */
        child_ready,  /* callback */
        NULL);        /* user_data */

    /* Connect some signals */
    g_signal_connect(window, "delete-event", gtk_main_quit, NULL);
    g_signal_connect(terminal, "child-exited", gtk_main_quit, NULL);
    g_signal_connect(terminal, "key-press-event", G_CALLBACK(on_key_press), GTK_WINDOW(window));

    /* Put widgets together and run the main loop */
     //PangoFontDescription *descr = pango_font_description_new();
     //pango_font_description_set_family(descr,"dina");
     //pango_font_description_set_size(descr,10240);
      //vte_terminal_set_font(terminal, descr);
 //pango_font_description_free(descr);


    gtk_container_add(GTK_CONTAINER(window), terminal);
    gtk_widget_show_all(window);
    gtk_main();
}
