#include "konsole.h"

#ifdef _WIN32
const char *dir = "dir ";
#elif linux
const char *dir = "ls -l ";
#endif

#ifdef linux
// const char *rot="\033[1;31m", *weinrot="\033[31m", *schwarz="\033[0m", *blau="\033[34m", *gelb="\033[33m"; // muss spaeter kompilerunabhaengig 
const char *schwarz="\33[1;30m", *dgrau="\33[1;30m", *drot="\33[0;31m", *rot="\33[1;31m"/*, *weinrot="\033[31m"*/, *gruen="\33[0;32m", *hgruen="\33[0;32m",
      *braun="\33[0;33m", *gelb="\33[1;33m", *blau="\33[0;34m", *hblau="\33[1;34m", *violett="\33[0;35m", *hviolett="\33[1;35m",
      *tuerkis="\33[0;36m", *htuerkis="\33[1;36m", *hgrau="\33[0;37m", *weiss="\33[1;37m";
const char *_drot=drot, *_rot=rot, *_schwarz=schwarz, *_blau=blau, *_gelb=gelb, *_tuerkis=tuerkis, *_hgrau=hgrau/*,*_weinrot=weinrot*/;
// char logdatei[PATH_MAX+1]="/DATA/down/log_termine.txt";
#define _access access
#include <sys/time.h>  // für gettimeofday()
#elif defined _WIN32
const char *drot="", *rot="", *schwarz="", *blau="", *gelb="", *tuerkis="", *hgrau="";
//offen: bei den Farben muss unterschieden werden zwischen cout (-> _drot) und 
printf(drot, unter windows escape-Sequenzen rausfielselen und durch SetConsoleTextAttribute-Aufrufe ersetzen)
  // char logdatei[PATH_MAX+1]="v:\log_termine.txt";
  template <class _Elem, class _Traits>
  std::basic_ostream<_Elem,_Traits>& operator<<(std::basic_ostream<_Elem,_Traits>& i, color& c){
    HANDLE hStdout=GetStdHandle(STD_OUTPUT_HANDLE); 
    SetConsoleTextAttribute(hStdout,c.m_color);
    return i;
  }
//char logdatei[PATH_MAX+1]="v:\\log_termine.txt";
#endif


const char *Txkonsolecl::TextC[T_konsoleMAX+1][2]={
  // T_pfad,
  {"pfad","path"},
  // T_kuerze_logdatei,
  {", kuerze logdatei: ",", shorten log file: "},
  // T_Logdatei,
  {"Logdatei ","Log file "},
  // T_gibt_es_noch_nicht_Kuerze_sie_daher_nicht,
  {" gibt es noch nicht. Kuerze sie daher nicht."," does not exist yet. Not shortening it therefore."},
  // T_Kann_Datei
  {"Kann Datei '","Cannot open file '"},
  //T_nicht_als_fstream_zum_Schreiben_oeffnen
  {"' nicht als fstream zum Schreiben oeffnen.","' as fstream for writing."},
  // T_nicht_als_fstream_zum_Lesen_oeffnen
  {"' nicht als fstream zum Lesen oeffnen.","' as fstream for reading."},
  // T_nicht_mit_fopen_zum_Schreiben_oeffnen 
  {"' nicht mit fopen zum Schreiben oeffnen.","' with fopen for writing."},
  // T_nicht_mit_fopen_zum_Lesen_oeffnen 
  {"' nicht mit fopen zum Lesen oeffnen.","' with fopen for reading."},
  // T_Variable_logdatei_leer
  {"Variable 'logdatei' leer!","Variable 'logdatei' empty!"},
  // T_nicht_als_fstream_zum_Anhaengen_oeffnen
  {"' nicht als fstream zum Anhaengen oeffnen.","' as fstream for appending."},
  // T_nicht_mit_fopen_zum_Anhaengen_oeffnen
  {"' nicht mit fopen zum Anhaengen oeffnen: ","' with fopen for appending."},
  // T_Bitte_mit
  {"Bitte mit '","Please exit with '"},
  // T_beenden
  {"' beenden ...","'"},
  // T_stern_zeile
  {"*zeile: ","*line: "},
  // T_Rueckmeldung
  {"Rueckmeldung: ","Feedback: "},
  // T_Suchbefehl
  {"Suchbefehl","Search command"},
  // T_Fuehre_aus
  {"Fuehre aus","Executing"},
  // T_fehlgeschlagen_bei
  {"fehlgeschlagen bei: ","failed at: "},
  // T_Fuehrte_aus
  {"Fuehrte aus","Executed "},
  // T_komma_Ergebnis
  {", Ergebnis: ",", result: "},
  // T_kein_Verzeichnis_nochmal
  {"' kein Verzeichnis, nochmal: ","' not a directory, once again: "},
  // T_nicht_gefunden_soll_ich_es_erstellen
  {"' nicht gefunden, soll ich es erstellen (","' not found, shall I create it ("},
  // T_Fehlender_Parameter_string_zu
  {"Fehlender Parameter <string> zu -","Missing Parameter <string> after -"},
  // T_oder
  {" oder --"," or --"},
  // T_Fehler_Parameter
  {"Fehler: Parameter -","Error: Stated Parameter -"},
  // T_ohne
  {"ohne","without"},
  // T_mit_Datei_als
  {"mit Datei als","with file as"},
  // T_mit_falschem
  {"mit falschem","with wrong"},
  // T_Pfad_angegeben
  {" Pfad angegeben!"," path!"},
  // T_Nicht_numerischer
  {"Nicht-numerischer","Not-numeric"},
  // T_Fehlender
  {"Fehlender","Missing"},
  // T_Parameter_nr_zu
  {" Parameter <nr> zu -"," Parameter <no> after -"},
  // T_Fehler
  {"Fehler ","Error "},
  // T_Erfolg
  {"Erfolg","Success"},
  {"",""}
};
class Txkonsolecl Txk;

const string drots=drot, rots=rot, schwarzs=schwarz, blaus=blau, gelbs=gelb, tuerkiss=tuerkis, violetts=violett;

string ersetzefuerdatei(string u) {
  static string ziel;
  ziel =u;
  replaceAll(&ziel,(const char*)"*",(const char*)"");
  replaceAll(&ziel,(const char*)":",(const char*)".");
  return ziel;
}

