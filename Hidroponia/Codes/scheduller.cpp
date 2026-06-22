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

//semaforo para esperar pin e hilo semáforo para insertar los monitoreos a la base de datos
void aguanta(int pin){
	while(pinbussy[pin]){/*esperar freedom*/}
	}
void usar(int pin){ pinbussy[pin]=true;}
void liberar(int pin){ pinbussy[pin]=false;}

void *monitoreo(void *arg){
	Hilos[0].name="monitoreo";
	Hilos[0].tid=syscall(SYS_gettid);
	conector cc;
	sleep(10);
	while(1){
		if(!consultas.empty()){
			cc.conectar();
			while(!consultas.empty()){
				try{
					cc.stmt->execute(consultas.front());
					consultas.pop();
					}
				catch(...){
					cout<<"error en el thread monitoreo handled with catch"<<endl;
					}
				}
			cc.borrar();
			}
		}
	pthread_exit(NULL);
	}

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
void setriego(){
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
//			tiempo=min_last_riego*(Ra[0]/Ra[1]) + (R[1]-min_last_riego)*(R[0]/R[1]);
			tiempo=R[0];
			printf("Riego actual %f %f %f \n",tiempo,R[1],R[2]);
			Ra[0]=R[0]; Ra[1]=R[1]; Ra[2]=R[2];
			printf("%i minutos desde last riego \n",min_last_riego);
			}
		else{ Ra[0]=R[0]; Ra[1]=R[1]; Ra[2]=R[2];
			tiempo=R[0];}
		}
	min_last_riego=sec_since_last_riego()/60;
	Tant=temp_amb;
	}

void *getValue(void *arg){
	string *type;
	type=(string *)arg;
	if(*type=="temperatura_ambiente"){
		Hilos[1].name="getvaluetemp";
		Hilos[1].tid=syscall(SYS_gettid);
		}
	else{
		Hilos[2].tid=syscall(SYS_gettid);
		Hilos[2].name="getvaluehum";
		}
	int muestreo,check,min;
	conector cc;
	cc.conectar();
	cc.res=cc.stmt->executeQuery("select muestreo from sensor where tipo='"+*type+"' and sistema="+to_string(Stm.parent));
	cc.res->next();
	muestreo=cc.res->getInt(1);
	cc.res=cc.stmt->executeQuery("select count(valor) from monitoreo_sensor where id_sensor=(select id from sensor where tipo='"+*type+"' and sistema="+to_string(Stm.parent)+")");
	cc.res->next();
	check=cc.res->getInt(1);
	while(!check){
		cout<<"esperar inicializar base de datos...\n";
		sleep(4);
		cc.res=cc.stmt->executeQuery("select count(valor) from monitoreo_sensor where id_sensor=(select id from sensor where tipo='"+*type+"' and sistema="+to_string(Stm.parent)+")");
		cc.res->next();
		check=cc.res->getInt(1);
		}
	cc.borrar();
	while(1){
		cc.conectar();
		min=sec_since_last_measure(*type)/60;
		while(min>40){
			cout<<"esperar lectura actual de "+*type+"\n";
			sleep(4);
			min=sec_since_last_measure(*type)/60;
			}
		cc.res=cc.stmt->executeQuery("select valor from monitoreo_sensor where id_sensor=(select id from sensor where tipo='"+*type+"' and sistema="+to_string(Stm.parent)+") order by id_monitoreo desc limit 1");
		cc.res->next();
		if(*type=="temperatura_ambiente"){ temp_amb=cc.res->getDouble(1);}
		if(*type=="humedad_ambiente"){ hum_amb=cc.res->getDouble(1);}
		cc.borrar();
		if(*type=="temperatura_ambiente"){
			setriego();
			}
		sleep(muestreo*60);
		}
	}

