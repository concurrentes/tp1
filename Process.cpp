#include "Process.h"

#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

void Process::start() {
  this->pid = fork();

  if (this->pid == 0) {
    srand(get_pid());
    int exit_status = run();
    _finalize();
    exit(exit_status);
  }
}

pid_t Process::get_pid() {
  if (this->pid == 0) {
    this->pid = getpid();
  }
  return this->pid;
}

bool Process::is_self() {
  return getpid() == get_pid();
}

int Process::wait_for_exit() {
  if (!this->is_self()) {
    int status;
    waitpid(this->get_pid(), &status, 0);
    return WIFEXITED(status) ? WEXITSTATUS(status) : -1;
  }
}

int Process::wait_for_child() {
  if (this->is_self()) {
    int status;
    wait(&status);
    return WIFEXITED(status) ? WEXITSTATUS(status) : -1;
  }
}

void Process::wait_for_children() {
  for (unsigned int i = 0; i < children.size(); ++i) {
    wait_for_child();
  }
}

void Process::_shutdown_children() {
  p_vec::iterator it;
  for (it = children.begin(); it != children.end(); ++it) {
    (*it)->shutdown();
  }
}

void Process::shutdown() {
  if (this->is_self()) {
   _shutdown_children();
   wait_for_children();
  } else {
    // TODO: Mandar señal para finalizar.
  }
}

void Process::_finalize() {
  _clear_children();
}

void Process::_clear_children() {
  p_vec::iterator it;
  for (it = children.begin(); it != children.end(); ++it) {
    delete *it;
  }
  children.clear();
}

void Process::spawn_child(const ProcessFactory &factory) {
  Process *p = factory.instantiate();
  p->start();
  children.push_back(p);
}

Process::~Process() {
  _finalize();
}