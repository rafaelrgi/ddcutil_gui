#include <stdio.h>
#include <stdlib.h>
#include "monitor.h"

int get_current_brightness()
{
  FILE *fp;
  char path[1035];
  int value = 50;

  // --terse facilita o sscanf
  fp = popen("ddcutil getvcp 10 --terse", "r");
  if (fp == NULL)
    return value;

  if (fgets(path, sizeof(path), fp) != NULL)
  {
    // O formato terse é: VCP 10 C <atual> <max>
    sscanf(path, "VCP 10 C %d", &value);
  }

  pclose(fp);
  return value;
}

void set_monitor_brightness(int value)
{
  char command[64];
  snprintf(command, sizeof(command), "ddcutil --display 1 setvcp 10 %d", value);
  system(command);
}