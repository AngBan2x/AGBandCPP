#ifndef NODOARCO_H
#define NODOARCO_H
template <typename Elemento> class NodoVertice;
template <typename Elemento> class NodoArco {
private:
  double peso;
  NodoVertice<Elemento> *info;
  NodoArco<Elemento> *sig;

public:
  // metodos publicos
  // constructores
  NodoArco() : info(nullptr), sig(nullptr){};
  NodoArco(NodoVertice<Elemento> *p, double n)
      : info(p), sig(nullptr), peso(n){};

  // getters
  double getPeso() { return peso; };
  NodoVertice<Elemento> *getInfo() { return info; };
  NodoArco<Elemento> *getSig() { return sig; };

  // setters
  void setPeso(double p) { peso = p; };
  void setInfo(NodoVertice<Elemento> *destino) { info = destino; };
  void setSig(NodoArco<Elemento> *siguiente) { sig = siguiente; };
};
#endif // !NODOARCO_H
