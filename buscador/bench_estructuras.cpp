// benchmark de estructuras de datos con firma comun:
//   insert(const string&,int) / search->int* / erase->bool / scan->long
// las implementaciones viven en estructuras/, una por fichero
// uso: ./bench_estructuras ch|cl|oa|tr|rx|av|btn|bp|bs
#include "estructura_datos.cpp"
#include <chrono>
#include <vector>
#include <fstream>
using namespace std::chrono;

#include "estructuras/hash_clasico.hpp"
#include "estructuras/flat_hash.hpp"
#include "estructuras/trie.hpp"
#include "estructuras/radix.hpp"
#include "estructuras/avl.hpp"
#include "estructuras/arbol_b.hpp"
#include "estructuras/arbol_b_estrella.hpp"
#include "estructuras/arbol_b_mas.hpp"

long ms(steady_clock::time_point a, steady_clock::time_point b){ return duration_cast<milliseconds>(b-a).count(); }
long rss_kb(){
  std::ifstream f("/proc/self/status"); string l;
  while (getline(f,l)) if (l.compare(0,6,"VmRSS:")==0) return atol(l.c_str()+6);
  return -1;
}

struct CH { // adaptador del close_hash de la practica
  close_hash<int,string> t;
  CH(int n): t(n,f_hash) {}
  bool insert(const string& k,int v){ return t.insert(k,v); }
  int* search(const string& k){ return t.search(k); }
  bool erase(const string& k){ return t.erase(k); }
  long scan(){ return -1; } // sin orden
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

int main(int argc, char** argv){
  const int N = 1000000, CUBETAS = 1<<20, REP = 3;
  string cual = argc>1? argv[1] : "cl";
  int cap = argc>2? atoi(argv[2]) : 0; // capacidad inicial opcional (fuerza crecimientos)
  std::vector<string> claves, ajenas;
  claves.reserve(N); ajenas.reserve(N);
  for (int i=0;i<N;i++){ claves.push_back("palabra" + std::to_string((long)i*7919%N) + "x" + std::to_string(i));
                         ajenas.push_back("ausente" + std::to_string(i)); }
  if      (cual=="oa") mide("flat_h ", new FlatHash(cap? cap : 1<<21), claves, ajenas, N, REP);
  else if (cual=="rx") mide("radix  ", new Radix(),   claves, ajenas, N, REP);
  else if (cual=="tr") mide("trie   ", new Trie(),    claves, ajenas, N, REP);
  else if (cual=="ch") mide("closeH ", new CH(cap? cap : CUBETAS), claves, ajenas, N, REP);
  else if (cual=="cl") mide("clasico", new ClassicHash(cap? cap : CUBETAS), claves, ajenas, N, REP);
  else if (cual=="av") mide("avl    ", new AVL(),   claves, ajenas, N, REP);
  else if (cual=="bt" or cual=="btn"){ BTree* b=new BTree(); mide("arbolB ", b, claves, ajenas, N, REP); fprintf(stderr,"nodos B : %ld\n", b->Nodos()); }
  else if (cual=="bs") { BStar* b=new BStar(); mide("arbolB*", b, claves, ajenas, N, REP); fprintf(stderr,"nodos B*: %ld\n", b->Nodos()); }
  else if (cual=="bp") { BPlus* b=new BPlus(); mide("arbolB+", b, claves, ajenas, N, REP); fprintf(stderr,"nodos B+: %ld\n", b->Nodos()); }
}
