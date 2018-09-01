#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "ProcessFactory.h"
#include "Process.h"

class ProcessA : public Process {
public:
  int run() {
    printf("Process A\n");
    return 0;
  }
};

class FactoryA : public ProcessFactory {
public:
  Process *instantiate() const {
    return new ProcessA;
  }
};

class ParentProcess : public Process {
public:
  int run() {
    printf("Launching parent process\n");
    FactoryA factory;
    spawn_child(factory);
    wait_for_children();
    printf("Parent process out\n");
  }
};

int main(int argc, char** argv) {
  ParentProcess parent;
  parent.start();
  wait(0);
  printf("Engine out\n");
  return 0;
}
