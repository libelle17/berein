#ifdef linux
// #include <pgsql/libpq-fe.h> // PGconn
#include <utime.h>
#include <iomanip>
#define vtz '/' // Verzeichnistrennzeichen
#elif __MINGW32__ || _MSC_VER
#include <sys/utime.h>
#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif
// #include "c:\programme\postgresql\9.1\include\libpq-fe.h"
// #include "..\..\..\..\programme\postgresql\9.2\include\libpq-fe.h"
#define vtz '\\' // Verzeichnistrennzeichen
#endif

#include <iostream>
// #include <fstream> // jetzt in konsole.h
#include <sstream>
#include <dirent.h> // DIR, dirent, opendir, readdir, PATH_MAX
#include <boost/algorithm/string.hpp>
#ifdef _MSC_VER
#define PATH_MAX MAX_PATH
#define realpath(x,y) _fullpath((y),(x),PATH_MAX)
#endif

#include "vmime/vmime.hpp"
#include "vmime/platforms/posix/posixHandler.hpp"
#include "konsole.h"
#include "DB.h"
// #include "iconverter.h"
// #include "datetime_utils.hpp"
#include <regex.h>
#define uchar unsigned char
// G.Schade 22.4.14 für Zusatz-Debugging (SQL): 255, sonst: 0
// #define ZDB 255
#define ZDB 0
// fuer die Prozessliste
#define PROC_VERZ "/proc/"
#define ZDB 0
#ifdef linux
#include <sys/utsname.h>
#endif

const char *logverz; // das reine Verzeichnis
string logpfad; // zusammengesetzt aus Verzeichnis und name
// const char lgs logdatei_a[PATH_MAX+1] ="/var/log/log_vmparse.txt";
// const char *logdatei = logdatei_a;
const char *logdatei; // muss dann auf logpfad zeigen
const char* logdatname;
const char *weinrot="\033[31m";
const char *_weinrot=weinrot;
svec zn;

int lies(const char *fname,Schluessel *conf,size_t cs,vector<string> *rest,const char tz,short obverb)
{
  int r=0;
  char zeile[1000],*p;
  if (rest) rest->clear();
  FILE *f=fopen(fname,"rt");
  if(!f) return r; // return perror(fname),r;
  if (obverb) cout<<"lies(fname,Schluessel...), fname: "<<fname<<endl;
  const char *scs=(string("%[^ ^")+tz+"\t#]"+tz+"[ ]").c_str();
  while( fgets(p=zeile,sizeof zeile,f) ) {
    if( strchr(p,'\n') ) *strchr(p,'\n')=0;
    if (*p=='#') continue;
    if (obverb)
      cout<<"Zeile: "<<zeile<<endl;
    if( (p=strchr(zeile,tz)) && 1==sscanf(zeile,scs,zeile) ) {
      size_t i=cs;
      while( i-- ) 
        if( !strcmp(conf[i].key,zeile) ) {
          ++r;
          do ++p; while (*p==' '); // Leerzeichen am Anfang entfernen
          if (*p=='"' && p[strlen(p)-1]=='"') {p++;p[strlen(p)-1]=0;}
          sprintf(conf[i].val,"%.*s",(int)sizeof conf->val-1,p);
        }
    } else {
      if (rest) rest->push_back(zeile);
      if (obverb) cout<<"kein Match: "<<zeile<<endl;
    }
  }
  fclose(f);
return r;
}


Schluessel conf[]={{"logdatei"}};
int verwendekonf(string pname,string* Verz, char** logdatei,svec *rest, uchar obverb=0)
{
  size_t s=sizeof conf/sizeof*conf;
  lies(pname.c_str(),conf,s,rest,'=',obverb);
  if (conf[0].val[0]) *Verz=conf[0].val;
  if (conf[5].val[0]) {
    cout<<logdatei<<endl;
    cout<<*((char**)&logdatei)<<endl;
    strncpy(*((char**)&logdatei),conf[5].val,PATH_MAX);
    (*((char**)&logdatei))[PATH_MAX]=0;
  }
  return 0;
}

