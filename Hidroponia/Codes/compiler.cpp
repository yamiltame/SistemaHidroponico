#include<stdlib.h>
#include<stdio.h>
#include<string>
#include<iostream>
#include<fcntl.h>
using namespace std;

/*compilar todos los archivos .cpp el el directorio Files para hacer objetos
en el directorio Objects*/

string readline(int file, int &p){
        string check;
        string line="";
        char buffer;
        while(p=read(file,&buffer,1)>0){
                check=buffer;
                if(check=="\n"){break;}
                line=line+buffer;
                }
        return line;
        }

int nuevafuncion(){
	return 0;
	}

int main(){
	//se compilan los objetos de los drivers partiendo de los archivos .cpp en Drivers
	//se actualiza el archivo libs.hpp con los contenidos de Drivers/*.hpp
	string archivo,libreria,comando,objeto,check;
	system("ls Codes/Drivers/*.cpp > Codes/Drivers/cppfiles");
	system("ls Codes/Drivers/*.hpp > Codes/Drivers/hppfiles");
	int i,p=1,fd=open("Codes/Drivers/cppfiles",O_RDONLY), f2=open("Codes/Drivers/hppfiles",O_RDONLY),f3=open("Codes/Headers/libs.hpp",O_WRONLY);
	archivo=readline(fd,p);
	while(p!=0){
		objeto="";
		for(i=0;i<archivo.size()-3;i++){
			check=archivo[i];
			objeto=objeto+archivo[i];
			if(check=="/"){objeto="";}
			}
		objeto=objeto+"o";
		comando="g++ -std=c++0x -c "+archivo+" -o Objects/"+objeto;
//		cout<<"command: "<<comando<<endl;
		system(comando.c_str());
		archivo=readline(fd,p);
		}
	p=1;
	libreria="#include \"../../"+readline(f2,p)+"\"";
	while(p!=0){
		write(f3,libreria.c_str(),libreria.size());
		write(f3,"\n",1);
		libreria="#include \"../../"+readline(f2,p)+"\"";
		}
	close(f2);
	close(f3);
	close(fd);
	}
