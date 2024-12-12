/**
 * @file arduino.c
 * @brief Implementation file for arduino as a slave for i2c communication
 *
 * @author Bhakti Ramani
 * @date December 12, 2024
 * @course ECEN 5813 Principles of Embedded Software
 *
 * This file provides implementation of communicating with Arduino as a slave via I2C protocol
 */
#include"arduino.h"

/******************************************************************************
* @function    arduino_send_msg
* @brief       Sends acknowledgment message to Arduino via I2C
* @param       None
* @return      None
*
* @details     Sends a confirmation message to Arduino after receiving and
*              displaying temperature/humidity data. Uses I2C1 peripheral
*              with auto-end feature.
*/
void arduino_send_msg()
{
	while((I2C1 -> ISR & I2C_ISR_BUSY));

	I2C1 -> CR2 = 0;
	char *buffer = "Recieved Temp and Hum";
	int len = strlen(buffer);
	I2C1 -> CR2 = I2C_CR2_AUTOEND | (len <<16) | (8 << 1 );
	I2C1 -> CR2 &= ~I2C_CR2_RD_WRN;


    i2c_start();

     for(int i = 0; i < len ; i++)
     {
        	//sending the actual data
        while (!(I2C1->ISR & I2C_ISR_TXE)){;}
        I2C1 -> TXDR = *buffer;
        buffer++;
     }

     while(!((I2C1 -> ISR & (1 << 5))));
     I2C1 -> ICR |= I2C_ICR_STOPCF;
}

/******************************************************************************
* @function    arduino_rcv_msg
* @brief       Receives temperature and humidity data from Arduino
* @param       str: Pointer to buffer for storing received data
* @param       len: Length of data to receive (expected 2 bytes)
* @return      None
*
* @details     Reads temperature and humidity values from Arduino using I2C1.
*              First byte is temperature, second byte is humidity.
*              Uses auto-end feature and handles busy flags.
******************************************************************************/
void arduino_rcv_msg(char *str, size_t len)
{
	while((I2C1 -> ISR & I2C_ISR_BUSY));

	I2C1 -> CR2 = 0;

	I2C1 -> CR2 = I2C_CR2_AUTOEND | I2C_CR2_RD_WRN |  (2<<16) | (8 << 1 );

	i2c_start();
    for (size_t i = 0; i < len; i++) {
        // Wait until the RXNE (Receive Not Empty) flag is set
        while (!(I2C1->ISR & I2C_ISR_RXNE)) {
            // Optionally, implement a timeout mechanism here
        }

        // Read a byte from the RXDR register
        str[i] = (char)(I2C1->RXDR & 0xFF);  // Mask to ensure only lower 8 bits are taken
    }



    while(!((I2C1 -> ISR & (1 << 5))));
    I2C1 -> ICR |= I2C_ICR_STOPCF;




}
