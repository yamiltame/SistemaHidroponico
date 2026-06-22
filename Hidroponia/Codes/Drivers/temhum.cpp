#include "temhum.hpp"
float Temp(int pin){
        float tcont=0,temp;
        int c=0;
        temp=sensorTem(pin);
        while(absolut(tcont-temp) > 1 || temp==0){
                tcont=temp;
                sleep(2);
                temp=sensorTem(pin);
                if(c++==3){
                        //cout<<"Revisar sensor Temperatura_ambiente"<<endl;
			temp=-1;
                        break;
                        }
                }
	return temp;
        }

float Hum(int pin){
        float hcont=0,hum;
        int c=0;
        hum=sensorHum(pin);
        while(absolut(hcont-hum) > 2 || hum==0){
                hcont=hum;
                sleep(2);
                hum=sensorHum(pin);
                if(c++==3){
                        //cout<<"Revisar sensor Humedad_ambiente"<<endl;
			hum=-1;
                        break;
                        }
                }
	return hum;
        }

float sensorTem(int pin){
        int pinMode, pinData,i;
        struct timeval  time1, time2;
        char inputBuffer = HIGH,path[256],pathmode[256],pathpin[256],valor[41];
        unsigned int ciclo=10000;
        for(i=0;i<40;i++){ valor[i]='0';}
        i=0;
        double time=0;
        memset(pathmode,0,sizeof(path));
        sprintf(pathmode, "%s%s%d", GPIO_MODE_PATH, GPIO_FILENAME, pin);
        pinMode = open(pathmode, O_RDWR);
        setPinMode(pinMode, OUTPUT);
        //coloca un valor LOW en el pin 2
        memset(pathpin,0,sizeof(path));
        sprintf(pathpin, "%s%s%d", GPIO_PIN_PATH, GPIO_FILENAME, pin);
        pinData = open(pathpin, O_RDWR);
        setPin(pinData, HIGH);
        //espera 36 milisegundos
        usleep(36000);
        setPin(pinData, LOW);
        //espera 36 milisegundos
        usleep(10000);
        //coloca un valor HIGH en el pin 2
        setPin(pinData, HIGH);
        //espera 40 microsegundos
        usleep(40);
        //configura el pin 2 en modo INPUT
        setPinMode(pinMode, INPUT);
        ciclo=10000;
        do{
                lseek(pinData, 0, SEEK_SET);
                read(pinData, &inputBuffer, 1);
                if(ciclo--==0){
                        close(pinMode);
                        close(pinData);
                        return 0;
                        }

                } while (inputBuffer == LOW);
         do{
                lseek(pinData, 0, SEEK_SET);
                read(pinData, &inputBuffer, 1);
                if(ciclo--==0){
                        close(pinMode);
                        close(pinData);
                        return 0;
                        }
                } while (inputBuffer == LOW);
         do{
                lseek(pinData, 0, SEEK_SET);
                read(pinData, &inputBuffer, 1);
                if(ciclo--==0){
                        close(pinMode);
                        close(pinData);
                        return 0;
                        }
                }while (inputBuffer == HIGH);
        for(i=0; i<40;i++){
                ciclo=10000;
                do{
                        lseek(pinData, 0, SEEK_SET);
                        read(pinData, &inputBuffer, 1);
                        if(ciclo--==0){
                                close(pinMode);
                                close(pinData);
                                return 0;
                                }
                        }while (inputBuffer == LOW);
               gettimeofday(&time1, NULL);
               ciclo=10000;
               do{
                        lseek(pinData, 0, SEEK_SET);
                        read(pinData, &inputBuffer, 1);
                        if(ciclo--==0){
                                usleep(10000);
                                close(pinMode);
                                close(pinData);
                                return 0;
                                }
                        } while (inputBuffer == HIGH);
                gettimeofday(&time2, NULL);
                time= (double) time2.tv_usec - time1.tv_usec;
                if(time>50.0) {
                        valor[i]='1';
                        time=0;
                        }
                }
        int temp=0;
        for(i=16;i<31;i++){
                 if(valor[i]=='1') temp=temp|1;
                 temp<<=1;
                }
        close(pinMode);
        close(pinData);
        return (float)temp/10;
        }

float sensorHum(int pin){
        int pinMode, pinData,i;
        struct timeval  time1, time2;
        char inputBuffer = HIGH,path[256],pathmode[256],pathpin[256],valor[41];
        unsigned int ciclo=10000;
        for(i=0;i<40;i++){ valor[i]='0';}
        i=0;
        double time=0;
        memset(pathmode,0,sizeof(path));
        sprintf(pathmode, "%s%s%d", GPIO_MODE_PATH, GPIO_FILENAME, pin);
        pinMode = open(pathmode, O_RDWR);
        setPinMode(pinMode, OUTPUT);
        //coloca un valor LOW en el pin 2
        memset(pathpin,0,sizeof(path));
        sprintf(pathpin, "%s%s%d", GPIO_PIN_PATH, GPIO_FILENAME, pin);
        pinData = open(pathpin, O_RDWR);
        setPin(pinData, HIGH);
        //espera 36 milisegundos
        usleep(36000);
        setPin(pinData, LOW);
        //espera 36 milisegundos
        usleep(10000);
        //coloca un valor HIGH en el pin 2
        setPin(pinData, HIGH);
        //espera 40 microsegundos
        usleep(40);
        //configura el pin 2 en modo INPUT
        setPinMode(pinMode, INPUT);
        ciclo=10000;
        do{
                lseek(pinData, 0, SEEK_SET);
                read(pinData, &inputBuffer, 1);
                if(ciclo--==0){
                        close(pinMode);
                        close(pinData);
                        return 0;
                        }
                } while (inputBuffer == LOW);
         do{
                lseek(pinData, 0, SEEK_SET);
                read(pinData, &inputBuffer, 1);
                if(ciclo--==0){
                        close(pinMode);
                        close(pinData);
                        return 0;
                        }
                } while (inputBuffer == LOW);
         do{
                lseek(pinData, 0, SEEK_SET);
                read(pinData, &inputBuffer, 1);
                if(ciclo--==0){
                        close(pinMode);
                        close(pinData);
                        return 0;
                        }
                }while (inputBuffer == HIGH);
        for(i=0; i<40;i++){
                ciclo=10000;
                do{
                        lseek(pinData, 0, SEEK_SET);
                        read(pinData, &inputBuffer, 1);
                        if(ciclo--==0){
                                close(pinMode);
                                close(pinData);
                                return 0;
                                }
                        }while (inputBuffer == LOW);
               gettimeofday(&time1, NULL);
               ciclo=10000;
               do{
                        lseek(pinData, 0, SEEK_SET);
                        read(pinData, &inputBuffer, 1);
                        if(ciclo--==0){
                                usleep(10000);
                                close(pinMode);
                                close(pinData);
                                return 0;
                                }
                        } while (inputBuffer == HIGH);
                gettimeofday(&time2, NULL);
                time= (double) time2.tv_usec - time1.tv_usec;
                if(time>50.0) {
                        valor[i]='1';
                        time=0;
                        }
                }
//        printf("\nValor: %s i=%d\n",valor,i);
        int humedad=0;
        for(i=0;i<15;i++){
                 if(valor[i]=='1') humedad=humedad|1;
                 humedad<<=1;
                }
        close(pinMode);
        close(pinData);
        return (float)humedad/10;
        }
