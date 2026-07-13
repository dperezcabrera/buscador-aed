// benchmark: familia B (B, B+, B*) contra hash clasico y AVL
// firma comun: insert(const string&,int) / search->int* / erase->bool
// fase scan: recorrido de todas las claves en orden (arboles)
#include "estructura_datos.cpp"
#include <chrono>
#include <vector>
#include <fstream>
using namespace std::chrono;

long ms(steady_clock::time_point a, steady_clock::time_point b){ return duration_cast<milliseconds>(b-a).count(); }
long rss_kb(){
  std::ifstream f("/proc/self/status"); string l;
  while (getline(f,l)) if (l.compare(0,6,"VmRSS:")==0) return atol(l.c_str()+6);
  return -1;
}

const int G = 32;

// ---- hash clasico (ancla de comparacion) ----
struct NodoC { string clave; int valor; NodoC* sig; };
struct ClassicHash {
  NodoC** cubetas; int tama;
  ClassicHash(int n): tama(n) { cubetas = new NodoC*[n](); }
  int idx(const string& k){ int h=f_hash(k)%tama; return h<0? h+tama:h; }
  bool insert(const string& k,int v){
    int h=idx(k);
    for(NodoC*p=cubetas[h];p;p=p->sig) if(p->clave==k) return false;
    cubetas[h]=new NodoC{k,v,cubetas[h]}; return true;
  }
  int* search(const string& k){
    for(NodoC*p=cubetas[idx(k)];p;p=p->sig) if(p->clave==k) return &p->valor;
    return NULL;
  }
  bool erase(const string& k){
    int h=idx(k);
    for(NodoC**pp=&cubetas[h];*pp;pp=&(*pp)->sig)
      if((*pp)->clave==k){ NodoC*m=*pp; *pp=m->sig; delete m; return true; }
    return false;
  }
  long scan(){ return -1; } // sin orden
};

// ---- AVL ----
struct NodoA { string clave; int valor; NodoA *izq, *der; int alt; bool muerto; };
struct AVL {
  NodoA* raiz = NULL;
  bool insertado;
  static int alt(NodoA* n){ return n? n->alt : 0; }
  static void act(NodoA* n){ n->alt = 1 + std::max(alt(n->izq), alt(n->der)); }
  static NodoA* rotd(NodoA* y){ NodoA* x=y->izq; y->izq=x->der; x->der=y; act(y); act(x); return x; }
  static NodoA* roti(NodoA* x){ NodoA* y=x->der; x->der=y->izq; y->izq=x; act(x); act(y); return y; }
  static NodoA* balancea(NodoA* n){
    act(n);
    int b = alt(n->izq) - alt(n->der);
    if (b >  1){ if (alt(n->izq->izq) < alt(n->izq->der)) n->izq = roti(n->izq); return rotd(n); }
    if (b < -1){ if (alt(n->der->der) < alt(n->der->izq)) n->der = rotd(n->der); return roti(n); }
    return n;
  }
  NodoA* inserta(NodoA* n, const string& k, int v){
    if (!n){ insertado=true; return new NodoA{k,v,NULL,NULL,1,false}; }
    if      (k < n->clave) n->izq = inserta(n->izq,k,v);
    else if (n->clave < k) n->der = inserta(n->der,k,v);
    else { insertado=false; return n; }
    return balancea(n);
  }
  bool insert(const string& k,int v){ raiz = inserta(raiz,k,v); return insertado; }
  int* search(const string& k){
    for(NodoA* n=raiz; n; ){
      if      (k < n->clave) n=n->izq;
      else if (n->clave < k) n=n->der;
      else return n->muerto? NULL : &n->valor;
    }
    return NULL;
  }
  bool erase(const string& k){ // marcado
    for(NodoA* n=raiz; n; ){
      if      (k < n->clave) n=n->izq;
      else if (n->clave < k) n=n->der;
      else { if (n->muerto) return false; n->muerto=true; return true; }
    }
    return false;
  }
  long acc;
  void inorden(NodoA* n){ if(!n) return; inorden(n->izq); if(!n->muerto) acc+=n->valor; inorden(n->der); }
  long scan(){ acc=0; inorden(raiz); return acc; }
};

