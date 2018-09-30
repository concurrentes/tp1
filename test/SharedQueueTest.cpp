#include "BlockingSharedQueue.h"

#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>

#include <list>

using namespace std;
using namespace smstruct;

const unsigned int QUEUE_ID = 0;
const uint32_t BUFFER_SIZE = 32;

void run_parent(BlockingSharedQueue &queue) {
  printf("[C] Running\n");

  char message[8] = {'H', 'E', 'Y', 'O', 'H', 'A', 'I', 0};

  queue.enqueue(&message, 8, 0);
  queue.enqueue(&message, 8, 0);

  printf("[C] Almost done\n");
}

void run_child(BlockingSharedQueue &queue) {
  printf("[P] Running\n");

  sleep(5);

  list<void *> taken;

  queue.take(6, taken);

  list<void *>::iterator it;

  for (it = taken.begin(); it != taken.end(); ++it) {
    char *current = (char *)(*it);
    printf("[P] Took %s\n", current);
    free(current);
  }

  printf("[P] Almost done\n");
}

int main(int argc, char **argv) {
  BlockingSharedQueue queue(QUEUE_ID);

  if (fork() == 0) {
    run_child(queue);
  } else {
    run_parent(queue);
  }

  return 0;
}