//funcion para crear alarmas de los sensores (thread alarma sensor)
void *sensar(void *arg){
	struct sensor *S;
	S = (struct sensor *) arg;
	Hilos[c].name=S->name;
	Hilos[c++].tid=syscall(SYS_gettid);
	string sql;
	int attempts,s;
	while(1){
		aguanta(S->pin);
		usar(S->pin);
		S->valor=-1;
		attempts=0;
		while(S->valor==-1){
			if(S->name=="temperatura_agua"){
				//matamos el thread de temperatura del agua porque estos datos los maneja el wateradmin
				Hilos[c-1].tid=-1;
				S->valor=273;
				}
			else if(S->name=="ph"){
				S->valor=sensorPH(S->pin);
				}
			else if(S->name=="temperatura_ambiente"){
				S->valor=Temp(S->pin);
				}
			else if(S->name=="humedad_ambiente"){
				S->valor=Hum(S->pin);
				}
			else if(S->name=="humedad_tierra"){
				S->valor=humtierra(S->pin);
				}
			else{
				cout<<"NO DRIVER FOR SENSOR "<< S->name <<endl; S->valor=273;
				Hilos[c-1].tid=-1;
				}
			if(S->valor==-1 && attempts++ < 2){
				sql="insert into error_sensor values(NULL,CURDATE(),CURTIME(),"+to_string(S->id)+",'error de lectura')";
				consultas.push(sql);
//				cout<<"Error de lectura, reintentando\n";
				}
			if(S->valor == 273){
				sql="insert into error_sensor values(NULL,CURDATE(),CURTIME(),"+to_string(S->id)+",'falta driver')";
				consultas.push(sql);
//				cout<<"matar thread sensor "<< S->name <<" por falta de driver"<<endl;
				pthread_exit(NULL);
				}
			}
		sql="insert into monitoreo_sensor values(NULL,CURDATE(),CURTIME(),"+to_string(S->valor)+","+to_string(S->id)+")";
		liberar(S->pin);
		consultas.push(sql);
		sleep(S->muestreo*60);
		}
	pthread_exit(NULL);
	}

//thread para controlar ph; no actuadores entonces salertas
//requiere Sensors[ph].valor i.e ph!=-1 from getindexs
void *controlph(void *arg){
	Hilos[3].name="controlph";
	Hilos[3].tid=syscall(SYS_gettid);
//	cout<<"thread control ph creado con tid: "<<Hilos[3].tid<<endl;
	sleep(2);
	string sql;
	while(1){
		if(Sensors[ph].valor!=0){
			if(Sensors[ph].valor > plan->phmax){
				//cout<<"-------------------------------------PH ALTO-------------------------------------"<<endl;
				Sensors[ph].valor=0;
				}
			if(Sensors[ph].valor < plan->phmin){
				//cout<<"-------------------------------------PH BAJO-------------------------------------"<<endl;
				Sensors[ph].valor=0;
				}
			}
		}
	}

float Temperatura(){
	float t;
	aguanta(Sensors[temperatura_ambiente].pin);
	usar(Sensors[temperatura_ambiente].pin);
	t=Temp(Sensors[temperatura_ambiente].pin);
	liberar(Sensors[temperatura_ambiente].pin);
	return t;
	}

float Humedad(){
	float h;
	aguanta(Sensors[humedad_ambiente].pin);
	usar(Sensors[humedad_ambiente].pin);
	h=Hum(Sensors[humedad_ambiente].pin);
	liberar(Sensors[humedad_ambiente].pin);
	return h;
	}

float Humedadtierra(){
	float ht;
	aguanta(Sensors[humedad_tierra].pin);
	usar(Sensors[humedad_tierra].pin);
	ht=humtierra(Sensors[humedad_tierra].pin);
	liberar(Sensors[humedad_tierra].pin);
	return ht;
	}

