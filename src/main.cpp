#include <gtkmm/application.h>
#include "MainWindow.hpp"

int main(int argc, char *argv[])
{
  auto app = Gtk::Application::create("com.rgi.ddcutil");
  auto settings = Gtk::Settings::get_default();
  settings->property_gtk_application_prefer_dark_theme() = true;
  return app->make_window_and_run<MainWindow>(argc, argv);
}