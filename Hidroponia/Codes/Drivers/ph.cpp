#include "ph.hpp"
using namespace std;
float sensorPH(int pin){
    int adc,i=pin,cont1=0,cont2=5,x=0;
    char path[64],ADCBuffer[16],ADCResult[6]="00000";
    memset(path, 0, sizeof(path));//limpia
    sprintf(path, "%s%s%d", ADC_IF_PATH, ADC_IF_FILE , i);
    adc = open(path, O_RDONLY);
    float pH=1;
    lseek(adc, 0, SEEK_SET);
    int res = read(adc, ADCBuffer, sizeof(ADCBuffer));
    cont2=5;
    for(cont1=res-2;cont1>4;cont1--){

                ADCResult[cont2]=ADCBuffer[cont1];
                cont2--;
        }
        x=atoi(ADCResult);
    pH=(((float)x*14)/4096)*0.8;
//	pH=((float)(x+574)/467);
    close(adc);
    return pH;
}

float avgph(int pin){
	float sum,prom=0;
	while(prom==0){
		sum=0;
		for(int i=0;i<16;i++){
			sum+=sensorPH(pin);
			usleep(1000);
			}
		prom=sum/16;
		}
	return prom;
	}
