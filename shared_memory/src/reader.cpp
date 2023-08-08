#include <cstdlib>
#include <cstring>
#include <fcntl.h>
#include <iostream>
#include <sys/mman.h>
#include <thread>
#include <unistd.h>

// when flag is 0, buffer is empty, when flag is 1, buffer is full
// if flag is 0, writer can write to buffer, if flag is 1, reader can read from
// buffer
struct SharedBuffer {
  volatile uint8_t flag; // 0: buffer is empty, 1: buffer is full
  uint32_t buffer[3];
};

const char SHM_NAME[] = "shared_memory";

int main() {

  int fd = shm_open(SHM_NAME, O_RDWR | O_CREAT, 0666);
  if (fd == -1) {
    std::cerr << "shm_open failed" << std::endl;
    exit(EXIT_FAILURE);
  }

  SharedBuffer *shared_buffer = (SharedBuffer *)mmap(
      NULL, sizeof(SharedBuffer), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

  while (true) {
    while (shared_buffer->flag == 0) {
      std::this_thread::yield(); // 让出CPU
    }

    std::cout << "reader read from buffer " << shared_buffer->buffer[0]
              << std::endl;
    shared_buffer->flag = 0;

    if (shared_buffer->buffer[0] == 100) {
      break;
    }
  }

  munmap(shared_buffer, sizeof(SharedBuffer));
  close(fd);

  return 0;
}