#include "../SharedStack.h"

#include <sys/types.h>
#include <sys/ipc.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

const unsigned short BUFFER_SIZE = 32;

int main(int argc, char **argv) {
  key_t key = ftok("/bin/bash", 0);

  SharedStack stack(key);

  char buffer_a[BUFFER_SIZE];
  char buffer_b[BUFFER_SIZE];

  memset(buffer_a, 'A', BUFFER_SIZE);
  memset(buffer_b, 'B', BUFFER_SIZE);

  buffer_a[BUFFER_SIZE - 1] = 0;
  buffer_b[BUFFER_SIZE - 1] = 0;

  stack.push(&buffer_a, BUFFER_SIZE);
  stack.push(&buffer_b, BUFFER_SIZE);

  void *b = stack.take();
  void *a = stack.take();

  printf("%s\n", (char *)a);
  printf("%s\n", (char *)b);

  free(b);
  free(a);

  return 0;
}
