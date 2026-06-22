//organizador de tareas para el sistema hidroponico
#include "Headers/tools.hpp"
#include<pthread.h>
#include<thread>
#include<chrono>
#include<ctime>
#include<iomanip>
#include<queue>
#include<iostream>
#include<fstream>
#include<sys/syscall.h>
using namespace std;
struct actuador *Actors;
struct sensor *Sensors;
struct hilo *Hilos;
struct sistema Stm;
planificador *plan;
bool pinbussy[20];
float Tant=100,tiempo=0,tfv=0,hfv=0,tfc=100,temp_amb,hum_amb,tem_amb_max=100,tem_amb_min=-100,hum_amb_max=100,hum_amb_min=-100;
int min_last_riego,numsens,numact,sys,bomba,temperatura_ambiente,humedad_ambiente,ventilador,ph,calefactor,temperatura_agua,humedad_tierra;
int c=7;
pid_t pid;
float R[3]={0,0,0},Ra[3]={0,0,0};
string hours[24]={"00","01","02","03","04","05","06","07","08","09","10","11","12","13","14","15","16","17","18","19","20","21","22","23"};
queue<string> consultas;


//funcion para checar si alguna temperatura está en el rango idealdel planificador
bool isin(float T){
	if(absolut(T-plan->tamb)<=plan->ran_amb){return true;}
	else{return false;}
	}

//obtener el rango de temperaturas más pequeño admisible para los subsistemas
void setidealrange(){
	conector cc;
	float t;
	cc.conectar();
	//first set minimo de las temperaturas maximas de los subsistemas
	cc.res=cc.stmt->executeQuery("select temperatura_ambiente+rango_ambiente from planificador where id in (select id_planificador from sistema where parent="+to_string(sys)+")");
	while(cc.res->next()){
		t=cc.res->getInt(1);
		if(t<tem_amb_max){ tem_amb_max=t;}
		}
	//ora el maximo de las temperaturas minimas
	cc.res=cc.stmt->executeQuery("select temperatura_ambiente-rango_ambiente from planificador where id in (select id_planificador from sistema where parent="+to_string(sys)+")");
	while(cc.res->next()){
		t=cc.res->getInt(1);
		if(t>tem_amb_min){ tem_amb_min=t;}
		}
	//lo mismo hum_amb_max y min
	cc.res=cc.stmt->executeQuery("select hmax from planificador where id in (select id_planificador from sistema where parent="+to_string(sys)+")");
	while(cc.res->next()){
		t=cc.res->getInt(1);
		if(t<hum_amb_max){ hum_amb_max=t;}
		}
	cc.res=cc.stmt->executeQuery("select hmin from planificador where id in (select id_planificador from sistema where parent="+to_string(sys)+")");
	while(cc.res->next()){
		t=cc.res->getInt(1);
		if(t>hum_amb_min){ hum_amb_min=t;}
		}
	cc.borrar();
	cout<<"rango ideal escogido"<<endl;
	}

//funcion que nos devuelve los segundos desde el último riego del sistema
int sec_since_last_riego(){
        time_t now;
        struct tm ultrieg;
        int seconds;
        time(&now);
        ultrieg=*localtime(&now);
        hora H;
        ultimoriego(H,sys);
//        printf("Ultimo Riego mes:%i dia:%i hr:%i min:%i seg:%i \n",H.ms,H.dy,H.hrs,H.min,H.seg);
//        printf("El presente mes:%i dia:%i hr:%i min:%i seg:%i \n",(ultrieg.tm_mon+1),ultrieg.tm_mday,ultrieg.tm_hour,ultrieg.tm_min,ultrieg.tm_sec);
        ultrieg.tm_hour=H.hrs; ultrieg.tm_min=H.min; ultrieg.tm_sec=H.seg;
        ultrieg.tm_mday=H.dy; ultrieg.tm_mon=(H.ms-1);
        seconds=difftime(now,mktime(&ultrieg));
//	cout<<"seconds since last riego: "<<seconds<<endl;
	return seconds;
	}

int sec_since_last_measure(string type){
        time_t now;
        struct tm lm;
        int seconds;
        time(&now);
        lm=*localtime(&now);
        hora H;
        lastmeasure(H,sys,type);
        lm.tm_hour=H.hrs; lm.tm_min=H.min; lm.tm_sec=H.seg;
        lm.tm_mday=H.dy; lm.tm_mon=(H.ms-1);
        seconds=difftime(now,mktime(&lm));
	return seconds;
	}

