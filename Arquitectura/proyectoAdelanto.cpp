#include "datatest.cpp"
#include <algorithm>
#include <cmath>
#include <fstream>
#include <iomanip>
#include <ios>
#include <iostream>
#include <vector>

// #include <string>

using namespace std;

ofstream archivoSalida;
ofstream archivoSalida2;
ofstream archivoSalida3;

void cache(int cache, int tbloque, int tconjunto);
void cache_directa(int bloqCache, int tbloque);
void cache_conjuntos(int bloqCache, int tbloque, int tconjunto);
void cache_asociativa(int bloqCache, int tbloque);

// const int memory_direction[] = {6,  214, 175, 214, 6,  84, 65, 174, 64,  105,
// 85, 215};

// const char *memory_direction = map1;

int main(int argc, char *argv[]) {
  int sizeCache;
  int sizeBloque;
  int sizeConjunto;

  // Preguntas al usuario para obtener información sobre la memoria caché
  cout << "Tamaño de la cache (Kb): ";
  cin >> sizeCache;
  cout << "Tamaño de los bloques (palabras): ";
  cin >> sizeBloque;
  cout << "Tamaño de los conjuntos (bloques): ";
  cin >> sizeConjunto;

  // archivoEntrada.open("entrada.txt");
  archivoSalida.open("salidaDir.txt");
  archivoSalida2.open("salidaConj.txt");
  archivoSalida3.open("salidaAsoc.txt");

  // cout << "filesize1: " << filesize1 << endl;

  // Llamada a la función `cache` del archivo `cache.cpp` con los parámetros
  // proporcionados por el usuario
  readFileWithMmap("amazon_products.csv");
  // for (off_t i = 0; i < 375936400; i++) {cout << map1[i];}
  // cout << endl;
  cache(sizeCache * 1024, sizeBloque, sizeConjunto);

  // cout << map1[0] << endl;

  cout << "Resultados escritos a los archivos de salida.";

  // Fin del programa
  return 0;
}

void cache(int cache, int tbloque, int tconjunto) {
  int palabras;
  int bloques;
  float auxiliar;

  // Ajusta el tamaño del bloque al siguiente número de potencia de 2
  auxiliar = log2(tbloque);
  auxiliar = ceil(auxiliar);
  tbloque = pow(2, auxiliar);

  // Calcula el número de palabras y bloques en la caché
  palabras = cache / 4;
  bloques = palabras / tbloque;

  // Muestra información sobre el tamaño de las palabras y bloques
  archivoSalida << "Palabras: " << palabras << '\n';
  archivoSalida << "Bloques: " << bloques << '\n';
  archivoSalida << left;
  archivoSalida2 << "Palabras: " << palabras << '\n';
  archivoSalida2 << "Bloques: " << bloques << '\n';
  archivoSalida2 << left;
  archivoSalida3 << "Palabras: " << palabras << '\n';
  archivoSalida3 << "Bloques: " << bloques << '\n';
  archivoSalida3 << left;

  // Llama a las funciones de caché específicas según el tipo de caché
  cout << "cache directa..." << endl;
  cache_directa(bloques, tbloque);

  cout << "cache conjuntos..." << endl;
  cache_conjuntos(bloques, tbloque, tconjunto);

  cout << "cache conjuntos..." << endl;
  cache_asociativa(bloques, tbloque);
}

