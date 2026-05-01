#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif
//#pragma once
// Visual c++:
// Project->Properties->C++->general->additional include directories
// C:\Programme\MySQL\Connector C 6.0.2\include
// mingw: 
// -I"C:\Programme\MySQL\Connector C 6.0.2\include"
#ifndef DB_H_DRIN
#define DB_H_DRIN
#include <vector>
#include <string>
#include <iostream> // fuer cout ggf.
#include <sstream>
// Project->Properties->C/C++ ->General->Additional Include Directories->"$(ProjectDir)\..\..\..\MySQL Connector C 6.0.2\include"
#include <mysql.h> 
// G.Schade 26.1.14: Den Rest aus der Datei braucht man scheinbar nicht; sonst Konflikt mit bitset unter -std=gnu++11
#ifndef HAVE_UCHAR
typedef unsigned char	uchar;	/* Short for unsigned char */
#endif
// #include "/usr/include/mysql/my_global.h"
// #include <my_global.h> 
using namespace std;
#ifdef linux
#include <string.h>
#endif
// Project->Properties->Linker->general->(mitte)->additional library directories:
// C:\Programme\MySQL\Connector C 6.0.2\lib\opt
// weiterhin: libmysql.dll ins Verzeichnis der Exe stellen
#ifdef _MSC_VER
//#pragma comment(lib, "libmysql.lib")
#endif

enum Txdb_ {
  T_DB_wird_initialisiert,
  T_Fehler_db,
  T_beim_Initialisieren_von_MySQL,
  T_Fehler_dp,
  T_bei_Befehl,
  T_Versuche_mysql_zu_starten,
  T_MySQL_erfolgreich_gestartet,
  T_Versuche_Datenbank,
  T_zu_erstellen,
  T_Fehler_beim_Verbinden,
  T_Erfolg_beim_Initialisieren_der_Verbindung_zu_mysql,
  T_Benoetige_MySQL_Passwort_fuer_Benutzer_root_fuer_Befehl,
  T_Das_MySQL_Passwort_ist_leer_Wollen_Sie_eines_festlegen,
  T_j,
  T_Bitte_geben_Sie_ein_MySQL_Passwort_fuer_Benutzer_root_ein,
  T_Bitte_geben_Sie_das_MySQL_Passwort_fuer_Benutzer_root_erneut_ein,
  T_Fuehre_aus_db,
  T_falsche_Fehlernr,
  T_bei_der_Abfrage_der_Spaltenlaenge_bei_Tabelle,
  T_und_Feld,
  T_mit,
  T_bei_Abfrage,
  T_Fehler_beim_Pruefen_auf_Vorhandensein_des_Datensatzes,
  T_Programmfehler,
  T_nicht_anfangen_bei_isql_empty_Aufruf_von_RS_insert_beim_ersten_Mal_ohn_anfangen_bei,
  T_dbMAX,
};

class Txdbcl
{
  public:
    Sprache lgn; // Sprache numerisch
    static const char *TextC[][2];
    inline const char* operator[](Txdb_ const& nr) const {
      return TextC[nr][lgn];
    }
};


//
enum DBSTyp {MySQL, Postgres};

class sqlft: public string {
  private:
    //	char dbuf[21];
    string *ersetze(const char* alt, const char* neu);
    string *sersetze( string src, string const& target, string const& repl);
    void druckeein(DBSTyp eDBS, tm *zt);
  public:
    //	string feld;
    sqlft(DBSTyp eDBS, string* vwert);
    sqlft(DBSTyp eDBS, char* vwert,char* zs);
    sqlft(DBSTyp eDBS, char* vwert,bool obzahl=0);
    sqlft(DBSTyp eDBS, time_t *zt);
    sqlft(DBSTyp eDBS, tm *zt);
    sqlft(DBSTyp eDBS, char c);
    sqlft(DBSTyp eDBS, uchar c);
    sqlft(DBSTyp eDBS, int i);
    sqlft(DBSTyp eDBS, long int i);
    sqlft(DBSTyp eDBS, unsigned int i);
    sqlft(DBSTyp eDBS, unsigned long int i);
    sqlft(DBSTyp eDBS, long long int i);
};