//funcion que asigna el riego de acuerdo a la temperatura ambiente
//requiere Sensors[temperatura_ambiente].valor
void setriego(int min_last){
	float tiempillo;
        if((isin(temp_amb) && !isin(Tant)) || (!isin(temp_amb) && isin(Tant)) || Tant==100){
		if(temp_amb>plan->tamb+plan->ran_amb){
			R[1]=plan->pmas;
			R[0]=plan->tmas;
			R[2]=plan->Tmas;
			printf("Riego mas t:%f p:%f T:%f \n",R[0],R[1],R[2]);
			}
		else if(temp_amb< plan->tamb-plan->ran_amb){
			R[1]=plan->pmen;
			R[0]=plan->tmen;
			R[2]=plan->Tmen;
			printf("Riego menos t:%f p:%f T:%f \n",R[0],R[1],R[2]);
			}
		else{
			R[1]=plan->pnor;
			R[0]=plan->tnor;
			R[2]=plan->Tnor;
			printf("Riego normal t:%f p:%f T:%f \n",R[0],R[1],R[2]);
			}
		if(Ra[0]!=0){
			cout<<"cambio de riego, ajustando los valores"<<endl;
			printf("Riego ant %f %f %f \t",Ra[0],Ra[1],Ra[2]);
			tiempo=R[0];
			printf("Riego actual %f %f %f \n",tiempo,R[1],R[2]);
			Ra[0]=R[0]; Ra[1]=R[1]; Ra[2]=R[2];
			printf("%i minutos desde last riego \n",min_last);
			}
		else{ Ra[0]=R[0]; Ra[1]=R[1]; Ra[2]=R[2];
			tiempo=R[0];}
		}
	Tant=temp_amb;
	}


void simulation(string file_name){
	int file=open(file_name.c_str(),O_RDONLY), p=1;
	int min_l_r=1000;
	while(p>0){
		string lectura=readline(file,p);
		temp_amb=stof(lectura);
		cout<<"temperatura: "<<temp_amb<<endl;
		setriego(min_l_r);
		if(min_l_r > R[1]){
			cout<<"regando "<<tiempo<<" y se vuelve a regar en "<<R[1]<<" minutos"<<endl;
			min_l_r=0;
			}
		min_l_r+=60;
		}
	close(file);
	}


int main(int argc,char* argv[]){
	pid=getpid();
	sys=atoi(argv[1]); //alojar el id del sistema
	//crear un logfile para los mensajes del sistema
	//y un errorfile para cuando el sistema se crashee nos avise
/*	string file="Logs/simulation";
	string errfile="Logs/Error/simulation";
	freopen(file.c_str(),"w",stdout);
	freopen(errfile.c_str(),"w",stderr);
	ofstream out(file.c_str());
	streambuf *coutbuf = cout.rdbuf();
	cout.rdbuf(out.rdbuf());
	//-----logfile*/
	cout<<"el pid del sistema es: "<<pid<<endl;
	string tipo,tipo2;
	int rc,indice;
	for(int i=0;i<20;i++){ pinbussy[i]=false;} //inicializar todos lo9s pines libres (20 pines en el duino)
	load_sistema(Stm,sys);
	if(Stm.id_plan!=0){
		cout<<"-----------------------------SUBSISTEMA: "<<Stm.name<<endl;
		//crear planificador asignado al subsystema
		planificador p(sys);
		plan=&p;
		plan->info();
		tem_amb_max=plan->tamb + plan->ran_amb;
		}
	else{
		setidealrange();
		cout<<"-------------------------------------SISTEMA: "<<Stm.name<<endl;
		conector cc;
		cc.conectar();
		cc.res=cc.stmt->executeQuery("select nombre,estado from sistema where parent="+to_string(sys));
		while(cc.res->next()){
			cout<<"subsistema: "<< cc.res->getString("nombre")<<" estado: "<< cc.res->getInt("estado")<<endl;
			}
		cc.borrar();
		}
	//Crear arreglos de sensores y actuadores, luego cargarlos con la base de datos
	simulation("data");
	//run healer//
	//main loop
	pthread_exit(NULL);
	return 0;
	}
/*falta crear un programa que verifique que los threads están vivos, usando cat /proc/pid/task/tid/status --------*/
