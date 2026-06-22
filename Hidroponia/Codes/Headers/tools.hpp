#include "conector.hpp"
#include "gpio_test.h"
#include "libs.hpp"

using namespace std;

//sistema
struct hilo{
        string name;
        pid_t tid;
        };
struct sistema{
	int id,id_plan,parent;
	bool estado;
	string name;
	};
void load_sistema(sistema &Sistema,int id);
bool running(string name);
int getnum(string mess);
struct hora{
	int ms,dy,hrs,min,seg;};
void ultimoriego(hora &H,int system);
void startriego(int system);
void lastmeasure(hora &H,int system,string type);
void kill(int sys,string name);
//---------------SENSORES-----------------
struct sensor{
	int id,muestreo,pin,sys;
	float valor;
	string name;
        };
int getindexs(sensor S[],string tipo,int numsens);
void setsensor(sensor &S,string nombre);
void infosensor(sensor &S);
void load_sensors(sensor Sensors[],int system);
int contar_sensores(int system);
int contar_sensores_total();
//--------------FIN SENSORES-----------------

//------------------ACTUADORES--------------------
struct actuador{
	int id,pin,seg,sys;
	string name;
	};
int getindexa(actuador A[],string tipo,int numact);
int count_actors(int system);
int count_actors_total();
void load_actors(actuador Actors[],int system);
void setactuador(struct actuador &A,string nombre);
void infoactuador(struct actuador &A);
//------------FIN--ACTUADORES-----------

//objeto con informacion del planificador
class planificador{
	public:
		string cultivo;
		int id,ran_amb,id_riego,id_planificador;
		double phmax,phmin,hmax,hmin,tamb,pnor,tnor,Tnor,pmas,tmas,Tmas,pmen,tmen,Tmen;
		void info();
		planificador(int plan_id);
	};

