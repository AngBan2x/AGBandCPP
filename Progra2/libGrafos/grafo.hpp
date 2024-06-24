// definicion de clase grafo
#ifndef GRAFO_H
#define GRAFO_H

#include "nodoArco.hpp"
#include "nodoVert.hpp"
#include <ios>
#include <iostream>
#include <list>

template <typename Elemento> class Grafo {
private:
  NodoVertice<Elemento> *primero;
  int nVertices, nArcos;

public:
  // constructores del objeto
  Grafo() : primero(nullptr), nVertices(0), nArcos(0){};
  Grafo(NodoVertice<Elemento> *pri) : primero(pri), nVertices(0), nArcos(0){};
  void construir(Grafo<Elemento> g); // TODO: constructor copia

  // getters de la clase
  NodoVertice<Elemento> *getPrimero() { return primero; };
  int getNVertices() { return nVertices; };
  int getNArcos() { return nArcos; };

  // setters
  void setPrimero(NodoVertice<Elemento> *p) { this->primero = p; };

  // getters del objeto
  bool esVacio(void) { return primero == nullptr; }; // vacio = 0 nodos
  bool esNulo(void) { return nArcos == 0; }; // nulo = >1 nodo sin arco alguno
  bool esTrivial(void);                      // trivial = grafo con un solo nodo
  bool existeArco(Elemento v, Elemento w);
  double getPesoArco(Elemento v, Elemento w);
  std::list<Elemento> sucesores(Elemento vertice);
  std::list<Elemento> predecesores(Elemento vertice);
  // std::list<Elemento> getVecinos(Elemento v); // para grafos no dirigidos

  // constructores al objeto
  void agregarVertice(Elemento v);
  void agregarArco(Elemento v, Elemento w, double pesArco);

  // setters al objeto
  void setPesoArco(Elemento v, Elemento w, double pesoNuevo);

  // destructores al objeto
  void eliminarArco(Elemento v, Elemento w);
  void eliminarVertice(Elemento v);

  // destructor del objeto
  void destruir(void); // TODO: implementar
};

template <typename E> bool Grafo<E>::esTrivial(void) {
  if (this->esVacio()) {
    return false;
  } else {
    return this->primero->getSig() == nullptr;
  }
}

template <typename E> void Grafo<E>::agregarVertice(E v) {
  NodoVertice<E> *nuevo = new NodoVertice<E>(v);

  if (this->primero == nullptr) {
    this->primero = nuevo;
    nuevo->setSig(nullptr);
  } else {
    nuevo->setSig(this->primero);
    this->primero = nuevo;
  }
  nuevo->setListaAdy(nullptr);
  this->nVertices += 1;
}

template <typename E> bool Grafo<E>::existeArco(E v, E w) {
  NodoVertice<E> *vAct = this->primero; // apuntar al 1er vertice
  NodoArco<E> *arcoAct;

  while (vAct != nullptr) {
    arcoAct = vAct->getListaAdy();                // apuntar al 1er arco
    if (vAct->getInfo() == v) {                   // si el vertice actual es v
      while (arcoAct != nullptr) {                // mientras no sea nulo
        if (arcoAct->getInfo()->getInfo() == w) { // si el arco va a w
          return true;                            // existe arco!!!
        }
        arcoAct = arcoAct->getSig(); // apuntar al sig arco del vertice
      }
    }
    vAct = vAct->getSig(); // apuntar al sig vertice
  }
  return false; // no existe arco :(
}

template <typename E> double Grafo<E>::getPesoArco(E v, E w) {
  NodoVertice<E> *vAct = this->primero;
  NodoArco<E> *arcoAct;

  while (vAct != nullptr) {
    arcoAct = vAct->getListaAdy();

    if (vAct->getInfo() == v) {
      while (arcoAct != nullptr) {
        if (arcoAct->getInfo()->getInfo() == w) {
          return arcoAct->getPeso();
        }
        arcoAct = arcoAct->getSig();
      }
    }

    vAct = vAct->getSig();
  }
  return -1; // como no existe arco, no existe peso :3
}

