#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include<iostream>
#include<fcntl.h>
#include<unistd.h>
#include "Headers/conector.hpp"
#include "Drivers/temagua.hpp"
using namespace std;
int main(){
	int muestreo,id;
	float valor;
	string rom;
	conector cc;
	while(1){
		cc.conectar();
		cc.res=cc.stmt->executeQuery("select muestreo from sensor where tipo='temperatura_agua' limit 1");
		cc.res->next();
		muestreo=cc.res->getInt(1);
		cc.res=cc.stmt->executeQuery("select id,rom from temagua where id in (select id from sensor where tipo='temperatura_agua' and sistema in (select id from sistema where estado=1))");
		while(cc.res->next()){
			id=cc.res->getInt(1);
			rom=cc.res->getString(2);
			valor=watertemp(rom);
			cc.stmt->execute("insert into monitoreo_sensor values(NULL,CURDATE(),CURTIME(),"+to_string(valor)+","+to_string(id)+")");
			}
		cc.borrar();
		sleep(muestreo*60);
		}
	}
