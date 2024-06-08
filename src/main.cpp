#include <fcntl.h>
#include <limits.h>
#include <linux/input-event-codes.h>
#include <linux/input.h>
#include <linux/uinput.h>
#include <poll.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <termios.h>
#include <unistd.h>

void emit(int fd, int type, int code, int val) {
  struct input_event ie;

  ie.type = type;
  ie.code = code;
  ie.value = val;
  /* timestamp values below are ignored */
  ie.time.tv_sec = 0;
  ie.time.tv_usec = 0;

  write(fd, &ie, sizeof(ie));
}

int main(int argc, char *argv[]) {

  // dev/uinput
  struct uinput_setup usetup;

  // dev/input
  int timeout_ms = -1;
  char *input_dev = argv[1];
  int ret;
  struct pollfd fds[1];

  // read device
  fds[0].fd = open(input_dev, O_RDONLY | O_NONBLOCK);

  // new device
  int fd = open("/dev/uinput", O_WRONLY | O_NONBLOCK);

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

  // new device
  memset(&usetup, 0, sizeof(usetup));
  usetup.id.bustype = BUS_USB;
  usetup.id.vendor = 0x1234;
  usetup.id.product = 0x5678;
  strcpy(usetup.name, "Enie");

  ioctl(fd, UI_DEV_SETUP, &usetup);
  ioctl(fd, UI_DEV_CREATE);

  sleep(1);

  if (fds[0].fd < 0) {
    printf("error unable open for reading '%s'\n", input_dev);
    return (0);
  }

  const int input_size = sizeof(struct input_event);
  printf("input_size=%d\n", input_size);

  struct input_event *input_data = (struct input_event *)malloc(input_size);
  if (input_data == NULL) {
    perror("Failed to allocate memory for input_data");
    return -1;
  }

  memset(input_data, 0, input_size);
  fds[0].events = POLLIN;
  while (true) {
    ret = poll(fds, 1, timeout_ms);

    if (ret > 0) {
      if (fds[0].revents) {
        ssize_t r = read(fds[0].fd, input_data, input_size);

        if (r < 0) {
          printf("error %d\n", (int)r);
          break;
        } else {
          if (input_data->code == KEY_SEMICOLON && input_data->value == 1) {

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

          memset(input_data, 0, input_size);
        }
      }
    }
  }
  // close listening
  close(fds[0].fd);

  // close reading
  ioctl(fd, UI_DEV_DESTROY);
  close(fd);
  return 0;
}
