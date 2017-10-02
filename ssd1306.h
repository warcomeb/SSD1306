/******************************************************************************
 * SSD1306 - Library for SSD1306 OLed Driver based on libohiboard
 * Copyright (C) 2017 Marco Giammarini
 *
 * Authors:
 *  Marco Giammarini <m.giammarini@warcomeb.it>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 ******************************************************************************/

#ifndef __WARCOMEB_SSD1306_H
#define __WARCOMEB_SSD1306_H

#define WARCOMEB_SSD1306_LIBRARY_VERSION     "1.0"
#define WARCOMEB_SSD1306_LIBRARY_VERSION_M   1
#define WARCOMEB_SSD1306_LIBRARY_VERSION_m   0
#define WARCOMEB_SSD1306_LIBRARY_TIME        0

#include "libohiboard.h"
#include "GDL/gdl.h"

/*
 * The user must define these label...
 * The name of the define is:
 *     #define WARCOMEB_SSD1306_HEIGHT         xx
 *     #define WARCOMEB_SSD1306_WIDTH          xx
 */
#ifndef __NO_BOARD_H
#include "board.h"
#endif

#if !defined(WARCOMEB_SSD1306_HEIGHT) & !defined(WARCOMEB_SSD1306_WIDTH)
#error "You must define height and width of display!"
#else
#define WARCOMEB_SSD1306_BUFFERDIMENSION (WARCOMEB_SSD1306_WIDTH*WARCOMEB_SSD1306_HEIGHT/8)
#endif

#if defined(WARCOMEB_SSD1306_HEIGHT) & ((WARCOMEB_SSD1306_HEIGHT < 16) | (WARCOMEB_SSD1306_HEIGHT > 64))
#error "The height must be between 16 and 64!"
#endif

#if defined(WARCOMEB_SSD1306_WIDTH) & ((WARCOMEB_SSD1306_WIDTH < 16) | (WARCOMEB_SSD1306_WIDTH > 128))
#error "The width must be between 16 and 128!"
#endif

typedef enum _SSD1306_Color
{
    SSD1306_COLOR_BLACK,
    SSD1306_COLOR_COLOR
} SSD1306_Color;

typedef enum _SSD1306_Product
{
    SSD1306_PRODUCT_ADAFRUIT_931   = 0x0001 | GDL_MODELTYPE_SSD1306,
} SSD1306_Product;

typedef struct SSD1306_Device
{
    GDL_Device gdl;                         /**< Common part for each device */

#if defined WARCOMEB_GDL_PARALLEL

#elif defined WARCOMEB_GDL_I2C

    Gpio_Pins rstPin;            /**< Reset pin used for start-up the display */

#elif defined WARCOMEB_GDL_SPI

#endif

    /** Buffer to store display data */
    uint8_t buffer [WARCOMEB_SSD1306_BUFFERDIMENSION];

} SSD1306_Device;

/**
 *
 */
void SSD1306_init (SSD1306_Device* dev);

/**
 *
 *
 * @param[in] dev The handle of the device
 * @param[in] xPos The x position
 * @param[in] yPos The y position
 * @param[in] color The color of the pixel
 * @return
 */
GDL_Errors SSD1306_drawPixel (SSD1306_Device* dev,
                              uint8_t xPos,
                              uint8_t yPos,
                              SSD1306_Color color);

/**
 * The function print a line in the selected position with the selected
 * color.
 *
 * @param[in] dev The handle of the device
 * @param[in] xStart The starting x position
 * @param[in] yStart The starting y position
 * @param[in] xStop The ending x position
 * @param[in] yStop The ending y position
 * @param[in] color The color of the line
 */
void SSD1306_drawLine (SSD1306_Device* dev,
                       uint8_t xStart,
                       uint8_t yStart,
                       uint8_t xStop,
                       uint8_t yStop,
                       SSD1306_Color color);

/**
 * The function print a horizontal line from the selected position with
 * the selected width and color.
 *
 * @param[in] dev The handle of the device
 * @param[in] xStart The starting x position
 * @param[in] yStart The starting y position
 * @param[in] width The width of the line
 * @param[in] color The color of the line
 */
void SSD1306_drawHLine (SSD1306_Device* dev,
                        uint8_t xStart,
                        uint8_t yStart,
                        uint8_t width,
                        SSD1306_Color color);

/**
 * The function print a vertical line from the selected position with
 * the selected height and color.
 *
 * @param[in] dev The handle of the device
 * @param[in] xStart The starting x position
 * @param[in] yStart The starting y position
 * @param[in] height The height of the line
 * @param[in] color The color of the line
 */
void SSD1306_drawVLine (SSD1306_Device* dev,
                        uint8_t xStart,
                        uint8_t yStart,
                        uint8_t height,
                        SSD1306_Color color);

/**
 *
 *
 * @param[in] dev The handle of the device
 */
void SSD1306_inverseDisplay (SSD1306_Device* dev);

/**
 *
 *
 * @param[in] dev The handle of the device
 */
void SSD1306_normalDisplay (SSD1306_Device* dev);

/**
 * This function clear the display setting off all pixel
 *
 * @param[in] dev The handle of the device
 */
void SSD1306_clear (SSD1306_Device* dev);

/**
 *
 *
 * @param[in] dev The handle of the device
 */
void SSD1306_flush (SSD1306_Device* dev);

#endif /* __WARCOMEB_SSD1306_H */
