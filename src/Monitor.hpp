#ifndef MONITOR_HPP
#define MONITOR_HPP

#include <string>
#include <vector>

struct MonitorInfo
{
  int id;
  std::string name;
};

int get_current_brightness(int display_id);
void set_monitor_brightness(int display_id, int value);
std::vector<MonitorInfo> list_controllable_monitors();

#endif