/*
** file: transmit.cpp
** Programme permettant de transferer des donnees entrantes vers la memoire externe (Donnee par donnee).
**
** date: 26 octobre 2018
**
** authors:
** Simon Berhanu
** Facundo Giannotti
** Vincent L'Ecuyer-Simard
** Xi Chen Shen
**
*/

#include <string.h>

#include "functions.h"
#include "debug.h"
#include "LED.h"
#include "memoire_24.h"

int main()
{
    DDRA=0x03;
    uint16_t dataAddress = 0x00;
    uint16_t codeSize;

    LED led;
    UART transmitter;
    Memoire24CXXX memory;

    // Recoit une donnée de UART (1 octet)
    uint8_t firstByte = transmitter.receive();
    codeSize = firstByte;

    // Ecrit une donnee en mémoire externe (1 octet)
    memory.ecriture(dataAddress, firstByte);
    dataAddress++;

    // Delai pour que la mémoire puisse compléter son cycle d'écriture sans problème
    _delay_ms(5);

    // Recoit une autre donnée de UART (1 octet)
    uint8_t secondByte = transmitter.receive();

    // Ecrit une donnee en mémoire externe (1 octet)
    memory.ecriture(dataAddress, secondByte);
    dataAddress++;

    // Delai pour que la mémoire puisse compléter son cycle d'écriture sans problème
    _delay_ms(5);

    // Calcule du nombre d'octets total du pseudo-code (taille - 2 octets)
    codeSize = ((codeSize << 8) | secondByte);

    // nous commencons a 0, donc on arrete a codeSize - 1
    while (dataAddress < codeSize)
    {
        // Recoit une donnée de UART (1 octet)
        uint8_t dataReceived = transmitter.receive();

        // Ecrit une donnee en mémoire externe (1 octet)
        memory.ecriture(dataAddress, dataReceived);

        // Delai pour que la mémoire puisse compléter son cycle d'écriture sans problème
        _delay_ms(5);

        // Passe a la prochaine adresse en incrementant
        dataAddress++;

        //DEBUG_PRINT(("\nNombre d'octets restant: "))
        //DEBUG_PRINT(codeSize))
    }


    led.green();
}