class abstand {
  static tm tmj,tmadj,tmvej; // tm jetzt, anfang des jahres, jetzt vor einem jahr
  tm tmd; //  tm datei
  static time_t ttj,ttadj,ttvej; // time_t jetzt, time_t anfang des jahres, time_t jetzt vor einem Jahr
  time_t ttd; // time_t datei
  char bufj[255];
  void init(short verbose,bool oblog);
  public:
  int jahre, ianr, innr, distt; // intervall alt nr, intervall neu nr, distanz in tagen;
  abstand(short verbose,bool oblog){ init(verbose,oblog); }
  abstand(tm tmakt,int profernemjahr,int proletztemjahr,short verbose,bool oblog){
    init(verbose,oblog);
    tmd=tmakt;
    ttd=mktime(&tmd);   
    distt=(ttj-ttd)/86400; // 60*60*24
    if (verbose>1) Log(string("Distanz in Tagen: ")+weinrot+ltoan(distt)+schwarz,verbose,oblog);
    if (ttd<ttvej){ // (difftime(ttj,ttd)>31536000) // 365*24*60*60 
      jahre=tmadj.tm_year - tmd.tm_year;
      if (verbose>1) Log(string("Jahre: ")+weinrot+ltoan(jahre)+schwarz,verbose,oblog);
      ianr=(ttd-ttadj+jahre*31536000)*profernemjahr/31536000;
      if (verbose>1) Log(string("altes Intervall Nr: ")+weinrot+ltoan(ianr)+schwarz,verbose,oblog);
    } else {
      jahre=0;
      innr=(ttd-ttvej)/(31536000/proletztemjahr);
      if (verbose>1) Log(string("neues Intervall Nr: ")+weinrot+ltoan(innr)+schwarz,verbose,oblog);
    }
  }
};

time_t abstand::ttj=0;
time_t abstand::ttadj=0;
time_t abstand::ttvej=0;
tm abstand::tmj;
tm abstand::tmadj;
tm abstand::tmvej;
void abstand::init(short verbose,bool oblog){
  if (!ttj) {
    time(&ttj);
    memcpy(&tmj,localtime(&ttj),sizeof tmj);
    tmadj.tm_sec=0;
    tmadj.tm_min=0;
    tmadj.tm_hour=0;
    tmadj.tm_mday=1;
    tmadj.tm_mon=0;
    tmadj.tm_year=tmj.tm_year;
    ttadj=mktime(&tmadj);
    tmvej=tmj;
    tmvej.tm_year--;
    tmvej.tm_mday=1;
    ttvej=mktime(&tmvej);
    strftime(bufj, sizeof(bufj), "%d.%m.%y %H:%M:%S %z", &tmj);
    Log(string("Jetzt: ")+weinrot+bufj+schwarz,verbose,oblog);
  }
}


