#include <sstream>
#include <iomanip>
#include <gtkmm/messagedialog.h>
#include "MainWindow.hpp"
#include "Loading.hpp"

const int AUTO_HEIGHT = -1;

MainWindow::MainWindow() : Gtk::Window()
{
  set_title("Brightness - DDCUtil");
  set_default_size(300, AUTO_HEIGHT);
  create_from_layout();

  list_monitors();
  on_monitor_changed();

  attach_events();
}

void MainWindow::create_from_layout()
{
  auto m_builder = Gtk::Builder::create_from_resource("/com/rgi/ddcutil/window.ui");

  auto main_box = m_builder->get_widget<Gtk::Box>("main_box");
  if (main_box)
    set_child(*main_box);

  m_main_box = m_builder->get_widget<Gtk::Box>("main_box");
  m_combo_monitors = m_builder->get_widget<Gtk::ComboBoxText>("cbxMonitors");
  m_lbl_value = m_builder->get_widget<Gtk::Label>("lblValue");
  m_sld_value = m_builder->get_widget<Gtk::Scale>("sldValue");
  m_btn_apply = m_builder->get_widget<Gtk::Button>("btnApply");
}

void MainWindow::attach_events()
{
  m_sld_value->signal_value_changed().connect(
      sigc::mem_fun(*this, &MainWindow::on_slider_changed));

  m_btn_apply->signal_clicked().connect(
      sigc::mem_fun(*this, &MainWindow::on_apply_clicked));

  m_combo_monitors->signal_changed().connect(
      sigc::mem_fun(*this, &MainWindow::on_monitor_changed));
}

void MainWindow::loading(bool loading)
{
  this->set_cursor(loading ? Gdk::Cursor::create("wait") : Gdk::Cursor::create("default"));
  auto native = get_native();
  if (native)
  {
    auto surface = native->get_surface();
    if (surface)
    {
      surface->set_cursor(loading ? Gdk::Cursor::create("wait") : Gdk::Cursor::create("default"));
    }
  }
  m_main_box->set_sensitive(!loading);
}

void MainWindow::list_monitors()
{
  auto monitors = list_controllable_monitors();

  m_combo_monitors->remove_all();

  if (monitors.empty())
  {
    fatal_error("No monitors found!");
    return;
  }

  for (const auto &m : monitors)
  {
    m_combo_monitors->append(std::to_string(m.id), m.name);
  }

  m_combo_monitors->set_active(0);
}

void MainWindow::show_value(double value)
{
  update_label(value);
  m_sld_value->set_value(static_cast<double>(value));
}
void MainWindow::update_label(double value)
{
  std::stringstream ss;
  ss << std::fixed << std::setprecision(0) << value << "%";
  m_lbl_value->set_text(ss.str());
}

void MainWindow::on_slider_changed()
{
  update_label(m_sld_value->get_value());
}

void MainWindow::on_monitor_changed()
{
  loading(true);
  int id = get_selected_monitor_id();
  Glib::signal_idle().connect([this, id]()
                              {
                                try
                                {
                                  int brightness = get_current_brightness(id);
                                  show_value(brightness);
                                }
                                catch (const std::exception &e)
                                {
                                  fatal_error("Could not retrive the current brightness");
                                }

                                loading(false);
                                return false; });
}

int MainWindow::get_selected_monitor_id()
{
  std::string active_id = m_combo_monitors->get_active_id();
  if (active_id.empty() || active_id == "0")
    return 0;
  return std::stoi(active_id);
}

void MainWindow::on_apply_clicked()
{
  loading(true);
  int id = get_selected_monitor_id();
  Glib::signal_idle().connect([this, id]()
                              {
                              try
                              {
                                int valor = static_cast<int>(m_sld_value->get_value());
                                set_monitor_brightness(id, valor);
                              }
                              catch(const std::exception& e) {  }
                              loading(false);
                              return false; });
}

void MainWindow::fatal_error(const Glib::ustring &message)
{
  auto dialog = new Gtk::MessageDialog(*this, message,
                                       false, Gtk::MessageType::ERROR,
                                       Gtk::ButtonsType::OK, true);
  dialog->set_secondary_text(" ");
  dialog->signal_response().connect([this, dialog](int response_id)
                                    {
    delete dialog;
    auto app = get_application();
    if (app)
      app->quit();
    else
      exit(0); });

  dialog->show();
}