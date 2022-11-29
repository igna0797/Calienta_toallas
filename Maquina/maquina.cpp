//=====[Libraries]=============================================================

#include "mbed.h"
#include "arm_book_lib.h"

#include "DHT.h"
#include "maquina.h"

//=====[Declaration of private defines]========================================


//=====[Declaration of private data types]=====================================
typedef enum {
    APAGADO,
    WAITING,
    INICIANDO,
    PRENDIDO,
    TOWAITING,
} state_t;

//=====[Declaration and initialization of public global objects]===============


//=====[Declaration of external public global variables]=======================


//=====[Declaration and initialization of public global variables]=============

state_t estado;
DHT sensor(PD_0, DHT11);
DigitalInOut rele(PG_3);
DigitalOut led(LED1);
DigitalIn boton(BUTTON1); 

//=====[Declaration and initialization of private global variables]============
// int temperatura_media =25;
time_t tiempoEncendido;
time_t tiempoInicio;
time_t tiempoActual;

//=====[Declarations (prototypes) of private functions]========================


//=====[Implementations of public functions]===================================
void maquina_de_estados_init(){
        estado=WAITING;
        rele.mode(OpenDrain);
        rele.input();
    }

void maquina_de_estados_update(){
        static int Tmax=0;
        static int Hmax=0;
        static bool starting = 1; //medio feo hay como una rutina de starting que tiene 2 
        int temperaturaActual = 0;
        int humedadActual = 0; //leerlo aca y nunca mas?
        switch(estado){
            case APAGADO:
                if (boton==1){
                    //estado = WAITING;
                }
                break;
            case WAITING:
                starting=1;
                rele.input(); //por si acaso se llego aca sin que se apagara.
                led=OFF;
                sensor.readData();
                if(sensor.ReadHumidity() > HUMIDITY_THRESHOLD || sensor.ReadTemperature(CELCIUS) > TEMPERATURE_THRESHOLD || boton==1 ){
                    estado=INICIANDO;
                }
                break;
            
            case INICIANDO:
                if (starting==1){
                    tiempoInicio=time(NULL);
                    Tmax=0;
                    Hmax=0;
                    starting=0;
                }
                tiempoActual=time(NULL);
                tiempoEncendido=tiempoActual - tiempoInicio;
                if (tiempoEncendido >= TIEMPO_HOLD_TO_TURN_OFF){
                    starting=0;
                    estado=APAGADO;
                    break;
                }
                if (boton == 0) {
                    rele.output();
                    rele = LOW;
                    led=ON;
                    estado=PRENDIDO;
                }
                break;
            
            case PRENDIDO: 
                
                sensor.readData();
                temperaturaActual = sensor.ReadTemperature(CELCIUS);
                humedadActual = sensor.ReadHumidity();

                Tmax=max(temperaturaActual,Tmax);
                Hmax=max(humedadActual,Hmax);
                if (boton || temperaturaActual< Tmax-TEMP_OFF_THRESHOLD || humedadActual < Hmax- HUM_OFF_THRESHOLD || tiempoEncendido > TIEMPO_MAXIMO_ENCENDIDO){
                    estado=TOWAITING;
                    rele.input();
                    led=OFF;
                break;
            case TOWAITING:
                wait_us(50);
                if (boton ==0){
                    estado=WAITING;
                }
            
            }
        }

}

//=====[Implementations of private functions]==================================


int timerinterrupt(){
    return 0;
}

