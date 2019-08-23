/* uDisplay library
 * Copyright (c) 2018-2019 George "AGlass0fMilk" Beckstein
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef MBED_LVGL_DRIVERS_ST7789_ST7789_REGISTERS_H_
#define MBED_LVGL_DRIVERS_ST7789_ST7789_REGISTERS_H_

#define ST77XX_NOP     0x00
#define ST77XX_SWRESET 0x01
#define ST77XX_RDDID   0x04
#define ST77XX_RDDST   0x09

#define ST77XX_SLPIN   0x10
#define ST77XX_SLPOUT  0x11
#define ST77XX_PTLON   0x12
#define ST77XX_NORON   0x13

#define ST77XX_INVOFF  0x20
#define ST77XX_INVON   0x21
#define ST77XX_DISPOFF 0x28
#define ST77XX_DISPON  0x29
#define ST77XX_CASET   0x2A
#define ST77XX_RASET   0x2B
#define ST77XX_RAMWR   0x2C
#define ST77XX_RAMRD   0x2E

#define ST77XX_PTLAR   0x30
#define ST77XX_TEOFF   0x34
#define ST77XX_TEON    0x35
#define ST77XX_COLMOD  0x3A
#define ST77XX_MADCTL  0x36

#define ST77XX_MADCTL_MY  0x80
#define ST77XX_MADCTL_MX  0x40
#define ST77XX_MADCTL_MV  0x20
#define ST77XX_MADCTL_ML  0x10
#define ST77XX_MADCTL_RGB 0x00

#define ST77XX_RDID1   0xDA
#define ST77XX_RDID2   0xDB
#define ST77XX_RDID3   0xDC
#define ST77XX_RDID4   0xDD

#endif /* MBED_LVGL_DRIVERS_ST7789_ST7789_REGISTERS_H_ */
