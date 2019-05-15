/*
 * UARTInterface.h
 *
 *  Created on: May 7, 2019
 *      Author: gdbeckstein
 */

#ifndef UDISPLAY_INTERFACES_UARTINTERFACE_H_
#define UDISPLAY_INTERFACES_UARTINTERFACE_H_

#include "DisplayInterface.h"
#include "drivers/UARTSerial.h"

#if (DEVICE_SERIAL && DEVICE_INTERRUPTIN) || defined(DOXYGEN_ONLY)

class UARTInterface : public mbed::UARTSerial, public DisplayInterface
{
public:

	UARTInterface(PinName tx, PinName rx,
			int baud = MBED_CONF_PLATFORM_DEFAULT_SERIAL_BAUD_RATE) : mbed::UARTSerial(tx, rx, baud), DisplayInterface() { }

	/**
	 * Writes a single-byte to the display interface
	 * @param[in] data Single byte to send to the display interface
	 * @param[in] is_cmd Is the byte a command (true) or data (false)?
	 */
	virtual void write(uint8_t data, bool is_cmd = true) {
		// TODO - Some displays may need a separate data pin?
		// For now ignore the data/cmd difference
		mbed::UARTSerial::write(&data, 1);
	}

	/**
	 * Writes a buffer to the display interface
	 * @param[in] buffer pointer to buffer of bytes to transmit
	 * @param[in] num_cmd_bytes Number of command bytes at beginning of buffer
	 * @param[in] buf_len Total number of bytes in payload buffer
	 */
	virtual void write(const uint8_t* buffer, uint32_t num_cmd_bytes, uint32_t buf_len) {
		mbed::UARTSerial::write(buffer, buf_len);
	}

	/**
	 * Reads a buffer from the display interface
	 * @note: May not be available
	 * @param[out] buffer to fill with data
	 * @param[in] size Size of buffer
	 * @retval actual number of bytes read (may always be 0 if unsupported)
	 */
	virtual uint8_t read(uint8_t* buffer, uint32_t size) {
		return mbed::UARTSerial::read(buffer, size);
	}

};

#endif

#endif /* UDISPLAY_INTERFACES_UARTINTERFACE_H_ */
