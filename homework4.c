/*
 * homework4.c
 *
 *  Created on: 4/4/2020
 *      Author: Andrew Koehn
 */

#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include "homework4.h"


int main(void)
{
    char rChar = NO_CHAR;
    char *response = "\n\n\r2534 is the best course in the curriculum!\r\n\n";

    // TODO: Declare the variables that main uses to interact with your state machine.
    bool completeString = false;


    // Stops the Watchdog timer.
    initBoard();
    // TODO: Declare a UART config struct as defined in uart.h.
    //       To begin, configure the UART for 9600 baud, 8-bit payload (LSB first), no parity, 1 stop bit.
    eUSCI_UART_ConfigV1 uartConfig =
    {
        EUSCI_A_UART_CLOCKSOURCE_SMCLK,
        19,
        8,
        0x55,
        EUSCI_A_UART_NO_PARITY,
        EUSCI_A_UART_LSB_FIRST,
        EUSCI_A_UART_ONE_STOP_BIT,
        EUSCI_A_UART_MODE,
        EUSCI_A_UART_OVERSAMPLING_BAUDRATE_GENERATION,
        EUSCI_A_UART_8_BIT_LEN,
    };


    // TODO: Make sure Tx AND Rx pins of EUSCI_A0 work for UART and not as regular GPIO pins.
    GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P1, GPIO_PIN3, GPIO_PRIMARY_MODULE_FUNCTION);
    GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P1, GPIO_PIN2, GPIO_PRIMARY_MODULE_FUNCTION);


    // TODO: Initialize EUSCI_A0
    UART_initModule(EUSCI_A0_BASE, &uartConfig);

    // TODO: Enable EUSCI_A0
    UART_enableModule(EUSCI_A0_BASE);


    while(1)
    {
        // TODO: Check the receive interrupt flag to see if a received character is available.
        //       Return 0xFF if no character is available.
        //non-blocking poll to check if a char is in the receive buffer
        if(UART_getInterruptStatus(EUSCI_A0_BASE, EUSCI_A_UART_RECEIVE_INTERRUPT_FLAG))
            rChar = UART_receiveData(EUSCI_A0_BASE);
        else
            rChar = NO_CHAR;


        // TODO: If an actual character was received, echo the character to the terminal AND use it to update the FSM.
        //       Check the transmit interrupt flag prior to transmitting the character.
        if(rChar != NO_CHAR)
        {
            //blocking poll to detect if transmit buffer is ready to transmit
            while(UART_getInterruptStatus(EUSCI_A0_BASE, EUSCI_A_UART_TRANSMIT_INTERRUPT_FLAG) == 0)
            {}
            UART_transmitData(EUSCI_A0_BASE, rChar);

            //update FSM with char
            completeString = charFSM(rChar);
        }



        // TODO: If the FSM indicates a successful string entry, transmit the response string.
        //       Check the transmit interrupt flag prior to transmitting each character and moving on to the next one.
        //       Make sure to reset the success variable after transmission.
        if(completeString)
        {
            int counter = 0;
            while(response[counter] != 0)
            {
                //blocking poll to detect if transmit buffer is ready to transmit
                while(UART_getInterruptStatus(EUSCI_A0_BASE, EUSCI_A_UART_TRANSMIT_INTERRUPT_FLAG) == 0)
                {}
                UART_transmitData(EUSCI_A0_BASE, response[counter]);
                counter++;
            }

            completeString = false;
        }
    }
}

void initBoard()
{
    WDT_A_hold(WDT_A_BASE);
}

// TODO: FSM for detecting character sequence.
bool charFSM(char rChar)
{
    static codeState passcodeState = SX;
    bool finished = false;

    switch(passcodeState)
    {
    case SX:
        if(rChar == '2')
            passcodeState = S2;
        else
            passcodeState = SX;
        break;
    case S2:
        if(rChar == '2')
            passcodeState = S2;
        else if(rChar == '5')
            passcodeState = S25;
        else
            passcodeState = SX;
        break;
    case S25:
        if(rChar == '2')
            passcodeState = S2;
        else if(rChar == '3')
            passcodeState = S253;
        else
            passcodeState = SX;
        break;
    case S253:
        if(rChar == '2')
            passcodeState = S2;
        else if(rChar == '4')
        {
            passcodeState = SX;
            finished = true;
        }
        else
            passcodeState = SX;
        break;
    }

    return finished;
}
