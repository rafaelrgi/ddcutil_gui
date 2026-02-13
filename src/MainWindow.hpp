#ifndef MAIN_WINDOW_HPP
#define MAIN_WINDOW_HPP

#include <gtkmm.h>
#include "monitor.h" // Funções C originais

class MainWindow : public Gtk::Window
{
public:
  MainWindow();

private:
  Glib::RefPtr<Gtk::Builder> m_builder;
  Gtk::Label *m_lbl_value;
  Gtk::Scale *m_sld_value;
  Gtk::Button *m_btn_apply;

  void on_slider_changed();
  void on_apply_clicked();
  void update_label(double value);
};

#endif