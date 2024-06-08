#pragma once
#include "Keyboard.hpp"

class DeviceHandler {
private:
  int timeout_ms = -1;
  char *input_dev;
  int ret;
  struct pollfd fds[1];

  struct input_event *input_data;
  const int input_size = sizeof(struct input_event);

  void init();
  void open_device();

public:
  DeviceHandler(char *input_dev);
  ~DeviceHandler();

  void loop(void (*v)(int, int));

  void close_device();
};
