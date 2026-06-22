#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include<iostream>
#include<fstream>
#include<fcntl.h>
#include<unistd.h>
#include<pthread.h>
#include "Headers/conector.hpp"
#include "Drivers/funciones.hpp"
using namespace std;

void kill(int sys,string name){
        int pinData,pin;
        char pathmode[256],pathpin[256];
        memset(pathmode,0,sizeof(pathmode));
        memset(pathpin,0,sizeof(pathpin));
        conector cc;
        cc.conectar();
        cc.stmt->execute("insert into power values(NULL,CURDATE(),CURTIME(),0,"+to_string(sys)+")");
        cc.res=cc.stmt->executeQuery("select pin from actuador where sistema="+to_string(sys));
        while(cc.res->next()){
                pin=cc.res->getInt("pin");
                sprintf(pathpin, "%s%s%d", GPIO_PIN_PATH, GPIO_FILENAME, pin);
                pinData= open(pathpin, O_RDWR);
                writeFile(pinData, LOW);// HIGH LOW
                close(pinData);
                }
        cc.borrar();
        string command="sudo pkill -f "+name;
        system(command.c_str());
        }


struct info{
	string name;
	int system;
	};

//funcion para checkar si un programa esta corriendo
bool running(string name){
        char buffer;
        string check,pidfile;
	pidfile="/home/Hidroponia/running/"+name;
	string cmd="pidof "+name+" > "+pidfile;
	system(cmd.c_str());
	int file=open(pidfile.c_str(),O_RDONLY);
	int bol=read(file,&buffer,1);
	close(file);
	if(bol==0){
		return false;
		}
	else{
		return true;
		}
        }

//pedir numero al usuario (pruebas de funcionamiento del software)
int getnum(string mess){
        cout<<mess;
        int num;
        while(!(cin>>num)||num<0){
                cout<<"numero invalido\n";
                cin.clear(); cin.ignore(100,'\n');}
        return num;
        }

//ejecutar un sistema en un thread
void *launchremote(void *arg){
	struct info *sistema;
	sistema=(struct info *)arg;
	string cmd="bash -c 'exec -a "+sistema->name+" /home/Hidroponia/Hidroponia "+to_string(sistema->system)+"'";
	system(cmd.c_str());
	pthread_exit(NULL);
	}

//ejecutar admin de aguas
void *launchwateradmin(void *arg){
	cout<<"creating wateradmin"<<endl;
	string cmd="bash -c 'exec sudo nice --7 /home/Hidroponia/wateradmin'";
	if(!running("wateradmin")){system(cmd.c_str());}
	pthread_exit(NULL);
	}


//Kills every system loaded on database regarding its state and also the admin program
void kill_todo(){
	conector cc;
	cc.conectar();
	cc.res=cc.stmt->executeQuery("select nombre,id from sistema order by parent desc");
	while(cc.res->next()){
		kill(cc.res->getInt(2),cc.res->getString(1));
		}
	cc.borrar();
	if(running("admin")){system("sudo pkill admin");}
	if(running("wateradmin")){system("sudo pkill wateradmin");}
	}

//carga los systems con la info de la database
void load_systems(){
	int id,numsys,rc,i=0;
	struct info sis;
	conector cc,sub;
	string cmd,name;
	cc.conectar();
	sub.conectar();
	cc.res=cc.stmt->executeQuery("select count(*) from sistema where estado=1");
	cc.res->next();
	numsys=cc.res->getInt(1);
	pthread_t Systems[numsys];
	cc.res=cc.stmt->executeQuery("select nombre,id from sistema where estado=1 and parent=0");
	while(cc.res->next()){
		sis.name=cc.res->getString(1);
		sis.system=cc.res->getInt(2);
		if(!running(sis.name)){
			cout<<"running "<<sis.name<<endl;
			cc.stmt->execute("insert into power values(NULL,CURDATE(),CURTIME(),1,"+to_string(sis.system)+")");
			rc = pthread_create(&Systems[i++], NULL, launchremote, (void *)&sis);//run parent systems remote
			if (rc) {
                       		cout << "Error:unable to create thread," << rc << endl;
                       		exit(-1);
                       		}
			sleep(1);
			}
		//selecting their child systems
		sub.res=sub.stmt->executeQuery("select nombre,id from sistema where estado=1 and parent="+to_string(sis.system));
		while(sub.res->next()){
			sis.name=sub.res->getString(1);
			sis.system=sub.res->getInt(2);
			cout<<sis.name<<"   "<<sis.system<<"   "<<running(sis.name)<<"   "<<endl;
			if(!running(sis.name)){
				cc.stmt->execute("insert into power values(NULL,CURDATE(),CURTIME(),1,"+to_string(sis.system)+")");
				rc = pthread_create(&Systems[i++], NULL, launchremote, (void *)&sis);//run parent systems remote
				if (rc) {
       	                		cout << "Error:unable to create thread," << rc << endl;
       	                		exit(-1);
       	                		}
				sleep(1);
				}
			}
		}
	cc.res=cc.stmt->executeQuery("select nombre,id from sistema where estado=0 order by parent asc");
	while(cc.res->next()){
		name=cc.res->getString(1);
		id=cc.res->getInt(2);
		if(running(name)){cout<<"killin "<<name<<endl; kill(id,name);}
		sub.res=sub.stmt->executeQuery("select nombre,id from sistema where parent="+to_string(id)+" and estado=1;");
		while(sub.res->next()){
			name=sub.res->getString(1);
			id=sub.res->getInt(2);
			if(running(name)){kill(id,name);}
			}
		}
	sub.borrar();
	cc.borrar();
	//ejecutar el administrador de temperaturas de agua
	pthread_t wateradmin;
	rc = pthread_create(&wateradmin, NULL, launchwateradmin, NULL);
        if (rc) {
        	cout << "Error:unable to create thread," << rc << endl;
                exit(-1);
                 }
	sleep(2);
	printf("sistemas caragados\n");
	}


int  main(int argc,char* argv[]){
//	freopen("Logs/adminlog","w",stdout);
//	ofstream out("Logs/adminlog");
//	streambuf *coutbuf=cout.rdbuf();
//	cout.rdbuf(out.rdbuf());
	//1.cargar sistemas local   2.load sys remote   3.actualizar sistema   4.Killtodo    5.pruebas hardware
        int opc=atoi(argv[1]);
        switch (opc){
                case 1:
			load_systems();
			while(1){/*ciclo infinito para mantener activo el admin*/}
			break;
                case 2:
			load_systems();
			sleep(3);
			cout<<"la informacion se guarda en la carpeta Logs"<<endl;
			break;
                case 3:
			kill_todo();
			cout<<"la idea es tener los archivos driver_x.hpp y driver_x.cpp en la carpeta Drivers/ falta editar la funcion sensar en scheduller.cpp\n";
			system("/home/ubuntu/Hidroponia/comp");
			cout<<"sistema actualizado, ya existe la funcion driver(int pin) en el objeto driver_x.o"<<endl;
			break;
		case 4:
			kill_todo();
			cout<<"killing everything running"<<endl;
			break;
                case 5:
			kill_todo();
	                system("/home/Hidroponia/Hardware_test");
			break;
                default:
			cout<<"OPCION INVALIDA"<<endl;
			break;
                }
        }

