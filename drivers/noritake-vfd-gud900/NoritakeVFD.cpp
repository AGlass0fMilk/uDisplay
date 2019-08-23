/*
 * NoritakeVFD.cpp
 *
 *  Created on: Oct 30, 2018
 *      Author: gdbeckstein
 */

#include "NoritakeVFD.h"

#include "platform/mbed_wait_api.h"

NoritakeVFD::NoritakeVFD(DisplayInterface& interface,
		PinName reset, uint32_t height, uint32_t width) :
		DisplayDriver(interface), _height(height), _width(width), _lines(
				height / 8) {
	if(reset != NC) {
		_reset = new mbed::DigitalOut(reset, 1);
	}
	else {
		_reset = NULL;
	}
}

NoritakeVFD::~NoritakeVFD(void) {
	if(_reset != NULL) {
		delete _reset;
		_reset = NULL;
	}
}

void NoritakeVFD::init(void) {
	reset();

	// Reset settings to defaults
	_interface.write(0x1b);
	_interface.write(0x40);
}

void NoritakeVFD::reset(void) {
	if(_reset != NULL) {
		// Pulse reset low
		*_reset = 0;
		wait_ms(2);
		*_reset = 1;
	}
}

void NoritakeVFD::back(void) {
	_interface.write(0x08);
}

void NoritakeVFD::forward(void) {
	_interface.write(0x09);
}

void NoritakeVFD::linefeed() {
	_interface.write(0x0a);
}

void NoritakeVFD::home() {
	_interface.write(0x0b);
}

void NoritakeVFD::carriage_return() {
	_interface.write(0x0d);
}

void NoritakeVFD::crlf() {
	_interface.write(0x0d);
	_interface.write(0x0a);
}

void NoritakeVFD::send_xy(unsigned x, unsigned y) {
	y /= 8;
	_interface.write(x);
	_interface.write(x >> 8);
	_interface.write(y);
	_interface.write(y >> 8);
}

void NoritakeVFD::send_xy1(unsigned x, unsigned y) {
	_interface.write(x);
	_interface.write(x >> 8);
	_interface.write(y);
	_interface.write(y >> 8);
}

void NoritakeVFD::us_command() {
	_interface.write(0x1f);
	_interface.write(0x28);
}

void NoritakeVFD::set_cursor(unsigned x, unsigned y) {
	_interface.write(0x1f);
	_interface.write('$');
	this->send_xy(x, y);
}

void NoritakeVFD::clear_screen() {
	_interface.write(0x0c);
}

void NoritakeVFD::cursor_on() {
	_interface.write(0x1f);
	_interface.write('C');
	_interface.write(0x01);
}

void NoritakeVFD::cursor_off() {
	_interface.write(0x1f);
	_interface.write('C');
	_interface.write(0x00);
}

void NoritakeVFD::dot_mode_8x16() {
	this->us_command();
	_interface.write(0x67);
	_interface.write(0x01);
	_interface.write(0x02);
}

void NoritakeVFD::use_multi_byte_chars(uint8_t enable) {
	this->us_command();
	_interface.write('g');
	_interface.write(0x02);
	_interface.write(enable);
}

void NoritakeVFD::set_multi_byte_char_set(uint8_t code) {
	this->us_command();
	_interface.write('g');
	_interface.write(0x0f);
	_interface.write(code);
}

void NoritakeVFD::use_custom_chars(uint8_t enable) {
	_interface.write(0x1b);
	_interface.write('%');
	_interface.write(enable);
}

uint8_t NoritakeVFD::get_column(uint8_t* src, int col) {
	uint8_t out = 0;

	for (int i = 0; i < 8; i++) {
		if (src[i] & (1 << (4 - col))) {
			out += 1 << (7 - 1);
		}
	}
	return out;
}

