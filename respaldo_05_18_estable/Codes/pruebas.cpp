#include "Headers/tools.hpp"
#include "Headers/libs.hpp"

void Sensar(struct sensor &S,float T[]){
	if(S.name=="temperatura_agua"){
		conector cc;
		cc.conectar();
		cc.res=cc.stmt->executeQuery("select rom from temagua where id="+to_string(S.id));
		cc.res->next();
		string rom=cc.res->getString(1);
		cout<<"Temperatura agua: "<<watertemp(rom)<<"°C"<<endl;
		cc.borrar();
		}
	else if(S.name=="ph"){ cout<<"Ph: "<<avgph(S.pin)<<endl;}
	else if(S.name=="temperatura_ambiente"){cout<<"T amb: "<<Temp(S.pin)<<"°C"<<endl;}
 	else if(S.name=="humedad_ambiente"){cout<<"Humedad amb: "<<Hum(S.pin)<<"%"<<endl;}
 	else if(S.name=="humedad_tierra"){cout<<"humedad tierra: "<<avglecture(S.pin)<<"%"<<endl;}
	else{ cout<<"sensor "<<S.name<<" sin driver para sensar"<<endl;}
	cout<<"----------------------------------------------"<<endl;
	}

void load_sensors(){
	float T[2]={0,0};
	conector cc;
	cc.conectar();
	cc.res=cc.stmt->executeQuery("select count(*) from sensor");
	cc.res->next();
	int numsensores=cc.res->getInt(1),opc;
	struct sensor Sensors[numsensores];
	cc.res=cc.stmt->executeQuery("select * from sensor");
	int i=0;
	while(cc.res->next()){
		Sensors[i].name=cc.res->getString("tipo");
		Sensors[i].id=cc.res->getInt("id");
		Sensors[i].muestreo=cc.res->getInt("muestreo");
		Sensors[i].pin=cc.res->getInt("pin");
		Sensors[i].sys=cc.res->getInt("sistema");
		i++;
		}
	cc.borrar();
	opc=getnum("0.Prueba completa   1.escoger sensor\n");
	switch(opc){
		case 0:
			for(i=0;i<numsensores;i++){
				infosensor(Sensors[i]);
				Sensar(Sensors[i],T);
				}
			break;
		case 1:
			int m=0;
			string menus="";
			for(i=0;i<numsensores;i++){
				menus=menus+to_string(i)+"."+Sensors[i].name+"_sys"+to_string(Sensors[i].sys)+"   ";
				}
			menus=menus+to_string(numsensores)+".Salir\n";
			while(m!=numsensores){
				m=getnum(menus);
				if(m>=0 && m<numsensores){
					infosensor(Sensors[m]);
					Sensar(Sensors[m],T);
					}
				if(m>numsensores){ cout<<"opcion invalida"<<endl;}
				}
			break;
		}
	}

void load_actors(){
	conector cc;
	cc.conectar();
	cc.res=cc.stmt->executeQuery("select count(*) from actuador");
	cc.res->next();
	int num=cc.res->getInt(1),opc;
	struct actuador Actors[num];
	cc.res=cc.stmt->executeQuery("select * from actuador");
	int i=0;
	while(cc.res->next()){
		Actors[i].name=cc.res->getString("tipo");
		Actors[i].id=cc.res->getInt("id");
		Actors[i].pin=cc.res->getInt("pin");
		Actors[i].sys=cc.res->getInt("sistema");
		i++;
		}
	cc.borrar();
	opc=getnum("0.Prueba completa   1.Escoger Actuador\n");
	switch(opc){
		case 0:
			cout<<"todos los actuadores durante 2 segundos"<<endl;
			for(i=0;i<num;i++){
				infoactuador(Actors[i]);
				actuar(2,Actors[i].pin,Actors[i].name+" actuando \n");
				}
			break;
		case 1:
			int m=0,seg;
			string menus="";
			for(i=0;i<num;i++){
				menus=menus+to_string(i)+"."+Actors[i].name+"_sys"+to_string(Actors[i].sys)+"   ";
				}
			menus=menus+to_string(num)+".Salir\n";
			while(m!=num){
				m=getnum(menus);
				if(m>=0 && m<num){
					infoactuador(Actors[m]);
					seg=getnum("segundos de accion: ");
					actuar(seg,Actors[m].pin,Actors[m].name+" actuando \n");
					}
				if(m>num){ cout<<"opcion invalida"<<endl;}
				}
			break;
		}
	}


int main(){
	int opc=3;
	while(opc!=0){
		opc=getnum("0.Salir   1.Sensores   2.Actuadores \n");
		if(opc==1){load_sensors();}
		if(opc==2){load_actors();}
		if(opc<0 || opc >2){cout<<"opcion invalida"<<endl;}
		}
	}
