# Trabajo Práctico Nº1

## Cosas fijas por configuración

* Cantidad de barcos
* Capacidad de los barcos ("tamaño de la cola" del barco?)

## Cosas a decidir

a. Identificar procesos 

b. Identificar casos de uso

c. Si los barcos tienen rutas definidas ("barco 1 va de Ciudad A a la Ciudad B y de vuelta únicamente; barco 2 de la B a la C, etc") o si paran en todos los muelles de forma secuencial

  - Si son de forma secuencial, van todos en la misma dirección? Es decir, de Ciudad X a Ciudad X + 1, y no hay barcos que vayan de Ciudad X a ciudad X - 1
  
d. Si el número de pasajeros es fijo o se van creando infinitamente con alguna distribución

e. Si los pasajeros se generan en una única ciudad originalmente o en cualquiera

f. Si los pasajeros siguen recorriendo ciudades infinitamente (se crea el pasajero 1 en la ciudad A y quiere ir a la B, al llegar cambia a otra ciudad aleatoriamente) o si tiene un fin (cuando llega a la ciudad B, "muere").

g. En qué caso sucede que un pasajero entre a un barco sin boleto, si al llegar al muelle tiene que comprar un ticket si no tiene?
  - Es posible que sea que quieran que el proceso "Inspector" elija aleatoriamente uno o más pasajeros para bajar, y que el pasajero no tenga que cumplir con una acción "comprar boleto"

h. Si los pasajeros sólo bajan en la ciudad a la que quieren ir (el barco llega a la ciudad X y bajan sólo los pasajeros que van a la ciudad X, entonces cómo es que los turistas bajan en una ciudad y caminan hacia otra?
  - Puede ser que si un pasajero es turista, al llegar el barco al muelle de la ciudad X, los turistas aleatoriamente decidan que se bajan igual aunque esa no sea su ciudad destino y caminen a la ciudad X + 1 para tomarse el barco de nuevo.

i. Estados del pasajero *(está como requerimiento del TP un diagrama de estados de pasajeros)*

j. Eventos a loggear ***(por indicación del profesor, tienen que tener un timestamp, y para debuggear conviene poner algún tipo de identificación del proceso)***


## Checklist de tareas a realizar

1. Diseño de procesos:
  a. Comunicación entre ellos
  b. Datos que se pasan entre ellos
2. Diagrama de clases
3. Listado y detalle de casos de uso
4. Diagrama de transición de estados de un pasajero


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

