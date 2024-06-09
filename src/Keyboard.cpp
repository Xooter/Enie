#include "Keyboard.hpp"

Keyboard::Keyboard() { init(); }
Keyboard::~Keyboard() { close_device(); }

void Keyboard::emit(const int fd, const int type, const int code,
                    const int val) {
  struct input_event ie;

  ie.type = type;
  ie.code = code;
  ie.value = val;

  ie.time.tv_sec = 0;
  ie.time.tv_usec = 0;

  write(fd, &ie, sizeof(ie));
}

bool Keyboard::transform(const vector<key> transformKeys, input_event *event,
                         stack<input_event> keys_buf) {
  for (int i = 0; i < transformKeys.size(); i++) {
    bool match = true;
    for (int j = 0; j < transformKeys[i].keys.size(); j++) {

      int key = transformKeys[i].keys[j];

      if (keys_buf.empty() || keys_buf.top().code != key) {
        match = false;
        break;
      }

      if (keys_buf.size() > 1)
        keys_buf.pop();
    }

    if (match) {
      enie(transformKeys[i].rollback, &transformKeys[i].keys);
      return true;
    }
  }
  return false;
}

void Keyboard::enie(const int rollback, const vector<int> *keys) {
  for (const auto &key : *keys) {
    pushRelease(KEY_BACKSPACE);
  }

  push(KEY_LEFTCTRL);
  push(KEY_LEFTSHIFT);
  push(KEY_U);
  release(KEY_LEFTCTRL);
  release(KEY_LEFTSHIFT);
  release(KEY_U);

  pushRelease(KEY_0);
  pushRelease(KEY_0);
  pushRelease(KEY_F);
  pushRelease(KEY_1);
  pushRelease(KEY_ENTER);

  this_thread::sleep_for(chrono::milliseconds(100));

  for (const auto &key : *keys) {
    pushRelease(key);
  }

  refresh();
}

void Keyboard::push(const int code) { emit(fd, EV_KEY, code, 1); }

void Keyboard::release(const int code) { emit(fd, EV_KEY, code, 0); }

void Keyboard::pushRelease(const int code) {
  push(code);
  release(code);
}

void Keyboard::refresh() { emit(fd, EV_SYN, SYN_REPORT, 0); }

void Keyboard::init() {
  fd = open("/dev/uinput", O_WRONLY | O_NONBLOCK);

  /*
   * The ioctls below will enable the device that is about to be
   * created, to pass key events, in this case the space key.
   */
  ioctl(fd, UI_SET_KEYBIT, KEY_A);
  ioctl(fd, UI_SET_KEYBIT, KEY_E);
  ioctl(fd, UI_SET_KEYBIT, KEY_O);

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
