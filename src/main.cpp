#include "DeviceHandler.hpp"

Keyboard keyboard;

void verify(int code, int value) {
  if (code == KEY_SEMICOLON && value == 1) {
    keyboard.enie();
  }
}

int main(int argc, char *argv[]) {
  DeviceHandler deviceHandler(argv[1]);

  deviceHandler.loop(&verify);

  deviceHandler.close_device();
  keyboard.close_device();

  return 0;
}
