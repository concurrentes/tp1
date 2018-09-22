#include "../SharedQueue.h"

#include <sys/types.h>
#include <sys/ipc.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

int test_basics() {
  return 0;
}

int main(int argc, char **argv) {
  key_t key = ftok("/bin/bash", 0);

  smstruct::SharedQueue queue(key);

  if (errno == smstruct::SharedQueue::SMQ_ERROR_COULD_NOT_GET_SHM) {
    printf("ERROR: COULD NOT GET SHM\n");
  }

  return 0;
}
