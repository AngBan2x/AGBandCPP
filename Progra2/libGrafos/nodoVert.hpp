#ifndef NODOVERT_H
#define NODOVERT_H

#include "nodoArco.hpp"

template <typename Elemento> class NodoVertice {
private:
  // atributos privados
  Elemento info;
  NodoVertice<Elemento> *sig;
  NodoArco<Elemento> *listaAdy;

public:
  // metodos publicos
  // constructores
  NodoVertice() : listaAdy(nullptr), sig(nullptr){};
  NodoVertice(Elemento info) : info(info), listaAdy(nullptr), sig(nullptr){};
  // getters
  Elemento getInfo() { return info; };
  NodoVertice<Elemento> *getSig() { return sig; };
  NodoArco<Elemento> *getListaAdy() { return listaAdy; };

  // setters
  void setInfo(Elemento e) { info = e; };
  void setSig(NodoVertice<Elemento> *ptr) { sig = ptr; };
  void setListaAdy(NodoArco<Elemento> *lista) { listaAdy = lista; };
};

#endif // !NODOVERT_H
