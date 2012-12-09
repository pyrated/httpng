#include <gtk/gtk.h>
#include <cairo/cairo.h>
#include <webkit/webkit.h>

static gboolean load_images = FALSE;
static gint device_width = 800;
static gint device_height = 600;
static gint width = 800;
static gint height = 600;
static gint zoom = 100;
static gint x = 0;
static gint y = 0;
static gchar* user_agent;
static gchar** rest = NULL;

static GOptionEntry entries[] = {
  { "images", 'i', 0, G_OPTION_ARG_NONE, &load_images, "Load images", NULL },
  { "width", 'w', 0, G_OPTION_ARG_INT, &width, "Image width", NULL },
  { "height", 'h', 0, G_OPTION_ARG_INT, &height, "Image height", NULL },
  { "dwidth", 0, 0, G_OPTION_ARG_INT, &device_width, "Device width", NULL },
  { "dheight", 0, 0, G_OPTION_ARG_INT, &device_height, "Device height", NULL },
  { "zoom", 'z', 0, G_OPTION_ARG_INT, &zoom, "Zoom level (25-500)%", NULL },
  { "xpan", 'x', 0, G_OPTION_ARG_INT, &x, "Horizontal panning", NULL },
  { "ypan", 'y', 0, G_OPTION_ARG_INT, &y, "Veritical panning", NULL },
  { "--user-agent", 'u', 0, G_OPTION_ARG_STRING, &user_agent, "User agent string", NULL },
  { G_OPTION_REMAINING, 0, 0, G_OPTION_ARG_STRING_ARRAY, &rest, NULL, NULL },

  { NULL }
};

int main(int argc, char** argv) {

  gtk_init(&argc, &argv);

  GError* error = NULL;
  GOptionContext* context;

  context = g_option_context_new("url filename.png");
  g_option_context_set_summary(context, "Take a thumbnail of a website.");
  g_option_context_add_main_entries(context, entries, NULL);
  g_option_context_add_group(context, gtk_get_option_group(TRUE));
  if (!g_option_context_parse(context, &argc, &argv, &error)) {
    g_print ("option parsing failed: %s\n", error->message);
    return 1;
  }

  GtkWidget* window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

  gtk_window_set_default_size(GTK_WINDOW(window), device_width, device_height);
  gtk_window_set_title(GTK_WINDOW(window), "httpng");

  GtkWidget* view = webkit_web_view_new();
  webkit_web_view_set_zoom_level(WEBKIT_WEB_VIEW(view), zoom/100.0f);
  WebKitWebSettings* settings = webkit_web_view_get_settings(WEBKIT_WEB_VIEW(view));
  g_object_set(G_OBJECT(settings), "enable-scripts", FALSE, NULL);
  g_object_set(G_OBJECT(settings), "enable-html5-database", FALSE, NULL);
  g_object_set(G_OBJECT(settings), "enable-html5-local-storage", FALSE, NULL);
  g_object_set(G_OBJECT(settings), "enable-offline-web-application-cache", FALSE, NULL);
  g_object_set(G_OBJECT(settings), "enable-plugins", FALSE, NULL);
  g_object_set(G_OBJECT(settings), "media-playback-allows-inline", FALSE, NULL);
  g_object_set(G_OBJECT(settings), "media-playback-requires-user-gesture", TRUE, NULL);
  
  g_object_set(G_OBJECT(settings), "user-agent", user_agent, NULL);
  g_object_set(G_OBJECT(settings), "auto-load-images", load_images, NULL);

  gtk_container_add(GTK_CONTAINER(window), view);

  webkit_web_view_load_uri(WEBKIT_WEB_VIEW(view), rest[0]);

  gtk_widget_show_all(window);

  gtk_main();

  return 0;
}