#ifndef SEMAFORO_H_
#define SEMAFORO_H_

#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <string>

class Semaforo {

private:
	int id;


public:
	Semaforo (const std::string& nombre);
	~Semaforo();

	int inicializar(int value) const;
	int p () const; // decrementa
	int v () const; // incrementa
	void eliminar () const;
};

#endif /* SEMAFORO_H_ */
