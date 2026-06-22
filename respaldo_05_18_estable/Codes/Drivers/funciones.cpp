#include "funciones.hpp"
using namespace std;
float absolut(float x){
        if(x<0){return -x;}
        else{return x;}
        }
void writeFile(int fileID, int value){
        char buffer[4];  // A place to build our four-byte string.
        memset((void *)buffer, 0, sizeof(buffer)); // clear the buffer out.
        sprintf(buffer, "%c", value);
        lseek(fileID, 0, SEEK_SET);   // Make sure we're at the top of the file!
        write(fileID, buffer, sizeof(buffer));
        }
void setPinMode(int pinID, int mode){writeFile(pinID, mode);}
void setPin(int pinID, int state){writeFile(pinID, state);}
void actuar(int seg,int pin,string mensaje){
        cout<<mensaje<<endl;
        int pinData,pinMode;
        int numrelay=pin;//1 motor,3 Ventilador
        char pathmode[256]; // direccion completa del archivo a escribir para e$
        memset(pathmode,0,sizeof(pathmode));// limpia la cadena de direcciion p$
        char pathpin[256]; // direccion completa del archivo a escrivir para le$
        memset(pathpin,0,sizeof(pathpin));// limpia la cadena de direcciion pat$
        sprintf(pathpin, "%s%s%d", GPIO_PIN_PATH, GPIO_FILENAME, numrelay);// c$
        pinData = open(pathpin, O_RDWR);
        writeFile(pinData, LOW);// HIGH LOW
        sprintf(pathmode, "%s%s%d", GPIO_MODE_PATH, GPIO_FILENAME, numrelay);//$
        pinMode = open(pathmode, O_RDWR);
        writeFile(pinMode, OUTPUT); // IMPUT, IMPUT_UP, etc.
        writeFile(pinData, HIGH);// HIGH LOW
        sleep(seg);
        writeFile(pinData, LOW);// HIGH LOw
        close(pinData);
        close(pinMode);
        }

void activar(int numrelay,int &pinData,int &pinMode){
        char pathmode[256]; // direccion completa del archivo a escribir para e$
        memset(pathmode,0,sizeof(pathmode));// limpia la cadena de direcciion p$
        char pathpin[256]; // direccion completa del archivo a escrivir para le$
        memset(pathpin,0,sizeof(pathpin));// limpia la cadena de direcciion pat$
        sprintf(pathpin, "%s%s%d", GPIO_PIN_PATH, GPIO_FILENAME, numrelay);// c$
        pinData = open(pathpin, O_RDWR);
        writeFile(pinData, LOW);// HIGH LOW
        sprintf(pathmode, "%s%s%d", GPIO_MODE_PATH, GPIO_FILENAME, numrelay);//$
        pinMode = open(pathmode, O_RDWR);
        writeFile(pinMode, OUTPUT); // IMPUT, IMPUT_UP, etc.
        writeFile(pinData, HIGH);// HIGH LOW
	}

void parar(int pinData,int pinMode){
        writeFile(pinData, LOW);// HIGH LOw
        close(pinData);
        close(pinMode);
	}
