# uDisplay - Display framework for Mbed-OS
This library contains abstractions for decoupling display communication specifications from a specific interface used to connect to the display (ie: I2C, SPI, UART, parallel, etc)

It is based on ARM Mbed-OS and follows a similar structure.

## drivers
This subdirectory contains display drivers for various available drivers. Many display drivers are found on multiple display panels, check your datasheet against the drivers here to determine if yours is supported.

## interfaces
This subdirectory contains display interfaces. A display interface abstracts away the specific physical transport used to exchange command and framebuffer data with the display driver IC.

## hal
This subdirectory contains C hardware abstraction layer specifications for physical interfaces that aren't available from Mbed-OS

## targets
This subdirectory contains target implementations of C HAL APIs.