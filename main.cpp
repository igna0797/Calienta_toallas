#include "mbed.h"
#include "Maquina/maquina.h"
#include "pc_serial_com/pc_serial_com.h"
// main() runs in its own thread in the OS
// boton 
//timer
//sensor

int main()
{
    maquina_de_estados_init();
    pcSerialComInit();

    while (true) {
        char receivedChar = pcSerialComCharRead();
        if( receivedChar != '\0' ) {
            pcSerialComCommandUpdate( receivedChar );
        }
        maquina_de_estados_update(receivedChar);
    }
    
}

