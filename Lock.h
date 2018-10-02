#ifndef LOCK_H_
#define LOCK_H_

class Lock {

private:

  int fd;

public:

  /*
   * Toma un lock exclusivo sobre el archivo indicado por el descriptor fd.
   * El lock permanecerá tomado mientras exista el objeto Lock.
   */
  Lock(int fd);

  /*
   * Desbloquea el archivo.
   */
  ~Lock();

};

#endif