//thread para controlar el ventilador, se activa dependiendo de la temperatura ambiente
//requiere Sensors[tem_amb].valor,Sensors[thum_amb].valor & Actors[ventilador]
void *controlventilador(void *arg){
	Hilos[4].name="ventilador";
	Hilos[4].tid=syscall(SYS_gettid);
//	cout<<"thread ventilador creado con tid: "<<Hilos[4].tid<<endl;
	sleep(2);
	float temp,hum/*,hfv*/;
	int pinData,pinMode;
	Actors[ventilador].seg=130;
	//cout<<"tiempo minimo ventilador: "<<Actors[ventilador].seg<<" segundos"<<endl;
	string sql;
	while(1){
		aguanta(Sensors[temperatura_ambiente].pin);
		temp=Sensors[temperatura_ambiente].valor;
		aguanta(Sensors[humedad_ambiente].pin);
		hum=Sensors[humedad_ambiente].valor;
		if(temp > tem_amb_max /*&& hum > hum_amb_min*/){
			//cout<<"inicia ventilador"<<endl;
			sql="insert into monitoreo_actuador values(NULL,CURDATE(),CURTIME(),"+to_string(temp)+","+to_string(Actors[ventilador].id)+",1)";
	                consultas.push(sql);
			activar(Actors[ventilador].pin,pinData,pinMode);
			sleep(130);
			tfv=Temperatura();
			//hfv=Humedad();
			while(tfv==-1){ sleep(2); tfv=Temperatura();}
			//while(hfv==-1){ sleep(2); hfv=Humedad();}
			while(tfv>=(tem_amb_max) /*&& hfv > hum_amb_min*/){
				temp=tfv;
				sleep(60);
				tfv=Temperatura();
				while(tfv==-1){ sleep(2); tfv=Temperatura();}
				}
			if((temp-tfv)<=0){ /*cout<<"no está enfriando"<<endl;*/}
			if(tfv < tem_amb_max){ /*cout<<"ya enfrió"<<endl;*/}
			//if(hfv < hum_amb_min){ cout<<"Muy poca humedad"<<endl;}
			sql="insert into monitoreo_actuador values(NULL,CURDATE(),CURTIME(),"+to_string(tfv)+","+to_string(Actors[ventilador].id)+",0)";
	                consultas.push(sql);
			Sensors[temperatura_ambiente].valor=tfv;
			//Sensors[humedad_ambiente].valor=hfv
			parar(pinData,pinMode);
			}
		}
	}

//thread para calefactor se ajusta a la temperatura indicada por el riego actual comparando la temperatura del agua
//require Sensorns[tem_agua].valor] & Actors[calefactor]
void *controlcalefactor(void *arg){
	Hilos[5].name="calefactor";
	Hilos[5].tid=syscall(SYS_gettid);
//	cout<<"thread calefactor creado con tid: "<<Hilos[5].tid<<endl;
	sleep(2);
	string sql,rom;
	float temp;
	int pinData,pinMode;
        conector cc;
        cc.conectar();
        cc.res=cc.stmt->executeQuery("select rom from temagua where id="+to_string(Sensors[temperatura_agua].id));
        cc.res->next();
        rom=cc.res->getString(1);
        cc.borrar();
	Actors[calefactor].seg=60;
	while(1){
		aguanta(Sensors[temperatura_agua].pin);
		temp=Sensors[temperatura_agua].valor;
		if((tfc-temp) > 1 && temp < R[2]){
			//cout<<"inicia el calefactor verifica cada "<<Actors[calefactor].seg<<" segundos\n";
			sql="insert into monitoreo_actuador values(NULL,CURDATE(),CURTIME(),"+to_string(Sensors[temperatura_agua].valor)+","+to_string(Actors[calefactor].id)+",1)";
	                consultas.push(sql);
			activar(Actors[calefactor].pin,pinData,pinMode);
			sleep(Actors[calefactor].seg);
			tfc=watertemp(rom);
			while((tfc-temp)>1 && tfc < R[2]){
				temp=tfc;
				sleep(Actors[calefactor].seg);
                                tfc=watertemp(rom);
				}
			if((tfc-temp)<=1){ /*cout<<"no esta calentando"<<endl;*/}
			if(tfc>=R[2]){ /*cout<<"ya calentó"<<endl;*/}
			sql="insert into monitoreo_actuador values(NULL,CURDATE(),CURTIME(),"+to_string(tfc)+","+to_string(Actors[calefactor].id)+",0)";
	                consultas.push(sql);
			Sensors[temperatura_agua].valor=tfc;
			parar(pinData,pinMode);
			}
		}
	}