// ---- arbol B puro (CLRS, particion preventiva en el descenso) ----
struct NodoB {
  int n; bool hoja;
  string claves[2*G-1]; int val[2*G-1]; bool muerto[2*G-1];
  NodoB* hijo[2*G];
  NodoB(): n(0), hoja(true) { for(int i=0;i<2*G;i++) hijo[i]=NULL; }
};
struct BTree {
  NodoB* raiz = new NodoB();
  long nodos = 1;
  int pos(NodoB* x, const string& k){ return std::lower_bound(x->claves, x->claves+x->n, k) - x->claves; }
  void split(NodoB* p, int i){
    NodoB *y=p->hijo[i], *z=new NodoB(); nodos++;
    z->hoja=y->hoja; z->n=G-1;
    for(int j=0;j<G-1;j++){ z->claves[j]=std::move(y->claves[j+G]); z->val[j]=y->val[j+G]; z->muerto[j]=y->muerto[j+G]; }
    if(!y->hoja) for(int j=0;j<G;j++) z->hijo[j]=y->hijo[j+G];
    y->n=G-1;
    for(int j=p->n;j>i;j--) p->hijo[j+1]=p->hijo[j];
    for(int j=p->n-1;j>=i;j--){ p->claves[j+1]=std::move(p->claves[j]); p->val[j+1]=p->val[j]; p->muerto[j+1]=p->muerto[j]; }
    p->hijo[i+1]=z;
    p->claves[i]=std::move(y->claves[G-1]); p->val[i]=y->val[G-1]; p->muerto[i]=y->muerto[G-1];
    p->n++;
  }
  virtual bool lleno_gestionado(NodoB* x, int i){ // en B puro: partir siempre
    split(x,i); return true;
  }
  bool insert(const string& k, int v){
    if (raiz->n == 2*G-1){ NodoB* s=new NodoB(); nodos++; s->hoja=false; s->hijo[0]=raiz; raiz=s; split(s,0); }
    NodoB* x=raiz;
    while(true){
      int i=pos(x,k);
      if (i<x->n and x->claves[i]==k) return false;
      if (x->hoja){
        for(int j=x->n-1;j>=i;j--){ x->claves[j+1]=std::move(x->claves[j]); x->val[j+1]=x->val[j]; x->muerto[j+1]=x->muerto[j]; }
        x->claves[i]=k; x->val[i]=v; x->muerto[i]=false; x->n++; return true;
      }
      if (x->hijo[i]->n == 2*G-1){ lleno_gestionado(x,i); continue; }
      x=x->hijo[i];
    }
  }
  int* search(const string& k){
    NodoB* x=raiz;
    while(true){
      int i=pos(x,k);
      if (i<x->n and x->claves[i]==k) return x->muerto[i]? NULL : &x->val[i];
      if (x->hoja) return NULL;
      x=x->hijo[i];
    }
  }
  bool erase(const string& k){ // marcado
    NodoB* x=raiz;
    while(true){
      int i=pos(x,k);
      if (i<x->n and x->claves[i]==k){ if (x->muerto[i]) return false; x->muerto[i]=true; return true; }
      if (x->hoja) return false;
      x=x->hijo[i];
    }
  }
  long acc;
  void inorden(NodoB* x){
    for(int i=0;i<x->n;i++){
      if(!x->hoja) inorden(x->hijo[i]);
      if(!x->muerto[i]) acc+=x->val[i];
    }
    if(!x->hoja) inorden(x->hijo[x->n]);
  }
  long scan(){ acc=0; inorden(raiz); return acc; }
};

// ---- arbol B*: redistribuye con los hermanos antes de partir ----
// (split 2->3 canonico omitido: la redistribucion ya logra el
//  relleno alto; con ambos hermanos llenos parte 1->2 como B)
struct BStar : BTree {
  void rota_izq(NodoB* x, int i){ // pasa 1 clave del hijo i al hermano i-1
    NodoB *izq=x->hijo[i-1], *c=x->hijo[i];
    izq->claves[izq->n]=std::move(x->claves[i-1]); izq->val[izq->n]=x->val[i-1]; izq->muerto[izq->n]=x->muerto[i-1];
    if(!izq->hoja) izq->hijo[izq->n+1]=c->hijo[0];
    izq->n++;
    x->claves[i-1]=std::move(c->claves[0]); x->val[i-1]=c->val[0]; x->muerto[i-1]=c->muerto[0];
    for(int j=0;j<c->n-1;j++){ c->claves[j]=std::move(c->claves[j+1]); c->val[j]=c->val[j+1]; c->muerto[j]=c->muerto[j+1]; }
    if(!c->hoja) for(int j=0;j<c->n;j++) c->hijo[j]=c->hijo[j+1];
    c->n--;
  }
  void rota_der(NodoB* x, int i){ // pasa 1 clave del hijo i al hermano i+1
    NodoB *der=x->hijo[i+1], *c=x->hijo[i];
    for(int j=der->n-1;j>=0;j--){ der->claves[j+1]=std::move(der->claves[j]); der->val[j+1]=der->val[j]; der->muerto[j+1]=der->muerto[j]; }
    if(!der->hoja) for(int j=der->n;j>=0;j--) der->hijo[j+1]=der->hijo[j];
    der->claves[0]=std::move(x->claves[i]); der->val[0]=x->val[i]; der->muerto[0]=x->muerto[i];
    if(!der->hoja) der->hijo[0]=c->hijo[c->n];
    der->n++;
    x->claves[i]=std::move(c->claves[c->n-1]); x->val[i]=c->val[c->n-1]; x->muerto[i]=c->muerto[c->n-1];
    c->n--;
  }
  bool lleno_gestionado(NodoB* x, int i){
    if (i>0      and x->hijo[i-1]->n < 2*G-2){ rota_izq(x,i); return true; }
    if (i<x->n   and x->hijo[i+1]->n < 2*G-2){ rota_der(x,i); return true; }
    split(x,i); return true;
  }
};