template <typename E> void Grafo<E>::agregarArco(E v, E w, double pesoArco) {
  NodoVertice<E> *itV{this->primero}; // iterador para recorrer los vertices
  NodoVertice<E> *foundV{}, *foundW{};

  NodoArco<E> *nuevo = new NodoArco<E>();
  nuevo->setPeso(pesoArco);
  nuevo->setSig(nullptr);

  if (v != w) { // no puedes agregar un arco hacia el mismo vertice

    while (itV != nullptr) {
      if (itV->getInfo() == v) {
        foundV = itV; // se encontro v
      } else if (itV->getInfo() == w) {
        foundW = itV; // se encontro w
      }
      itV = itV->getSig();
    }

    if (foundV != nullptr && foundW != nullptr) { // si se encontraron v y w

      if (!this->existeArco(v, w)) {          // si no existe arco entre v y w
        nuevo->setInfo(foundW);               // apuntar arco a w
        nuevo->setSig(foundV->getListaAdy()); // el primer arco de v = nuevo
        foundV->setListaAdy(nuevo);
        this->nArcos += 1;
      }

    } else if (foundV == nullptr && foundW != nullptr) { // si no esta v
      this->agregarVertice(v);
      this->primero->setListaAdy(nuevo);
      this->nArcos += 1;

    } else if (foundV != nullptr && foundW == nullptr) { // si no esta w
      this->agregarVertice(w);
      this->primero->setListaAdy(nuevo);
      this->nArcos += 1;

    } else { // si no esta ninguno
      this->agregarVertice(v);
      foundV = this->primero;
      this->agregarVertice(w);
      foundW = this->primero;
      foundV->setListaAdy(nuevo);
      nuevo->setInfo(foundW);
      this->nArcos += 1;
    }
  }
}

template <typename E> void Grafo<E>::setPesoArco(E v, E w, double pesoNuevo) {
  NodoVertice<E> *itV = this->primero; // iterador para recorrer los vertices
  NodoArco<E> *itA;                    // iterador para recorrer los arcos
  NodoVertice<E> *foundV{nullptr};     // para guardar V, de haberlo encontrado
  NodoArco<E> *foundW{nullptr};        // para guardar W, de haberlo encontrado

  while (itV != nullptr && foundV == nullptr) {
    if (itV->getInfo() == v)
      foundV = itV; // se encontro V !!!!!!!

    itV = itV->getSig();
  }

  if (foundV != nullptr) { // si se encontro el vertice V
    itA = foundV->getListaAdy();

    while (itA != nullptr && foundW == nullptr) {
      if (itA->getInfo()->getInfo() == w)
        foundW = itA; // se encontro W !!!!!!!!!

      itA = itA->getSig();
    }
    if (foundW != nullptr) { // si se encontro el arco de V a W
      foundW->setPeso(pesoNuevo);
    }
  }
}

template <typename E> void Grafo<E>::eliminarArco(E v, E w) {
  NodoVertice<E> *itV{this->primero}; // iterador para recorrer los vertices
  NodoArco<E> *itA;    // iterador para recorrer los arcos del vertice
  NodoArco<E> *itAntA; // iterador que va antes de itA
  bool foundV{false};  // bandera para indicar si se encontro V
  bool foundW{false};  // bandera para indicar si se encontro W
  NodoArco<E> *arcAux; // arco auxiliar

  while (itV != nullptr && !foundV) {
    if (itV->getInfo() == v) {
      foundV = true; // se encontro V!!!!!!!
    } else {
      itV = itV->getSig();
    }
  }

  if (foundV) { // si se encontro V
    itA = itV->getListaAdy();

    while (itA != nullptr && !foundW) {
      if (itA->getInfo()->getInfo() == w) {
        foundW = true; // se encontro W!!!!!!!!
        arcAux = itA;
      } else {
        itAntA = itA;
        itA = itA->getSig();
      }
    }

    if (foundW) { // si se encontro W
      itA->setSig(arcAux->getSig());
      delete arcAux;
      this->nArcos -= 1;
      // std::cout << "arcos decrementados\n";
    }
  }
}

