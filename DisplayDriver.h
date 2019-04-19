/**
 * @file DisplayDriver.h
 * @brief Abstract base class for LittleVGL display drivers
 *
 *  Created on: Jun 14, 2018
 *      Author: gdbeckstein
 */
#ifndef LVGL_DRIVERS_DISPLAYDRIVER_H_
#define LVGL_DRIVERS_DISPLAYDRIVER_H_

#include "DisplayInterface.h"

class DisplayDriver
{
	public:

		/*
		 * Instantiates a DisplayDriver with a given DisplayInterface
		 */
		DisplayDriver(DisplayInterface& interface) : _interface(interface)
		{ }

		virtual ~DisplayDriver() { }

		/*
		 * Initializes the display driver
		 */
		virtual void init(void) = 0;


	protected:

		DisplayInterface& _interface;

};

#endif /* LVGL_DRIVERS_DISPLAYDRIVER_H_ */