void NoritakeVFD::define_custom_char(uint8_t code, uint8_t format,
		uint8_t* data) {
	int j = 0;
	int count;

	_interface.write(0x1b);
	_interface.write('&');
	_interface.write(0x01);
	_interface.write(code);
	_interface.write(code);

	switch (format) {
	case 0: //GUD9005x7Format
		_interface.write(0x05);
		count = 5;
		while (count != 0) {
			_interface.write(data[j]);
			j++;
			count--;
		}
		break;
	case 1: //GUD9007x8Format
		_interface.write(0x07);
		count = 7;
		while (count != 0) {
			_interface.write(data[j]);
			j++;
			count--;
		}
		break;
	case 2: //CUUFormat
		_interface.write(0x05);
		for (int i = 0; i < 5; i++) {
			_interface.write(this->get_column(data, i));
		}
		break;
	}
}

void NoritakeVFD::delete_custom_char(uint8_t code) {
	_interface.write(0x1b);
	_interface.write('?');
	_interface.write(0x01);
	_interface.write(code);
}

void NoritakeVFD::set_ascii_variant(uint8_t code) {
	if (code < 0x0d) {
		_interface.write(0x1b);
		_interface.write('R');
		_interface.write(code);
	}
}

void NoritakeVFD::set_char_set(uint8_t code) {
	if (code < 0x05 || (0x10 <= code && code <= 0x13)) {
		_interface.write(0x1b);
		_interface.write('t');
		_interface.write(code);
	}
}

void NoritakeVFD::set_scroll_mode(uint8_t mode) {
	_interface.write(0x1f);
	_interface.write(mode);
}

void NoritakeVFD::set_horizontal_scroll_speed(uint8_t speed) {
	_interface.write(0x1f);
	_interface.write('s');
	_interface.write(speed);
}

void NoritakeVFD::invert_off() {
	_interface.write(0x1f);
	_interface.write('r');
	_interface.write(0x00);
}

void NoritakeVFD::invert_on() {
	_interface.write(0x1f);
	_interface.write('r');
	_interface.write(0x01);
}

void NoritakeVFD::set_composition_mode(uint8_t mode) {
	_interface.write(0x1f);
	_interface.write('w');
	_interface.write(mode);
}

void NoritakeVFD::set_screen_brightness(unsigned level) {
	if (level == 0) {
		this->display_off();
	} else if (level <= 100) {
		this->display_on();
		_interface.write(0x1f);
		_interface.write('X');
		_interface.write((level * 10 + 120) / 125);
	}
}

void NoritakeVFD::wait(uint8_t wait) {
	this->us_command();
	_interface.write('a');
	_interface.write(0x01);
	_interface.write(wait);
}

void NoritakeVFD::scroll_screen(unsigned x, unsigned y, unsigned times,
		uint8_t speed) {
	unsigned pos = (x * _lines) + (y / 8);
	this->us_command();
	_interface.write('a');
	_interface.write(0x10);
	_interface.write(pos);
	_interface.write(pos >> 8);
	_interface.write(times);
	_interface.write(times >> 8);
	_interface.write(speed);
}

void NoritakeVFD::blink_screen_off() {
	this->us_command();
	_interface.write('a');
	_interface.write(0x11);
	_interface.write(0x00);
	_interface.write(0x00);
	_interface.write(0x00);
	_interface.write(0x00);
}

void NoritakeVFD::blink_screen_on(uint8_t enable, uint8_t reverse,
		uint8_t onTime, uint8_t offTime, uint8_t cycles) {
	this->us_command();
	_interface.write('a');
	_interface.write(0x11);
	_interface.write(enable ? (reverse ? 2 : 1) : 0);
	_interface.write(onTime);
	_interface.write(offTime);
	_interface.write(cycles);
}

void NoritakeVFD::display_off() {
	this->us_command();
	_interface.write('a');
	_interface.write(0x40);
	_interface.write(0x00);
}

void NoritakeVFD::display_on() {
	this->us_command();
	_interface.write('a');
	_interface.write(0x40);
	_interface.write(0x01);
}

void NoritakeVFD::screen_saver(uint8_t mode) {
	this->us_command();
	_interface.write('a');
	_interface.write(0x40);
	_interface.write(mode);
}

void NoritakeVFD::set_font_style(uint8_t proportional, uint8_t evenSpacing) {
	this->us_command();
	_interface.write('g');
	_interface.write(0x03);
	_interface.write(proportional * 2 + evenSpacing);
}