char* charersetze(char *u, char alt, char neu) {
  for(size_t i=0;i<strlen(u);i++)
    if (u[i]==alt) 
      u[i]=neu;
  return u;
} // charersetze(char *u, const char* alt, const char* neu)

wstring ersetze(wstring u, const wchar_t* alt, const wchar_t* neu) {
  wstring erg=L"";
  if (alt[0]==0 || !wcscmp(alt,neu)) {
    erg=u;
  } else {
    for(wchar_t* p=(wchar_t*)u.c_str();*p;p++)  {
      wchar_t *pi = (wchar_t*)alt;
      int i=0,gleich=1;
      for(;*(pi+i);i++)
        if (*(pi+i)!=*(p+i))
        {gleich=0;break;}
      if (gleich) {erg+=neu;p+=i-1;} else erg+=(*p);
    }
  }
  return erg;
} // ersetze(char *u, const char* alt, const char* neu)

string ersetze(string u, const char* alt, const char* neu) {
  string erg="";
  if (alt[0]==0 || !strcmp(alt,neu)) {
    erg=u;
  } else {
    //    for(char* p=(char*)u.c_str();*p;p++)  {
    for(char* p=(char*)u.c_str();p<(char*)u.c_str()+u.length();p++)  {
      char *pi = (char*)alt;
      int i=0,gleich=1;
      for(;*(pi+i);i++)
        if (*(pi+i)!=*(p+i))
        {gleich=0;break;}
      if (gleich) {erg+=neu;p+=i-1;} else erg+=(*p);
    }
  }
  return erg;
  } // ersetze(char *u, const char* alt, const char* neu)

  string caseersetze(string u, const char* alt, const char* neu) {
    string erg="";
    if (alt[0]==0 || !strcmp(alt,neu)) {
      erg=u;
    } else {
      //    for(char* p=(char*)u.c_str();*p;p++)  {
      for(char* p=(char*)u.c_str();p<(char*)u.c_str()+u.length();p++)  {
        char *pi = (char*)alt;
        int i=0,gleich=1;
        for(;*(pi+i);i++)
          if (tolower(*(pi+i))!=tolower(*(p+i)))
          {gleich=0;break;}
        if (gleich) {erg+=neu;p+=i-1;} else erg+=(*p);
      }
    }
    return erg;
    } // ersetze(char *u, const char* alt, const char* neu)

    string *ersetze(string *u, const char* alt) {
      size_t len=strlen(alt);
      size_t pos;
      while ((pos=u->find(alt))!=string::npos) {
        u->erase(pos,len);
      }
      return u;
    } // ersetze(char *u, const char* alt, const char* neu)

    void replaceAll(string *str, const char* from, const char* to) 
    {
      if(!*from)
        return;
      string wsRet;
      wsRet.reserve(str->length());
      size_t fromlen = strlen(from);
      size_t start_pos = 0, pos;
      while((pos = str->find(from, start_pos)) != string::npos) {
        wsRet += str->substr(start_pos, pos - start_pos);
        wsRet += to;
        pos += fromlen;
        start_pos = pos;
      }
      wsRet += str->substr(start_pos);
      str->swap(wsRet); // faster than *str = wsRet;
    }

    // ergibt die Zahl der Vorkommen von *was in *str
    size_t zahlin(string *str, const char* was) 
    {
      size_t erg=0,pos=0;
      if (*was) {
        while ((pos=str->find(was,pos)+1)) {
          erg++;
        }
      }
      return erg;
    }

    void replaceAll(string *str, const string& from, const string& to) 
    {
      if(from.empty())
        return;
      string wsRet;
      wsRet.reserve(str->length());
      size_t start_pos = 0, pos;
      while((pos = str->find(from, start_pos)) != string::npos) {
        wsRet += str->substr(start_pos, pos - start_pos);
        wsRet += to;
        pos += from.length();
        start_pos = pos;
      }
      wsRet += str->substr(start_pos);
      str->swap(wsRet); // faster than str = wsRet;
    }

    void chersetze(string str, string *wsRet, const string& from, const char to) 
    {
      //  wsRet->reserve(str.length());
      wsRet->clear();
      size_t start_pos = 0, pos;
      while((pos = str.find_first_of(from, start_pos)) != string::npos) {
        *wsRet += str.substr(start_pos, pos - start_pos);
        *wsRet += to;
        pos ++;
        start_pos = pos;
      }
      *wsRet += str.substr(start_pos);
    }

    string ersetze(const char *u, const char* alt, const char* neu) {
      string erg="";
      if (alt[0]==0 || !strcmp(alt,neu)) {
        erg.append(u);
      } else {
        for(char* p=(char*)u;*p;p++)  {
          char *pi = (char*)alt;
          int i=0,gleich=1;
          for(;*(pi+i);i++)
            if (*(pi+i)!=*(p+i))
            {gleich=0;break;}
          if (gleich) {erg+=neu;p+=i-1;} else erg+=(*p);
        }
      }
      return erg;
    } // ersetze(char *u, const char* alt, const char* neu)

    string sersetze( string src, string const& target, string const& repl) {
      if (target.length()) {
        if (src.length()) {
          size_t idx = 0;
          for (;;) {
            idx = src.find( target, idx);
            if (idx == string::npos)  break;
            src.replace( idx, target.length(), repl);
            idx += repl.length();
          }
        }
      }
      return src;
    } // sersetze( string src, string const& target, string const& repl)

    void getstammext(string *ganz, string *stamm, string *exten) {
      size_t posp = ganz->rfind('.');
      if (posp!=std::string::npos) {
        *exten=ganz->substr(posp+1);
        *stamm=ganz->substr(0,posp);
      } else {
        *exten="";
        *stamm=string(*ganz);
      }
    } // getstammext(

#if defined(__MINGW32__) || defined(_MSC_VER)
    int getcols() {
      CONSOLE_SCREEN_BUFFER_INFO info; //Schrecklicher Name ;)
      GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE),&info);
      return info.dwSize.X;
    }
#elif linux
    int getcols() {
      winsize w;
      ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
      return w.ws_col;
    }
