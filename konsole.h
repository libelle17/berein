#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#ifndef KONSOLE_H_DRIN
#define KONSOLE_H_DRIN
#ifdef _WIN32
#include <winsock2.h> // entfaellt bei mysql
#include <io.h> // fuer isatty und _isatty
#endif

#include <iostream>
//#define obfstream
#ifdef obfstream
#include <fstream> // fstream
#endif  

#ifdef _MSC_VER
#include <string>
#endif

#ifdef linux
#define vtz '/' // Verzeichnistrennzeichen
#define vtzs "/" // Verzeichnistrennzeichenstring
#elif __MINGW32__ || _MSC_VER
#define vtz '\\' // Verzeichnistrennzeichen
#define vtzs "\\" // Verzeichnistrennzeichen
#endif

using namespace std;
int Log(string text,short screen=1,short file=1,bool oberr=0,short klobverb=0);

#ifdef _MSC_VER
#define fileno _fileno // sonst Warnung posix deprecated
#define isatty _isatty // sonst Warnung posix deprecated
#endif

#ifdef __MINGW32__
#undef PATH_MAX
#endif

#ifdef _WIN32
#define PATH_MAX MAX_PATH
// #include <limits.h>
#include <time.h> // fuer mktime und time in kuerzelogdatei
#define lstat stat

#elif linux
#include <string.h> // strcpy, strtok_r, strerror, memcmp, strcmp, strstr, 
#include <errno.h> // errno, ENOENT
#include <pwd.h>   // getuid, getpwuid
#include <sys/param.h>  // MAXHOSTNAMELEN
#include <sys/ioctl.h> // winsize, TIOCGWINST, w, ioctl
#include <stdlib.h>
#include <dirent.h> // DIR, dirHandle, opendir, readdir, PATH_MAX
#include <unistd.h>
#endif

#ifndef _MSC_VER
#include <stdio.h> // fuer fopen, perror, fgets, sscanf, fputs, fclose, stdout, fileno, stderr
#else
#define snprintf _snprintf
#endif

#include <sys/stat.h> // stat, lstat, S_IFMT, S_IFDIR ...
// #include <boost/algorithm/string.hpp> // clock, numeric_limits
#include <time.h>
#include <limits>



typedef unsigned char uchar;
enum binaer:uchar {falsch,wahr};
enum Sprache {deutsch,englisch};
extern const char *dir;
//extern const string datei;
// extern const char *rot, *weinrot, *schwarz, *blau, *gelb; // muss spaeter kompilerunabhaengig
extern const char *schwarz, *dgrau, *drot, *rot, *gruen, *hgruen, *braun, *gelb, *blau, *hblau, *violett, *hviolett,
       *tuerkis, *htuerkis, *hgrau, *weiss;
extern const string drots, rots, schwarzs, blaus, gelbs, tuerkiss, hgraus,violetts;
#ifdef linux
extern const char *_rot, *_hrot, *_schwarz, *_blau, *_gelb, *_tuerkis, *_hgrau;
#endif
#include <fstream> // kopiere
#include <algorithm>    // std::transform

enum Tkonsole_ {
  T_pfad,
  T_kuerze_logdatei,
  T_Logdatei,
  T_gibt_es_noch_nicht_Kuerze_sie_daher_nicht,
  T_Kann_Datei,
  T_nicht_als_fstream_zum_Schreiben_oeffnen,
  T_nicht_als_fstream_zum_Lesen_oeffnen,
  T_nicht_mit_fopen_zum_Schreiben_oeffnen,
  T_nicht_mit_fopen_zum_Lesen_oeffnen,
  T_Variable_logdatei_leer,
  T_nicht_als_fstream_zum_Anhaengen_oeffnen,
  T_nicht_mit_fopen_zum_Anhaengen_oeffnen,
  T_Bitte_mit,
  T_beenden,
  T_stern_zeile,
  T_Rueckmeldung,
  T_Suchbefehl,
  T_Fuehre_aus,
  T_fehlgeschlagen_bei,
  T_Fuehrte_aus,
  T_komma_Ergebnis,
  T_kein_Verzeichnis_nochmal,
  T_nicht_gefunden_soll_ich_es_erstellen,
  T_Fehlender_Parameter_string_zu,
  T_oder,
  T_Fehler_Parameter,
  T_ohne,
  T_mit_Datei_als,
  T_mit_falschem,
  T_Pfad_angegeben,
  T_Nicht_numerischer,
  T_Fehlender,
  T_Parameter_nr_zu,
  T_Fehler,
  T_Erfolg,
  T_konsoleMAX,
};

