//=====[#include guards - begin]===============================================

#ifndef _PC_SERIAL_COM_H_
#define _PC_SERIAL_COM_H_

#include "DHT.h"

//=====[Declaration of public defines]=========================================

//=====[Declaration of public data types]======================================

//=====[Declarations (prototypes) of public functions]=========================

void pcSerialComInit();
char pcSerialComCharRead();
void pcSerialComStringWrite( const char* str );
void pcSerialComIntWrite( int number );
void pcSerialComUpdate();

void pcSerialComCommandUpdate( char receivedChar );
void commandShowReleState(DigitalOut rele);
void commandShowCurrentTemperatureInCelsius(DHT sensor);
void commandShowCurrentTemperatureInFahrenheit(DHT sensor);
//=====[#include guards - end]=================================================

#endif // _PC_SERIAL_COM_H_