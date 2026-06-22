#include<stdio.h>
#include<string.h>
#include<fcntl.h>
#include<iostream>
#include "../Headers/gpio_test.h"
using namespace std;
float absolut(float x);
void writeFile(int fileID, int value);
void setPinMode(int pinID, int mode);
void setPin(int pinID, int state);
void actuar(int seg,int pin,string mensaje);
void activar(int numrelay,int &pinData,int &pinMode);
void parar(int pinData,int pinMode);