template<typename T, size_t N> T * end(T (&ra)[N]) {
  return ra + N;
}

class instyp {
  private:
    //	char dbuf[21];
    inline string ersetze(const char *u, const char* alt, const char* neu);
    inline string sersetze( string src, string const& target, string const& repl);
  public:
    string feld;
    string wert;
    unsigned char obkeinwert; // bei update wird <wert> nicht als Wert, sondern als Feld o.ae. verwendet (z.B. update xy set altdatum = datum)
    template <typename tC> explicit instyp (DBSTyp eDBS, char* vfeld, tC vwert) {
      feld=vfeld;
      wert=sqlft(eDBS,vwert);
      obkeinwert=0;
    }
    template <typename tC> explicit instyp (DBSTyp eDBS, const char* vfeld, tC vwert) {
      feld=vfeld;
      wert=sqlft(eDBS,vwert);
      obkeinwert=0;
    }
    void init(){feld="";wert="";obkeinwert=0;}
    instyp (DBSTyp eDBS, char* vfeld, char *vwert) {
      feld=vfeld;
      wert=sqlft(eDBS,vwert,false);
      obkeinwert=0;
    }
    instyp (DBSTyp eDBS, char* vfeld, char *vwert,char* zs) {
      feld=vfeld;
      wert=sqlft(eDBS,vwert,zs);
      obkeinwert=0;
    }
    instyp (DBSTyp eDBS, char* vfeld, char *vwert,bool obzahl) {
      feld=vfeld;
      wert=sqlft(eDBS,vwert,obzahl);
      obkeinwert=0;
    }
    instyp (DBSTyp eDBS, const char* vfeld, const char *vwert,unsigned char vobkeinwert) {
      feld=vfeld;
      wert=vwert;
      obkeinwert=vobkeinwert;
    }
};
// delimiter value begin
inline char dvb(DBSTyp DBS) {
  switch(DBS) {
    case MySQL: return '\'';
    case Postgres: return '\"';
    default: return '"';
  }
}
// delimiter value end
inline char dve(DBSTyp DBS) {
  switch(DBS) {
    case MySQL: return '\'';
    case Postgres: return '\"';
    default: return '"';
  }
}

// delimiter name begin
inline char dnb(DBSTyp DBS) {
  switch(DBS) {
    case MySQL: return '`';
    case Postgres: return '\"';
    default: return '"';
  }
}
// delimiter name end
inline char dne(DBSTyp DBS) {
  switch(DBS) {
    case MySQL: return '`';
    case Postgres: return '\"';
    default: return '"';
  }
}

class Feld {
  public:
    string name;
    string typ;
    string lenge;
    string prec;
    string comment;
    bool obind;
    bool obauto;
    bool nnull;
    string defa;
    Feld(string vname, string vtyp="", string vlenge="", string vprec="", string vcomment="", bool vind=0, bool vobauto=0, bool vnnull=0, string vdefa="");
};

class Index {
  public:
    string name;
    int feldzahl;
    Feld *felder;
    Index(string vname, Feld *vfelder, int vfeldzahl);
};

class Tabelle {
  public:
    string name;
    string comment;
    Feld *felder;
    int feldzahl;
    Index *indices;
    int indexzahl;
    string engine;
    string charset;
    string collate;
    string rowformat;
    Tabelle(string vname,Feld *vfelder, int vfeldzahl, Index *vindices, int vindexzahl,string vcomment="",
        string engine="InnoDB",string charset="latin1",string collate="latin1_german2_ci",string rowformat="DYNAMIC");
};

class RS;

