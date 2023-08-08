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
  shm_unlink(SHM_NAME);

  int fd = shm_open(SHM_NAME, O_RDWR | O_CREAT, 0666);
  if (fd == -1) {
    std::cerr << "shm_open failed" << std::endl;
    exit(EXIT_FAILURE);
  }

  if (ftruncate(fd, sizeof(SharedBuffer)) == -1) {
    std::cerr << "ftruncate failed" << std::endl;
    exit(EXIT_FAILURE);
  }

  SharedBuffer *shared_buffer = (SharedBuffer *)mmap(
      NULL, sizeof(SharedBuffer), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

  memset(shared_buffer, 0, sizeof(SharedBuffer));
  shared_buffer->buffer[0] = 0x00000000;
  shared_buffer->buffer[1] = 0x00000000;
  shared_buffer->buffer[2] = 0x00000000;

  int i = 0;

  while (true) {
    // every time buffer num add 1
    // 等待缓冲区为空
    while (shared_buffer->flag == 1) {
      std::this_thread::yield(); // 让出CPU
    }

    shared_buffer->buffer[0] += 1;
    shared_buffer->buffer[1] += 1;
    shared_buffer->buffer[2] += 1;
    shared_buffer->flag = 1;
    std::cout << "writer write to buffer " << shared_buffer->buffer[0]
              << std::endl;
    i++;
    if (i == 100) {
      break;
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(500));
  }

  munmap(shared_buffer, sizeof(SharedBuffer));
  close(fd);

  return 0;
}