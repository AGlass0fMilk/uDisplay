/**
 * @file SPI4Wire.h
 * @brief Brief Description
 * 
 * Detailed Description
 *
 * Link to [markdown like this](@ref PageTag)
 * Make sure you tag the markdown page like this:
 * # Page title {#PageTag}
 * 
 * <a href='MyPDF.pdf'> Link to PDF documents like this</a>
 * If you add document files, make sure to add them into a directory inside a "docs" folder
 * And then run hud-devices/tools/copy-dox-files.py 
 *
 * To use images, make sure they're in an "images" folder and follow the doxygen user manual to add images.
 * You must run copy-dox-files.py after adding images as well.
 *
 * @copyright Copyright &copy; 2018 Heads Up Display, Inc.
 *
 *  Created on: Jul 26, 2018
 *      Author: gdbeckstein
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
		 * @param[in] sclk SCLK pin for interface
		 * @param[in] cs Chip select pin for interface
		 * @param[in] dc Data/Command pin for interface
		 */
		SPI4Wire(PinName mosi, PinName sclk, PinName cs, PinName dc) :
			_chip_select(cs, 1), _data_command(dc, 0), _shared_bus(false)
		{
			_spi = new mbed::SPI(mosi, NC, sclk, NC);
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
