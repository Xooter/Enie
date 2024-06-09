#include "DeviceHandler.hpp"

int main(int argc, char *argv[]) {
  DeviceHandler deviceHandler(argv[1]);

  deviceHandler.loop();

  deviceHandler.close_device();

  return 0;
}
