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

/*!
 * \file  /ssd1306.c
 * \brief
 */

#include "ssd1306.h"

#define SSD1306_SEND_COMMAND                   0x00
#define SSD1306_SEND_DATA                      0x40

#define SSD1306_CMD_SETADDRESSINGMODE          0x20
#define SSD1306_CMD_SETCOLUMNADDRESS           0x21
#define SSD1306_CMD_SETPAGEADDRESS             0x22
#define SSD1306_CMD_DEACTIVATESCROLL           0x2E
#define SSD1306_CMD_ACTIVATESCROLL             0x2F
#define SSD1306_CMD_SETDISPLAYSTARTLINE        0x40
#define SSD1306_CMD_SETCONTRAST                0x81
#define SSD1306_CMD_CHARGEPUMP                 0x8D
#define SSD1306_CMD_SEGMENTREMAP               0xA0
#define SSD1306_CMD_DISPLAYONRAM               0xA4
#define SSD1306_CMD_DISPLAYALLON               0xA5
#define SSD1306_CMD_DISPLAYNORMAL              0xA6
#define SSD1306_CMD_DISPLAYINVERSE             0xA7
#define SSD1306_CMD_SETMUXRATIO                0xA8
#define SSD1306_CMD_SETIREF                    0xAD
#define SSD1306_CMD_DISPLAYOFF                 0xAE
#define SSD1306_CMD_DISPLAYON                  0xAF
#define SSD1306_CMD_COMSCANDIRECTIONUP         0xC0
#define SSD1306_CMD_COMSCANDIRECTIONDOWN       0xC8
#define SSD1306_CMD_SETDISPLAYOFFSET           0xD3
#define SSD1306_CMD_SETDISPLAYCLK              0xD5
#define SSD1306_CMD_COMPINS                    0xDA
#define SSD1306_CMD_SETDESELECTLEVEL           0xDB

#define SSD1306_CMDVALUE_CHARGEPUMP_DISABLE    0x10
#define SSD1306_CMDVALUE_CHARGEPUMP_ENABLE     0x14

#define SSD1306_CMD_COMPINS_COMMON_BASE             0x02
#define SSD1306_CMD_COMPINS_COMMON_SEQUENTIAL       0x00
#define SSD1306_CMD_COMPINS_COMMON_ALTERNATIVE      0x10
#define SSD1306_CMD_COMPINS_COMMON_LEFTRIGHT_NORMAL 0x00
#define SSD1306_CMD_COMPINS_COMMON_LEFTRIGHT_FLIP   0x20

#define SSD1306_ADDRESSING_HORIZONTAL_MODE     0x00
#define SSD1306_ADDRESSING_VERTICAL_MODE       0x01
#define SSD1306_ADDRESSING_PAGE_MODE           0x02

#define SSD1306_CMD_SETIREF_INTERNAL           0x10
#define SSD1306_CMD_SETIREF_EXTERNAL           0x00

static inline void sendCommand (SSD1306_DeviceHandle_t dev, uint8_t command)
{
    uint8_t cmd = command;

    switch (dev->protocolType)
    {
    case GDL_PROTOCOLTYPE_PARALLEL:
        {

        }
        break;
    case GDL_PROTOCOLTYPE_I2C:
        {
            uint8_t retry = 3;
            System_Errors err = ERRORS_NO_ERROR;
            do
            {
                Iic_writeRegister(dev->config.iicDev,
                                  dev->address,
                                  SSD1306_SEND_COMMAND,
                                  IIC_REGISTERADDRESSSIZE_8BIT,
                                  &cmd,
                                  1,
                                  100);
                retry--;
            } while (retry > 0 && err != ERRORS_NO_ERROR);
        }
        break;
    case GDL_PROTOCOLTYPE_SPI:
        {

        }
        break;
    default:
        ohiassert(0);
    }
}

static inline void sendData (SSD1306_DeviceHandle_t dev, uint8_t value)
{
    uint8_t data = value;

    switch (dev->protocolType)
    {
    case GDL_PROTOCOLTYPE_PARALLEL:
        {

        }
        break;
    case GDL_PROTOCOLTYPE_I2C:
        {
            uint8_t retry = 3;
            System_Errors err = ERRORS_NO_ERROR;
            do
            {
                err = Iic_writeRegister(dev->config.iicDev,
                                        dev->address,
                                        SSD1306_SEND_DATA,
                                        IIC_REGISTERADDRESSSIZE_8BIT,
                                        &data,
                                        1,
                                        100);
                retry--;
            } while (retry > 0 && err != ERRORS_NO_ERROR);
        }
        break;
    case GDL_PROTOCOLTYPE_SPI:
        {

        }
        break;
    default:
        ohiassert(0);
    }
}

