#ifndef LOCK_H_
#define LOCK_H_
#include <fcntl.h>
#include <string>
class Lock {

private:

  int fd;
  struct flock lock;
  std::string name;

public:

  /*
   * Toma un lock exclusivo sobre el archivo indicado por el pathname name + ".lock"
   * El lock permanecerá tomado mientras exista el objeto Lock.
   */
  Lock(std::string name);

  /*
   * Desbloquea el archivo.
   */
  ~Lock();

};

#endif
