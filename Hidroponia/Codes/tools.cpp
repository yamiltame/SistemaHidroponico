#include "Headers/tools.hpp"
#include <sys/time.h>//libreria encargada del manejo de times slepp,usleep
#define ADC_IF_PATH "/proc/" //Definimos los procesos que se busca --
#define ADC_IF_FILE "adc"    //--para la interaccion de muestro microcontrolador

using namespace std;

bool running(string name){
        char buffer;
        string check,pid;
        string cmd="pidof "+name+" > /home/Hidroponia/Carpeta/pid";
        system(cmd.c_str());
        int file=open("/home/Hidroponia/Carpeta/pid",O_RDONLY);
        int bol=read(file,&buffer,1);
        close(file);
        if(bol==0){
                return false;
                }
        else{
                return true;
                }
        }

void startriego(int system){
	int id;
	conector cc;
	cc.conectar();
	cc.res=cc.stmt->executeQuery("select count(*) from monitoreo_actuador where id_actuador=(select id from actuador where tipo=\"bomba_agua\" and sistema="+to_string(system)+") and accion=\"0\" order by id_monitoreo desc limit 1;");
	cc.res->next();
	if(cc.res->getInt(1)==0){
			cc.res=cc.stmt->executeQuery("select id from actuador where tipo=\"bomba_agua\" and sistema="+to_string(system));
			cc.res->next();
			id=cc.res->getInt(1);
			cc.stmt->execute("insert into monitoreo_actuador(fecha,hora,valor,id_actuador,accion) values('2018-01-01','00:00:00',0,"+to_string(id)+",0);");
			}
	cc.borrar();
	}

void ultimoriego(hora &H,int system){
	int i;
        conector cc;
        string timer,date,month,day,hrs,min,seg;
        month=day=hrs=min=seg="";
        cc.conectar();
	string sql="select fecha,hora from monitoreo_actuador where id_actuador=(select id from actuador where tipo=\"bomba_agua\" and sistema="+to_string(system)+") and accion=\"0\" order by fecha desc limit 1;";
        cc.res=cc.stmt->executeQuery(sql);
        cc.res->next();
        date=cc.res->getString("fecha");
	timer=cc.res->getString("hora");
	for(i=0;i<2;i++){hrs+=timer[i];}
	for(i=3;i<5;i++){min+=timer[i];}
	for(i=6;i<8;i++){seg+=timer[i];}
	for(i=5;i<7;i++){month+=date[i];}
	for(i=8;i<10;i++){day+=date[i];}
	H.ms=stoi(month); H.dy=stoi(day); H.hrs=stoi(hrs); H.min=stoi(min); H.seg=stoi(seg);
	cc.borrar();
	}

void lastmeasure(hora &H,int system,string type){
	int i;
        conector cc;
        string timer,date,month,day,hrs,min,seg;
        month=day=hrs=min=seg="";
        cc.conectar();
	string sql="select fecha,hora from monitoreo_sensor where id_sensor=(select id from sensor where tipo='"+type+"' and sistema=(select parent from sistema where id="+to_string(system)+")) order by id_monitoreo desc limit 1;";
        cc.res=cc.stmt->executeQuery(sql);
        cc.res->next();
        date=cc.res->getString(1);
	timer=cc.res->getString(2);
	for(i=0;i<2;i++){hrs+=timer[i];}
	for(i=3;i<5;i++){min+=timer[i];}
	for(i=6;i<8;i++){seg+=timer[i];}
	for(i=5;i<7;i++){month+=date[i];}
	for(i=8;i<10;i++){day+=date[i];}
	H.ms=stoi(month); H.dy=stoi(day); H.hrs=stoi(hrs); H.min=stoi(min); H.seg=stoi(seg);
	cc.borrar();
	}

//---------------------INICIO--SENSORES--------------------------
//metodo para imprimir la info del sensor
void infosensor(struct sensor &S){
        cout<<"Id sensor "+S.name+": "<< S.id<<endl;
        cout<<"muestreo sensor "+S.name+": "<< S.muestreo<<endl;
        cout<<"pin sensor "+S.name+": "<< S.pin<<endl;
        cout<<"sistema: "<< S.sys<<endl;
	cout<<"----------------------------"<<endl;
        }

int getindexs(sensor S[],string tipo,int numsens){
	if(S[0].name=="vacio"){return -1;}
        for(int i=0;i<numsens;i++){
                if(S[i].name==tipo){ return i;}
                }
        return -1;
        }

int getindexa(actuador A[],string tipo,int numact){
	if(A[0].name=="vacio"){return -1;}
        for(int i=0;i<numact;i++){
                if(A[i].name==tipo){ return i;}
                }
        return -1;
        }


int contar_sensores(int system){
	conector cc;
        cc.conectar();
        cc.res=cc.stmt->executeQuery("select count(*) from sensor where sistema="+to_string(system));
        cc.res->next();
	int num=cc.res->getInt(1);
	cc.borrar();
	return num;
	}

int count_actors(int system){
	conector cc;
        cc.conectar();
        cc.res=cc.stmt->executeQuery("select count(*) from actuador where sistema="+to_string(system));
        cc.res->next();
	int num=cc.res->getInt(1);
	cc.borrar();
	return num;
	}

