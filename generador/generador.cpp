//////////////////////////////////////////////////////////
// TIPO DE DATOS: Generador
//////////////////////////////////////////////////////////

#include "generador.hpp"

#define _TAMANO_BUFFER      300
#define _PAGINAS_POR_DIR    100
#define _PALABRAS_POR_LINEA 14

////////////////////
// IMPLEMENTACION //
////////////////////

int numDominios= 10;

char const *dominios[]= {"es", "com", "org", "net",
      "uk", "fr", "it", "pt", "gob", "mil"};

using namespace std;

/////////////////////////////////////////////////////////

int tolower2 (int v)
{
  if (isalpha(v)) return tolower(v);
  else switch(v) {
    case 'Á': case 'á': return 'a';
    case 'É': case 'é': return 'e';
    case 'Í': case 'í': return 'i';
    case 'Ó': case 'ó': return 'o';
    case 'Ú': case 'ú': return 'u';
    case 'Ñ': return 'ñ';
    default: return 0;
  }
}

bool comparaStr(char *cad1, char *cad2)
{
  int i;
  for (i= 0; cad1[i] && cad2[i]; i++)
    if (tolower2(cad1[i])!=tolower2(cad2[i]))
      return false;
  return tolower2(cad1[i])==tolower2(cad2[i]);
}


/////////////////////////////////////////////////////////


Generador::Generador (char *fichBase, char const *subdirec, int npMin, int npMax)
/* Inicializa un generador de paginas web, a partir de un fichero base de
   palabras cuyo nombre es pasado como parametro. */
{
  FILE *f;
                             //1. Abrir el fichero base de palabras
  f= fopen(fichBase, "r");
  assert(f!=NULL);           //Precondicion: el fichero debe existir
  fseek(f, 0, SEEK_END);
  numLetras= ftell(f);
  fseek(f, 0, SEEK_SET);
  letras= new char[numLetras+1];
  palabras= new Bolsa<char *>;
  distintas= new Conjunto<char *>(&comparaStr);
                             //2. Leer palabras y meterlas en la bolsa
  bool ultEspacio= false, actEspacio;
  int pos= 0;
  char *palAct= letras;
  while (!feof(f)) {
    letras[pos]= fgetc(f);
    actEspacio= isspace(letras[pos]);
    if (actEspacio) {
      letras[pos]= 0;
      if (!ultEspacio) {
        palabras->insertar(palAct);
        distintas->insertar(palAct);
      }
      palAct= letras+pos+1;
    }
    ultEspacio= actEspacio;
    pos++;
  }
  fclose(f);
                             //3. Inicializar los otros miembros 
  subdir = subdirec;
  if (subdir==NULL || strlen(subdir)==0)
    subdir= ".";
//else if (subdir[strlen(subdir)-1]=='/')
//   subdir[strlen(subdir)-1]= 0;
  numPalMin= npMin;
  numPalMax= npMax;
  idPagina= 0;
  nombreAct= new char[_TAMANO_BUFFER];
  url= new char[_TAMANO_BUFFER];
  titulo= new char[_TAMANO_BUFFER];
  avanzar();
}

Generador::~Generador ()
{
  delete[] letras;
  delete palabras;
  delete distintas;
  delete[] nombreAct;
  delete[] url;
  delete[] titulo;
}

void Generador::avanzar ()
/* Actualiza la informacion descripcion de la pagina, para una nueva
   pagina: nombre del fichero, url, titulo y relevancia. */
{
  int i, lon;
                                   //1. Nombre del fichero de pagina
  sprintf(nombreAct, "%s/%d/%d.html", subdir,
          idPagina/_PAGINAS_POR_DIR+1, idPagina%_PAGINAS_POR_DIR);
  if (idPagina%_PAGINAS_POR_DIR == 0) {
    char nuevoDir[_TAMANO_BUFFER];
    sprintf(nuevoDir, "%s/%d", subdir, idPagina/_PAGINAS_POR_DIR+1);
    mkdir(nuevoDir, 0777);
  }
                                   //2. URL de la pagina
  char aux[_TAMANO_BUFFER];
  strcpy(aux, ALEATORIO(4)?"http://":ALEATORIO(3)?"https://":"ftp://");
  if (ALEATORIO(3))
    strcat(aux, "www.");
  for (i= 0, lon= ALEATORIO(3)+1; i<lon; i++) {
    strcat(aux, unaPalabra(20));
    strcat(aux, ".");
  }
  strcat(aux, dominios[ALEATORIO(numDominios)]);
  strcat(aux, "/");
  for (i= 0, lon= ALEATORIO(4)+1; i<lon; i++) {
    strcat(aux, unaPalabra(10));
    if (i<lon-1) strcat(aux, "/");
  }
  strcat(aux,  ALEATORIO(3)?".html": ALEATORIO(3)?".htm":".txt");
  strcpy(url, aux);
                                   //3. Titulo de la pagina
  strcpy(aux, unaPalabra(20));
  aux[0]= toupper(aux[0]);
  strcat(aux, " ");
  for (i= 0, lon= ALEATORIO(5)+1; i<lon; i++) {
    strcat(aux, unaPalabra(20));
    if (i<lon-1) strcat(aux, " ");
  }
  strcpy(titulo, aux);
                                   //4. Relevancia y numero de palabras
  relevancia= strlen(titulo) + ALEATORIO(50);
  numPalabras= numPalMin + ALEATORIO(numPalMax-numPalMin+1);
  idPagina++;
}

void Generador::escribirDescrip (FILE *fmaestro)
{
  fprintf(fmaestro, "%s  %s  %d  %s\n", url, nombreAct, relevancia, titulo);
}

void Generador::escribirPagina ()
{
  assert(nombreAct!=NULL && numPalabras>0 && palabras!=NULL);  //Precondicion
  FILE *f;
  f= fopen(nombreAct, "w");
  assert(f!=NULL);
  for (int i= 0; i<numPalabras; i++) {
    fprintf(f, "%s", palabras->cualquiera());
    if (i%_PALABRAS_POR_LINEA == _PALABRAS_POR_LINEA-1)
      fprintf(f, ".\n");
    else fprintf(f, " ");
  }
  fclose(f);
}

void Generador::escribirInfo ()
{
  cout << "Numero total de caracteres leidos: " << numLetras << "\n";
  cout << "Numero total de palabras: " << palabras->numElems() << "\n";
  cout << "Numero de palabras distintas: " << distintas->numElems() << "\n";
}

char const *Generador::unaPalabra (unsigned maxLong, int maxIntentos)
/* Describe una palabra aleatoria, de longitud maxima maxLong. */
{
  char *res;
  do {
    res= palabras->cualquiera();
    if (isalpha(res[0]) && isalpha(res[strlen(res)-1]) && strlen(res)<=maxLong)
      return res;
  } while (maxIntentos--);
  return "a";
}