void NoritakeVFD::set_font_size(uint8_t x, uint8_t y, uint8_t tall) {
	if (x <= 4 && y <= 2) {
		this->us_command();
		_interface.write('g');
		_interface.write(0x40);
		_interface.write(x);
		_interface.write(y);
		this->us_command();
		_interface.write('g');
		_interface.write(0x01);
		_interface.write(tall + 1);
	}
}

void NoritakeVFD::select_window(uint8_t window) {
	if (window <= 4) {
		_interface.write(0x10 + window);
	}
}

void NoritakeVFD::define_window(uint8_t window, unsigned x, unsigned y,
		unsigned width, unsigned height) {
	this->us_command();
	_interface.write('w');
	_interface.write(0x02);
	_interface.write(window);
	_interface.write(0x01);
	this->send_xy(x, y);
	this->send_xy(width, height);
}

void NoritakeVFD::delete_window(uint8_t window) {
	this->us_command();
	_interface.write('w');
	_interface.write(0x02);
	_interface.write(window);
	_interface.write(0x00);
	this->send_xy(0, 0);
	this->send_xy(0, 0);
}

void NoritakeVFD::join_screens() {
	this->us_command();
	_interface.write('w');
	_interface.write(0x10);
	_interface.write(0x01);
}

void NoritakeVFD::separate_screens() {
	this->us_command();
	_interface.write('w');
	_interface.write(0x10);
	_interface.write(0x00);
}

/**
 Calculate the minimum value between two given values.

 @param  x Value 1.
 @param  y Value 2.
 @return The smaller integer.
 */
int min(int x, int y) {
	int result = (x < y) ? x : y;
	return result;
}

void NoritakeVFD::fill_rect(unsigned x0, unsigned y0, unsigned x1, unsigned y1,
		uint8_t on) {
	x0 = min(_width, x0);
	x1 = min(_width, x1);
	y0 = min(_height, y0);
	y1 = min(_height, y1);
	if (y1 <= y0 || x1 <= x0) {
		return;
	}
	uint8_t bufw = 8;
	uint8_t bufh = (y1 - y0 + 7) / 8 * 8;
	// Allocate an empty buffer array
	uint8_t *buf = new uint8_t[(bufh / 8 * bufw)]();
	for (unsigned x = 0; x < x1 - x0; x += bufw) {
		uint8_t part = (x + bufw < x1 - x0) ? bufw : (x1 - x0) - x;
		if (on) {
			for (uint8_t col = 0; col < part; col++) {
				for (uint8_t py = y0 % 8; py < y0 % 8 + min(y1 - y0, 8); py++) {
					buf[col * bufh / 8] |= 1 << (7 - py);
				}
				for (uint8_t row = (y0 + 7) / 8; row < y1 / 8; row++) {
					buf[row - y0 / 8 + col * bufh / 8] = 0xff;
				}
				if (y0 / 8 != y1 / 8) {
					for (uint8_t py = 0; py < y1 % 8; py++) {
						buf[(y1 - y0) / 8 + col * bufh / 8] |= 1 << (7 - py);
					}
				}
			}
			this->set_cursor(x + x0, y0);
			this->draw_image(bufw, bufh, buf);
		}
	}
	// Make sure to delete the dynamically allocated buffer!
	delete[] buf;
}

void NoritakeVFD::draw_image(unsigned width, uint8_t height, uint8_t* data) {
	if (height > _height) {
		return;
	}
	this->us_command();
	_interface.write('f');
	_interface.write(0x11);
	this->send_xy(width, height);
	_interface.write(0x01);
	for (unsigned i = 0; i < (height / 8) * width; i++) {
		_interface.write(data[i]);
	}
}

void NoritakeVFD::draw_dot_unit_image(unsigned x, uint8_t y, unsigned width,
		uint8_t height, uint8_t* data) {
	this->us_command();
	_interface.write('d');
	_interface.write(0x21);
	this->send_xy1(x, y);
	this->send_xy1(width, height);
	_interface.write(0x01);
	_interface.write(data, 0, ((height / 8) * width));
//	for (unsigned i = 0; i < (height / 8) * width; i++) {
//		_interface.write(data[i]);
//	}
}

