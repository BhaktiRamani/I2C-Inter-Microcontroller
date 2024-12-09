#include "oled.h"

#define SSD1306_SEND_CMD(cmd) 		I2C_WriteCommand(0x00, cmd)
#define SSD1306_SEND_DATA(data)		I2C_WriteCommand(0x40, data)
static uint8_t cursor_column_x = 0;
static uint8_t cursor_page_y = 0;



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
	uint8_t data[(128 * 64 )/ 8];
	memset(data, 0x00, sizeof(data));
	i2c_multi_write(0x40, data, 128);
    //oled_set_cursor(0, 0);
//    for(int i = 0; i < 128 * 8; i++)
//    {
//        SSD1306_SEND_CMD(0x00);
//    }
//    oled_set_cursor(0, 0);
}