//same as setriego();
void *riego(void *arg){
	Hilos[6].name="riego";
	Hilos[6].tid=syscall(SYS_gettid);
//	cout<<"thread riego creado con tid: "<<Hilos[6].tid<<endl;
        string sql;
        while(1){
                if(min_last_riego > R[1]){
                        cout<< "bomba activada...\n";
			sql="insert into monitoreo_actuador values(NULL,CURDATE(),CURTIME(),"+to_string(temp_amb)+","+to_string(Actors[bomba].id)+",1)";
                        consultas.push(sql);
                        actuar(tiempo*60,Actors[bomba].pin,"Regando "+to_string(tiempo)+" minutos");
			sql="insert into monitoreo_actuador values(NULL,CURDATE(),CURTIME(),"+to_string(temp_amb)+","+to_string(Actors[bomba].id)+",0)";
                        consultas.push(sql);
                        cout<<"se vuelve a regar dentro de "<<R[1]<<" minutos"<<endl;
                        sleep(1);
			min_last_riego=sec_since_last_riego()/60;
			tiempo=R[0];
                        }
                }
        pthread_exit(NULL);
        }

void *riego2(void *arg){
	Hilos[6].name="riego2";
	Hilos[6].tid=syscall(SYS_gettid);
//	cout<<"thread riego2 creado con tid: "<<Hilos[6].tid<<endl;
        string sql;
	int pindata,pinmode;
	sleep(3);
	float ht;
	ht=Humedadtierra();
        while(1){
		ht=Sensors[humedad_tierra].valor;
                if(ht < 70){
                        cout<< "bomba activada...\n";
			sql="insert into monitoreo_actuador values(NULL,CURDATE(),CURTIME(),"+to_string(ht)+","+to_string(Actors[bomba].id)+",1)";
                        consultas.push(sql);
                        activar(Actors[bomba].pin,pindata,pinmode);
			sleep(60);
			ht=Humedadtierra();
			while(ht < 82){
				sleep(30);
				ht=Humedadtierra();
				}
			parar(pindata,pinmode);
			sql="insert into monitoreo_actuador values(NULL,CURDATE(),CURTIME(),"+to_string(ht)+","+to_string(Actors[bomba].id)+",0)";
                        consultas.push(sql);
                        cout<<"Humedad tierra "<<ht<<"%"<<endl;
			Sensors[humedad_tierra].valor=ht;
                        }
                }
        pthread_exit(NULL);
        }

bool running(pid_t tid,string name){
        char buffer;
        string check,tidfile;
        tidfile="./Threads/"+name+"sys"+to_string(sys);
        string cmd="cat /proc/"+to_string(pid)+"/task/"+to_string(tid)+"/status > "+tidfile;
        system(cmd.c_str());
        int file=open(tidfile.c_str(),O_RDONLY);
        int bol=read(file,&buffer,1);
        close(file);
        if(bol==0){
                return false;
                }
        else{
                return true;
                }
        }