#endif


    int kuerzelogdatei(const char* logdatei,int obverb)
    {
      // zutun: nicht erst in Vektor einlesen, sondern gleich in die tmp-Datei schreiben 10.6.12

      //	vector<string> Zeilen;   //Der Vektor Zeilen enthält String-Elemente
      char Zeile[256]; //Die maximale Zeilenlänge beträgt 255 Zeichen, weil ein String mit einer Null endet
      if (obverb>1) {
        cout<<"verbose: "<<(int)obverb<<Txk[T_kuerze_logdatei]<<drot<<logdatei<<schwarz<<endl;
      }
      //  Log(string("kuerzelogdatei: ") + drot + logdatei + schwarz,obverb,0);
      // ersetze(logdatei,"\\","\\\\")
      struct stat stat;
      if (lstat(logdatei,&stat)){
        if (obverb>1) {
          cout<<Txk[T_Logdatei]<<drot<<logdatei<<schwarz<<Txk[T_gibt_es_noch_nicht_Kuerze_sie_daher_nicht]<<endl;
        }
        return 1;
      }
      if (obverb>1) {
        cout<<rot<<"Logdatei: \n"<<schwarz; 
        system((string(dir) + "\"" + logdatei + "\"").c_str());
      }
      string ofil=string(logdatei)+"tmp";
      int abhier=0;
#ifdef obfstream	
      fstream outfile(ofil.c_str(),ios::out);
      if (!outfile) {
        perror((string("\nkuerzelogdatei: ")+Txk[T_Kann_Datei]+ofil+Txk[T_nicht_als_fstream_zum_Schreiben_oeffnen]).c_str());
        return 1;
      }
      fstream logf (logdatei, ios::in);  //Die Zeilen dieser Datei sollen in einen Vektor geschrieben werden.
      if (!logf) {
        perror((string("\nkuerzelogdatei: ")+Txk[T_Kann_Datei]+logdatei+Txk[T_nicht_als_fstream_zum_Lesen_oeffnen]).c_str());
        return 1;
      }
      while (logf.getline (Zeile, sizeof(Zeile))) {
        //		Zeilen.push_back(Zeile); //hängt einfach den Inhalt der Zeile als Vektorelement an das Ende des Vektors
#else	
        FILE *outfile = fopen(ofil.c_str(),"a");
        if (!outfile) {
          perror((string("\nkuerzelogdatei: ")+Txk[T_Kann_Datei]+ofil+Txk[T_nicht_mit_fopen_zum_Schreiben_oeffnen]).c_str());
          return 1;
        }
        FILE *logf;
        if (!(logf=fopen(logdatei,"r"))) {
          Log(string("\nkuerzelogdatei: ")+Txk[T_Kann_Datei]+logdatei+Txk[T_nicht_mit_fopen_zum_Lesen_oeffnen],1,0);
          return 1;
        }
        while (fgets(Zeile, sizeof Zeile, logf)) {
          //	 Zeilen.push_back(Zeile);
#endif	
          if (!abhier) {
            tm *atm = new tm; time_t gesz; long sekunden; // int aktz;
            //	for(aktz=Zeilen.size()-1;aktz>=0;aktz--) {
            //         Log(string("aktz=") + ltoa_(aktz,buffer,10),obverb,0);
            int verwertbar=0, index;
            for(unsigned j=0;j<2;j++) {
              if (verwertbar) {
                index = verwertbar-1;
                j=2;
              } else {
                index = j;
              }
              switch (index) {
                case 0: 
                  if (sscanf(Zeile,"%2d.%2d.%2d %2d:%2d:%2d%*s",&atm->tm_mday,&atm->tm_mon,&atm->tm_year,&atm->tm_hour,&atm->tm_min,&atm->tm_sec)==6) {
                    if (!verwertbar) {
                      verwertbar=1;
                      j=2;
                    }
                    atm->tm_mon--;
                    atm->tm_year+=100; // 2000-1900
                    //	  <<atm->tm_mday<<"."<<atm->tm_mon+1<<"."<<atm->tm_year<<"."<<atm->tm_hour<<"."<<atm->tm_min<<"."<<atm->tm_sec<<endl;
                    atm->tm_isdst=-1; // sonst wird zufällig ab und zu eine Stunde abgezogen
                  } else if (verwertbar) verwertbar=0;
                  break;
                case 1:
                  if (strptime(Zeile,"%a %b %d %T %Y", atm)) {
                    if (!verwertbar) {
                      verwertbar=2;
                      j=2;
                    } 
                  } else if (verwertbar) verwertbar=0;
              }
            }
            if (verwertbar) {
              gesz=mktime(atm);
              //          	  char tbuf[20];
              //              strftime(tbuf, 18,"%d.%m.%y %X",localtime(&gesz));
              //              <<"Datum: "<<tbuf<<endl;
              //              exit(0);
              time_t jetzt;
              jetzt=time(0);
              sekunden=(long)(jetzt-gesz);
              if (sekunden<1209600) {// jünger als zwei Wochen => behalten
                abhier=1;
              }
              //	  <<jetzt<<"- "<<gesz<<"="<<sekunden<<endl;
            } // if (sscanf(Zeile
            delete atm;
          } // (!abhier)
          if (abhier) {
#ifdef obfstream
            outfile<<Zeile<<endl;
          }
          }
          logf.close();
          outfile.close();
#else
          fputs(Zeile,outfile);
          //          fputs("\n",outfile);
        } // (abhier)
      } // while (fgets(Zeile
      fclose(logf);
      fclose(outfile);