/*!
 * Sets Internal Iref
 *
 * \note This command is usable only with SSD1308 driver.
 *
 * \param[in] dev: The handle of the device
 */
static void setInternalIref (SSD1306_DeviceHandle_t dev)
{
  sendCommand(dev,SSD1306_CMD_SETIREF);
  sendCommand(dev,SSD1306_CMD_SETIREF_INTERNAL);
}

/*!
 * Sets External Iref (default)
 *
 * \note This command is usable only with SSD1308 driver.
 *
 * \param[in] dev: The handle of the device.
 */
static void setExternalIref (SSD1306_DeviceHandle_t dev)
{
    sendCommand(dev,SSD1306_CMD_SETIREF);
    sendCommand(dev,SSD1306_CMD_SETIREF_EXTERNAL);
}

/*!
 * This function specifies page start address and end address of the display data RAM.
 *
 * \param[in]   dev: The handle of the device.
 * \param[in] start: The page start address.
 * \param[in]   end: The page end address.
 */
static void setPageAddress (SSD1306_DeviceHandle_t dev, uint8_t start, uint8_t end)
{
    sendCommand(dev,SSD1306_CMD_SETPAGEADDRESS);
    sendCommand(dev,start);
    sendCommand(dev,end);
}

/*!
 * This function specifies column start address and end address of the display data RAM.
 *
 * \param[in]   dev: The handle of the device.
 * \param[in] start: The column start address.
 * \param[in]   end: The column end address.
 */
static void setColumnAddress (SSD1306_DeviceHandle_t dev, uint8_t start, uint8_t end)
{
    sendCommand(dev,SSD1306_CMD_SETCOLUMNADDRESS);
    sendCommand(dev,start);
    sendCommand(dev,end);
}