// ---- arbol B+: valores solo en hojas, hojas enlazadas ----
struct BPlus {
  struct N {
    bool hoja; int n;
    string k[2*G];
    int v[2*G]; bool m[2*G]; N* sig; // hojas
    N* h[2*G+1];                     // internos
    N(bool ho): hoja(ho), n(0), sig(NULL) { for(int i=0;i<2*G+1;i++) h[i]=NULL; }
  };
  N* raiz = new N(true);
  long nodos = 1;
  bool dup;
  N* inserta(N* x, const string& clave, int valor, string& sep){
    if (x->hoja){
      int i = std::lower_bound(x->k, x->k+x->n, clave) - x->k;
      if (i<x->n and x->k[i]==clave){ dup=true; return NULL; }
      for(int j=x->n-1;j>=i;j--){ x->k[j+1]=std::move(x->k[j]); x->v[j+1]=x->v[j]; x->m[j+1]=x->m[j]; }
      x->k[i]=clave; x->v[i]=valor; x->m[i]=false; x->n++;
      if (x->n < 2*G) return NULL;
      N* d = new N(true); nodos++;
      for(int j=0;j<G;j++){ d->k[j]=std::move(x->k[j+G]); d->v[j]=x->v[j+G]; d->m[j]=x->m[j+G]; }
      d->n=G; x->n=G;
      d->sig=x->sig; x->sig=d;
      sep = d->k[0]; // el separador se COPIA arriba
      return d;
    }
    int i = std::upper_bound(x->k, x->k+x->n, clave) - x->k;
    string s;
    N* nuevo = inserta(x->h[i], clave, valor, s);
    if (!nuevo) return NULL;
    for(int j=x->n-1;j>=i;j--) x->k[j+1]=std::move(x->k[j]);
    for(int j=x->n;j>i;j--)    x->h[j+1]=x->h[j];
    x->k[i]=std::move(s); x->h[i+1]=nuevo; x->n++;
    if (x->n < 2*G) return NULL;
    N* d = new N(false); nodos++;
    for(int j=0;j<G-1;j++) d->k[j]=std::move(x->k[j+G+1]);
    for(int j=0;j<G;j++)   d->h[j]=x->h[j+G+1];
    d->n=G-1;
    sep = std::move(x->k[G]); // el separador interno SUBE
    x->n=G;
    return d;
  }
  bool insert(const string& clave,int valor){
    dup=false; string s;
    N* nuevo = inserta(raiz, clave, valor, s);
    if (nuevo){
      N* r=new N(false); nodos++;
      r->h[0]=raiz; r->h[1]=nuevo; r->k[0]=std::move(s); r->n=1;
      raiz=r;
    }
    return not dup;
  }
  N* hoja_de(const string& clave){
    N* x=raiz;
    while(!x->hoja){ int i=std::upper_bound(x->k,x->k+x->n,clave)-x->k; x=x->h[i]; }
    return x;
  }
  int* search(const string& clave){
    N* x=hoja_de(clave);
    int i=std::lower_bound(x->k,x->k+x->n,clave)-x->k;
    return (i<x->n and x->k[i]==clave and not x->m[i])? &x->v[i] : NULL;
  }
  bool erase(const string& clave){ // marcado
    N* x=hoja_de(clave);
    int i=std::lower_bound(x->k,x->k+x->n,clave)-x->k;
    if (i<x->n and x->k[i]==clave and not x->m[i]){ x->m[i]=true; return true; }
    return false;
  }
  long scan(){ // recorrido secuencial por las hojas enlazadas
    long acc=0;
    N* x=raiz;
    while(!x->hoja) x=x->h[0];
    for( ; x ; x=x->sig)
      for(int i=0;i<x->n;i++) if(!x->m[i]) acc+=x->v[i];
    return acc;
  }
};