void NoritakeVFD::print_dot_unit_char(unsigned x, uint8_t y, uint8_t* buffer,
		uint8_t len) {
	this->us_command();
	_interface.write('d');
	_interface.write(0x30);
	this->send_xy(x, y * 8);
	_interface.write(0x00);
	_interface.write(len);
	while (len--) {
		_interface.write(*buffer++);
	}
}

void NoritakeVFD::FROM_image_definition(uint8_t aL, uint8_t aH, uint8_t aE,
		unsigned length, uint8_t lE, uint8_t* data) {
	_interface.write(0x1f);
	_interface.write(0x28);
	_interface.write(0x65);
	_interface.write(0x10);
	_interface.write(aL);
	_interface.write(aH);
	_interface.write(aE);
	_interface.write(length);
	_interface.write(length >> 8);
	_interface.write(lE);
	while (length--) {
		_interface.write(*data++);
	}
}

void NoritakeVFD::draw_FROM_image(unsigned x, unsigned y, uint8_t memory,
		uint8_t aL, uint8_t aH, uint8_t aE, uint8_t yL, uint8_t yH,
		unsigned xOffset, unsigned yOffset, unsigned xSize, unsigned ySize) {
	_interface.write(0x1f);
	_interface.write(0x28);
	_interface.write(0x64);
	_interface.write(0x20);
	this->send_xy1(x, y);
	_interface.write(memory);
	_interface.write(aL);
	_interface.write(aH);
	_interface.write(aE);
	_interface.write(yL);
	_interface.write(yH);
	this->send_xy1(xOffset, yOffset);
	this->send_xy1(xSize, ySize);
	_interface.write(0x01);
}

void NoritakeVFD::enter_user_setup_mode() {
	_interface.write(0x1f);
	_interface.write(0x28);
	_interface.write(0x65);
	_interface.write(0x01);
	_interface.write(0x49);
	_interface.write(0x4e);
}

void NoritakeVFD::end_user_setup_mode() {
	_interface.write(0x1f);
	_interface.write(0x28);
	_interface.write(0x65);
	_interface.write(0x02);
	_interface.write(0x4f);
	_interface.write(0x55);
	_interface.write(0x54);
}

void NoritakeVFD::touch_status_read_all() {
	_interface.write(0x1f);
	_interface.write(0x4b);
	_interface.write(0x10);
}

void NoritakeVFD::touch_status_read(uint8_t switchNum) {
	_interface.write(0x1f);
	_interface.write(0x4b);
	_interface.write(0x11);
	_interface.write(switchNum);
}

void NoritakeVFD::touch_set(uint8_t mode) {
	_interface.write(0x1f);
	_interface.write(0x4b);
	_interface.write(0x18);
	_interface.write(mode);
}

void NoritakeVFD::touch_level_read() {
	_interface.write(0x1f);
	_interface.write(0x4b);
	_interface.write(0x14);
}

void NoritakeVFD::touch_change_param(uint8_t mode, uint8_t value) {
	_interface.write(0x1f);
	_interface.write(0x4b);
	_interface.write(0x70);
	_interface.write(mode);
	_interface.write(value);
}

void NoritakeVFD::IO_port_setting(uint8_t portSetting) {
	_interface.write(0x1f);
	_interface.write(0x28);
	_interface.write(0x70);
	_interface.write(0x01);
	_interface.write(0x00);
	_interface.write(portSetting);
}

void NoritakeVFD::IO_port_output(uint8_t portValue) {
	_interface.write(0x1f);
	_interface.write(0x28);
	_interface.write(0x70);
	_interface.write(0x10);
	_interface.write(0x00);
	_interface.write(portValue);
}

void NoritakeVFD::IO_port_input() {

	_interface.write(0x1f);
	_interface.write(0x28);
	_interface.write(0x70);
	_interface.write(0x20);
	_interface.write(0x00);
}