void cache_directa(int bloqCache, int tbloque) {
  vector<int> cache(bloqCache);   // Vector para almacenar las etiquetas de los
                                  // bloques en la caché
  int despBloque = log2(tbloque); // Desplazamiento necesario para obtener el
                                  // índice del bloque en la caché
  int indice;
  int etiqueta;
  int instruccion;
  int error = 0;

  archivoSalida << "\n\n--------------------------------------------------\n";
  archivoSalida << "                     CACHE DIRECTA";
  archivoSalida << "\n--------------------------------------------------\n";

  archivoSalida << setfill('-');
  archivoSalida << setw(12) << "DIRECCION" << setw(12) << "ETIQUETA" << setw(12)
                << "INDICE" << setw(12) << "RESULTADO" << '\n';
  archivoSalida << setfill(' ');

  // cout << "\n\n--------------------------------------------------\n";
  // cout << "                     CACHE DIRECTA";
  // cout << "\n--------------------------------------------------\n";

  // cout << setfill('-');
  // cout << setw(12) << "DIRECCION" << setw(12) << "ETIQUETA" << setw(12)
  // << "INDICE" << setw(12) << "RESULTADO" << '\n';
  // cout << setfill(' ');
  // Limpiar la caché
  for (int i = 0; i < bloqCache; i++)
    cache[i] = -1;

  // Iterar sobre las instrucciones de memoria
  for (int i = 0; i < filesize1; i++) {
    instruccion = map1[i];
    etiqueta = instruccion >> despBloque;
    indice = etiqueta % bloqCache;
    bool esError = false;

    // Verificar si hay un error de caché
    if (cache[indice] != etiqueta) {
      cache[indice] = etiqueta;
      error++;
      esError = true;
    }

    // Mostrar la tabla de resultados
    archivoSalida << setw(12) << instruccion << setw(12) << etiqueta << setw(12)
                  << indice << setw(12) << ((esError) ? "Acierto" : "Error")
                  << '\n';
    // cout << setw(12) << instruccion << setw(12) << etiqueta << setw(12)
    // << indice << setw(12) << ((esError) ? "Acierto" : "Error") << '\n';
  }

  archivoSalida << '\n';
  // cout << endl;

  // Mostrar estadísticas de fallos
  archivoSalida << setw(12) << ' ' << setw(12) << ' ' << setw(12)
                << "FALLOS:" << setw(12) << error << '\n';
  archivoSalida << setw(12) << ' ' << setw(12) << ' ' << setw(12)
                << "POSICIONES:" << setw(12) << bloqCache << '\n';

  // cout << setw(12) << ' ' << setw(12) << ' ' << setw(12)
  // << "FALLOS:" << setw(12) << error << '\n';
  // cout << setw(12) << ' ' << setw(12) << ' ' << setw(12)
  // << "POSICIONES:" << setw(12) << bloqCache << '\n';
}

void cache_conjuntos(int bloqCache, int tbloque, int tconjunto) {
  vector<vector<int>> vecConjunto(
      bloqCache /
      tconjunto); // Vector de vectores para simular caché por conjuntos
  int conjuntos = bloqCache / tconjunto;
  int despBloque = log2(tbloque);

  int etiqueta;
  int indiceConjunto;
  int instruccion;
  int error = 0;

  archivoSalida2 << "\n\n--------------------------------------------------\n";
  archivoSalida2 << "              CACHE POR CONJUNTOS";
  archivoSalida2 << "\n--------------------------------------------------\n";

  archivoSalida2 << setfill('-');
  archivoSalida2 << setw(12) << "DIRECCION" << setw(12) << "ETIQUETA"
                 << setw(12) << "INDICE" << setw(12) << "RESULTADO" << '\n';
  archivoSalida2 << setfill(' ');

  // Iterar sobre las instrucciones de memoria
  for (int i = 0; i < filesize1; i++) {
    instruccion = map1[i];
    etiqueta = instruccion >> despBloque;
    indiceConjunto = etiqueta % conjuntos;

    auto it = find((vecConjunto[indiceConjunto]).begin(),
                   (vecConjunto[indiceConjunto]).end(), etiqueta);

    int tipoResultado = 0;

    // Verificar si hay un error en el conjunto
    if (it != vecConjunto[indiceConjunto].end()) {
      vecConjunto[indiceConjunto].erase(it);
      vecConjunto[indiceConjunto].insert(vecConjunto[indiceConjunto].begin(),
                                         etiqueta);
    } else if (vecConjunto[indiceConjunto].size() <
               static_cast<size_t>(tconjunto)) {
      tipoResultado = 1;
      vecConjunto[indiceConjunto].push_back(etiqueta);
      error++;
    } else {
      tipoResultado = 2;
      vecConjunto[indiceConjunto].pop_back();
      vecConjunto[indiceConjunto].insert(vecConjunto[indiceConjunto].begin(),
                                         etiqueta);
      error++;
    }

    // Mostrar la tabla de resultados
    archivoSalida2 << setw(12) << instruccion << setw(12) << etiqueta
                   << setw(12) << indiceConjunto << setw(12)
                   << ((tipoResultado == 0)
                           ? "Acierto"
                           : ((tipoResultado == 1) ? "Error con espacio"
                                                   : "Error sin espacio"))
                   << '\n';
  }
  archivoSalida2 << '\n';

  // Mostrar estadísticas de fallos
  archivoSalida2 << setw(12) << ' ' << setw(12) << ' ' << setw(12)
                 << "FALLOS:" << setw(12) << error << '\n';
  archivoSalida2 << setw(12) << ' ' << setw(12) << ' ' << setw(12)
                 << "CONJUNTOS:" << setw(12) << conjuntos << '\n';
}

