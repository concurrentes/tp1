#ifndef HALF_DUPLEX_CHANNEL_H_
#define HALF_DUPLEX_CHANNEL_H_

#include <string>

class HDuplexChannel {

private:

  static const std::string BASE_PATH;

  std::string path;

  int fd;

  void open_file(const char *path, int mode);

public:

  enum Mode { READ, WRITE };

  /*
   * Abre un canal con el id provisto.
   */
  HDuplexChannel(unsigned int id, Mode mode);

  /*
   * Envía count bytes a través del canal, comenzando a leer
   * desde la dirección data.
   */
  int push(void *data, ssize_t count);

  /*
   * Recibe count bytes, almacenando los datos recibidos
   * a partir de la dirección buff.
   */
  void recv(void *buff, ssize_t count);

  /*
   * Igual a recv, pero si no hay datos para leer, no sigue esperando.
   * En caso de no haber datos en el FIFO, no lee nada;
   * en caso de haber menos de count bytes, leerá lo que pueda.
   */
  ssize_t recv_or_continue(void *buff, ssize_t count);

  /*
   * Cierra el descriptor; el canal ya no puede ser utilizado.
   */
  void shutdown();

  /*
   * Cierra el descriptor y desenlaza el archivo.
   */
  ~HDuplexChannel();

};

#endif
