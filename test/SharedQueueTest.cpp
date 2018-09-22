#include "../SharedQueue.h"

#include <sys/types.h>
#include <sys/ipc.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

const uint32_t BUFFER_SIZE = 32;

void initialize_buffer(void *buffer, char c) {
  memset(buffer, c, BUFFER_SIZE);
  ((char *)buffer)[BUFFER_SIZE - 1] = 0;
}

int main(int argc, char **argv) {
  key_t key = ftok("/bin/bash", 0);

  smstruct::SharedQueue queue(key);

  if (errno == smstruct::SharedQueue::SMQ_ERROR_COULD_NOT_GET_SHM) {
    printf("ERROR: COULD NOT GET SHM\n");
  }

  char buffer_a[BUFFER_SIZE];
  char buffer_b[BUFFER_SIZE];
  char buffer_c[BUFFER_SIZE];

  initialize_buffer(buffer_a, 'A');
  initialize_buffer(buffer_b, 'B');
  initialize_buffer(buffer_c, 'C');


  queue.enqueue(&buffer_a, BUFFER_SIZE, 0);
  queue.enqueue(&buffer_b, BUFFER_SIZE, 1);
  queue.enqueue(&buffer_c, BUFFER_SIZE, 1);

  char *a = (char *)queue.next();
  char *b = (char *)queue.next();
  char *c = (char *)queue.next();

  printf("%s\n", a);
  printf("%s\n", b);
  printf("%s\n", c);

  free(a);
  free(b);
  free(c);
 
  queue.enqueue(&buffer_a, BUFFER_SIZE, 0);
  queue.enqueue(&buffer_b, BUFFER_SIZE, 0);

  a = (char *)queue.next();
  b = (char *)queue.next();

  printf("%s\n", a);
  printf("%s\n", b);

  free(a);
  free(b);
  return 0;
}