class Txkonsolecl
{
  public:
    Sprache lgn; // Sprache numerisch
    static const char *TextC[][2];
    inline const char* operator[](Tkonsole_ const& nr) const {
      return TextC[nr][lgn];
    }
};


inline string zustr(int _Val) {
  char _Buf[2 * sizeof(long long)];
  snprintf(_Buf,2*sizeof(long long), "%d", _Val);
  return string(_Buf);
}
inline std::string right(std::string const& source, size_t const length) {
  if (length >= source.size()) { return source; }
  return source.substr(source.size() - length);
} // tail}
inline std::string mitvtz(std::string const& vz) {
  if (right(vz,1)==string(1,vtz)) {
    return vz;
  } else {
    return vz+vtz;
  }
}
inline int istelnr(std::string str) {
  if	(str.find_first_not_of("+ 0123456789.,")==string::npos && str.find_first_not_of(" ")!=string::npos) 
    return 1;
  return 0;
}

inline int isnumeric(std::string str)
{
  if	(str.find_first_not_of(" 0123456789.,")==string::npos && str.find_first_not_of(" ")!=string::npos) 
    return 1;
  return 0;
}

inline int isnumeric(char* str)
{
  while (*str) {
    if(!strchr(" 0123456789.,",*str)) return 0;
    str++;
  }
  return 1;
}

inline std::string *gtrim(std::string *str) {
  str->erase(0, str->find_first_not_of(' '));       //prefixing spaces
  str->erase(str->find_last_not_of(' ')+1);         //surfixing spaces
  return str;
}

inline std::string *ltrim(std::string *str) {
  str->erase(0, str->find_first_not_of(' '));       //prefixing spaces
  return str;
}


inline void rtrim(char* str,size_t size) {
  for (int nd=0,i=size-1;i;i--) {
    if (!str[i]) nd=1; 
    else if (nd && str[i]!=32) break;
    str[i]=0;
  }
}

void replaceAll(string *str, const string& from, const string& to);
void replaceAll(string *str, const char* from, const char* to);
string ersetze(string u, const char* alt, const char* neu);
string caseersetze(string u, const char* alt, const char* neu); 
string *ersetze(string *u, const char* alt);
string ersetze(const char *u, const char* alt, const char* neu);
string sersetze( string src, string const& target, string const& repl);
wstring ersetze(wstring u, const wchar_t* alt, const wchar_t* neu); 
char* charersetze(char *u, char alt, char neu);
void chersetze(string u, string *z, const string& alt, const char neu);
string ersetzefuerdatei(string u);
size_t zahlin(string *str, const char* was);
long cmpmem( char* feld, const char* search, int len_feld); // , int len_search
// fuer lies (Konfigurationsdatei lesen)
char* ltoan(long value, int base=10, uchar obtz=0, uchar minstel=0); 
char* ltoa_(long value, char* result, int base); 

