#include "temagua.hpp"
using namespace std;


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

float watertemp(string rom){
	string linea,lectura;
	float value=85;
	system("/home/Hidroponia/TempAgua/temagua");
	int file=open("/home/Hidroponia/TempAgua/info",O_RDONLY),p=1;
	linea=readline(file,p);
	while(value>50){
		while(linea!=rom){
			close(file);
			p=1;
			system("/home/Hidroponia/TempAgua/temagua");
			file=open("/home/Hidroponia/TempAgua/info",O_RDONLY);
			linea=readline(file,p);
			}
		lectura=readline(file,p);
		value=stof(lectura);
		}
	close(file);
	return value;
	}