void load_sensors(sensor Sensors[],int system){
	conector cc;
	cc.conectar();
        cc.res=cc.stmt->executeQuery("select * from sensor where sistema="+to_string(system));
        int i=0;
        while(cc.res->next()){
                Sensors[i].name=cc.res->getString("tipo");
                Sensors[i].id=cc.res->getInt("id");
                Sensors[i].muestreo=cc.res->getInt("muestreo");
                Sensors[i].pin=cc.res->getInt("pin");
                Sensors[i].sys=cc.res->getInt("sistema");
		Sensors[i].valor=0;
                i++;
                }
	cc.borrar();
	}

void load_sistema(sistema &Sistema,int id){
	conector cc;
	cc.conectar();
        cc.res=cc.stmt->executeQuery("select * from sistema where id="+to_string(id));
        cc.res->next();
        Sistema.name=cc.res->getString("nombre");
        Sistema.id=cc.res->getInt("id");
        Sistema.id_plan=cc.res->getInt("id_planificador");
        Sistema.estado=cc.res->getInt("estado");
        Sistema.parent=cc.res->getInt("parent");
	cc.borrar();
	}

void load_actors(actuador Actors[],int system){
	conector cc;
	cc.conectar();
        cc.res=cc.stmt->executeQuery("select * from actuador where sistema="+to_string(system)+" or sistema=0");
        int i=0;
        while(cc.res->next()){
                Actors[i].name=cc.res->getString("tipo");
                Actors[i].id=cc.res->getInt("id");
		//5 segundos de Default
                Actors[i].seg=5;
                Actors[i].pin=cc.res->getInt("pin");
                Actors[i].seg=5;
                Actors[i].sys=cc.res->getInt("sistema");
                i++;
                }
	cc.borrar();
	}

//-------------------FIN--SENSORES------------------------

//-------------INICIO--ACTUADORES----------------------------------
//metodo para imprimir la info del actuador
void infoactuador(struct actuador &A){
        cout<<"Id actuador "+A.name+": "<< A.id<<endl;
        cout<<"pin actuador "+A.name+": "<< A.pin<<endl;
        cout<<"seg actuador "+A.name+": "<< A.seg<<endl;
        cout<<"sistema: "<< A.sys<<endl;
	cout<<"--------------------------------"<<endl;
        }

//-----------PLANIFICADOR----------------
planificador::planificador(int id_sistema){
	id=id_sistema;
        conector cc;
	cc.conectar();
	cc.res=cc.stmt->executeQuery("select id_planificador from sistema where id="+to_string(id));
	cc.res->next();
	id_planificador=cc.res->getInt(1);
        cc.res=cc.stmt->executeQuery("select * from planificador where id="+to_string(id_planificador));
        cc.res->next();
        ran_amb=cc.res->getInt("rango_ambiente");
	id_riego=cc.res->getInt("id_riego");
        cultivo=cc.res->getString("cultivo");
        phmax=cc.res->getDouble("phmax");
        phmin=cc.res->getDouble("phmin");
        hmin=cc.res->getDouble("hmin");
        hmax=cc.res->getDouble("hmax");
        tamb=cc.res->getDouble("temperatura_ambiente");
        cc.res=cc.stmt->executeQuery("select * from riego where id="+to_string(id_riego));
        cc.res->next();
        pnor=cc.res->getDouble("p_nor");
        tnor=cc.res->getDouble("t_nor");
        Tnor=cc.res->getDouble("Te_nor");
        pmas=cc.res->getDouble("p_mas");
        tmas=cc.res->getDouble("t_mas");
        Tmas=cc.res->getDouble("Te_mas");
        pmen=cc.res->getDouble("p_men");
        tmen=cc.res->getDouble("t_men");
        Tmen=cc.res->getDouble("Te_men");
        cc.borrar();
        }

void planificador::info(){
	cout<<"id sistema: "<<id<<endl;
	cout<<"id planificador: "<<id_planificador<<endl;
	cout<<"rango ambiente: "<<ran_amb<<endl;
	cout<<"id riego: "<<id_riego<<endl;
	cout<<"cultivo: "<<cultivo<<endl;
	cout<<"ph max: "<<phmax<<endl;
	cout<<"ph min: "<<phmin<<endl;
	cout<<"humedad maxima: "<<hmax<<endl;
	cout<<"humedad minima: "<<hmin<<endl;
	cout<<"temperatura ambiente: "<<tamb<<endl;
	cout<<"-----------------------RIEGO\n";
	cout<<"Periodo normal: "<<pnor<<endl;
	cout<<"tiempo normal: "<<tnor<<endl;
	cout<<"Temperatura normal: "<<Tnor<<endl;
	cout<<"periodo mas: "<<pmas<<endl;
	cout<<"tiempo mas: "<<tmas<<endl;
	cout<<"Temperatura mas: "<<Tmas<<endl;
	cout<<"periodo menos: "<<pmen<<endl;
	cout<<"tiempo menos: "<<tmen<<endl;
	cout<<"Temperatura menos: "<<Tmen<<endl;
	cout<<"-------------------------------------------------"<<endl;
	}
//---------FIN--PLANIFICADOR-----------

//-----------PRUEBAS------------------
int getnum(string mess){
        cout<<mess;
        int num;
        while(!(cin>>num)||num<0){
                cout<<"numero invalido\n";
                cin.clear(); cin.ignore(100,'\n');}
        return num;
        }