void SSD1306_init (SSD1306_DeviceHandle_t dev, SSD1306_Config_t* config)
{
    ohiassert (config != NULL);
    if (config == NULL)
    {
        return;
    }
    else
    {
        // Initialize the device pointer
        memset(dev, 0, sizeof (SSD1306_Device_t));
        // Save current configuration
        dev->config = *config;
    }

    // Set the device model
    dev->gdl.model = (uint8_t)((dev->config.product & 0xFF00) >> 8);

    // Save device informations
    switch (dev->config.product)
    {
    case SSD1306_PRODUCT_ADAFRUIT_931:
        dev->gdl.height   = 32;
        dev->gdl.width    = 128;
        dev->page         = 4;
        dev->column       = 128;
        dev->protocolType = GDL_PROTOCOLTYPE_I2C;
        dev->address      = 0x3C;
        break;
    case SSD1306_PRODUCT_SEEEDSTUDIO_OLED_1_1:
        dev->gdl.height   = 64;
        dev->gdl.width    = 128;
        dev->page         = 8;
        dev->column       = 128;
        dev->protocolType = GDL_PROTOCOLTYPE_I2C;
        dev->address      = 0x3C;
        break;
    default:
        ohiassert(0);
        break;
    }

    // Save default font size
    dev->gdl.useCustomFont = FALSE;

    // Save callback for drawing pixel
    dev->gdl.drawPixel = SSD1306_drawPixel;
    memset(dev->buffer, 0x00, SSD1306_BUFFER_DIMENSION);

    // Configure periphearl and pins
    switch (dev->protocolType)
    {
    case GDL_PROTOCOLTYPE_PARALLEL:
        {
            // TODO
        }
        break;
    case GDL_PROTOCOLTYPE_I2C:
        {
            ohiassert(dev->config.iicDev != NULL);
            Iic_init(dev->config.iicDev, &dev->config.iicConfig);
        }
        break;
    case GDL_PROTOCOLTYPE_SPI:
        {
            // TODO
        }
        break;
    default:
        ohiassert(0);
    }
    // setup reset pin
    if (dev->config.rstPin != GPIO_PINS_NONE)
    {
        Gpio_config(dev->config.rstPin,GPIO_PINS_OUTPUT);

        // Reset sequence
        Gpio_set(dev->config.rstPin);
        System_delay(1);
        Gpio_clear(dev->config.rstPin);
        System_delay(10);
        Gpio_set(dev->config.rstPin);
    }

    // Starting init procedure
    // Turn off the display
    sendCommand(dev,SSD1306_CMD_DISPLAYOFF);
    System_delay(10);

    // Set display offset to NO OFFSET
    sendCommand(dev,SSD1306_CMD_SETDISPLAYOFFSET);
    sendCommand(dev,0x00);
    // Set display start line to first
    sendCommand(dev,SSD1306_CMD_SETDISPLAYSTARTLINE | 0x00);

    // Set addressing mode to HORIZONTAL - it is the DEAFULT!
    sendCommand(dev,SSD1306_CMD_SETADDRESSINGMODE);
    sendCommand(dev,SSD1306_ADDRESSING_HORIZONTAL_MODE);

    // Select segment re-map, COM scan direction, COM hardware configuration and
    // de-select level
    // They depend on producer choice
    switch (dev->config.product)
    {
    case SSD1306_PRODUCT_ADAFRUIT_931:
        sendCommand(dev,SSD1306_CMD_SEGMENTREMAP | 0x01);
        sendCommand(dev,SSD1306_CMD_COMSCANDIRECTIONDOWN);
        sendCommand(dev,SSD1306_CMD_COMPINS);
        sendCommand(dev,SSD1306_CMD_COMPINS_COMMON_BASE);

        // Set internal clock div to default value
        sendCommand(dev,SSD1306_CMD_SETDISPLAYCLK);
        sendCommand(dev,0x80);

        // Set Multiplex ratio
        sendCommand(dev,SSD1306_CMD_SETMUXRATIO);
        sendCommand(dev,dev->gdl.height-1);

        dev->isChargePump = TRUE;
        break;
    case SSD1306_PRODUCT_SEEEDSTUDIO_OLED_1_1:
        // column address 0 is mapped to SEG0 (Reset)
        sendCommand(dev,SSD1306_CMD_SEGMENTREMAP | 0x00);
        // row address 0 is mapped to COM0 (Reset)
        sendCommand(dev,SSD1306_CMD_COMSCANDIRECTIONUP);
        sendCommand(dev,SSD1306_CMD_COMPINS);
        sendCommand(dev,SSD1306_CMD_COMPINS_COMMON_BASE        |
                        SSD1306_CMD_COMPINS_COMMON_ALTERNATIVE |
                        SSD1306_CMD_COMPINS_COMMON_LEFTRIGHT_NORMAL);

        setInternalIref(dev);

        // Set internal clock
        sendCommand(dev,SSD1306_CMD_SETDISPLAYCLK);
        sendCommand(dev,0x70);

        // Set Multiplex ratio
        sendCommand(dev,SSD1306_CMD_SETMUXRATIO);
        sendCommand(dev,dev->gdl.height-1);

        dev->isChargePump = FALSE;
        // WARNING: nothing to do!
        break;
    default:
        ohiassert(0);
        break;
    }

    // Set contrast to default value
    sendCommand(dev,SSD1306_CMD_SETCONTRAST);
    sendCommand(dev,0x8F);

    // Set normal display
    sendCommand(dev,SSD1306_CMD_DISPLAYNORMAL);


#if 0
    // Choice to enable or disable internal charge pump
    sendCommand(dev,SSD1306_CMD_CHARGEPUMP);
    if (dev->isChargePump)
    {
        sendCommand(dev,SSD1306_CMDVALUE_CHARGEPUMP_ENABLE);
    }
    else
    {
        sendCommand(dev,SSD1306_CMDVALUE_CHARGEPUMP_DISABLE);
    }
#endif

    // Disable scrolling... by default!
    SSD1306_scroll(dev, FALSE);

    // Enable display on with RAM content
    sendCommand(dev,SSD1306_CMD_DISPLAYONRAM);

    // Turn on the display
    // FIXME: Are you shure you want the display ON?
    sendCommand(dev,SSD1306_CMD_DISPLAYON);
}

GDL_Errors_t SSD1306_drawPixel (SSD1306_DeviceHandle_t dev,
                                uint8_t xPos,
                                uint8_t yPos,
                                SSD1306_Color_t color)
{
    if ((xPos >= dev->gdl.width) || (yPos >= dev->gdl.height))
        return GDL_ERRORS_WRONG_POSITION;

    uint16_t pos = (uint16_t) xPos + ((uint16_t) (yPos/8)*dev->gdl.width);

    if (color == SSD1306_COLOR_COLOR)
        dev->buffer[pos] |= (1 << (yPos%8));
    else
        dev->buffer[pos] &= ~(1 << (yPos%8));

    return GDL_ERRORS_SUCCESS;
}

void SSD1306_drawLine (SSD1306_DeviceHandle_t dev,
                       uint8_t xStart,
                       uint8_t yStart,
                       uint8_t xStop,
                       uint8_t yStop,
                       SSD1306_Color_t color)
{
    if (color == SSD1306_COLOR_BLACK)
        GDL_drawLine(&(dev->gdl),xStart,yStart,xStop,yStop,0);
    else
        GDL_drawLine(&(dev->gdl),xStart,yStart,xStop,yStop,1);
}

