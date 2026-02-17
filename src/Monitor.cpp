#include "Monitor.hpp"
#include <iostream>
#include <cstdio>
#include <memory>
#include <array>
#include <sstream>

std::string exec_command(const std::string &cmd)
{
  std::array<char, 128> buffer;
  std::string result;

  std::unique_ptr<FILE, void (*)(FILE *)> pipe(popen(cmd.c_str(), "r"), [](FILE *f)
                                               {
        if (f) pclose(f); });

  if (!pipe)
    return "";

  while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr)
  {
    result += buffer.data();
  }
  return result;
}

int get_current_brightness(int display_id)
{
  if (display_id <= 0)
    return 50;

  std::string cmd = "ddcutil --display " + std::to_string(display_id) + " getvcp 10 --terse --sleep-multiplier .1";
  std::string output = exec_command(cmd);

  int value = 50;
  if (!output.empty())
  {
    // VCP 10 C <atual> <max>
    std::sscanf(output.c_str(), "VCP 10 C %d", &value);
  }
  return value;
}

void set_monitor_brightness(int display_id, int value)
{
  if (display_id <= 0)
    return;

  std::string cmd = "ddcutil --display " + std::to_string(display_id) +
                    " setvcp 10 " + std::to_string(value) +
                    " --noverify --brief --sleep-multiplier .1";

  [[maybe_unused]] int unused = std::system(cmd.c_str());
}

std::vector<MonitorInfo> list_controllable_monitors()
{
  std::vector<MonitorInfo> monitors;
  std::string output = exec_command("ddcutil detect --terse");

  std::stringstream ss(output);
  std::string line;
  int current_id = -1;
  bool valid_section = false;

  while (std::getline(ss, line))
  {
    if (line.empty())
      continue;

    if (line.find("Display") == 0)
    {
      if (std::sscanf(line.c_str(), "Display %d", &current_id) == 1)
      {
        valid_section = true;
      }
    }
    else if (line.find("Invalid display") != std::string::npos)
    {
      valid_section = false;
    }

    if (valid_section && current_id != -1)
    {
      size_t pos = line.find("Monitor:");
      if (pos != std::string::npos)
      {
        std::string name = line.substr(pos + 8);
        // Trim
        name.erase(0, name.find_first_not_of(" \t"));
        name.erase(name.find_last_not_of(" \t\n\r") + 1);

        monitors.push_back({current_id, name});
      }
    }
  }
  return monitors;
}