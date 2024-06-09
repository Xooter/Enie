#include "DeviceHandler.hpp"

DeviceHandler::DeviceHandler(char *input_dev) {
  this->input_dev = input_dev;
  init();
}

DeviceHandler::~DeviceHandler() { close_device(); }

void DeviceHandler::loop() {
  while (true) {
    ret = poll(fds, 1, timeout_ms);

    if (ret > 0) {
      if (fds[0].revents) {
        ssize_t r = read(fds[0].fd, input_data, input_size);

        if (r < 0) {
          printf("error %d\n", (int)r);
          break;
        } else {
          verify(input_data);
          memset(input_data, 0, input_size);
        }
      }
    }
  }
}

// ["nie",";;"] => "<uni>00f1"
//
void DeviceHandler::verify(input_event *event) {

  if (event->value != 1 || event->type != EV_KEY)
    return;

  if (last_keys.size() > 9 || event->code == KEY_ENTER) {
    ClearLastKeys();
  }

  if (event->code == KEY_BACKSPACE && !last_keys.empty()) {
    last_keys.pop();
  } else {
    last_keys.push(*event);
  }

  bool result = keyboard.transform(keys, event, last_keys);
  if (result) {
    ClearLastKeys();
  }
}

void DeviceHandler::ClearLastKeys() {
  while (!last_keys.empty()) {
    last_keys.pop();
  }
}

void DeviceHandler::init() {
  open_device();

  input_data = (struct input_event *)malloc(input_size);
  if (input_data == NULL) {
    throw std::runtime_error("Failed to allocate memory");
  }

  memset(input_data, 0, input_size);
  fds[0].events = POLLIN;
}

void DeviceHandler::open_device() {
  fds[0].fd = open(input_dev, O_RDONLY | O_NONBLOCK);
  if (fds[0].fd < 0) {
    throw std::runtime_error("Error unable open for reading device\n");
  }
}

void DeviceHandler::close_device() {
  keyboard.close_device();
  close(fds[0].fd);
}