template<class Tabla>
void mide(const char* nombre, Tabla* t, std::vector<string>& claves, std::vector<string>& ajenas, int N, int REP){
  long r0=rss_kb();
  auto t0=steady_clock::now();
  for(int i=0;i<N;i++) t->insert(claves[i], i);
  auto t1=steady_clock::now(); long tins=ms(t0,t1); long r1=rss_kb();
  long acum=0; int fallos=0, borrados=0;
  t0=steady_clock::now();
  for(int r=0;r<REP;r++) for(int i=0;i<N;i++) acum += *t->search(claves[i]);
  t1=steady_clock::now(); long thit=ms(t0,t1);
  t0=steady_clock::now();
  for(int r=0;r<REP;r++) for(int i=0;i<N;i++) fallos += (t->search(ajenas[i])==NULL);
  t1=steady_clock::now(); long tmiss=ms(t0,t1);
  long sacc=0;
  t0=steady_clock::now();
  for(int r=0;r<REP;r++) sacc = t->scan();
  t1=steady_clock::now(); long tscan=ms(t0,t1);
  t0=steady_clock::now();
  for(int i=0;i<N;i++) borrados += t->erase(claves[i]);
  t1=steady_clock::now(); long tera=ms(t0,t1);
  printf("%s mem:%ld_KB ins:%ld hit:%ld miss:%ld scan:%ld era:%ld chk:%ld/%d/%ld/%d\n",
         nombre, r1-r0, tins, thit, tmiss, tscan, tera, acum, fallos, sacc, borrados);
}

struct CH { // adaptador del close_hash de la practica
  close_hash<int,string> t;
  CH(int n): t(n,f_hash) {}
  bool insert(const string& k,int v){ return t.insert(k,v); }
  int* search(const string& k){ return t.search(k); }
  bool erase(const string& k){ return t.erase(k); }
  long scan(){ return -1; } // sin orden
};

// ---- direccionamiento abierto: sondeo lineal, hash cacheado ----
// el "hash cerrado" canonico: todo en arrays planos, cero punteros
struct OpenAddr {
  int tama; // potencia de 2
  string* claves; int* val; unsigned* hcache; char* estado; // 0 vacio, 1 ocupado, 2 tumba
  OpenAddr(int t): tama(t){
    claves=new string[t]; val=new int[t]; hcache=new unsigned[t]; estado=new char[t]();
  }
  bool insert(const string& k,int v){
    unsigned hh=(unsigned)f_hash(k);
    int tumba=-1;
    for(unsigned i=hh&(tama-1);;i=(i+1)&(tama-1)){
      if (estado[i]==0){
        int d = tumba>=0? tumba : (int)i;
        claves[d]=k; val[d]=v; hcache[d]=hh; estado[d]=1;
        return true;
      }
      if (estado[i]==2){ if (tumba<0) tumba=(int)i; }
      else if (hcache[i]==hh and claves[i]==k) return false;
    }
  }
  int* search(const string& k){
    unsigned hh=(unsigned)f_hash(k);
    for(unsigned i=hh&(tama-1);;i=(i+1)&(tama-1)){
      if (estado[i]==0) return NULL;
      if (estado[i]==1 and hcache[i]==hh and claves[i]==k) return &val[i];
    }
  }
  bool erase(const string& k){ // tumba: el borrado canonico en abierto
    unsigned hh=(unsigned)f_hash(k);
    for(unsigned i=hh&(tama-1);;i=(i+1)&(tama-1)){
      if (estado[i]==0) return false;
      if (estado[i]==1 and hcache[i]==hh and claves[i]==k){
        estado[i]=2; claves[i]=string(); return true;
      }
    }
  }
  long scan(){ return -1; } // sin orden
};

