#ifndef MAIN_WINDOW_HPP
#define MAIN_WINDOW_HPP

#include <gtkmm.h>
#include "Monitor.hpp"

class MainWindow : public Gtk::Window
{
public:
  MainWindow();

private:
  Gtk::Box *m_main_box;
  Gtk::ComboBoxText *m_combo_monitors;
  Gtk::Label *m_lbl_value;
  Gtk::Scale *m_sld_value;
  Gtk::Button *m_btn_apply;

  void loading(bool loading);
  void create_from_layout();
  void attach_events();
  void list_monitors();
  void on_monitor_changed();
  void on_slider_changed();
  void on_apply_clicked();
  int get_selected_monitor_id();
  void update_label(double value);
  void show_value(double value);
  void fatal_error(const Glib::ustring &message);
};

#endif