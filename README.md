# Trabajo Práctico Nº1

## Checklist de tareas a realizar

1. Diseño de procesos:
  a. Comunicación entre ellos
  b. Datos que se pasan entre ellos
  
2. Diagrama de clases

3. Listado y detalle de casos de uso

4. Diagrama de transición de estados de un pasajero


TODO (cosas relativamente fáciles):

* Modo debug y mandar los logs a un archivo en vez de mandarlos a la salida estándar.

* Los logs parecen estár mostrando los timestamps en segundos, a pesar de que pedimos microsegundos.

* Sleeps aleatorios; en la configuración hay muchos tiempos promedio; habría que poner quizás algún delta y ajustar los sleeps que hay por ahí dando vueltas para que duren un tiempo aleatorio.

* Implementar paseo entre ciudades para los turistas. La forma más simple sería con una cola bloqueante como las que tienen las ciudades; cuando un turista baja de un barco, hay una probabilidad de acolarlo en la cola, que es global. Después habría un proceso adicional que entre intervalos aleatorios lee de la cola y manda el turista que saca a una ciudad aleatoria

