/*
 * DisplayInterface.h
 *
 * Interface for communicating with a display
 *
 * Allows the underlying transport (Parallel, I2C, SPI, etc)
 * to be changed without modifying the behavior of the
 * DisplayDriver using the interface
 *
 *  Created on: Jul 16, 2018
 *      Author: gdbeckstein
 */

#ifndef DRIVERS_DISPLAYINTERFACE_H_
#define DRIVERS_DISPLAYINTERFACE_H_

class DisplayInterface
{

public:

	virtual ~DisplayInterface(void) { }

	/**
	 * Writes a buffer to the display interface
	 * @param[in] buffer pointer to buffer of bytes to transmit
	 * @param[in] num_cmd_bytes Number of command bytes at beginning of buffer
	 * @param[in] buf_len Total number of bytes in payload buffer
	 */
	virtual void write(const uint8_t* buffer, uint32_t num_cmd_bytes, uint32_t buf_len) = 0;

	/**
	 * Reads a buffer from the display interface
	 * @note: May not be available
	 * @param[out] buffer to fill with data
	 * @param[in] size Size of buffer
	 * @retval actual number of bytes read (may always be 0 if unsupported)
	 */
	virtual uint8_t read(uint8_t* buffer, uint32_t size) = 0;

};

#endif /* DRIVERS_DISPLAYINTERFACE_H_ */
