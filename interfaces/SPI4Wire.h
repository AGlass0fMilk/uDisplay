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

#ifndef MBED_LVGL_DRIVERS_INTERFACES_SPI4WIRE_H_
#define MBED_LVGL_DRIVERS_INTERFACES_SPI4WIRE_H_

#include "DisplayInterface.h"

#include "drivers/SPI.h"
#include "drivers/DigitalOut.h"

#if defined(DEVICE_SPI)

/** Low logic level on D/C pin means command */
#define SPI4WIRE_COMMAND_LOGIC_LEVEL 	0

/** High logic level on D/C pin means data */
#define SPI4WIRE_DATA_LOGIC_LEVEL		1

/**
 * 4-wire SPI display interface driver
 * This type of interface is supported by all mbed targets
 * that support SPI and GPIO digital outputs (almost universal).
 * That means it does not need a HAL interface
 */
class SPI4Wire : public DisplayInterface
{
	public:

		/**
		 * Instantiate a 4-wire SPI display interface
		 * @note This constructor does not allow a shared SPI bus
		 *
		 * @param[in] mosi MOSI pin for interface
		 * @param[in] miso MISO pin for interface
		 * @param[in] sclk SCLK pin for interface
		 * @param[in] cs Chip select pin for interface
		 * @param[in] dc Data/Command pin for interface
		 */
		SPI4Wire(PinName mosi, PinName miso, PinName sclk, PinName cs, PinName dc) :
			_chip_select(cs, 1), _data_command(dc, 0), _shared_bus(false)
		{
			_spi = new mbed::SPI(mosi, miso, sclk, NC);
		}

		/**
		 * Instantiate a 4-wire SPI display interface
		 * @note This constructor allows a shared SPI bus.
		 * Not recommended for maximum drawing rate.
		 *
		 * @param[in] spi Shared SPI bus handle
		 * @param[in] cs Chip select pin for interface
		 * @param[in] dc Data/Command pin for interface
		 */
		SPI4Wire(mbed::SPI* spi, PinName cs, PinName dc) :
			_chip_select(cs, 1), _data_command(dc, 0), _shared_bus(true)
		{
			_spi = spi;
		}

		virtual ~SPI4Wire(void)
		{
			// If it's an unshared bus then we instantiated the driver
			// So we are responsible for deleting it
			if(!_shared_bus && _spi)
			{
				delete _spi;
				_spi = NULL;
			}
		}

		/**
		 * Writes a single-byte to the display interface
		 * @param[in] data Single byte to send to the display interface
		 * @param[in] is_cmd Is the byte a command (true) or data (false)?
		 */
		virtual void write(uint8_t data, bool is_cmd = true) {
			_spi->lock();
			_chip_select = 0;
			if(is_cmd) {
				_data_command = SPI4WIRE_COMMAND_LOGIC_LEVEL;
			} else {
				_data_command = SPI4WIRE_DATA_LOGIC_LEVEL;
			}
			_spi->write(data);
			_chip_select = 1;
			_spi->unlock();
		}

		/**
		 * Writes a buffer to the display interface
		 * @param[in] buffer pointer to buffer of bytes to transmit
		 * @param[in] num_cmd_bytes Number of command bytes at beginning of buffer
		 * @param[in] buf_len Total number of bytes in payload buffer
		 */
		virtual void write(const uint8_t* buffer, uint32_t num_cmd_bytes, uint32_t buf_len) {
			_spi->lock();
			_chip_select = 0;
			if(num_cmd_bytes) {
				_data_command = SPI4WIRE_COMMAND_LOGIC_LEVEL;
				_spi->write((const char*) buffer, num_cmd_bytes, NULL, 0);
			}
			_data_command = SPI4WIRE_DATA_LOGIC_LEVEL;
			_spi->write((const char*)(buffer+num_cmd_bytes), (buf_len - num_cmd_bytes), NULL, 0);
			_chip_select = 1;
			_spi->unlock();
		}

		/**
		 * Reads a buffer from the display interface
		 * @note: May not be available
		 * @param[out] buffer to fill with data
		 * @param[in] size Size of buffer
		 * @retval actual number of bytes read (may always be 0 if unsupported)
		 */
		virtual uint8_t read(uint8_t* buffer, uint32_t size) { return 0; } // Not supported

		/**
		 * Sets the frequency of the underlying SPI interface
		 */
		void frequency(int hz)
		{
			_spi->frequency(hz);
		}


		/** Configure the data transmission format.
		 *
		 *  @param bits Number of bits per SPI frame (4 - 16).
		 *  @param mode Clock polarity and phase mode (0 - 3).
		 *
		 * @code
		 * mode | POL PHA
		 * -----+--------
		 *   0  |  0   0
		 *   1  |  0   1
		 *   2  |  1   0
		 *   3  |  1   1
		 * @endcode
		 */
		void format(int bits, int mode = 0)
		{
			_spi->format(bits, mode);
		}

	protected:

		/** Interface SPI bus handle */
		mbed::SPI* _spi;

		/** Chip select output */
		mbed::DigitalOut _chip_select;

		/** Data/Command output */
		mbed::DigitalOut _data_command;

		/** Indicates if the SPI bus is shared */
		const bool _shared_bus;

};

#endif



#endif /* MBED_LVGL_DRIVERS_INTERFACES_SPI4WIRE_H_ */
