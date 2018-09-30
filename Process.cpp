#include "Logger.h"
#include "Process.h"

#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <assert.h>

void Process::start() {
  this->graceful_quit = 0;
  this->pid = fork();

  if (this->pid == 0) {
    LOG(LOG_DEBUG, "Proceso " << get_pid() << " instanciado.");
    SignalHandler::getInstance()->registrarHandler(SIGINT, this);
    srand(get_pid());
    int exit_status = run();
    _finalize();
    exit(exit_status);
  } else {
    LOG(LOG_DEBUG, "Objeto Process(pid: " << this->get_pid() << ") iniciado.");
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
    this->graceful_quit = 1;
    _shutdown_children();
    wait_for_children();
  } else {
    LOG(LOG_DEBUG, "Enviando SIGINT a " << this->get_pid() << ".");
    kill(this->get_pid(), SIGINT);
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

void Process::spawn_child(ProcessFactory &factory) {
  Process *p = factory.instantiate();
  p->start();
  children.push_back(p);
}

void Process::clean_zombies() {
  if (this->is_self()) {
    p_vec::iterator it;
    for (it = children.begin(); it != children.end(); ++it) {
      int status;
      if (waitpid((*it)->get_pid(), &status, WNOHANG) == (*it)->get_pid()) {
        delete *it;
        children.erase(it);
      }
    }
  }
}

int Process::handle_signal(int signum) {
  assert(signum == SIGINT);
  this->graceful_quit = 1;
  shutdown();
  return 0;
}

bool Process::should_quit_gracefully() {
  return graceful_quit == 1;
}

Process::~Process() {
  if (this->is_self()) {
    _finalize();
    LOG(LOG_DEBUG, "Objeto Process(pid: " << this->get_pid() << ") destruido.");
  }
}
