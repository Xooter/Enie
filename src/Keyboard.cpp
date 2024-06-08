#include "Keyboard.hpp"

Keyboard::Keyboard() { init(); }
Keyboard::~Keyboard() { close_device(); }

void Keyboard::emit(int fd, int type, int code, int val) {
  struct input_event ie;

  ie.type = type;
  ie.code = code;
  ie.value = val;
  /* timestamp values below are ignored */
  ie.time.tv_sec = 0;
  ie.time.tv_usec = 0;

  write(fd, &ie, sizeof(ie));
}

void Keyboard::enie() {
  emit(fd, EV_KEY, KEY_BACKSPACE, 1);
  emit(fd, EV_KEY, KEY_BACKSPACE, 0);

  emit(fd, EV_KEY, KEY_LEFTCTRL, 1);
  emit(fd, EV_KEY, KEY_LEFTSHIFT, 1);
  emit(fd, EV_KEY, KEY_U, 1);
  emit(fd, EV_KEY, KEY_LEFTCTRL, 0);
  emit(fd, EV_KEY, KEY_LEFTSHIFT, 0);
  emit(fd, EV_KEY, KEY_U, 0);

  emit(fd, EV_SYN, SYN_REPORT, 0);

  emit(fd, EV_KEY, KEY_0, 1);
  emit(fd, EV_KEY, KEY_0, 0);
  emit(fd, EV_KEY, KEY_0, 1);
  emit(fd, EV_KEY, KEY_0, 0);
  emit(fd, EV_KEY, KEY_F, 1);
  emit(fd, EV_KEY, KEY_F, 0);
  emit(fd, EV_KEY, KEY_1, 1);
  emit(fd, EV_KEY, KEY_1, 0);
  emit(fd, EV_KEY, KEY_ENTER, 1);
  emit(fd, EV_KEY, KEY_ENTER, 0);

  emit(fd, EV_SYN, SYN_REPORT, 0);
}

void Keyboard::init() {
  fd = open("/dev/uinput", O_WRONLY | O_NONBLOCK);

  /*
   * The ioctls below will enable the device that is about to be
   * created, to pass key events, in this case the space key.
   */
  ioctl(fd, UI_SET_EVBIT, EV_KEY);
  ioctl(fd, UI_SET_KEYBIT, KEY_U);
  ioctl(fd, UI_SET_KEYBIT, KEY_F);
  ioctl(fd, UI_SET_KEYBIT, KEY_1);
  ioctl(fd, UI_SET_KEYBIT, KEY_0);
  ioctl(fd, UI_SET_KEYBIT, KEY_ENTER);
  ioctl(fd, UI_SET_KEYBIT, KEY_LEFTSHIFT);
  ioctl(fd, UI_SET_KEYBIT, KEY_LEFTCTRL);
  ioctl(fd, UI_SET_KEYBIT, KEY_BACKSPACE);

  create_device();
}

void Keyboard::create_device() {
  memset(&usetup, 0, sizeof(usetup));
  usetup.id.bustype = BUS_USB;
  usetup.id.vendor = 0x1234;
  usetup.id.product = 0x5678;
  strcpy(usetup.name, "Enie");

  ioctl(fd, UI_DEV_SETUP, &usetup);
  ioctl(fd, UI_DEV_CREATE);
}

void Keyboard::close_device() {
  ioctl(fd, UI_DEV_DESTROY);
  close(fd);
}
