/*
 * DisplaySPI.h
 *
 *  Created on: Jun 9, 2019
 *      Author: gdbeckstein
 */

#ifndef MBED_LVGL_UDISPLAY_TARGETS_TARGET_NORDIC_TARGET_MCU_NRF52840_DISPLAYSPI_H_
#define MBED_LVGL_UDISPLAY_TARGETS_TARGET_NORDIC_TARGET_MCU_NRF52840_DISPLAYSPI_H_

#include "PinNames.h"
#include "platform/mbed_assert.h"
#include "platform/Callback.h"

#include "nrfx_spim.h"

#include "DisplayInterface.h"

#if defined(DEVICE_SPI)

static const nrfx_spim_t m_spi_master_3 = NRFX_SPIM_INSTANCE(3);

extern "C" {
	void spim3_event_handler(nrfx_spim_evt_t const * p_event, void * p_context);
}

/**
 * Hardware-accelerated SPI4Wire interface
 * On the nRF52840, the SPIM3 interface features
 * a hardware-controlled Data/Command pin
 * commonly used with displays over SPI
 *
 * @note Not synchronized. Should be synchronized externally by
 * user application code using the event callback.
 */
class DisplaySPI : public DisplayInterface
{
	public:
		/**
		 * Instantiate a 4-wire SPI display interface
		 * @note This constructor does not allow a shared SPI bus
		 *
		 * @param[in] mosi MOSI pin for interface
		 * @param[in] sclk SCLK pin for interface
		 * @param[in] cs Chip select pin for interface
		 * @param[in] dcx Data/Command pin for interface
		 */
		DisplaySPI(PinName mosi, PinName sclk, PinName cs, PinName dcx) :
			user_callback(NULL) {

			/**
			 * Initialize the SPIM3 peripheral
			 */
		    nrfx_spim_config_t spi_config = NRFX_SPIM_DEFAULT_CONFIG;
		    spi_config.frequency      = NRF_SPIM_FREQ_8M;
		    spi_config.ss_pin         = cs;
		    spi_config.miso_pin       = NRFX_SPIM_PIN_NOT_USED;
		    spi_config.mosi_pin       = mosi;
		    spi_config.sck_pin        = sclk;
		    spi_config.dcx_pin        = dcx;
		    spi_config.use_hw_ss      = true;
		    spi_config.ss_active_high = false;
		    nrfx_err_t err = nrfx_spim_init(&m_spi_master_3, &spi_config,
		    		spim3_event_handler,
				this);	// Pass a handle to this instance for event processing

		    MBED_ASSERT(err == NRF_SUCCESS);

		}

		virtual ~DisplaySPI(void) {

			/**
			 * Deinitialize the SPIM3 peripheral
			 */
			nrfx_spim_uninit(&m_spi_master_3);

		}

		/**
		 * Attach a callback to be executed when a SPIM transfer is done
		 */
		void attach(mbed::Callback<void(nrfx_spim_evt_t const *)> cb) {
			user_callback = cb;
		}

		/**
		 * Writes a single-byte to the display interface
		 * @param[in] data Single byte to send to the display interface
		 * @param[in] is_cmd Is the byte a command (true) or data (false)?
		 */
		virtual void write(uint8_t data, bool is_cmd = true) {
			this->write(&data, 1, 1);
		}

		/**
		 * Writes a buffer to the display interface
		 * @param[in] buffer pointer to buffer of bytes to transmit
		 * @param[in] num_cmd_bytes Number of command bytes at beginning of buffer
		 * @param[in] buf_len Total number of bytes in payload buffer
		 */
		virtual void write(const uint8_t* buffer, uint32_t num_cmd_bytes, uint32_t buf_len) {

			nrfx_spim_xfer_desc_t xfer_desc = NRFX_SPIM_XFER_TX(buffer, buf_len);
			nrfx_spim_xfer_dcx(&m_spi_master_3, &xfer_desc, 0, num_cmd_bytes);

		}

		/**
		 * Reads a buffer from the display interface
		 * @note: May not be available
		 * @param[out] buffer to fill with data
		 * @param[in] size Size of buffer
		 * @retval actual number of bytes read (may always be 0 if unsupported)
		 */
		virtual uint8_t read(uint8_t* buffer, uint32_t size) {
			return 0; // Not supported
		}

		/**
		 * Private function to be called from C code
		 */
		void _spim_event(nrfx_spim_evt_t const* evt) {
			if(this->user_callback) {
				this->user_callback(evt); // Notify the application
			}
		}

	private:

		mbed::Callback<void(nrfx_spim_evt_t const *)> user_callback;

};

void spim3_event_handler(nrfx_spim_evt_t const * p_event, void * p_context) {
	// Cast the context into a DisplaySPI pointer
	DisplaySPI* interface = (DisplaySPI*) p_context;
	if(interface != NULL) {
		interface->_spim_event(p_event);
	}
}

#endif /** defined(DEVICE_SPI) */

#endif /* MBED_LVGL_UDISPLAY_TARGETS_TARGET_NORDIC_TARGET_MCU_NRF52840_DISPLAYSPI_H_ */
