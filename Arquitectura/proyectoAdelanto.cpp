#include <iomanip>
#include <ios>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <cmath>
#include <string>

using namespace std;

ifstream archivoEntrada;
ofstream archivoSalida;

void cache(int cache,int tbloque,int tconjunto);
void cache_directa(int bloqCache, int tbloque);
void cache_conjuntos(int bloqCache,int tbloque, int tconjunto);
//void cache_asociativa(int bloqCache,int tbloque); //este se usara en el proyecto final

const int memory_direction[] = {6, 214, 175, 214, 6, 84, 65, 174, 64, 105, 85, 215};
const int memory_direction_size = sizeof(memory_direction) / sizeof(int);

int main(int argc, char * argv[]) {
    int sizeCache;
    int sizeBloque;
    int sizeConjunto;
    
    // Preguntas al usuario para obtener información sobre la memoria caché
    cout << "Tamaño de la cache (Kb): ";            cin >> sizeCache;
    cout << "Tamaño de los bloques (palabras): ";   cin >> sizeBloque;
    cout << "Tamaño de los conjuntos (bloques): ";  cin >> sizeConjunto;

    archivoEntrada.open("entrada.txt");
    archivoSalida.open("salida.txt");

    // Llamada a la función `cache` del archivo `cache.cpp` con los parámetros proporcionados por el usuario
    cache(sizeCache * 1024, sizeBloque, sizeConjunto);

    archivoEntrada.close();
    archivoSalida.close();

    cout << "Resultados escritos al archivo 'salida.txt'.";
    
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

    // Llama a las funciones de caché específicas según el tipo de caché
    cache_directa(bloques, tbloque);
    cache_conjuntos(bloques, tbloque, tconjunto);
}

void cache_directa(int bloqCache, int tbloque) {
    vector<int> cache(bloqCache);  // Vector para almacenar las etiquetas de los bloques en la caché
    int despBloque = log2(tbloque); // Desplazamiento necesario para obtener el índice del bloque en la caché
    int indice;
    int etiqueta;
    int instruccion;
    int error = 0;

    archivoSalida << "\n\n--------------------------------------------------\n";
    archivoSalida << "                     CACHE DIRECTA";
    archivoSalida << "\n--------------------------------------------------\n";

    archivoSalida << setfill('-');
    archivoSalida << setw(12) << "DIRECCION"
                  << setw(12) << "ETIQUETA"
                  << setw(12) << "INDICE"
                  << setw(12) << "RESULTADO" << '\n';
    archivoSalida << setfill(' ');

    // Limpiar la caché
    for (int i = 0; i < bloqCache; i++)
        cache[i] = -1;

    // Iterar sobre las instrucciones de memoria
    for (int i = 0; i < memory_direction_size; i++)
    {
        instruccion = memory_direction[i];
        etiqueta = instruccion >> despBloque;
        indice = etiqueta % bloqCache;
        bool esError = false;

        // Verificar si hay un error de caché
        if (cache[indice] != etiqueta)
        {
            cache[indice] = etiqueta;
            error++;
            esError = true;
        }

        // Mostrar la tabla de resultados
        archivoSalida << setw(12) << instruccion
                      << setw(12) << etiqueta
                      << setw(12) << indice
                      << setw(12) << ((esError) ? "Acierto" : "Error") << '\n';
    }

    archivoSalida << '\n';

    // Mostrar estadísticas de fallos
    archivoSalida << setw(12) << ' '
                  << setw(12) << ' '
                  << setw(12) << "FALLOS:"
                  << setw(12) << error << '\n';
    archivoSalida << setw(12) << ' '
                  << setw(12) << ' '
                  << setw(12) << "POSICIONES:"
                  << setw(12) << bloqCache << '\n';
}

void cache_conjuntos(int bloqCache, int tbloque, int tconjunto) {
    vector<vector<int>> vecConjunto(bloqCache / tconjunto);  // Vector de vectores para simular caché por conjuntos
    int conjuntos = bloqCache / tconjunto;
    int despBloque = log2(tbloque);

    int etiqueta;
    int indiceConjunto;
    int instruccion;
    int error = 0;

    archivoSalida << "\n\n--------------------------------------------------\n";
    archivoSalida << "              CACHE POR CONJUNTOS";
    archivoSalida << "\n--------------------------------------------------\n";


    archivoSalida << setfill('-');
    archivoSalida << setw(12) << "DIRECCION"
                  << setw(12) << "ETIQUETA"
                  << setw(12) << "INDICE"
                  << setw(12) << "RESULTADO" << '\n';
    archivoSalida << setfill(' ');

    // Iterar sobre las instrucciones de memoria
    for (int i = 0; i < memory_direction_size; i++) {
        instruccion = memory_direction[i];
        etiqueta = instruccion >> despBloque;
        indiceConjunto = etiqueta % conjuntos;

        auto it = find((vecConjunto[indiceConjunto]).begin(), (vecConjunto[indiceConjunto]).end(), etiqueta);

        int tipoResultado = 0;

        // Verificar si hay un error en el conjunto
        if (it != vecConjunto[indiceConjunto].end()) {
            vecConjunto[indiceConjunto].erase(it);
            vecConjunto[indiceConjunto].insert(vecConjunto[indiceConjunto].begin(), etiqueta);
        } else if (vecConjunto[indiceConjunto].size() < static_cast<size_t>(tconjunto)) {
            tipoResultado = 1;
            vecConjunto[indiceConjunto].push_back(etiqueta);
            error++;
        } else {
            tipoResultado = 2;
            vecConjunto[indiceConjunto].pop_back();
            vecConjunto[indiceConjunto].insert(vecConjunto[indiceConjunto].begin(), etiqueta);
            error++;
        }

        // Mostrar la tabla de resultados
        archivoSalida << setw(12) << instruccion
                      << setw(12) << etiqueta
                      << setw(12) << indiceConjunto
                      << setw(12) << ((tipoResultado == 0) ? "Acierto" : ((tipoResultado == 1) ? "Error con espacio" : "Error sin espacio")) << '\n';
    }
    archivoSalida << '\n';

    // Mostrar estadísticas de fallos
    archivoSalida << setw(12) << ' '
                  << setw(12) << ' '
                  << setw(12) << "FALLOS:"
                  << setw(12) << error << '\n';
    archivoSalida << setw(12) << ' '
                  << setw(12) << ' '
                  << setw(12) << "CONJUNTOS:"
                  << setw(12) << conjuntos << '\n';
}
