#include "oled.h"

/* SSD1306 width in pixels */
#define SSD1306_WIDTH            128

/* SSD1306 LCD height in pixels */
#define SSD1306_HEIGHT           64

static uint8_t cursor_column_x = 0;
static uint8_t cursor_page_y = 0;

/* SSD1306 data buffer */
//static uint8_t SSD1306_Buffer[SSD1306_WIDTH * SSD1306_HEIGHT / 8];

// Example OLED Initialization Commands
void oled_trial_commands(void) {
	// Basic initialization sequence for SSD1306
	SSD1306_SEND_CMD(0xAE);  // Display OFF
	SSD1306_SEND_CMD(0xD5);  // Set Display Clock Divide Ratio
	SSD1306_SEND_CMD(0x80);  // Default value
	SSD1306_SEND_CMD(0xA8);  // Set Multiplex Ratio
	SSD1306_SEND_CMD(0x3F);  // 64 COM lines
	SSD1306_SEND_CMD(0xD3);  // Set Display Offset
	SSD1306_SEND_CMD(0x00);  // No offset
	SSD1306_SEND_CMD(0x40);
	// Set Start Line (Line #0)
	SSD1306_SEND_CMD(0x8D);  // Charge Pump Setting
	SSD1306_SEND_CMD(0x14);  // Enable Charge Pump
	SSD1306_SEND_CMD(0x20);  // Memory Addressing Mode
	SSD1306_SEND_CMD(0x00);  // Horizontal Addressing Mode

	SSD1306_SEND_CMD(0xA1);  // Segment Remap
	SSD1306_SEND_CMD(0xC8);  // COM Output Scan Direction
	SSD1306_SEND_CMD(0xDA);  // COM Pins Configuration
	SSD1306_SEND_CMD(0x12);  // Alternative COM pin config
	SSD1306_SEND_CMD(0x81);  // Contrast Control
	SSD1306_SEND_CMD(0x34);  // Brightness value
	SSD1306_SEND_CMD(0xD9);  // Pre-charge Period
	SSD1306_SEND_CMD(0xF1);  //
	SSD1306_SEND_CMD(0xDB);  // VCOMH Deselect Level
	SSD1306_SEND_CMD(0x40);  //
	SSD1306_SEND_CMD(0xA4);  // Entire Display On/Off
	SSD1306_SEND_CMD(0xA6);  // Normal display (not inverted)
	SSD1306_SEND_CMD(0xAF);  // Display ON }

	SSD1306_SEND_CMD(0x2E);  // Display ON }


}

// Function to send data to OLED
void OLED_SendData(uint8_t data) {
    while((I2C1 -> ISR & I2C_ISR_BUSY));

    I2C1 -> CR2 = 0;
    I2C1 -> CR2 = I2C_CR2_AUTOEND | (2<<16) | (0x3C << 1);

    i2c_start();

    while (!(I2C1->ISR & I2C_ISR_TXE));
    I2C1 -> TXDR = 0x40;  // Control byte for data

    while (!(I2C1->ISR & I2C_ISR_TXE));
    I2C1 -> TXDR = data;

    while(!((I2C1 -> ISR & (1 << 5))));
    I2C1 -> ICR |= I2C_ICR_STOPCF;

    delay(10);
}

// SSD1306 Command Definitions
#define OLED_DISPLAY_OFF          0xAE
#define OLED_DISPLAY_ON           0xAF
#define OLED_SET_CONTRAST         0x81
#define OLED_SET_SEGMENT_REMAP    0xA1
#define OLED_COM_SCAN_DIR_NORMAL  0xC0
#define OLED_COM_SCAN_DIR_INVERTED 0xC8
#define OLED_CHARGE_PUMP_SETTING  0x8D