#endif
      /*	
          Log(string("kuerzelogdatei, nach Einlesen"),obverb,0);	
          tm *atm = new tm; time_t gesz; long sekunden; int aktz;
          for(aktz=Zeilen.size()-1;aktz>=0;aktz--) {
      //         Log(string("aktz=") + ltoa_(aktz,buffer,10),obverb,0);
      if (sscanf(Zeilen[aktz].c_str(),"%2d.%2d.%2d %2d:%2d:%2d%*s", &atm->tm_mday,&atm->tm_mon,&atm->tm_year,&atm->tm_hour,&atm->tm_min,&atm->tm_sec)==6)
      {
      atm->tm_mon-=1;
      atm->tm_year+=100; // 2000-1900
      //	  <<atm->tm_mday<<"."<<atm->tm_mon+1<<"."<<atm->tm_year<<"."<<atm->tm_hour<<"."<<atm->tm_min<<"."<<atm->tm_sec<<endl;
      gesz=mktime(atm);
      //	  char tbuf[20];
      //      strftime(tbuf, 18,"%d.%m.%y %X",localtime(&gesz));
      //      <<tbuf<<endl;
      time_t jetzt;
      jetzt=time(0);
      sekunden=(long)(jetzt-gesz);
      if (sekunden>1209600) {// älter als zwei Wochen => abschneiden
      break;
      }
      //	  <<jetzt<<"- "<<gesz<<"="<<sekunden<<endl;
      }
      }
      Log(string("kuerzelogdatei, nach Datumspruefung"),obverb,0);	
      string ofil=string(logdatei)+"tmp";
#ifdef obfstream
fstream outfile(ofil.c_str(),ios::out);
if (!outfile) {
perror((string("\nkuerzelogdatei: Kann Datei '")+ofil+"' nicht als fstream zum Schreiben oeffnen.").c_str());
return 1;
} else {
for(int j=aktz+1;j<Zeilen.size();j++) {
outfile<<Zeilen[j]<<endl;
}
}
#else
logf = fopen(ofil.c_str(),"w");
if (!logf) {
perror((string("\nkuerzelogdatei: Kann Datei '")+ofil+"' nicht mit logf zum Schreiben oeffnen.").c_str());
return 1;
} else {
for(unsigned j=aktz+1;j<Zeilen.size();j++) {
fputs(Zeilen[j].c_str(),logf);
fputs("\n",logf);
} 
fclose(logf);
}
#endif
       */	
      if (abhier) {
        remove(logdatei);
        rename(ofil.c_str(),logdatei);
      }else{
        remove(ofil.c_str());
      }
return 0;
//  << "Alle Zeilen:" << endl;
// unsigned int ii; //unsigned, weil ansonsten Vergleich von signed- und unsigned-Werten. signed=vorzeichenbehaftet
// for(ii=0; ii < Zeilen.size(); ii++ddddddddds) {
//  << Zeilen[ii] << endl;
// }
//  << endl;
//************************************
}	


int Log(string text,short screen,short file,bool oberr,short klobverb)
{
  static unsigned int cols=0;
  static bool letztesmaloZ;
  const bool naechstezeile=0;
  // screen=0 = schreibt nicht auf den Bildschirm, 1 = schreibt, -1 = schreibt ohne Zeilenwechsel, -2 = schreibt bleibend ohne Zeilenwechsel
  // <<"Log: "<<text<<", screen: "<<screen<<", file: "<<file<<endl;
  if (file || screen) {
    if (screen) {
      if (!cols) cols=getcols();
      cout<<text;
      if (letztesmaloZ && (cols>=text.length())) {
        cout<<string(cols-text.length(),' ');
      }	
      if (screen==-2); else if (screen==-1 && !naechstezeile) {cout<<"\r";cout.flush();} else cout<<endl; 
      letztesmaloZ = (screen==-1);
    } // if (screen) 
    if (file) {
      if (!logdatei || !*logdatei) {
        cerr<<rot<<Txk[T_Variable_logdatei_leer]<<schwarz<<endl;
      } else {
        string zwi=text; 
        zwi= ersetze(zwi.c_str(),drot,""); // "\033[31m"
        zwi= ersetze(zwi.c_str(),schwarz,""); // "\033[0m"
        zwi= ersetze(zwi.c_str(),tuerkis,""); // "\033[0m"
        zwi= ersetze(zwi.c_str(),blau,""); // "\033[0m"
        zwi= ersetze(zwi.c_str(),rot,""); // "\033[0m"
        static bool erstaufruf=1;

        char tbuf[20];
        time_t jetzt=time(0);
        strftime(tbuf,19,"%d.%m.%y %X ",localtime(&jetzt));
#ifdef obfstream
        fstream logf;	
        if (erstaufruf) {
          kuerzelogdatei(logdatei,klobverb);  // screen
          //          Log("nach kuerzelogdatei",screen,0);
          //      logf.open(logdatei,ios::out);
          erstaufruf=0;
        } // else
        logf.open(logdatei,ios::out|ios::app);
        if (!logf) {
          perror((string("\nLog: ")+Txk[T_Kann_Datei]+logdatei+Txk[T_nicht_als_fstream_zum_Anhaengen_oeffnen]).c_str());
          return 1;
        } else {
          logf<<zwi<<endl; 
          logf.close();
        }
#else	
        FILE *logf;
        if (erstaufruf) {
          kuerzelogdatei(logdatei,klobverb); // screen
          //          Log("nach kuerzelogdatei",screen,0);
          erstaufruf=0;
        }	  
        if (!(logf=fopen(logdatei,"a"))) {
          //perror((string("\nLog: Kann Datei '")+logdatei+"' nicht mit fopen zum Anhaengen oeffnen.").c_str()); // ergebnisgleich wie:
          cerr<<"\nLog: "<<Txk[T_Kann_Datei]<<logdatei<<Txk[T_nicht_mit_fopen_zum_Anhaengen_oeffnen]<<strerror(errno)<<endl;
          return 1;
        } else {
          fputs(string(string(tbuf)+zwi).c_str(),logf);
          fputs("\n",logf);
          fclose(logf);
        }
#endif

      } // if (!logdatei || !*logdatei) _gKLA_ _gKLZ_ else _gKLA_
      if (oberr) {
        //      string hstrerror=strerror(errno); // da errno trotz richtiger Fallunterscheidung bei isatty(fileno(stdout)) gesetzt wird
        // wenn cerr woanders hingeht als cout oder die Meldung gar nicht an screen gerichtet ist, hier ohne Berücks.v.screen==-1
        if (isatty(fileno(stdout))!=isatty(fileno(stderr)) || !screen) {
          cerr<<text<<endl; // <<": "<<hstrerror<<endl;
        }
        errno=0;
      }
    } // (file)
  } // if (file || screen) 
  // <<"Screen: "<<screen<<"letztesmaloZ: "<<letztesmaloZ;
  // <<"und dann: "<<letztesmaloZ<<endl;
  return 0;
} // Log(string text)


