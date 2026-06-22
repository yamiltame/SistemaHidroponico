#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include<iostream>
#include<fstream>
#include<fcntl.h>
#include<unistd.h>
#include<pthread.h>
#include "Headers/conector.hpp"
using namespace std;

bool running(string name){
        char buffer;
        string check,pidfile;
        pidfile="running/"+name;
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

void stats(string tipo,string sistema){
        string query,hora;
        conector cc;
        cc.conectar();
        cc.res=cc.stmt->executeQuery("select substr(curtime(),1,2)");
	cc.res->next();
        hora=cc.res->getString(1);
	query="select count(*) from estadistica_hora where fecha=CURDATE() and tipo='"+tipo+"' and sistema="+sistema;
        cc.res=cc.stmt->executeQuery(query);
	cc.res->next();
        if(cc.res->getInt(1)==0){
                query="insert into estadistica_hora values (curdate(),'"+tipo+"',"+sistema+",0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0)";
                cc.stmt->execute(query);
                }
        query="update estadistica_hora set estadistica_hora."+hora+"=Format((select avg(valor) from monitoreo_sensor where id_sensor=(select id from sensor where tipo='"+tipo+"' and sistema="+sistema+") and fecha=CURDATE() and hora like '"+hora+
                "%'),2) where fecha=curdate() and tipo='"+tipo+"' and sistema="+sistema;
	cout<<query<<endl;
        cc.stmt->execute(query);
        cc.borrar();
        }


int  main(){
	int id;
	string file,errfile,query,query2,name,type;
        file="Logs/stats";
        errfile="Logs/Error/statserr";
        freopen(file.c_str(),"w",stdout);
        freopen(errfile.c_str(),"w",stderr);
	conector cc,kk;
	while(1){
		cc.conectar(); kk.conectar();
		query2="SELECT nombre,id from sistema where estado=1";
		cc.res=cc.stmt->executeQuery(query2);
		while(cc.res->next()){
			name=cc.res->getString(1);
			id=cc.res->getInt(2);
			query="select tipo from sensor where sistema="+to_string(id);
			kk.res=cc.stmt->executeQuery(query);
			while(kk.res->next()){
				if(running(name)){ stats(kk.res->getString(1),to_string(id));}
				}
		        }
		cc.borrar(); kk.borrar();
		sleep(1200);
		}
	}
