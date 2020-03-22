# REDar - infraRed Radar 

Using MLX90640 and ESP32, read an IR image of 32x24 pixels

## Hardware Requirements
- ESP32
- MLX90640
- Mac, Linux or Windows for programming

## Software Requirements
- [ESP-IDF](https://github.com/espressif/esp-idf) v4.0 (Probably works with other versions too)

## How-To
This assumes command-line and ESP-IDF knowledge.
- Install ESP-IDF on your system and set it up properly (source the environment & provide path to your USB device)
- Wire the MLX sensor to ESP32 power and ESP32 I2C bus 
- SDA_PIN 21 & SCL_PIN 22 are defaults in this code

- Clone this repo 
- Within this repo: make flash & make monitor

## Program entry point
- redar.c in main folder

## What it does
Right now it only reads out cyclically the temperature array properly

## References
- [MLX90640 generic C driver](https://github.com/melexis/mlx90640-library)
- [MLX90640 Datasheet](https://www.melexis.com/-/media/files/documents/datasheets/mlx90640-datasheet-melexis.pdf)