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

#include <stdint.h>

#ifndef DRIVERS_DISPLAYINTERFACE_H_
#define DRIVERS_DISPLAYINTERFACE_H_

class DisplayInterface
{

public:

	virtual ~DisplayInterface(void) { }

	/**
	 * Writes a single-byte to the display interface
	 * @param[in] data Single byte to send to the display interface
	 * @param[in] is_cmd Is the byte a command (true) or data (false)?
	 */
	virtual void write(uint8_t data, bool is_cmd = true) = 0;

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
