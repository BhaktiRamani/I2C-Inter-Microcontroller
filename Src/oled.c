#include "oled.h"

/* SSD1306 width in pixels */
#define SSD1306_WIDTH            128

/* SSD1306 LCD height in pixels */
#define SSD1306_HEIGHT           64

static uint8_t cursor_column_x = 0;
static uint8_t cursor_page_y = 0;

/* SSD1306 data buffer */
static uint8_t SSD1306_Buffer[SSD1306_WIDTH * SSD1306_HEIGHT / 8];

typedef struct {
	uint16_t CurrentX;
	uint16_t CurrentY;
	uint8_t Inverted;
	uint8_t Initialized;
} SSD1306_t;

static SSD1306_t SSD1306;

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

	SSD1306_SEND_CMD(0x2E);  // Deactivate scroll


	fill_SSD1306();
	updateScreen_SSD1306();
	gotoXY_SSD1306 (25,5);
	puts_SSD1306 ("PES PROJECT", &Font_7x10, SSD1306_COLOR_WHITE);
	updateScreen_SSD1306();

	/* Set default values */
	SSD1306.CurrentX = 0;
	SSD1306.CurrentY = 0;
	
	/* Initialized OK */
	SSD1306.Initialized = 1;


}


void fill_SSD1306() {
	/* Set memory */
	memset(SSD1306_Buffer,  0x00, sizeof(SSD1306_Buffer));
}

void updateScreen_SSD1306(void) {
	uint8_t m;

	for (m = 0; m < 8; m++) {
		SSD1306_SEND_CMD(0xB0 + m);
		SSD1306_SEND_CMD(0x00); //lower colunm asddress
		SSD1306_SEND_CMD(0x10);	//upper colunm address

		/* Write multi data */
		i2c_multi_write(0x40, &SSD1306_Buffer[SSD1306_WIDTH * m],  SSD1306_WIDTH);
	}
}

void gotoXY_SSD1306(uint16_t x, uint16_t y) {
	/* Set write pointers */
	SSD1306.CurrentX = x;
	SSD1306.CurrentY = y;
}

char putc_SSD1306(char ch, FontDef_t* Font, SSD1306_COLOR_t color) {
	uint32_t i, b, j;

	/* Check available space in LCD */
	if (
		SSD1306_WIDTH <= (SSD1306.CurrentX + Font->FontWidth) ||
		SSD1306_HEIGHT <= (SSD1306.CurrentY + Font->FontHeight)
	) {
		/* Error */
		return 0;
	}

	/* Go through font */
	for (i = 0; i < Font->FontHeight; i++) {
		b = Font->data[(ch - 32) * Font->FontHeight + i];
		for (j = 0; j < Font->FontWidth; j++) {
			if ((b << j) & 0x8000) {
				drawPixel_SSD1306(SSD1306.CurrentX + j, (SSD1306.CurrentY + i), (SSD1306_COLOR_t) color);
			} else {
				drawPixel_SSD1306(SSD1306.CurrentX + j, (SSD1306.CurrentY + i), (SSD1306_COLOR_t)!color);
			}
		}
	}

	/* Increase pointer */
	SSD1306.CurrentX += Font->FontWidth;

	/* Return character written */
	return ch;
}

char puts_SSD1306(char* str, FontDef_t* Font, SSD1306_COLOR_t color) {
	/* Write characters */
	while (*str) {
		/* Write character by character */
		if (putc_SSD1306(*str, Font, color) != *str) {
			/* Return error */
			return *str;
		}

		/* Increase string pointer */
		str++;
	}

	/* Everything OK, zero should be returned */
	return *str;
}


void drawPixel_SSD1306(uint16_t x, uint16_t y, SSD1306_COLOR_t color) {
	if (
		x >= SSD1306_WIDTH ||
		y >= SSD1306_HEIGHT
	) {
		/* Error */
		return;
	}

	/* Check if pixels are inverted */
	if (SSD1306.Inverted) {
		color = (SSD1306_COLOR_t)!color;
	}

	/* Set color */
	if (color == SSD1306_COLOR_WHITE) {
		SSD1306_Buffer[x + (y / 8) * SSD1306_WIDTH] |= 1 << (y % 8);
	} else {
		SSD1306_Buffer[x + (y / 8) * SSD1306_WIDTH] &= ~(1 << (y % 8));
	}
}



void drawLine_SSD1306(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, SSD1306_COLOR_t c) {
	int16_t dx, dy, sx, sy, err, e2, i, tmp;

    /* Check for and correct any overflow in coordinates */
    if (x0 >= SSD1306_WIDTH) {
        x0 = SSD1306_WIDTH - 1;
    }
    if (x1 >= SSD1306_WIDTH) {
        x1 = SSD1306_WIDTH - 1;
    }
    if (y0 >= SSD1306_HEIGHT) {
        y0 = SSD1306_HEIGHT - 1;
    }
    if (y1 >= SSD1306_HEIGHT) {
        y1 = SSD1306_HEIGHT - 1;
    }

    // Calculate differences and steps in x and y directions.
    dx = (x0 < x1) ? (x1 - x0) : (x0 - x1);
    dy = (y0 < y1) ? (y1 - y0) : (y0 - y1);
    sx = (x0 < x1) ? 1 : -1;
    sy = (y0 < y1) ? 1 : -1;
    err = ((dx > dy) ? dx : -dy) / 2;

    // Handle vertical lines.
    if (dx == 0) {
        // Swap y0 and y1 if y0 is greater.
        if (y1 < y0) {
            tmp = y1;
            y1 = y0;
            y0 = tmp;
        }

        // Draw vertical line.
        for (i = y0; i <= y1; i++) {
            drawPixel_SSD1306(x0, i, c);
        }

        return; // Exit function after drawing vertical line.
    }

    // Handle horizontal lines.
    if (dy == 0) {
        // Swap x0 and x1 if x0 is greater.
        if (x1 < x0) {
            tmp = x1;
            x1 = x0;
            x0 = tmp;
        }

        // Draw horizontal line.
        for (i = x0; i <= x1; i++) {
            drawPixel_SSD1306(i, y0, c);
        }

        return; // Exit function after drawing horizontal line.
    }

    // Draw diagonal lines using Bresenham's algorithm.
    while (1) {
        drawPixel_SSD1306(x0, y0, c);
        if (x0 == x1 && y0 == y1) {
            break; // Exit loop when end point is reached.
        }
        e2 = err;
        if (e2 > -dx) {
            err -= dy;
            x0 += sx;
        }
        if (e2 < dy) {
            err += dx;
            y0 += sy;
        }
    }
}

void oled_set_cursor(uint8_t column, uint8_t page)
{
    cursor_column_x = column;
    cursor_page_y = page;

    //for columns
    SSD1306_SEND_CMD(0x21);
    SSD1306_SEND_CMD(column);
    SSD1306_SEND_CMD(column);

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
//    for (int i = 0; i < 1024; i++) {
//    	SSD1306_SEND_DATA(0x00);
//    }

    // Reset cursor
    oled_set_cursor(0, 0);

}
