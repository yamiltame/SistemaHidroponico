#include<iostream>
#include <fcntl.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "../Headers/gpio_test.h"
#include<sys/time.h>
#include "funciones.hpp"
using namespace std;
float Temp(int pin);
float Hum(int pin);
float sensorTem(int pin);
float sensorHum(int pin);
