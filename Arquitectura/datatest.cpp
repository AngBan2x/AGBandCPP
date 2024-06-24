#include <fcntl.h>
#include <iostream>
#include <string>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

char *map1;
off_t filesize1;
void readFileWithMmap(const std::string &filename) {
  // Abrir el archivo
  int fd = open(filename.c_str(), O_RDONLY);
  if (fd == -1) {
    std::cerr << "Error abriendo el archivo: " << filename << std::endl;
    return;
  }

  // Obtener el tamaño del archivo
  struct stat sb;
  if (fstat(fd, &sb) == -1) {
    std::cerr << "Error obteniendo el tamaño del archivo." << std::endl;
    close(fd);
    return;
  }
  off_t filesize = sb.st_size;

  // Mapear el archivo en la memoria
  char *map = static_cast<char *>(
      mmap(nullptr, filesize, PROT_READ, MAP_PRIVATE, fd, 0));
  if (map == MAP_FAILED) {
    std::cerr << "Error mapeando el archivo en memoria." << std::endl;
    close(fd);
    return;
  }

  // Leer el archivo desde la memoria
  // std::cout << "Contenido del archivo:" << std::endl;
  // for (off_t i = 0; i < filesize; ++i) {std::cout << map[i];}

  map1 = map;
  filesize1 = filesize;
  std::cout << "filesize: " << filesize << std::endl;
  // Deshacer el mapa
  // if (munmap(map, filesize) == -1) {std::cerr << "Error deshaciendo el mapa."
  // << std::endl;}

  close(fd);
}
