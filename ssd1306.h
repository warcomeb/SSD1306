/*
 * SSD1306 - Library for SSD1306 OLed Driver based on libohiboard
 * Copyright (C) 2017-2019 Marco Giammarini
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
 */

#ifndef __WARCOMEB_SSD1306_H
#define __WARCOMEB_SSD1306_H

#define WARCOMEB_SSD1306_LIBRARY_VERSION_MAJOR   (0x1ul)
#define WARCOMEB_SSD1306_LIBRARY_VERSION_MINOR   (0x0ul)
#define WARCOMEB_SSD1306_LIBRARY_VERSION_BUG     (0x0ul)
#define WARCOMEB_SSD1306_LIBRARY_VERSION         ((WARCOMEB_GDL_LIBRARY_VERSION_MAJOR << 16)\
                                                 |(WARCOMEB_GDL_LIBRARY_VERSION_MINOR << 8 )\
                                                 |(WARCOMEB_GDL_LIBRARY_VERSION_BUG        ))
#define WARCOMEB_SSD1306_LIBRARY_TIME            0

#include "ssd1306type.h"
#include "../GDL/gdl.h"

#define SSD1306_MAX_DISPLAY_HEIGHT               64
#define SSD1306_MAX_DISPLAY_WIDTH                128
#define SSD1306_BUFFER_DIMENSION                 (SSD1306_MAX_DISPLAY_WIDTH*SSD1306_MAX_DISPLAY_HEIGHT/8)

/*!
 * SSD1306 configuration struct.
 * An object of this class must be used to save all module configurations.
 */
typedef struct _SSD1306_Config_t
{
    uint16_t product;

    Gpio_Pins rd;
    Gpio_Pins dc;
    Gpio_Pins rs;
    Gpio_Pins cs;
    Gpio_Pins wr;

    Gpio_Pins d0;
    Gpio_Pins d1;
    Gpio_Pins d2;
    Gpio_Pins d3;
    Gpio_Pins d4;
    Gpio_Pins d5;
    Gpio_Pins d6;
    Gpio_Pins d7;

    Gpio_Pins rstPin;            /*!< Reset pin used for start-up the display */

#if defined (LIBOHIBOARD_IIC)

    Iic_DeviceHandle iicDev;
    Iic_Config       iicConfig;

    //Iic_SdaPins sdaPin;
    //Iic_SclPins sclPins;

#endif

#if defined (LIBOHIBOARD_SPI)


#endif

} SSD1306_Config_t;

/*!
 * SSD1306 device class.
 */
typedef struct _SSD1306_Device_t
{
    GDL_Device_t gdl;                         /*!< Common part for each device */

    SSD1306_Config_t config;

    bool isChargePump;

    uint8_t protocolType;

#if defined (LIBOHIBOARD_IIC)
    uint8_t address;
#endif

    /** Buffer to store display data */
    uint8_t buffer [SSD1306_BUFFER_DIMENSION];

} SSD1306_Device_t, *SSD1306_DeviceHandle_t;

/*!
 *
 * \param[in]    dev:
 * \param[in] config:
 */
void SSD1306_init (SSD1306_DeviceHandle_t dev, SSD1306_Config_t* config);

/**
 *
 *
 * @param[in] dev The handle of the device
 * @param[in] xPos The x position
 * @param[in] yPos The y position
 * @param[in] color The color of the pixel
 * @return
 */
GDL_Errors_t SSD1306_drawPixel (SSD1306_DeviceHandle_t dev,
                                uint8_t xPos,
                                uint8_t yPos,
                                SSD1306_Color_t color);

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
void SSD1306_drawLine (SSD1306_DeviceHandle_t dev,
                       uint8_t xStart,
                       uint8_t yStart,
                       uint8_t xStop,
                       uint8_t yStop,
                       SSD1306_Color_t color);

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
void SSD1306_drawHLine (SSD1306_DeviceHandle_t dev,
                        uint8_t xStart,
                        uint8_t yStart,
                        uint8_t width,
                        SSD1306_Color_t color);

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
void SSD1306_drawVLine (SSD1306_DeviceHandle_t dev,
                        uint8_t xStart,
                        uint8_t yStart,
                        uint8_t height,
                        SSD1306_Color_t color);

/**
 * The function draw a rectangle. It can be fill or not.
 *
 * @param[in] dev The handle of the device
 * @param[in] xStart The starting x position
 * @param[in] yStart The starting y position
 * @param[in] width The ending x position
 * @param[in] height The ending y position
 * @param[in] color The color of the rectangle
 * @param[in] isFill If TRUE the rectangle will be fill
 */
void SSD1306_drawRectangle (SSD1306_DeviceHandle_t dev,
                            uint16_t xStart,
                            uint16_t yStart,
                            uint16_t width,
                            uint16_t height,
                            uint8_t color,
                            bool isFill);

/**
 * The function print a char in the selected position with the selected
 * color and size.
 * The starting point is the top-left corner of the char.
 *
 * @param[in] dev The handle of the device
 * @param[in] xPos The x position
 * @param[in] yPos The y position
 * @param[in] c The char to be draw
 * @param[in] color The foreground color of the char
 * @param[in] size The size for the char, if 0 use default dimension
 * @return GDL_ERRORS_WRONG_POSITION if the dimension plus position of the char
 *         exceeds the width or height of the display, GDL_ERRORS_OK otherwise.
 */
GDL_Errors_t SSD1306_drawChar (SSD1306_DeviceHandle_t dev,
                               uint16_t xPos,
                               uint16_t yPos,
                               uint8_t c,
                               uint8_t color,
                               uint8_t size);

GDL_Errors_t SSD1306_drawString (SSD1306_DeviceHandle_t dev,
                                 uint16_t xPos,
                                 uint16_t yPos,
                                 const char* text,
                                 uint8_t color,
                                 uint8_t size);

/**
 *
 *
 * @param[in] dev The handle of the device
 */
void SSD1306_inverseDisplay (SSD1306_DeviceHandle_t dev);

/**
 *
 *
 * @param[in] dev The handle of the device
 */
void SSD1306_normalDisplay (SSD1306_DeviceHandle_t dev);

/**
 * This function clear the display setting off all pixel
 *
 * @param[in] dev The handle of the device
 */
void SSD1306_clear (SSD1306_DeviceHandle_t dev);

/**
 *
 *
 * @param[in] dev The handle of the device
 */
void SSD1306_flush (SSD1306_DeviceHandle_t dev);

void SSD1306_on (SSD1306_DeviceHandle_t dev);

void SSD1306_off (SSD1306_DeviceHandle_t dev);

void SSD1306_setContrast (SSD1306_DeviceHandle_t dev, uint8_t value);

#endif // __WARCOMEB_SSD1306_H