void SSD1306_drawHLine (SSD1306_DeviceHandle_t dev,
                        uint8_t xStart,
                        uint8_t yStart,
                        uint8_t width,
                        SSD1306_Color_t color)
{
    SSD1306_drawLine(dev,xStart,yStart,xStart+width,yStart,color);
}

void SSD1306_drawVLine (SSD1306_DeviceHandle_t dev,
                        uint8_t xStart,
                        uint8_t yStart,
                        uint8_t height,
                        SSD1306_Color_t color)
{
    SSD1306_drawLine(dev,xStart,yStart,xStart,yStart+height,color);
}

void SSD1306_drawRectangle (SSD1306_DeviceHandle_t dev,
                            uint16_t xStart,
                            uint16_t yStart,
                            uint16_t width,
                            uint16_t height,
                            uint8_t color,
                            bool isFill)
{
    if (color == SSD1306_COLOR_BLACK)
        GDL_drawRectangle(&(dev->gdl),xStart,yStart,width,height,0,isFill);
    else
        GDL_drawRectangle(&(dev->gdl),xStart,yStart,width,height,1,isFill);
}

GDL_Errors_t SSD1306_drawChar (SSD1306_DeviceHandle_t dev,
                               uint16_t xPos,
                               uint16_t yPos,
                               uint8_t c,
                               uint8_t color,
                               uint8_t size)
{
    if (color == SSD1306_COLOR_BLACK)
    {
        return GDL_drawChar(&(dev->gdl),xPos,yPos,c,0,1,size);
    }
    else
    {
        return GDL_drawChar(&(dev->gdl),xPos,yPos,c,1,0,size);
    }
}

GDL_Errors_t SSD1306_drawString (SSD1306_DeviceHandle_t dev,
                                 uint16_t xPos,
                                 uint16_t yPos,
                                 const char* text,
                                 uint8_t color,
                                 uint8_t size)
{
    uint8_t charWidth = size * GDL_DEFAULT_FONT_WIDTH;
    GDL_Errors_t error;

    for (uint8_t i=0; text[i] != '\n' && text[i] != '\0'; i++)
    {
        error = SSD1306_drawChar(dev,(xPos + charWidth * i),yPos,text[i],color,size);
        if (error != GDL_ERRORS_SUCCESS) return error;
    }
    return GDL_ERRORS_SUCCESS;
}

GDL_Errors_t SSD1306_drawPicture (SSD1306_DeviceHandle_t dev,
                                  uint16_t xPos,
                                  uint16_t yPos,
                                  uint16_t width,
                                  uint16_t height,
                                  const uint8_t* picture)
{
    return GDL_drawPicture(dev, xPos, yPos, width, height, picture, GDL_PICTURETYPE_1BIT);
}

void SSD1306_inverseDisplay (SSD1306_DeviceHandle_t dev)
{
    sendCommand(dev,SSD1306_CMD_DISPLAYINVERSE);
}

void SSD1306_normalDisplay (SSD1306_DeviceHandle_t dev)
{
    sendCommand(dev,SSD1306_CMD_DISPLAYNORMAL);
}

void SSD1306_scroll (SSD1306_DeviceHandle_t dev, bool scroll)
{
    if (scroll)
    {
        sendCommand(dev,SSD1306_CMD_ACTIVATESCROLL);
    }
    else
    {
        sendCommand(dev,SSD1306_CMD_DEACTIVATESCROLL);
    }
}

void SSD1306_flush (SSD1306_DeviceHandle_t dev)
{
    // Set start column address and page address
    // They depend on producer choice and device type
    setPageAddress(dev, 0x00, dev->page-1);
    setColumnAddress(dev, 0x00, dev->column-1);

    uint16_t count = dev->column * dev->page;
    for (uint16_t i = 0; i < count; ++i)
    {
        sendData(dev,dev->buffer[i]);
    }
}

void SSD1306_clear (SSD1306_DeviceHandle_t dev)
{
    // Reset memory buffer
    memset(dev->buffer, 0x00, SSD1306_BUFFER_DIMENSION);
    // Flush the new buffer
    SSD1306_flush(dev);
}

void SSD1306_on (SSD1306_DeviceHandle_t dev)
{
    sendCommand(dev,SSD1306_CMD_DISPLAYON);
}

void SSD1306_off (SSD1306_DeviceHandle_t dev)
{
    sendCommand(dev,SSD1306_CMD_DISPLAYOFF);
}

void SSD1306_setContrast (SSD1306_DeviceHandle_t dev, uint8_t value)
{
    sendCommand(dev,SSD1306_CMD_SETCONTRAST);
    sendCommand(dev, value);
}