template <typename E> std::list<E> Grafo<E>::sucesores(E v) {
  std::list<E> result;                // lista resultante a retornar
  NodoVertice<E> *itV{this->primero}; // iterador para recorrer los vertices
  NodoArco<E> *itA;                   // iterador para recorrer los arcos
  bool foundV{false};                 // bandera para indicar si se encontro V

  while (itV != nullptr && !foundV) {
    if (itV->getInfo() == v) {
      foundV = true; // se encontro V!!!!!!!
    }
    itV = itV->getSig();
  }

  if (foundV) { // si se encontro V
    itA = itV->getListaAdy();

    // recorrer los arcos de V
    while (itA != nullptr) {
      result.push_back(itA->getInfo()->getInfo()); // agregar el elemento
      itA = itA->getSig();
    }
  }

  return result;
}

template <typename E> std::list<E> Grafo<E>::predecesores(E v) {
  std::list<E> result;                // lista resultante a retornar
  NodoVertice<E> *itV{this->primero}; // iterador para recorrer los vertices
  NodoArco<E> *itA;                   // iterador para recorrer los arcos

  while (itV != nullptr) {
    if (itV->getInfo() != v) { // si el vertice NO es V
      itA = itV->getListaAdy();

      while (itA != nullptr) {
        if (itA->getInfo()->getInfo() == v) {          // si el arco va a V
          result.push_back(itA->getInfo()->getInfo()); // encolar a la lista
        }
        itA = itA->getSig();
      }
    }

    itV = itV->getSig();
  }

  return result;
}

template <typename E> void Grafo<E>::eliminarVertice(E v) {
  NodoVertice<E> *itV{this->primero}; // iterador para recorrer los vertices
  NodoVertice<E> *itAntV;             // iterador que esta detras de itV
  NodoVertice<E> *auxV;    // auxiliar para guardar (y luego eliminar) V
  NodoVertice<E> *auxAntV; // auxiliar: el vertice anterior a V
  bool foundV{false};      // bandera para indicar que se encontro V
  NodoArco<E> *auxArcV; // auxiliar para guardar (y luego eliminar) el arco de V

  if (itV->getInfo() == v) { // si V es el primer vertice
    auxV = itV;
    auxArcV = auxV->getListaAdy();

    while (auxArcV != nullptr) { // eliminar los arcos de V
      auxV->setListaAdy(
          auxArcV->getSig()); // apuntar el primer arco al siguiente
      delete auxArcV;         // eliminar el arco guardado
      this->nArcos -= 1;      // decrementar el numero de arcos
      // auxArcV = nullptr; // apuntar a null despues de eliminar (quizas
      // innecesario)
      auxArcV = auxV->getListaAdy(); // almacenar el siguiente arco
    }

    this->primero = auxV->getSig(); // asignar el primer vertice al siguiente
    delete auxV;                    // finalmente eliminar siguiente
    this->nVertices -= 1;           // decrementar la cantidad de vertices

  } else {

    itAntV = itV;
    itV = itV->getSig();

    while (itV != nullptr) {
      if (itV->getInfo() == v) {
        foundV = true; // se encontro V!!!!!!!!
        auxAntV = itAntV;
        auxV = itV;
      } else {
        if (existeArco(itV->getInfo(), v)) {
          eliminarArco(itV->getInfo(), v);
        }
      }
      itAntV = itV;
      itV = itV->getSig();
    }

    if (foundV) { // si se encontro, entonces eliminar sus arcos y luego el vert
      auxArcV = auxV->getListaAdy();

      while (auxArcV != nullptr) { // eliminar los arcos de V
        auxV->setListaAdy(auxArcV->getSig());
        delete auxArcV;
        this->nArcos -= 1;
        // auxArcV = nullptr; //  (quizas innecesario)
        auxArcV = auxV->getListaAdy();
      }

      auxAntV->setSig(auxV->getSig());
      delete auxV;
      this->nVertices -= 1;
    }
  }
}

#endif //! GRAFO_H