class Schluessel {
  public:
    char key[90];
    char val[100];
    template <typename T> void zuweis(T *var) { *var=atol(val); }
    template <typename T> void beleg(T *var) { strncpy(val,ltoan(*var),sizeof val-1);val[sizeof val-1]=0; }
};
template <> inline void Schluessel::zuweis < char* > (char** var) { *var = val; }
template <> inline void Schluessel::zuweis < const char* > (const char** var) { *var = val; }
template <> inline void Schluessel::zuweis < string > (string *var) { *var = val; }
template <> inline void Schluessel::zuweis < binaer > (binaer *var) { *var = (binaer)atoi(val); }
template <> inline void Schluessel::beleg < char* > (char** var) { strncpy(val,*var,sizeof val-1);val[sizeof val-1]=0; }
template <> inline void Schluessel::beleg < const char* > (const char** var) { strncpy(val,*var,sizeof val-1);val[sizeof val-1]=0; }
template <> inline void Schluessel::beleg < string > (string *var) { strncpy(val,var->c_str(),sizeof val-1);val[sizeof val-1]=0;}

class cppSchluess {
  public:
    string name;
    string wert;
    template <typename T> void zuweis(T *var) { *var=atol(wert.c_str()); }
    template <typename T> void beleg(T *var) { wert=ltoan(*var); }
};
template <> inline void cppSchluess::zuweis < char* > (char** var) {*var = (char*)wert.c_str(); }
template <> inline void cppSchluess::zuweis < const char* > (const char** var) {*var = wert.c_str(); }
template <> inline void cppSchluess::zuweis < string > (string *var) {*var = wert; }
template <> inline void cppSchluess::zuweis < binaer > (binaer *var) { *var = (binaer)atoi(wert.c_str()); }
template <> inline void cppSchluess::beleg < char* > (char** var) {wert=*var; }
template <> inline void cppSchluess::beleg < const char* > (const char** var) {wert=*var;}
template <> inline void cppSchluess::beleg < string > (string *var) {wert=*var;}

#ifdef _MSC_VER
extern inline void wait();
#else
// #define wait();
#endif

#ifdef _WIN32
extern const char *drot, *rot, *schwarz, *blau, *gelb, *tuerkis, *hgrau;
extern const string drots, rots, schwarzs, blaus, gelbs, tuerkis, hgrau;
inline std::ostream& _drot(std::ostream &s){
  //    HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE); 
  SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED|FOREGROUND_INTENSITY);
  return s;
}
inline std::ostream& _blau(std::ostream &s){
  SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_BLUE|FOREGROUND_INTENSITY);
  return s;
}
inline std::ostream& _rot(std::ostream &s){
  //    HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE); 
  SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED|FOREGROUND_GREEN|FOREGROUND_INTENSITY);
  return s;
}
inline std::ostream& _schwarz(std::ostream &s){
  SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED|FOREGROUND_GREEN|FOREGROUND_BLUE);
  return s;
}
inline std::ostream& _gelb(std::ostream &s){
  SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED|FOREGROUND_GREEN);
  return s;
}
struct color {
  color(WORD attribute):m_color(attribute){};
  WORD m_color;
};

#endif
#include <vector>
class svec: public vector<std::string> {
  public:
    svec& operator<<(const std::string& str) {
      this->push_back(str);
      return *this;
    }
};
//svec& operator<<(svec& v, const std::string& str);
template<class T>
class tsvec: public vector<T> {
  public:
    tsvec<T>& operator<<(const T& str) {
      this->push_back(str);
      ((T&)str).init();
      return *this;
    }
};

class confdat {
  public:
    ifstream *f;
    uchar obgelesen=0;
    svec zn;
    size_t richtige;
    confdat(string fname, uchar obverb);
    confdat(string fname,cppSchluess *conf, size_t csize, uchar oberb=0, char tz='=');
    int lies(string fname,uchar obverb);
    void auswert(cppSchluess *conf, size_t csize, uchar obverb=0, char tz='=');
};

// fuer Commandline-Optionen
enum part:uchar {sons,verz,file,zahl}; // Parameterart: Sonstiges, Verzeichnis, Zahl