#ifdef _MSC_VER
inline void wait () {
  cout<<Txk[T_Bitte_mit]<<_drot<<"return"<<_schwarz<<Txk[T_beenden];
  // Löscht etwaige Fehlerzustände die das Einlesen verhindern könnten
  cin.clear();
  // Ignoriert soviele Zeichen im Puffer wie im Puffer vorhanden sind
  // (= ignoriert alle Zeichen die derzeit im Puffer sind)
  cin.ignore(cin.rdbuf()->in_avail());
  // Füge alle eingelesenen Zeichen in den Puffer bis ein Enter gedrückt wird
  // cin.get() liefert dann das erste Zeichen aus dem Puffer zurück, welches wir aber ignorieren (interessiert uns ja nicht)
  cin.get();
} 
#endif

// braucht nur 1/300 von FindStringInBuffer
long cmpmem( char* feld, const char* search, int len_feld) // , int len_search
{
  /*#ifdef _DEBUG
    gettimeofday(&perfStart, 0); 
#endif
   */
  long i=0;
  int j=-1;
  int len_search=strlen(search);
  for(; i<len_feld-len_search; feld++,i++){
    j=memcmp(feld, search, len_search);
    if (!j) 
      break;
  }
  /*
#ifdef _DEBUG
gettimeofday(&perfEnd, 0); 
#endif
#ifdef _DEBUG
#ifdef _WIN32
perf2= (((double)(perfEnd-perfStart))/((double)freq)); 
#else
perf2= static_cast<double> (perfEnd.tv_sec * 1000000 + perfEnd.tv_usec- perfStart.tv_sec * 1000000 - perfStart.tv_usec) / 1000000;
#endif
#endif
   */
  return !j?i:-1;
  //    return (i == len_feld-len_search)?0:feld;  

}  

char* ltoan(long value, int base, uchar obtz, uchar minstel) {
  /**
   * C++ version 0.4 char* style "itoa":
   * Written by Lukás Chmela
   * Released under GPLv3.
   */
  const uchar resultlenge=30;
  static char result[resultlenge];
  // check that the base is valid
  if (base < 2 || base > 36) { *result = '\0'; return result; }

  char* ptr = result, *ptr1 = result, tmp_char;
  long tmp_value;

  uchar stelle=0,stellen=0;
  do {
    tmp_value = value;
    value /= base;
    *ptr++ = "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz" [35 + (tmp_value - value * base)];
    stellen++;
    if (obtz) if (value) if (++stelle==3) {*ptr++ ='.';stelle=0;stellen++;}
  } while ( value );
  while (stellen++<minstel && stellen<resultlenge) *ptr++ = ' ';

  // Apply negative sign
  if (tmp_value < 0) *ptr++ = '-';
  *ptr-- = '\0';
  while(ptr1 < ptr) {
    tmp_char = *ptr;
    *ptr--= *ptr1;
    *ptr1++ = tmp_char;
  }
  return result;
} // ltoa_(long value, char* result, int base)

char* ltoa_(long value, char* result, int base=10) {
  /**
   * C++ version 0.4 char* style "itoa":
   * Written by Lukás Chmela
   * Released under GPLv3.
   */
  // check that the base if valid
  if (base < 2 || base > 36) { *result = '\0'; return result; }

  char* ptr = result, *ptr1 = result, tmp_char;
  long tmp_value;

  do {
    tmp_value = value;
    value /= base;
    *ptr++ = "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz" [35 + (tmp_value - value * base)];
  } while ( value );

  // Apply negative sign
  if (tmp_value < 0) *ptr++ = '-';
  *ptr-- = '\0';
  while(ptr1 < ptr) {
    tmp_char = *ptr;
    *ptr--= *ptr1;
    *ptr1++ = tmp_char;
  }
  return result;
} // ltoa_(long value, char* result, int base)

void kopiere(string *quelle, string *ziel)
{
  ifstream fileIn(quelle->c_str(),ios::binary);
  ofstream fileOut(ziel->c_str(),ios::trunc | ios::binary);
  fileOut<<fileIn.rdbuf();
}

void aufSplit(vector<string> *tokens, const char *text, char sep, bool nichtdoppelt)
{
  string texts=text;
  aufSplit(tokens,&texts,sep,nichtdoppelt);
}

void aufSplit(vector<string> *tokens, const string *text, char sep,bool nichtdoppelt) 
{
  int start = 0, end = 0;
  tokens->clear();
  while ((end = text->find(sep, start)) != (int)string::npos) {
    if (nichtdoppelt || end!=start) tokens->push_back(text->substr(start, end - start));
    start = end + 1;
  }
  tokens->push_back(text->substr(start));
}

void aufiSplit(vector<string> *tokens, const string *text, const char* sep,bool nichtdoppelt) 
{
  int start = 0, end = 0;
  size_t len = strlen(sep);
  char* usep = new char[len];
  strcpy(usep,sep);
  for (char *p=usep ; *p; ++p) *p = toupper(*p);
  string ltext;
  transform(text->begin(),text->end(),std::back_inserter(ltext),::toupper);
  tokens->clear();
  while ((end = ltext.find(usep, start)) != (int)string::npos) {
    if (nichtdoppelt || end!=start) {
      tokens->push_back(text->substr(start, end - start));
    }
    start = end + len;
  }
  tokens->push_back(text->substr(start));
  delete[] usep;
}

void aufSplit(vector<string> *tokens, const string *text, char* sep,bool nichtdoppelt) 
{
  int start = 0, end = 0;
  int len = strlen(sep);
  tokens->clear();
  while ((end = text->find(sep, start)) != (int)string::npos) {
    if (nichtdoppelt || end!=start) tokens->push_back(text->substr(start, end - start));
    start = end + len;
  }
  tokens->push_back(text->substr(start));
}


int confdat::lies(string fname,uchar obverb)
{
  string zeile;
  ifstream f(fname.c_str());
  if (f.is_open()) {
    if (obverb) cout<<"confdat::lies(fname...), fname: "<<fname<<endl;
    while (getline(f,zeile)) {
      zn<<zeile;
    }
    obgelesen=1;
    return 0;
  }
  return 1;
}

