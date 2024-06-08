#pragma once

#include <fcntl.h>
#include <limits.h>
#include <linux/input-event-codes.h>
#include <linux/input.h>
#include <linux/uinput.h>
#include <poll.h>
#include <stdbool.h>
#include <stdexcept>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <termios.h>
#include <unistd.h>

class Keyboard {
private:
  struct uinput_setup usetup;
  int fd;

  void emit(int fd, int type, int code, int val);

  void push(int code);
  void release(int code);
  void pushRelease(int code);
  void refresh();

  void init();

  void create_device();

public:
  Keyboard();
  ~Keyboard();

  void enie();

  void close_device();
};
