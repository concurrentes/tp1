# Trabajo Práctico Nº1

TODO (cosas relativamente fáciles):

* Mandar los logs a un archivo en vez de mandarlos a la salida estándar.

* Los logs parecen estár mostrando los timestamps en segundos, a pesar de que pedimos microsegundos.

* Prefectura en los muelles.

* Sleeps aleatorios; en la configuración hay muchos tiempos promedio; habría que poner quizás algún delta y ajustar los sleeps que hay por ahí dando vueltas para que duren un tiempo aleatorio.

* Cargar la configuración desde un archivo.

TODO (cosas más complicadas)

* Identificadores para personas; por ahora todas las personas tienen ID 0; se podría tener un contador global en memoria compartida, lockeado mediante algún mecanismo. Se puede usar la clase de memoria compartida que propusieron en clase; solo es cuestión de wrappearla con un lock; después solo hay que asegurarse que todos los procesos tengan el mismo key.

* Implementar paseo entre ciudades para los turistas. La forma más simple sería con una cola bloqueante como las que tienen las ciudades; cuando un turista baja de un barco, hay una probabilidad de acolarlo en la cola, que es global. Después habría un proceso adicional que entre intervalos aleatorios lee de la cola y manda el turista que saca a una ciudad aleatoria.

* Prefectura en el agua (no se si tiene mucho sentido que solo estén en los muelles, aunque puede servir para safar). Esto podía ser difícil; se puede implementar mandando una señal a un bote, y que setee un flag si un rand cae dentro de un cierto intervalo. Si cuando el bote llega al muelle el flag está seteado, la gente tiene que bajar y se detiene el proceso del bote.