void confdat::auswert(cppSchluess *conf, size_t csize, uchar obverb, char tz)
{
  richtige=0;
  if (obgelesen) {
    string scs=string(" %[^ ^")+tz+"\t#]"+tz+"[ ]";
    for(size_t i=0;i<zn.size();i++) {
      string *zeile=&zn[i];
      if (!zeile->empty()) {
        if (zeile->at(0)=='#') continue;
        if (obverb>1)
          Log(string(Txk[T_stern_zeile])+*zeile,obverb);
        size_t pos=zeile->find(tz);
        if (pos!=string::npos && 1==sscanf(zeile->c_str(),scs.c_str(),zeile->c_str())) {
          size_t i=csize;
          while( i-- ) 
            if( !strcmp(conf[i].name.c_str(),zeile->c_str()) ) {
              ++richtige;
              // <<"*zeile: '"<<blau<<*zeile<<schwarz<<"', pos: "<<pos<<", '"<<conf[i].name<<"'"<<endl;
              conf[i].wert=zeile->substr(pos+1);
              gtrim(&conf[i].wert);
              if (conf[i].wert.length()) if (conf[i].wert[0]==conf[i].wert[conf[i].wert.length()-1] && strchr("\"'",conf[i].wert[0])) {
                conf[i].wert.erase(conf[i].wert.length()-1);
                conf[i].wert.erase(0,1);
              }
            }
        }
      }
    }
  }
}

confdat::confdat(string fname,uchar obverb)
{
  lies(fname,obverb);
}

confdat::confdat(string fname, cppSchluess *conf, size_t csize, uchar obverb, char tz)
{
  lies(fname,obverb);
  auswert(conf,csize,obverb,tz);
}
/*
   int cpplies(string fname,cppSchluess *conf,size_t csize,vector<string> *rest,char tz,short obverb)
   {
   int r=0;
   string zeile;
   ifstream f(fname.c_str());
   if (f.is_open()) {
   if (obverb) cout<<"cpplies(fname,Schluessel...), fname: "<<fname<<endl;
   string scs=string(" %[^ ^")+tz+"\t#]"+tz+"[ ]";
   while(getline(f,zeile)) {
   if (zeile[0]=='#') continue;
   if (obverb)
   cout<<"Zeile: "<<zeile<<endl;
   size_t pos=zeile.find(tz);
   if (pos!=string::npos && 1==sscanf(zeile.c_str(),scs.c_str(),zeile.c_str())) {
   size_t i=csize;
   while( i-- ) 
   if( !strcmp(conf[i].name.c_str(),zeile.c_str()) ) {
   ++r;
// <<"zeile: '"<<blau<<zeile<<schwarz<<"', pos: "<<pos<<", '"<<conf[i].name<<"'"<<endl;
conf[i].wert=zeile.substr(pos+1);
gtrim(&conf[i].wert);
if (conf[i].wert.length()) if (conf[i].wert[0]==conf[i].wert[conf[i].wert.length()-1] && strchr("\"'",conf[i].wert[0])) {
conf[i].wert.erase(conf[i].wert.length()-1);
conf[i].wert.erase(0,1);
}
}
} else {
if (rest) rest->push_back(zeile);
if (obverb) cout<<"kein Schluesselbegriff: "<<zeile<<endl;
}
}
}
return r;
} // cpplies
 */

/*
// header: int lies(const char *fname,Schluessel *conf,size_t csize,vector<string> *rest=0,char tz='=',short obverb=0);
int liesalt(const char *fname,Schluessel *conf,size_t csize,vector<string> *rest,char tz,short obverb)
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
size_t i=csize;
while( i-- ) 
if( !strcmp(conf[i].key,zeile) ) {
++r;
do ++p; while (*p==' '); // Leerzeichen am Anfang entfernen
if (*p=='"' && p[strlen(p)-1]=='"') {p++;p[strlen(p)-1]=0;}
sprintf(conf[i].val,"%.*s",(int)sizeof conf->val-1,p);
}
} else {
if (rest) rest->push_back(zeile);
if (obverb) cout<<"kein Schluesselbegriff: "<<zeile<<endl;
}
}
fclose(f);
return r;
} // lies(const char 

int liesalt(string fname,Schluessel *conf,size_t csize,vector<string> *rest,char tz,short obverb) {
return liesalt(fname.c_str(), conf, csize, rest, tz, obverb);
}
 */

string XOR(string value,string key)
{
  string retval(value);
  short unsigned int klen=key.length(), vlen=value.length(), k=0, v;
  for(v=0;v<vlen;v++)
  {
    retval[v]=value[v]^key[k];
    k=(++k<klen?k:0);
  }
  return retval;
}

int cppschreib(string fname, cppSchluess *conf, size_t csize)
{
  ofstream f(fname.c_str());
  if (f.is_open()) {
    for (size_t i = 0;i<csize;i++) {
      f<<conf[i].name<<" = \""<<conf[i].wert<<"\""<<endl;
    }

    return 0;
  }
  return 1;
}

int multicppschreib(string fname, cppSchluess **conf, size_t *csizes, size_t cszahl)
{
  ofstream f(fname.c_str());
  if (f.is_open()) {
    for (size_t j=0;j<cszahl;j++) {
      for (size_t i = 0;i<csizes[j];i++) {
        f<<conf[j][i].name<<" = \""<<conf[j][i].wert<<"\""<<endl;
      }
    }
    return 0;
  }
  return 1;
}

int schreib(const char *fname, Schluessel *conf, size_t csize)
{
  FILE *f=fopen(fname,"wt");
  if (!f) return 1;
  for (size_t i = 0;i<csize;i++) {
    fprintf(f,"%s = \"%s\"\n",conf[i].key,conf[i].val);
  }
  fclose(f);
  return 0;
}

std::string base_name(std::string const & path)
{
  return path.substr(path.find_last_of("/\\") + 1);
}

std::string dir_name(std::string const & path)
{
  return path.substr(0,path.find_last_of("/\\"));
}

