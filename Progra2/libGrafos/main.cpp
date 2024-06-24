#include "grafo.hpp"
#include "nodoVert.hpp"
#include <ios>
#include <iostream>

template <typename E> void imprimirVertices(Grafo<E> g) {
  NodoVertice<E> *itV{g.getPrimero()};

  while (itV != nullptr) {
    std::cout << itV->getInfo() << ' ';
    itV = itV->getSig();
  }
  std::cout << std::endl;
}

int main() {
  Grafo<int> g;

  g.agregarVertice(7);
  g.agregarVertice(6);
  g.agregarVertice(5);
  g.agregarVertice(4);
  g.agregarVertice(3);
  g.agregarVertice(2);
  g.agregarVertice(1);

  g.agregarArco(2, 3, 1);
  g.agregarArco(2, 5, 1);

  imprimirVertices(g);

  g.eliminarVertice(3);

  imprimirVertices(g);

  g.eliminarArco(2, 5);

  return 0;
}
