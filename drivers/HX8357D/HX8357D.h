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

#ifndef LVGL_DRIVERS_HX8357D_H_
#define LVGL_DRIVERS_HX8357D_H_

#include "DisplayDriver.h"

#include "hx8357d_registers.h"

class HX8357D : public DisplayDriver
{
	public:

		HX8357D(DisplayInterface& interface) : DisplayDriver(interface) { }

		virtual ~HX8357D() {}

		/*
		 * Initializes the display driver
		 */
		virtual void init(void);

		/**
		 * Resets the display driver
		 */
		void reset(void);

		/**
		 * Gets the display driver's ID
		 * @retval id 32-bit id of the display driver
		 */
		uint32_t get_id(void);

		/**
		 * Gets the display's power mode
		 * @retval mode
		 */
		uint8_t get_power_mode(void);

		/**
		 * Gets the display's address mode
		 */
		uint8_t get_address_mode(void);

		/**
		 * Inverts the display
		 */
		void invert_on(void);

		/**
		 * Deinverts the display
		 */
		void invert_off(void);

		/**
		 * Sets the gamma curve
		 */
		void set_gamma_curve(uint8_t curve);

		/**
		 * Turns the display on
		 */
		void display_on(void);

		/**
		 * Turns the display off
		 */
		void display_off(void);

		void set_column_address(uint16_t col_start, uint16_t col_end);

		void set_row_address(uint16_t row_start, uint16_t row_end);

		void write_memory_start();

		void read_memory_start();

		void set_tear_on();

		void set_tear_off();

		void set_address_mode(uint8_t mode);

		void enter_idle_mode();

		void exit_idle_mode();

		void enter_sleep_mode();

		void exit_sleep_mode();

		void set_extc();

		void set_rgb(uint8_t* mode);

		void set_osc(uint8_t mode);

		void set_panel(uint8_t mode);

		void set_power(uint8_t* mode);

		void set_stba(uint8_t* mode);

		void set_cyc(uint8_t* mode);

		void set_color_mode(uint8_t mode);

		void set_com(uint8_t mode);

};

#endif /* LVGL_DRIVERS_HX8357D_H_ */