int systemrueck(string cmd, char obverb, uchar oblog, vector<string> *rueck, binaer ob0heissterfolg, binaer obergebnisanzeig, string ueberschr)
{
  uchar neurueck=0;
  uchar weiter=0;
  int erg;
  string meld(Txk[T_Rueckmeldung]);
  if (ueberschr.empty()) { 
    if (cmd.substr(0,5)=="find ") {
      ueberschr=Txk[T_Suchbefehl];
    } else {
      ueberschr=Txk[T_Fuehre_aus];
      weiter=1;
    }
  }
  Log(ueberschr+": "+blau+cmd+schwarz+" ...",obverb>0?-1:0,oblog);
  if (!rueck) if (obergebnisanzeig) {neurueck=1;rueck=new vector<string>;}
  if (rueck) {
    if (FILE* pipe = popen(cmd.c_str(), "r")) {
      while(!feof(pipe)) {
        char buffer[1280];
        // in der folgenden Zeile werden auch ggf. sterr-Meldungen ausgegeben/weitergeleitet
        if(fgets(buffer, sizeof buffer, pipe) != NULL){
          size_t posi;
          if (buffer[posi=strlen(buffer)-1]==10) buffer[posi]=0;
          rueck->push_back(string(buffer));
        } // if(fgets(buffer, sizeof buffer, pipe) != NULL)
      } // while(!feof(pipe)) 
      /* if (obverb>1 || oblog) */ if (rueck->size()) {
        for(unsigned i=0;i<rueck->size();i++)
          meld=meld+"\n"+tuerkis+rueck->at(i)+schwarz;
      }
      erg = pclose(pipe)/256;
    } else {
      perror((string("popen() ")+Txk[T_fehlgeschlagen_bei]+cmd).c_str());
      erg=1;
    }
  } else {
    erg= system(cmd.c_str());
  }
  if (weiter) ueberschr=Txk[T_Fuehrte_aus];
  string ergebnis;
  if (ob0heissterfolg) {
    if (erg) {
      ergebnis=rots+string(Txk[T_Fehler])+ltoan(erg)+schwarz;
      if (obverb>=0) obergebnisanzeig=wahr;
      obverb++;
    } else {
      ergebnis=Txk[T_Erfolg];
    }
  } else {
    ergebnis=ltoan(erg);
  }
  Log(ueberschr+": "+blau+cmd+schwarz+Txk[T_komma_Ergebnis]+blau+ergebnis+schwarz,obverb>0?obverb:0,oblog);
  if (obergebnisanzeig) if (rueck->size()) 
    Log(meld,obverb>1,oblog);
  if (neurueck) delete rueck;
  return erg; 
} // systemrueck

string aktprogverz()
{
  char pBuf[300];
#if defined(__MINGW32__) || defined(_MSC_VER)
  int bytes = GetModuleFileName(NULL, pBuf, sizeof pBuf);
  if(bytes == 0) pBuf[0]=0;
#elif linux
  char szTmp[32];
  sprintf(szTmp, "/proc/%d/exe", getpid());
  int bytes = MIN(readlink(szTmp, pBuf, sizeof pBuf), sizeof pBuf - 1);
  if(bytes >= 0) pBuf[bytes] = 0;
#endif
  return string(pBuf);
} // string aktprogverz()

char holbuchst(string frage, const char* moegl,const char *erkl[], const char* erlaubt, const char *vorgabe) 
{
  return holbuchst(frage.c_str(), moegl,erkl, erlaubt, vorgabe);
}

char holbuchst(string frage, string moegl,const char *erkl[], const char* erlaubt, const char *vorgabe) 
{
  return holbuchst(frage.c_str(), moegl.c_str(), erkl, erlaubt, vorgabe);
}

char holbuchst(const char *frage, string moegl,const char *erkl[], const char* erlaubt, const char *vorgabe) 
{
  return holbuchst(frage, moegl.c_str(), erkl, erlaubt, vorgabe);
}