void doberein(string *Verz, string *aktz,uchar obloe,short verbose, bool oblog){
  ulong loez=0,erhz=0; // Zahl der zu loeschenden, zu erhaltenden
  svec spl;
  abstand labst(verbose,oblog);
  string muster,profernemjahr,proletztemjahr,tageschutz;
  int tageschutzi;
  aufSplit(&spl,aktz,' ');
  //  for(size_t j=0;j<spl.size();j++)
  //    <<"spl["<<j<<"]: "<<spl[j]<<endl;
  //  
  if (spl.size()>0){
    muster=spl[0];
    if (spl.size()>1) profernemjahr=spl[1]; else profernemjahr="1";
    if (spl.size()>2) proletztemjahr=spl[2]; else proletztemjahr="12";
    if (spl.size()>3) tageschutz=spl[3]; else tageschutz="10";
    Log(string("Muster: ")+weinrot+muster+schwarz+", pro fernem Jahr: "+weinrot+profernemjahr+schwarz+", pro letztem Jahr: "+weinrot+proletztemjahr+schwarz+", Tage Schutz: "+weinrot+tageschutz+schwarz,verbose,oblog);
    tageschutzi=atoi(tageschutz.c_str());
    struct stat entrystat;
    if (!lstat(Verz->c_str(),&entrystat)) {
      int gefzahl;
      {
        string cmd=string("find \"")+*Verz+"\" -maxdepth 1 -type f -iname \""+muster+"\" | wc -l";
        if (FILE* pipe = popen(cmd.c_str(), "r")){
          while(!feof(pipe)) {
            char buffer[1280];
            if(fgets(buffer, sizeof buffer, pipe) != NULL){
              gefzahl=atoi(buffer);
            }
          }
          pclose(pipe);
        }
      }
      // <<"gefzahl: "<<gefzahl<<endl;
      if (gefzahl) {
        string cmd=string("find \"")+*Verz+"\" -maxdepth 1 -type f -iname \""+muster+"\" -print0 | xargs -0 ls -l --time-style=full-iso | sort -k 6,7";
        Log(string("Suchbefehl: ")+weinrot+cmd+schwarz,verbose,oblog);
        if (FILE* pipe = popen(cmd.c_str(), "r")){
          while(!feof(pipe)) {
            char buffer[1280];
            if(fgets(buffer, sizeof buffer, pipe) != NULL){
              bool bleibt=1;
              size_t posi;
              if (buffer[posi=strlen(buffer)-1]==10) buffer[posi]=0;
              svec ispl;
              string bufs=buffer;
              aufSplit(&ispl,&bufs,' ',0);
//    for(size_t j=0;j<ispl.size();j++) <<"ispl["<<j<<"]: "<<ispl[j]<<endl;
              struct tm tmakt;
              char buf[255];
              if (ispl.size()>7) {
                memset(&tmakt, 0, sizeof tmakt);
                string datum=ispl[5]+" "+ispl[6].substr(0,8)+" "+ispl[7]; // .substr(0,3); // +":"+ispl[7].substr(3));
                strptime(datum.c_str(), "%Y-%m-%d %H:%M:%S %z", &tmakt);
                strftime(buf, sizeof(buf), "%d.%m.%y %H:%M:%S %z", &tmakt);
                //                <<weinrot<<buf<<endl;
                abstand abst(tmakt,atoi(profernemjahr.c_str()),atoi(proletztemjahr.c_str()),verbose,oblog);
                // <<"abst.jahre: "<<abst.jahre<<", labst.jahre: "<<labst.jahre<<", abst.innr: "<<abst.innr<<", labst.innr: "<<labst.innr<<endl;
                if (abst.distt>tageschutzi &&
                    (abst.jahre==labst.jahre && 
                     ((!abst.jahre && abst.innr==labst.innr)||(abst.jahre && abst.ianr==labst.ianr))))
                  bleibt=0; 
                labst=abst;
              }
              string farbe;
              if (bleibt) {
                erhz++;
                farbe=blau;
              } else {
                loez++;
                farbe=rot;
              }
              Log(farbe+(verbose>1?" ":"")+buffer+schwarz,verbose,oblog);
              if (!bleibt && obloe) {
                string ldatei=ispl[8];
                for(unsigned k=9;k<ispl.size();k++) {
                 ldatei+=" ";
                 ldatei+=ispl[k];
                }
                string cmd=string("rm \"")+ldatei+"\"";
                Log(string(gelb)+cmd+schwarz,verbose,oblog);
                system(cmd.c_str());
              }
            }
          } // while(fpipe)
          pclose(pipe);
        }
        string erhzs=ltoan(erhz);
        Log(string("In ")+weinrot+*Verz+schwarz+" nach Muster "+weinrot+muster+schwarz+(obloe?" geloescht ":" zu loeschen ")+weinrot+ltoan(loez)+schwarz+", "+(obloe?"erhalten ":"zu erhalten ")+weinrot+erhzs+schwarz+" Dateien",1,1);
      }
    }
  }
}

