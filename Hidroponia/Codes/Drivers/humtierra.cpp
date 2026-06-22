#include "humtierra.hpp"
using namespace std;
float humtierra(int pin){
    int adc,cont1=0,cont2=5,x=0;
    char path[64],ADCBuffer[16],ADCResult[6]="00000";
    memset(path, 0, sizeof(path));//limpia
    sprintf(path, "%s%s%d", ADC_IF_PATH, ADC_IF_FILE , pin);
    adc = open(path, O_RDONLY);
    float Ht=1;
    lseek(adc, 0, SEEK_SET);
    int res = read(adc, ADCBuffer, sizeof(ADCBuffer));
    cont2=5;
    for(cont1=res-2;cont1>4;cont1--){
                ADCResult[cont2]=ADCBuffer[cont1];
                cont2--;
        }
        x=atoi(ADCResult);
    Ht=(float)(100*(x-4095)/(2610-4095));
    close(adc);
    if(Ht>100){ Ht=100;}
    return Ht;
}

float avglecture(int pin){
	float sum=0;
	for(int c=0;c<1000;c++){
		sum+=humtierra(pin);
		}
	return sum/1000;
	}
