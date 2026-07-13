# WiFi CSI Beacon

## Overview

A robust ESP32-based firmware designed to act as an active WiFi transmitter for Channel State Information (CSI) data collection. The beacon connects to a designated WiFi network and continuously broadcasts UDP "PING" packets to a target receiver at high frequency. This uninterrupted stream of wireless frames enables the receiver to capture fine-grained CSI data, which is essential for advanced WiFi sensing applications such as human presence detection, gesture recognition, and indoor localization.

## Features

- **Reliable WiFi Connectivity**: Configurable SSID and password with automatic retry mechanisms for resilient network connections.
- **High-Frequency UDP Blaster**: Transmits continuous UDP payloads at a precise, configurable interval (e.g., 50 packets per second by default).
- **LVGL-Powered UI**: Integrated display support provides real-time visual feedback on the device's state (Booting, Connecting, Connected, Transmitting, Error).
- **Real-Time Telemetry**: The UI actively displays the local IP address and the total count of UDP packets sent.
- **Modular Architecture**: Clean separation of concerns across WiFi management (`app_wifi`), User Interface (`app_ui`), Display Drivers (`app_display`), and UDP Transmission (`udp_blaster`).

## Hardware Requirements

- ESP32 Development Board with a built-in display (e.g., an ESP32-S3 LCD evaluation board).
- A 2.4 GHz WiFi network.
- A receiver device (another ESP32, Raspberry Pi, or PC) configured to listen on the target UDP port and capture CSI data.

## Configuration

Before building the project, adjust the constants in `main/app_config.h` to match your network and target receiver settings:

- `WIFI_SSID` & `WIFI_PASS`: Your network credentials.
- `DR_SIMI_RX_IP` & `DR_SIMI_RX_PORT`: The IP address and UDP port of the receiver device.
- `UDP_BLAST_INTERVAL_MS`: The transmission interval in milliseconds (default is 20ms).
- `DISPLAY_BRIGHTNESS_PERCENT`: Display backlight brightness level.

## Building and Flashing

This project requires the Espressif IoT Development Framework (ESP-IDF).

1. Set up your ESP-IDF environment.
2. Build the project:
   ```bash
   idf.py build
   ```
3. Flash the firmware and launch the monitor:
   ```bash
   idf.py -p (PORT) flash monitor
   ```

## License

This project is open-source and available under the standard MIT License.