void OLED_Test_Commands(void) {
    // Array of commands to test
    uint8_t test_commands[] = {
        // Contrast setting
        OLED_SET_CONTRAST, 0x7F,   // Mid-level contrast

        // Segment remap (horizontal flip)
        OLED_SET_SEGMENT_REMAP, 0x01,

        // COM scan direction
        OLED_COM_SCAN_DIR_INVERTED,

        // Charge pump setting
        OLED_CHARGE_PUMP_SETTING, 0x14,  // Enable charge pump

        // Briefly turn off and on
        OLED_DISPLAY_OFF,
        OLED_DISPLAY_ON
    };

    while(1)
    {


    // Iterate through commands
    for (int i = 0; i < sizeof(test_commands); i++) {
        // Wait if I2C is busy
        while((I2C1 -> ISR & I2C_ISR_BUSY));

        // Reset CR2
        I2C1 -> CR2 = 0;

        // Configure transmission
        I2C1 -> CR2 = I2C_CR2_AUTOEND | (2<<16) | (0x3C << 1);

        // Start transmission
        i2c_start();

        // Wait for TXE and send control byte (0x00 for command)
        while (!(I2C1->ISR & I2C_ISR_TXE));
        I2C1 -> TXDR = 0x3C;  // Control byte for command

        // Wait for TXE and send command
        while (!(I2C1->ISR & I2C_ISR_TXE));
        I2C1 -> TXDR = test_commands[i];

        // Wait for stop bit
        while(!((I2C1 -> ISR & (1 << 5))));
        I2C1 -> ICR |= I2C_ICR_STOPCF;

        // Delay between commands
        delay(50);
    }
    }
}

//void fill_SSD1306() {
//	/* Set memory */
//	memset(SSD1306_Buffer,  0x00, sizeof(SSD1306_Buffer));
//}
//
//void updateScreen_SSD1306(void) {
//	uint8_t m;
//
//	for (m = 0; m < 8; m++) {
//		SSD1306_SEND_COMMAND(0xB0 + m);
//		SSD1306_SEND_COMMAND(0x00); //lower colunm asddress
//		SSD1306_SEND_COMMAND(0x10);	//upper colunm address
//
//		/* Write multi data */
//		writeMultRegisterI2C(SSD1306_I2C_ADDR, 0x40, SSD1306_WIDTH, &SSD1306_Buffer[SSD1306_WIDTH * m]);
//	}
//}


void oled_set_cursor(uint8_t column, uint8_t page)
{
    cursor_column_x = column;
    cursor_page_y = page;

    //for columns
    SSD1306_SEND_CMD(0x21);
    SSD1306_SEND_CMD(column);
    SSD1306_SEND_CMD(0x7f);

    //for page / rows
    SSD1306_SEND_CMD(0x22);
    SSD1306_SEND_CMD(page);
    SSD1306_SEND_CMD(7);


}

void oled_print_char(char c)
{
    if(c < 32 || c > 126) return;

    const uint8_t * char_data = font[c - 32];

    for(int i = 0; i< 5; i++)
    {
        SSD1306_SEND_CMD(char_data[i]);
    }
    SSD1306_SEND_CMD(0x00);

    //updating the cursor
    cursor_column_x += 6;
    if(cursor_column_x > 122)
    {
        cursor_column_x = 0;
        cursor_page_y += 1;
    }
}

void oled_print_string(const char *str)
{
    while(*str != '\0')
    {
        oled_print_char(*str++);
    }
}

void oled_clear(void)
{
    // Set cursor to start
	oled_set_cursor(0, 0);

    // Clear entire display (1024 bytes for 128x64 display)
    for (int i = 0; i < 1024; i++) {
        OLED_SendData(0x00);
    }

    // Reset cursor
    oled_set_cursor(0, 0);
//	uint8_t data[(128 * 64 )/ 8];
//	memset(data, 0x00, sizeof(data));
//	i2c_multi_write(0x40, data, 128);
    //oled_set_cursor(0, 0);
//    for(int i = 0; i < 128 * 8; i++)
//    {
//        SSD1306_SEND_CMD(0x00);
//    }
//    oled_set_cursor(0, 0);
}