class DB {
  public:
    MYSQL *conn;
    //	MYSQL_RES *result;
    //	MYSQL_ROW row;
    DBSTyp DBS;
    char dnb; // delimiter name begin
    char dne; // delimiter name end
    char dvb; // delimiter value begin
    char dve; // delimiter value end
    //	char* ConStr;
    //  const char* db;
    string db;
    //  const char* host;
    string host;
    //  const char* user;
    string user;
    string passwd;
    string myloghost; // einheitliche Benennung von 'localhost' bzw. '%'
    string rootpwd; // root-Passwort
    unsigned int fehnr;
    const char* ConnError;
    string sql;
    uchar miterror;
    vector<string> myr;
    string cmd;
    RS *spalt=nullptr;
    char **spnamen=nullptr,**splenge=nullptr;
    int usedb(string db);
    void pruefrpw(string wofuer, unsigned versuchzahl);
    void setzrpw();
    my_ulonglong arows;
    vector< vector<instyp> > ins;
    void erweitern(const char* tab, vector<instyp> einf,uchar obstumm,uchar obsammeln=0, const unsigned long *maxl=0);
    uchar tuerweitern(const char* tab, string feld,long wlength,uchar obstumm);
    //	DB(DBSTyp DBS, const char* host, const char* user,const char* passwd, const char* db, unsigned int port, const char *unix_socket, unsigned long client_flag);
    DB();
    DB(DBSTyp nDBS, const char* phost, const char* user,const char* ppasswd, const char* uedb="", unsigned int port=0, const char *unix_socket=NULL, unsigned long client_flag=0,uchar obverb=0,uchar oblog=0,int versuchzahl=10);
    DB(DBSTyp nDBS, const char* phost, const char* user,const char* ppasswd, const char* prootpwd, const char* uedb="", unsigned int port=0, const char *unix_socket=NULL, unsigned long client_flag=0,uchar obverb=0,uchar oblog=0,int versuchzahl=10);
    DB(DBSTyp nDBS, string phost, string puser,string ppasswd, string uedb, unsigned int port, const char* unix_socket, unsigned long client_flag,
        uchar obverb,uchar oblog,int versuchzahl=10);
    void init(DBSTyp nDBS, const char* phost, const char* user,const char* ppasswd, const char* uedb="", unsigned int port=0, const char *unix_socket=NULL, unsigned long client_flag=0,uchar obverb=0,uchar oblog=0,unsigned versuchzahl=10);
    ~DB(void);
    /*
       int Abfrage(const char* sql,const char** erg=(const char**)&"", uchar obstumm=0);
       int Abfrage(string sql,const char** erg=(const char**)&"", uchar obstumm=0);
       int AbfragemE(const char* sql,const char** erg=(const char**)&"", uchar obstumm=0); // mit Ende
       int AbfragemE(string sql,const char** erg=(const char**)&"", uchar obstumm=0);      // mit Ende
     */
    //	int insert(const char* tab, vector<instyp> einf, const char** erg=(const char**)&"",uchar anfangen=1,uchar sammeln=0);
    //	void AbfrageEnde();
    void LetzteID(string* erg);
    char* tmtosql(tm *tmh,char* buf);
    char* tmtosqlmZ(tm *tmh,char* buf);
    //	char** HolZeile();
    my_ulonglong affrows(); // unsigned __int64
    void lesespalten(Tabelle *tab);
    int prueftab(Tabelle *tab,bool verbose=0);
};


class RS {
  public:
    DB* db;
    string sql;
    string isql; // insert-sql
    uchar obfehl;
    string fehler;
    unsigned int fnr;
    MYSQL_RES *result;
    unsigned long *lengths;
    MYSQL_ROW row;
    unsigned int num_fields;
    unsigned long long  num_rows;
    string table;
    vector<string> feld;
    vector<string> typ;
    vector<long> lenge;
    vector<long> prec;
    vector<string> kommentar;
    RS(DB* pdb);
    char*** HolZeile();
    void weisezu(DB* pdb);
    void clear();
    template<typename sT> 
      int Abfrage(sT psql,uchar obstumm=0){
        int erg=0;
        this->sql=psql;
        if (!sql.empty()) {
          erg = doAbfrage(obstumm);
        }
        return erg;
      }

    RS(DB* pdb,const char* psql, uchar obstumm=0);
    RS(DB* pdb,string psql, uchar obstumm=0);
    RS(DB* pdb,stringstream psqls, uchar obstumm=0);
    ~RS();
    void update(const char* tab, vector<instyp> einf,uchar obstumm,string *bedingung);
    void insert(const char* tab, vector<instyp> einf, uchar anfangen=1,uchar sammeln=0,uchar obstumm=0,string *id=0,uchar eindeutig=0);
  private:
    int doAbfrage(uchar obstumm);
};

//string ersetze(const char *u, const char* alt, const char* neu);
#endif // DB_H_DRIN
