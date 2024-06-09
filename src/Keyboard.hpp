#pragma once

#include <fcntl.h>
#include <limits.h>
#include <linux/input-event-codes.h>
#include <linux/input.h>
#include <linux/uinput.h>
#include <poll.h>
#include <stack>
#include <stdbool.h>
#include <stdexcept>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <termios.h>
#include <thread>
#include <unistd.h>
#include <vector>

using namespace std;

struct key {
  vector<int> keys;
  int rollback;
};

class Keyboard {
private:
  struct uinput_setup usetup;
  int fd;

  void emit(const int fd, const int type, const int code, const int val);

  void push(const int code);
  void release(const int code);
  void pushRelease(const int code);
  void refresh();

  void init();

  void create_device();

public:
  Keyboard();
  ~Keyboard();

  void enie(const int rollback, const vector<int> *keys);
  bool transform(const vector<key> transformKeys, input_event *event,
                 stack<input_event> keys_buf);

  void close_device();
};
