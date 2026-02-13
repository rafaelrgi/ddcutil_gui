#ifndef LOADING_HPP
#define LOADING_HPP

#include <gtkmm.h>

class Loading
{
  Gtk::Button *btn;
  Glib::RefPtr<Gdk::Surface> surface;

public:
  Loading(Gtk::Button *b, Glib::RefPtr<Gdk::Surface> s) : btn(b), surface(s)
  {
    if (btn)
      btn->set_sensitive(false);
    if (surface)
      surface->set_cursor(Gdk::Cursor::create("wait"));
  }

  ~Loading()
  {
    if (btn)
      btn->set_sensitive(true);
    if (surface)
      surface->set_cursor(Gdk::Cursor::create("default"));
  }
};

#endif