void cache_asociativa(int bloqCache, int tbloque) {
  int indice;
  int instruccion;
  int despBloque = log2(tbloque);
  int etiqueta;
  int error = 0;
  vector<int> cola;

  archivoSalida3 << "\n--------------------------------------------------"
                 << endl;
  archivoSalida3 << "                 CACHE POR ASOCIATIVA";
  archivoSalida3 << "\n--------------------------------------------------"
                 << endl;

  // printf("%-12s%-12s%-12s\n", "DIRECCION", "ETIQUETA", "RESULTADO");

  archivoSalida3 << setfill('-');
  archivoSalida3 << setw(12) << "DIRECCION" << setw(12) << "ETIQUETA"
                 << setw(12) << "RESULTADO" << '\n';
  archivoSalida3 << setfill(' ');

  // cout << setfill('-');
  // cout << setw(12) << "DIRECCION" << setw(12) << "ETIQUETA" << setw(12)
  // << "RESULTADO" << '\n';
  // cout << setfill(' ');

  // Iterar sobre las instrucciones de memoria
  for (int i = 0; i < filesize1; i++) {
    instruccion = map1[i];
    etiqueta = instruccion >> despBloque;
    auto it = find(cola.begin(), cola.end(), etiqueta);
    int tipoResultado = 0;

    // Verificar si hay un error en la caché asociativa
    if (it != cola.end()) {
      cola.erase(it);
      cola.insert(cola.begin(), etiqueta);

    } else if (cola.size() < static_cast<size_t>(bloqCache)) {
      cola.push_back(etiqueta);
      error++;
      tipoResultado = 1;

    } else {
      cola.pop_back();
      cola.insert(cola.begin(), etiqueta);
      error++;
      tipoResultado = 2;
    }

    archivoSalida3 << setw(12) << instruccion << setw(12) << etiqueta
                   << setw(12)
                   << ((tipoResultado == 0)
                           ? "Acierto"
                           : ((tipoResultado == 1) ? "Error con espacio"
                                                   : "Error sin espacio"))
                   << endl;

    // cout << setw(12) << instruccion << setw(12) << etiqueta << setw(12)
    // << ((tipoResultado == 0)
    // ? "Acierto"
    // : ((tipoResultado == 1) ? "Error con espacio"
    // : "Error sin espacio"))
    // << endl;
  }

  archivoSalida3 << setw(24) << "";
  archivoSalida3 << setw(12) << "FALLOS:" << setw(12) << error << endl;
  archivoSalida3 << setw(24) << "";
  archivoSalida3 << setw(12) << "TAMANO:" << setw(12) << cola.size() << endl;

  // cout << setw(24) << "";
  // cout << setw(12) << "FALLOS:" << setw(12) << error << endl;
  // cout << setw(24) << "";
  // cout << setw(12) << "TAMANO:" << setw(12) << cola.size() << endl;
}