// ---- radix trie (Patricia): trie con caminos comprimidos ----
struct NodoR { string trozo; bool tiene; int valor; NodoR *hijo, *hermano; };
struct Radix {
  NodoR* raiz = new NodoR{"", false, 0, NULL, NULL};
  bool insert(const string& k, int v){
    NodoR* p = raiz;
    unsigned i = 0;
    while (true){
      if (i == k.size()){
        if (p->tiene) return false;
        p->tiene = true; p->valor = v; return true;
      }
      NodoR* q = p->hijo;
      while (q and q->trozo[0] != k[i]) q = q->hermano;
      if (!q){
        p->hijo = new NodoR{k.substr(i), true, v, NULL, p->hijo};
        return true;
      }
      unsigned j = 0;
      while (j < q->trozo.size() and i+j < k.size() and q->trozo[j] == k[i+j]) j++;
      if (j < q->trozo.size()){ // divergencia a mitad de arista: partir
        q->hijo = new NodoR{q->trozo.substr(j), q->tiene, q->valor, q->hijo, NULL};
        q->trozo.resize(j);
        q->tiene = false;
      }
      p = q; i += j;
    }
  }
  NodoR* baja(const string& k){
    NodoR* p = raiz; unsigned i = 0;
    while (true){
      if (i == k.size()) return p;
      NodoR* q = p->hijo;
      while (q and q->trozo[0] != k[i]) q = q->hermano;
      if (!q or k.compare(i, q->trozo.size(), q->trozo) != 0) return NULL;
      i += q->trozo.size(); p = q;
    }
  }
  int* search(const string& k){
    NodoR* p = baja(k);
    return (p and p->tiene)? &p->valor : NULL;
  }
  bool erase(const string& k){ // marcado, sin fusionar aristas
    NodoR* p = baja(k);
    if (p and p->tiene){ p->tiene=false; return true; }
    return false;
  }
  long scan(){ return -1; } // hermanos sin ordenar
};

// ---- bloom filter delante del hash clasico ----
// no compite: complementa — mata los miss sin tocar la tabla
struct BloomHash {
  ClassicHash t;
  unsigned long* bits;
  static const unsigned M = 1u<<24; // 16M bits = 2 MB, 16 bits/clave, 8 sondas
  BloomHash(int cub): t(cub) { bits = new unsigned long[M/64](); }
  static unsigned fnv(const string& k){
    unsigned x=2166136261u;
    for(unsigned i=0;i<k.size();i++){ x^=(unsigned char)k[i]; x*=16777619u; }
    return x;
  }
  void marca(const string& k){
    unsigned h1=(unsigned)f_hash(k), h2=fnv(k)|1;
    for(int i=0;i<8;i++){ unsigned b=(h1+i*h2)&(M-1); bits[b>>6] |= 1ul<<(b&63); }
  }
  bool quiza(const string& k){
    unsigned h1=(unsigned)f_hash(k), h2=fnv(k)|1;
    for(int i=0;i<8;i++){ unsigned b=(h1+i*h2)&(M-1); if(!(bits[b>>6]>>(b&63)&1)) return false; }
    return true;
  }
  bool insert(const string& k,int v){ marca(k); return t.insert(k,v); }
  int* search(const string& k){ return quiza(k)? t.search(k) : NULL; }
  bool erase(const string& k){ return t.erase(k); } // el filtro queda sucio: correcto
  long scan(){ return -1; }
};

int main(int argc, char** argv){
  const int N = 1000000, CUBETAS = 1<<20, REP = 3;
  string cual = argc>1? argv[1] : "cl";
  std::vector<string> claves, ajenas;
  claves.reserve(N); ajenas.reserve(N);
  for (int i=0;i<N;i++){ claves.push_back("palabra" + std::to_string((long)i*7919%N) + "x" + std::to_string(i));
                         ajenas.push_back("ausente" + std::to_string(i)); }
  if      (cual=="oa") mide("abierto", new OpenAddr(1<<21), claves, ajenas, N, REP);
  else if (cual=="rx") mide("radix  ", new Radix(),   claves, ajenas, N, REP);
  else if (cual=="bl") mide("bloom+h", new BloomHash(CUBETAS), claves, ajenas, N, REP);
  else if (cual=="ch") mide("closeH ", new CH(CUBETAS), claves, ajenas, N, REP);
  else if (cual=="cl") mide("clasico", new ClassicHash(CUBETAS), claves, ajenas, N, REP);
  else if (cual=="av") mide("avl    ", new AVL(),   claves, ajenas, N, REP);
  else if (cual=="bt") mide("arbolB ", new BTree(), claves, ajenas, N, REP);
  else if (cual=="bs") { BStar* b=new BStar(); mide("arbolB*", b, claves, ajenas, N, REP); fprintf(stderr,"nodos B*: %ld\n", b->nodos); }
  else if (cual=="bp") { BPlus* b=new BPlus(); mide("arbolB+", b, claves, ajenas, N, REP); fprintf(stderr,"nodos B+: %ld\n", b->nodos); }
  else if (cual=="btn"){ BTree* b=new BTree(); mide("arbolB ", b, claves, ajenas, N, REP); fprintf(stderr,"nodos B : %ld\n", b->nodos); }
}
