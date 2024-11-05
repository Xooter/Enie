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

  const vector<key> keys = {
      {{KEY_SEMICOLON, KEY_SEMICOLON, KEY_SEMICOLON}, 0},
  };

  // {{KEY_O, KEY_I, KEY_N}, 1}, // incomodo
  // {{KEY_A, KEY_I, KEY_N}, 1}, // incomodo
  // {{KEY_E, KEY_I, KEY_N}, 1}, // incomodo

  Keyboard keyboard;
  stack<input_event> last_keys;

  void init();
  void open_device();
  void verify(input_event *event);

  void ClearLastKeys();

public:
  DeviceHandler(char *input_dev);
  ~DeviceHandler();

  void loop();

  void close_device();
};