class optioncl
{
  public:
    string kurz;
    string lang;
    string erkl;
    uchar *pptr; // Zeiger auf Parameter, der hier eingestellt werden kann
    int wert; // Wert, der pptr zugewiesen wird, falls dieser Parameter gewaehlt wird
    string *zptr; // zusatzzeiger auf Parameter, der hier eingegeben werden kann
    part art;
    optioncl(string kurz, string lang, string erkl) : kurz(kurz), lang(lang), erkl(erkl), zptr(0) {}
    optioncl(string kurz, string lang, string erkl, string *zptr, part art) : kurz(kurz), lang(lang), erkl(erkl), pptr(0), zptr(zptr),art(art) {}
    optioncl(string kurz, string lang, string erkl, uchar *pptr, int wert) : kurz(kurz), lang(lang), erkl(erkl), pptr(pptr), wert(wert), zptr(0) {}
    optioncl(string kurz, string lang, string erkl, binaer *pptr, int wert) : 
      kurz(kurz), lang(lang), erkl(erkl), pptr((uchar*)pptr), wert(wert), zptr(0) {}
    int pruefp(int argc, char** argv, int akt, uchar *hilfe);
    void hilfezeile();
};

#endif // KONSOLE_H_DRIN

extern const char *logdatei;

int kuerzelogdatei(const char* logdatei,int obverb);
void kopiere(string *quelle, string *ziel);
void aufSplit(vector<string> *tokens, const string *text, char sep, bool nichtdoppelt=1);
void aufSplit(vector<string> *tokens, const char *text, char sep, bool nichtdoppelt=1);
void aufiSplit(vector<string> *tokens, const string *text, const char* sep,bool nichtdoppelt=1);
void aufSplit(vector<string> *tokens, const string *text, char* sep,bool nichtdoppelt=1);
void getstammext(string *ganz, string *stamm, string *exten);
// int cpplies(string fname,cppSchluess *conf,size_t csize,vector<string> *rest=0,char tz='=',short obverb=0);
// int liesalt(const char *fname,Schluessel *conf,size_t csize,vector<string> *rest=0,char tz='=',short obverb=0);
// int liesalt(string fname,Schluessel *conf,size_t csize,vector<string> *rest=0,char tz='=',short obverb=0);
string XOR(string value,string key);
int schreib(const char *fname, Schluessel *conf, size_t csize);
int cppschreib(string fname, cppSchluess *conf, size_t csize);
int multicppschreib(string fname, cppSchluess **conf, size_t *csizes, size_t cszahl);
std::string base_name(std::string const & path);
std::string dir_name(std::string const & path);
int systemrueck(string cmd, char obverb=0, uchar oblog=0, vector<string> *rueck=0, binaer ob0heissterfolg=wahr, binaer obergebnisanzeig=wahr, string ueberschr="");
string aktprogverz();
char holbuchst(string frage, const char* moegl,const char *erkl[], const char* erlaubt=0, const char *vorgabe=0);
char holbuchst(string frage, string moegl,const char *erkl[], const char* erlaubt=0, const char *vorgabe=0);
char holbuchst(const char *frage, const char* moegl,const char *erkl[], const char* erlaubt=0, const char *vorgabe=0);
char holbuchst(const char *frage, string moegl,const char *erkl[], const char* erlaubt=0, const char *vorgabe=0);
string holstrings(const char *frage, char* moegl[], char *vorgabe=0); // das letzte Element von moegl muss 0 sein
string holstrings(const char *frage, vector<string> *moegl, string *vorgabe=0); 
string holzahl(const char *frage, const char *vorgabe=0);
string holzahl(const char *frage, string *vorgabe);
string holstring(const char *frage, string *vorgabe=0);
char* holcstring(const char *frage, char* buf, unsigned long buflen, const char* vorgabe="");
string holstring(string frage, string *vorgabe=0);
string holverz(const char *frage,string *vorgabe=0);
uchar VerzeichnisGibts(const char* vname);
