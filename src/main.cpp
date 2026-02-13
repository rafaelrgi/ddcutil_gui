#include <gtkmm/application.h>
#include "MainWindow.hpp"

int main(int argc, char *argv[])
{
  auto app = Gtk::Application::create("com.rgi.ddcutil");
  return app->make_window_and_run<MainWindow>(argc, argv);
}