char holbuchst(const char *frage, const char* moegl,const char *erkl[], const char* erlaubt, const char *vorgabe) 
{
  string input;
  if (!erlaubt) erlaubt=moegl;
  while(1) {
    cout<<blau<<frage<<schwarz<<" (";
    for(unsigned i=0;i<strlen(moegl);i++) {
      cout<<"'"<<drot<<moegl[i]<<schwarz<<"'";
      if (erkl) cout<<" = "<<blau<<erkl[i]<<schwarz;
      if (i<strlen(moegl)-1) cout<<", ";
    }
    cout<<")"<<(!vorgabe?"":string("['")+tuerkis+vorgabe+schwarz+"']")<<"?: ";
    input="";
    getline(cin,input);
    if (cin.fail()) { cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n'); }
    if (input=="" && vorgabe) {input=vorgabe;break;}
    if (input[0]) if (strchr(erlaubt,(int)input[0])) break;
  }
  return input[0];
} // holbuchst

// bisher nicht verwendet, 15.12.15
string holstrings(const char *frage, char* moegl[], char *vorgabe)
  // das letzte Element von moegl muss 0 sein
{
  string input;
  while(1) {
    char passt=0;
    cout<<blau<<frage<<schwarz<<" (";
    for(unsigned i=0;moegl[i];i++) {
      if (i) cout<<", ";
      cout<<"'"<<drot<<moegl[i]<<schwarz<<"'";
    }
    cout<<")"<<(vorgabe?"":string("['")+tuerkis+vorgabe+schwarz+"']")<<"?: ";
    input="";
    getline(cin,input);
    if (cin.fail()) { cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n'); }
    if (input=="" && vorgabe) {input=vorgabe;break;}
    if (input[0]) {
      for(unsigned i=0;moegl[i];i++) {
        if (!strcmp(moegl[i],input.c_str())) {passt=1;break;}
      }
    }
    if (passt) break; 
  }
  return input;
} // holstrings

string holstrings(const char *frage, vector<string> *moegl, string *vorgabe)
{
  string input;
  while(1) {
    char passt=0;
    cout<<blau<<frage<<schwarz<<" (";
    for(unsigned i=0;i<moegl->size();i++) {
      if (i) cout<<",";
      //      cout<<"'"<<drot<<moegl->at(i)<<schwarz<<"'";
      cout<<drot<<moegl->at(i)<<schwarz;
    }
    cout<<")"<<(vorgabe->empty()?"":string("['")+tuerkis+*vorgabe+schwarz+"']")<<"?: ";
    input="";
    getline(cin,input);
    if (cin.fail()) { cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n'); }
    if (input.empty() && !vorgabe->empty()) {input=*vorgabe;break;}
    if (input[0]) {
      for(unsigned i=0;i<moegl->size();i++) {
        if (moegl->at(i)==input) {passt=1;break;}
      }
    }
    if (passt) break; 
  }
  return input;
} // holstrings


string holzahl(const char *frage, const char *vorgabe) 
{
  string input;
  while(1) {
    cout<<blau<<frage<<schwarz<<(!vorgabe?"":string(" [")+tuerkis+vorgabe+schwarz+"]")<<"? ";
    input="";
    getline(cin,input);
    if (cin.fail()) { cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n'); }
    if (input=="" && vorgabe) {input=vorgabe;break;}
    if (isnumeric(input)) break;
  }
  return input;
} // holzahl

string holzahl(const char *frage, string *vorgabe) 
{
  return holzahl(frage,(vorgabe?vorgabe->c_str():0)); 
}

char* holcstring(const char *frage, char* buf, unsigned long buflen, const char* vorgabe) 
{
  string vstr=string(vorgabe);
  strncpy(buf,holstring(string(frage),&vstr).c_str(),buflen-1);
  buf[buflen-1]=0;
  return buf;
} // holcstring

string holstring(const char *frage, string *vorgabe) 
{
  return holstring(string(frage), vorgabe);
} // holstring

string holstring(string frage, string *vorgabe) 
{
  string input;
  while(1) {
    cout<<blau<<frage<<schwarz<<(!vorgabe || vorgabe->empty()?"":string(" [\"")+tuerkis+*vorgabe+schwarz+"\"]")<<"? ";
    input="";
    getline(cin,input);
    if (cin.fail()) { cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n'); }
    if (input=="" && vorgabe) {input=*vorgabe;break;}
    break;
  }
  return input;
} // holstring

string holverz(const char *frage,string *vorgabe) 
{
  string input, vg2="n"; uchar fertig=0;
  while(1) {
    cout<<blau<<frage<<schwarz<<(!vorgabe || vorgabe->empty()?"":string(" [\"")+tuerkis+*vorgabe+schwarz+"\"]")<<"? ";
    input="";
    getline(cin,input);
    if (cin.fail()) { cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n'); }
    if (input=="" && vorgabe) {input=*vorgabe;}
    struct stat st;
    //    <<"input: '"<<rot<<input<<schwarz<<"'"<<endl;
    while (1) {
      if (!lstat(input.c_str(), &st)) {
        if(S_ISDIR(st.st_mode)) {
          fertig=1;
          break;
        } else {
          cout<<"'"<<rot<<input<<schwarz<<Txk[T_kein_Verzeichnis_nochmal];
          break;
        }
      } else {
        string inpi;
        cout<<"'"<<rot<<input<<schwarz<<Txk[T_nicht_gefunden_soll_ich_es_erstellen]<<drot<<"j"<<schwarz;
        cout<<"/"<<drot<<"n"<<schwarz<<")(\""<<tuerkis<<vg2<<schwarz<<"\")?";
        getline(cin,inpi);
        if (cin.fail()) { cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n'); }
        if (inpi=="") {inpi=vg2;break;}
        if (strchr("jyJY",inpi[0])) {
          system((string("mkdir -p ")+input).c_str());
        } else {
          break;
        }
      }
    }
    if (fertig) break;
  }
  return input;
} // holverz

uchar VerzeichnisGibts(const char* vname)
{
  DIR *verz;
  if ((verz=opendir(vname))){
    closedir(verz);
    return 1;
  }
  return 0;
} // VerzeichnisGibts

int optioncl::pruefp(int argc, char** argv, int akt, uchar *hilfe) // 1 = das war der Parameter, 0 = nicht
{
  uchar trifft=0;
  if (akt<argc) if (argv[akt]) {
    if (strlen(argv[akt])>1) {
      if (argv[akt][0]=='-'||argv[akt][0]=='/') {
        if (!strcmp(&(argv[akt][1]),kurz.c_str())) trifft=1; 
      }
      if (!trifft) if (strlen(argv[akt])>2) if (argv[akt][0]=='-'&&argv[akt][1]=='-') 
        if (!strcmp(&(argv[akt][2]),lang.c_str()))
          trifft=1;
    }
    if (trifft) {
      if (pptr) {
        *pptr=wert;
      } else {
        int hakt=akt+1;
        char *pstr=0;
        uchar falsch=0;
        if (akt<argc-1) if (argv[hakt]) {
          struct stat entryarg;
          switch (art) {
            case sons:
              if (!strchr("-/",argv[hakt][0])) {
                pstr=argv[hakt];
              }
              break;
            case verz:
            case file:
              if (argv[hakt][0]!='-') {
                if (stat(argv[hakt],&entryarg)) falsch=1; 
                else if ((art==verz)^(S_ISDIR(entryarg.st_mode))) falsch=2;
                else pstr=argv[hakt];
              }
              break;
            case zahl:
              if (!strchr("-/",argv[hakt][0])) {
                if (!isnumeric(argv[hakt])) falsch=1;
                else pstr=argv[hakt];
              }
              break;
          }
        }
        if (pstr) {
          *zptr=pstr; 
        } else {
          switch (art) {
            case sons:
              Log(drots+Txk[T_Fehlender_Parameter_string_zu]+kurz+Txk[T_oder]+lang+"!"+schwarz,1,1);
              break;
            case verz:
            case file:
              Log(drots+Txk[T_Fehler_Parameter]+kurz+Txk[T_oder]+lang+" "+(falsch==1?Txk[T_ohne]:falsch==2?
                    Txk[T_mit_Datei_als]:Txk[T_mit_falschem])+Txk[T_Pfad_angegeben]+schwarz,1,1);
              break;
            case zahl:
              Log(drots+(falsch==1?Txk[T_Nicht_numerischer]:Txk[T_Fehlender])+Txk[T_Parameter_nr_zu]+kurz+Txk[T_oder]+lang+"!"+schwarz,1,1);
              break;
          }
          *hilfe=1;
        }
      }
      return 1;
    }
  }
  return 0;
} // pruefp

void optioncl::hilfezeile()
{
  if (!erkl.empty()) {
    cout<<drot<<" -"<<kurz<<", --"<<lang;
    if (zptr) {if (art==sons || art==file) cout<<" <string>"; else if (art==verz) cout<<" <"<<Txk[T_pfad]<<">"; else cout<<" <zahl>";}
    cout<<schwarz<<": "<<erkl;
    if (zptr) cout<<" '"<<drot<<*zptr<<schwarz<<"'";
    cout<<endl;
  }
} // hilfezeile
