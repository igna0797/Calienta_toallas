//=====[Libraries]=============================================================

#include "mbed.h"
#include "arm_book_lib.h"

#include "pc_serial_com.h"

#include "DHT.h"
#include "maquina.h"
//=====[Declaration of private defines]========================================

//=====[Declaration of private data types]=====================================

typedef enum{
    PC_SERIAL_COMMANDS,
    PC_SERIAL_GET_DATA
} pcSerialComMode_t;

//=====[Declaration and initialization of public global objects]===============

UnbufferedSerial uartUsb(USBTX, USBRX, 115200);

//=====[Declaration of external public global variables]=======================

//=====[Declaration and initialization of public global variables]=============


//=====[Declaration and initialization of private global variables]============

static pcSerialComMode_t pcSerialComMode = PC_SERIAL_COMMANDS;


//=====[Declarations (prototypes) of private functions]========================

static void pcSerialComCharWrite( char chr );
static void pcSerialComStringRead( char* str, int strLength );

//static void pcSerialComCommandUpdate( char receivedChar );

static void availableCommands();
static void commandShowCurrentGasDetectorState();
static void commandShowCurrentOverTemperatureDetectorState();
/*
static void commandShowReleState(DigitalOut rele);
static void commandShowCurrentTemperatureInCelsius(DHT sensor);
static void commandShowCurrentTemperatureInFahrenheit(DHT sensor);
*/

//=====[Implementations of public functions]===================================

void pcSerialComInit()
{
    availableCommands();
}

char pcSerialComCharRead()
{
    char receivedChar = '\0';
    if( uartUsb.readable() ) {
        uartUsb.read( &receivedChar, 1 );
    }
    return receivedChar;
}

void pcSerialComStringWrite( const char* str )
{
    uartUsb.write( str, strlen(str) );
}

void pcSerialComIntWrite( int number )
{
    char str[4] = "";
    sprintf( str, "%d", number );
    pcSerialComStringWrite( str );
}

void pcSerialComUpdate() //TODO : volar esto
{
    char receivedChar = pcSerialComCharRead();
    if( receivedChar != '\0' ) {
            pcSerialComCommandUpdate( receivedChar );
        }
    }    

//=====[Implementations of private functions]==================================

static void pcSerialComCharWrite( char chr )
{
    char str[2] = "";
    sprintf (str, "%c", chr);
    uartUsb.write( str, strlen(str) );
}

static void pcSerialComStringRead( char* str, int strLength )
{
    int strIndex;
    for ( strIndex = 0; strIndex < strLength; strIndex++) {
        uartUsb.read( &str[strIndex] , 1 );
        uartUsb.write( &str[strIndex] ,1 );
    }
    str[strLength]='\0';
}



void pcSerialComCommandUpdate( char receivedChar )
{
    switch (receivedChar) {
        case '1': break; 
        case '2':  break;
        case '3':  break;
        case 'c': case 'C':  break;
        case 'f': case 'F':  break;
        default: availableCommands(); break;
    } 
}

static void availableCommands()
{
    pcSerialComStringWrite( "Available commands:\r\n" );
    pcSerialComStringWrite( "Press '1' to get the alarm state\r\n" );
    pcSerialComStringWrite( "Press '2' to get the gas detector state\r\n" );
    pcSerialComStringWrite( "Press '3' to get the over temperature detector state\r\n" );
    pcSerialComStringWrite( "Press 'f' or 'F' to get lm35 reading in Fahrenheit\r\n" );
    pcSerialComStringWrite( "Press 'c' or 'C' to get lm35 reading in Celsius\r\n" );

    pcSerialComStringWrite( "\r\n" );
}

void commandShowReleState(DigitalOut rele)
{
    if ( rele == ON) {
        pcSerialComStringWrite( "The relay is activated\r\n");
    } else {
        pcSerialComStringWrite( "The relay is not activated\r\n");
    }
}
/*
static void commandShowCurrentGasDetectorState()
{
    if ( gasDetectorStateRead() ) {
        pcSerialComStringWrite( "Gas is being detected\r\n");
    } else {
        pcSerialComStringWrite( "Gas is not being detected\r\n");
    }    
}

static void commandShowCurrentOverTemperatureDetectorState()
{
    if ( overTemperatureDetectorStateRead() ) {
        pcSerialComStringWrite( "Temperature is above the maximum level\r\n");
    } else {
        pcSerialComStringWrite( "Temperature is below the maximum level\r\n");
    }
}
*/
void commandShowCurrentTemperatureInCelsius(DHT sensor)
{
    char str[100] = "";
    float temperature =0.5 ;
    sensor.readData();
    //temperature = sensor.ReadTemperature(CELCIUS) ;
    sprintf ( str, "Temperature: %.2f \xB0 C\r\n", temperature);
    pcSerialComStringWrite( str );  
}

void commandShowCurrentTemperatureInFahrenheit(DHT sensor)
{
    char str[100] = "";
    sprintf ( str, "Temperature: %.2f \xB0 C\r\n",
                   sensor.ReadTemperature(FARENHEIT) );
    pcSerialComStringWrite( str );  
}