int getpar(int argc, char** argv, uchar *obloe, short *verbose, bool *oblog, uchar *logfilenew, string *cfgdatei, uchar *hilfe)
{
  for(int i=argc-1;i>0;i--){if (argv[i][0]==0) argc--;} // damit für das Compilermakro auch im bash-script argc stimmt
  for(int i=1;i<argc;i++) {
    if (0) {
    } else if (!strcmp(argv[i],"-l") || !strcmp(argv[i],"/l") || !strcmp(argv[i],"--log")){
      *oblog=1;
    } else if (!strcmp(argv[i],"-lfn") || !strcmp(argv[i],"/lfn") || !strcmp(argv[i],"--logfilenew") || !strcmp(argv[i],"--logfileneu")){
      *logfilenew=1;
    } else if (!strcmp(argv[i],"-lf") || !strcmp(argv[i],"/lf") || !strcmp(argv[i],"--logfile")){
      if (i<argc-1 && argv[i+1][0]) {
        logdatname=argv[++i];
        logpfad = string(logverz)+logdatname;
        logdatei= logpfad.c_str();
      } else {
        Log(string(weinrot)+"Fehler: Parameter -lf oder --logfile ohne Datei angegeben!"+schwarz,1,1);
        *hilfe=1;
      }
    } else if (!strcmp(argv[i],"-h") || !strcmp(argv[i],"/h")|| !strcmp(argv[i],"-?")  || !strcmp(argv[i],"/?") || !strcmp(argv[i],"--hilfe")|| !strcmp(argv[i],"--help")){
      *hilfe=1;
    } else if (!strcmp(argv[i],"-d") || !strcmp(argv[i],"/d") || !strcmp(argv[i],"--delete")){
      *obloe=1;
    } else if (!strcmp(argv[i],"-v") || !strcmp(argv[i],"/v") || !strcmp(argv[i],"--verbose")){
      (*verbose)++;
    } else if (!strcmp(argv[i],"-cfg")) {
      if (i<argc-1 && argv[i+1][0]) {
        *cfgdatei=argv[++i];
      } else {
        Log(string(weinrot)+"Fehler: Parameter -cfg ohne Datei angegeben!"+schwarz,1,1);
        *hilfe=1;
      }
    }
  }
//  Log(string("Fertig mit Parsen der Befehlszeile"),*verbose,*oblog);
  if (*hilfe){
    cout<<"Gebrauch: "<<_weinrot<<argv[0]<<" [-p <pfad>] [-l] [-v] [-h]"<<schwarz<<endl; 
    cout<<"Loescht alte Dateien nach Muster und Intervall in "<<weinrot<<*cfgdatei<<schwarz<<endl;
    cout<<_weinrot<<" -cfg <pfad>"<<_schwarz<<": verwendet die Konfigurationsdatei <pfad> anstatt '"<<weinrot<<*cfgdatei<<schwarz<<"'\n";
    cout<<_weinrot<<" -v, --verbose"<<_schwarz<<": Bildschirmausgabe gespraechig\n";
    cout<<_weinrot<<" -d, --delete"<<_schwarz<<": Dateien tatsaechlich loeschen\n";
    cout<<_weinrot<<" -l, --log"<<_schwarz<<": protokolliert ausfuehrlich in Datei '"<<_weinrot<<logpfad<<_schwarz<<"' (sonst knapper)\n";
    cout<<_weinrot<<" -lf, --logfile <logdatei>"<<_schwarz<<": logdatei wird auf <logdatei> (im Pfad '"<<weinrot<<logverz<<schwarz<<"') geaendert\n";
    cout<<_weinrot<<" -lfn, --logfilenew"<<_schwarz<<": logdatei vorher loeschen\n";
    cout<<_weinrot<<" -h, --hilfe"<<_schwarz<<": Zeigt diesen Bildschirm an\n";
    return 0;
  }
  return 1;
} // getpar
void oertliches(){
  logverz = "/var/log/";
  logdatname = "autofax.log";
  logpfad = string(logverz)+logdatname;
  logdatei = logpfad.c_str();
}
int main(int argc, char** argv) 
{
  uchar obloe=0;
  uchar logfilenew=0;
  uchar hilfe=0;
  short verbose=0;
  bool oblog=0;
  string stamm,exten; 
  string a0=argv[0];
  char buf[PATH_MAX+1];
  realpath(a0.c_str(),buf);
  string a0name=buf;
  string Verz;
  svec rest;
  oertliches();
  getstammext(&a0name, &stamm, &exten);
  string konfdatei = stamm+".cfg";
  if (!getpar(argc, argv, &obloe, &verbose, &oblog, &logfilenew, &konfdatei, &hilfe))
    exit(0);
  Log(string("Konfigurationsdatei: ")+weinrot+konfdatei+schwarz,1,1);
  verwendekonf(konfdatei,&Verz,(char**)&logdatei,&rest);
  for(size_t i=0;i<rest.size();i++){
    string vcmp="Verzeichnis";
    if (rest[i].substr(0,vcmp.size())==vcmp) {
      Verz=rest[i].substr(vcmp.size());
      gtrim(&Verz);
      Log(vcmp+": "+gelb+Verz+schwarz,verbose,oblog);
    }
    else
      doberein(&Verz,&rest[i],obloe,verbose,oblog);
  }
}

