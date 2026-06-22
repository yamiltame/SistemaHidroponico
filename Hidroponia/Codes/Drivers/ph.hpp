#include<stdio.h>
#include<string.h>
#include<fcntl.h>
#include<iostream>
#include<stdlib.h>
#define ADC_IF_PATH "/proc/" //Definimos los procesos que se busca --
#define ADC_IF_FILE "adc"
using namespace std;
float sensorPH(int pin);
float avgph(int pin);
