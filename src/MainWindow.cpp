#include "MainWindow.hpp"
#include "Loading.hpp"
#include <iomanip>
#include <sstream>

MainWindow::MainWindow()
    : Gtk::Window()
{
  set_title("Brightness - DDCUtil");
  set_default_size(300, -1); // -1 mantém a altura automática

  // Carrega o recurso
  m_builder = Gtk::Builder::create_from_resource("/com/rgi/ddcutil/window.ui");

  auto main_box = m_builder->get_widget<Gtk::Box>("main_box");
  if (main_box)
    set_child(*main_box);

  // Obtendo referências do arquivo .ui gerado pelo Blueprint
  m_lbl_value = m_builder->get_widget<Gtk::Label>("lblValue");
  m_sld_value = m_builder->get_widget<Gtk::Scale>("sldValue");
  m_btn_apply = m_builder->get_widget<Gtk::Button>("btnApply");

  // Configuração inicial
  int current_brightness = get_current_brightness();
  m_sld_value->set_value(static_cast<double>(current_brightness));
  update_label(current_brightness);

  // Conexão de sinais (Slots)
  m_sld_value->signal_value_changed().connect(
      sigc::mem_fun(*this, &MainWindow::on_slider_changed));

  m_btn_apply->signal_clicked().connect(
      sigc::mem_fun(*this, &MainWindow::on_apply_clicked));
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

void MainWindow::on_apply_clicked()
{
  Loading loading(m_btn_apply, get_native()->get_surface());
  auto surface = get_native()->get_surface();
  if (surface)
    surface->set_cursor(Gdk::Cursor::create("wait"));

  try
  {
    this->m_btn_apply->set_sensitive(false);

    int valor = static_cast<int>(m_sld_value->get_value());
    set_monitor_brightness(valor);
  }
  catch (...)
  {
  }

  if (surface)
    surface->set_cursor(Gdk::Cursor::create("default"));
  this->m_btn_apply->set_sensitive(true);
}