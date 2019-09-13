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

#ifndef __WARCOMEB_SSD1306_TYPE_H
#define __WARCOMEB_SSD1306_TYPE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "../GDL/gdltype.h"

/*!
 * \defgroup SSD1306_Type
 * \ingroup SSD1306
 * \{
 */

typedef enum _SSD1306_Color_t
{
    SSD1306_COLOR_BLACK,
    SSD1306_COLOR_COLOR
} SSD1306_Color_t;

/*!
 * \defgroup SSD1306_Type_Product
 * \{
 */
#define SSD1306_PRODUCT_ADAFRUIT_931             (0x0001 | GDL_MODELTYPE_SSD1306)
/*!
 * \}
 */

/*!
 * \}
 */

#ifdef __cplusplus
}
#endif

#endif // __WARCOMEB_SSD1306_TYPE_H