int main(int argc,char* argv[]){
	pid=getpid();
	sys=atoi(argv[1]); //alojar el id del sistema
	//crear un logfile para los mensajes del sistema
	//y un errorfile para cuando el sistema se crashee nos avise
	string file="Logs/logfilesys"+to_string(sys);
	string errfile="Logs/Error/errfilesys"+to_string(sys);
	freopen(file.c_str(),"w",stdout);
	freopen(errfile.c_str(),"w",stderr);
	ofstream out(file.c_str());
	streambuf *coutbuf = cout.rdbuf();
	cout.rdbuf(out.rdbuf());
	//-----logfile
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
	numsens=contar_sensores(sys);
	numact=count_actors(sys);
	if(numact==0){ numact=1;}
	struct actuador A[numact];
	numact=count_actors(sys);
	if(numact==0){
		A[0].pin=-1;
		Actors=A;
		numact=1;
		}
	else{
		load_actors(A,sys);
		Actors=A;
		for(int i=0;i<numact;i++){
			infoactuador(Actors[i]);
			}
		cout<<"actuadores listos"<<endl;
		}
	if(numsens==0){numsens=1;}
	struct sensor S[numsens];
	pthread_t hilossensores[numsens];
	numsens=contar_sensores(sys);
	if(numsens==0){
		S[0].pin=-1;
		Sensors=S;
		numsens=1;
		}
	else{
		load_sensors(S,sys);
		Sensors=S;
		for(int i=0;i<numsens;i++){
			infosensor(Sensors[i]);
			}
		}
	//crear threads de los sensores
	struct hilo T[7+numsens];
	for(int j=0;j<7+numsens;j++){T[j].tid=-1;
		T[j].name="";}
	Hilos=T;
	for(int i=0;i<numsens;i++){
	        rc = pthread_create(&hilossensores[i], NULL, sensar, (void *)&Sensors[i]);
	        if (rc) {
	        	cout << "Error:unable to create thread," << rc << endl;
	        	exit(-1);
	        	}
		sleep(1);
		}
	cout<<"sensores listos"<<endl;
	//declarar threads
	pthread_t semaforo_monitoreo,hilotemp,hilohum,hiloriego,hilovent,hilocalef,hiloph;
	//------------------
        rc = pthread_create(&semaforo_monitoreo, NULL, monitoreo, NULL);
        if (rc) {
        	cout << "Error:unable to create thread," << rc << endl;
        	exit(-1);
        	}
	//checar si hay sensor de tem_amb, si no, tomar temp_amb de la base de datos
	temperatura_ambiente=getindexs(Sensors,"temperatura_ambiente",numsens);
	if(temperatura_ambiente==-1){
		tipo="temperatura_ambiente";
		cout<<"index of temperatura ambiente on Sensors[]: "<<temperatura_ambiente<<endl;
	        rc = pthread_create(&hilotemp, NULL, getValue, &tipo);
	        if (rc) {
	        	cout << "Error:unable to create thread," << rc << endl;
	        	exit(-1);
	        	}
		}
	humedad_ambiente=getindexs(Sensors,"humedad_ambiente",numsens);
	if(humedad_ambiente==-1){
		tipo2="humedad_ambiente";
		cout<<"index of humedad ambiente on Sensors[]: "<<humedad_ambiente<<endl;
	        rc = pthread_create(&hilohum, NULL, getValue, &tipo2);
	        if (rc) {
	        	cout << "Error:unable to create thread," << rc << endl;
	        	exit(-1);
	        	}
		}
	humedad_tierra=getindexs(Sensors,"humedad_tierra",numsens);
	bomba=getindexa(Actors,"bomba_agua",numact);
	if(bomba!=-1){
		cout<<"creando hilo riego\n";
		//inicializar la base de datos con un último riegoen caso de que no haya registros
		startriego(sys);
	        rc = pthread_create(&hiloriego, NULL, riego, NULL);
	        if (rc) {
	        	cout << "Error:unable to create thread," << rc << endl;
	        	exit(-1);
	        	}
		}
	ph=getindexs(Sensors,"ph",numsens);
	if(ph!=-1){
		cout<<"creando hilo ph\n";
	        rc = pthread_create(&hiloph, NULL, controlph, NULL);
	        if (rc) {
	        	cout << "Error:unable to create thread," << rc << endl;
	        	exit(-1);
	        	}
		}
	ventilador=getindexa(Actors,"ventilador",numact);
	if(ventilador!=-1 && temperatura_ambiente !=-1){
		cout<<"creando hilo ventilador\n";
	        rc = pthread_create(&hilovent, NULL, controlventilador, NULL);
	        if (rc) {
	        	cout << "Error:unable to create thread," << rc << endl;
	        	exit(-1);
	        	}
		}
	calefactor=getindexa(Actors,"calefactor",numact);
	temperatura_agua=getindexs(Sensors,"temperatura_agua",numsens);
	if(calefactor!=-1 && temperatura_agua!=-1){
		cout<<"creando hilo calefactor\n";
	        rc = pthread_create(&hilocalef, NULL, controlcalefactor, NULL);
	        if (rc) {
	        	cout << "Error:unable to create thread," << rc << endl;
	        	exit(-1);
	        	}
		}
	sleep(3);
	cout<<"Threads\n"<<endl;
	for(int j=0;j<7+numsens;j++){
		if(Hilos[j].tid!=-1){
			cout<<Hilos[j].name<<" con tid: "<<Hilos[j].tid<<endl;
			}
		}
	while(1){
		for(int j=0;j<7+numsens;j++){
			if(Hilos[j].tid!=-1){
//				cout<<Hilos[j].name<<" "<<Hilos[j].tid<<endl;
				if(!running(Hilos[j].tid,Hilos[j].name)){
					cout<<"   not running, going to heal..."<<endl;
					if(j==0){
					        rc = pthread_create(&semaforo_monitoreo, NULL, monitoreo, NULL);
        					if (rc) {
                					cout << "Error:unable to create thread," << rc << endl;
                					exit(-1);
                					}
						}
					if(j==1){
						tipo="temperatura_ambiente";
				                rc = pthread_create(&hilotemp, NULL, getValue, &tipo);
                				if (rc) {
                        				cout << "Error:unable to create thread," << rc << endl;
                        				exit(-1);
                        				}
						}
					if(j==2){
						tipo="temperatura_ambiente";
				                rc = pthread_create(&hilotemp, NULL, getValue, &tipo);
                				if (rc) {
                        				cout << "Error:unable to create thread," << rc << endl;
                        				exit(-1);
                        				}
						}
					if(j==3){
				                rc = pthread_create(&hiloph, NULL, controlph, NULL);
                				if (rc) {
                        				cout << "Error:unable to create thread," << rc << endl;
                        				exit(-1);
                        				}
						}
					if(j==4){
				                rc = pthread_create(&hilovent, NULL, controlventilador, NULL);
                				if (rc) {
                        				cout << "Error:unable to create thread," << rc << endl;
                        				exit(-1);
                        				}
						}
					if(j==5){
				                rc = pthread_create(&hilocalef, NULL, controlcalefactor, NULL);
                				if (rc) {
                        				cout << "Error:unable to create thread," << rc << endl;
                        				exit(-1);
                        				}
						}
					if(j==6){
				                rc = pthread_create(&hiloriego, NULL, riego, NULL);
                				if (rc) {
                        				cout << "Error:unable to create thread," << rc << endl;
                        				exit(-1);
                        				}
						}
					if(j>6){
						if(Hilos[j].name=="humedad_ambiente"){ indice=humedad_ambiente;}
						if(Hilos[j].name=="ph"){ indice=ph;}
						if(Hilos[j].name=="humedad_tierra"){ indice=humedad_tierra;}
						if(Hilos[j].name=="temperatura_ambiente"){ indice=temperatura_ambiente;}
					        rc = pthread_create(&hilossensores[indice], NULL, sensar, (void *)&Sensors[indice]);
			        		if (rc) {
			               			cout << "Error:unable to create thread," << rc << endl;
			                        	exit(-1);
			                        	}
						else{cout<<"thread revivido"<<endl;}
						}
					}
				}
			}
		sleep(3600);
		}
	//run healer//
	//main loop
	pthread_exit(NULL);
	return 0;
	}
/*falta crear un programa que verifique que los threads están vivos, usando cat /proc/pid/task/tid/status --------*/
