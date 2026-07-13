# WiFi CSI Beacon

## Descripción General

Un firmware robusto basado en ESP32 diseñado para actuar como un transmisor WiFi activo para la recolección de datos de Información de Estado de Canal (CSI, por sus siglas en inglés). El *beacon* se conecta a una red WiFi designada y transmite continuamente paquetes UDP "PING" a un receptor objetivo con una alta frecuencia. Este flujo ininterrumpido de tramas inalámbricas permite que el receptor capture datos CSI súper detallados, los cuales son esenciales para aplicaciones avanzadas de detección por WiFi, como la detección de presencia humana, el reconocimiento de gestos y la localización en interiores.

## Características Principales

- **Conectividad WiFi Confiable**: Configuración de SSID y contraseña con mecanismos automáticos de reintento para mantener la red estable.
- **Transmisor UDP de Alta Frecuencia**: Transmite cargas útiles UDP de forma continua a un intervalo preciso y configurable (por ejemplo, 50 paquetes por segundo por defecto).
- **Interfaz de Usuario con LVGL**: El soporte de pantalla integrado proporciona retroalimentación visual en tiempo real sobre el estado del dispositivo (Iniciando, Conectando, Conectado, Transmitiendo, Error).
- **Telemetría en Tiempo Real**: La interfaz muestra en pantalla la dirección IP local y el conteo total de paquetes UDP enviados.
- **Arquitectura Modular**: Separación limpia del código en módulos para la gestión de WiFi (`app_wifi`), Interfaz de Usuario (`app_ui`), Controladores de Pantalla (`app_display`) y Transmisión UDP (`udp_blaster`).

## Requisitos de Hardware

- Placa de desarrollo ESP32 con pantalla integrada (por ejemplo, una placa de evaluación ESP32-S3 LCD).
- Una red WiFi de 2.4 GHz.
- Un dispositivo receptor (otro ESP32, una Raspberry Pi o una PC) configurado para escuchar en el puerto UDP objetivo y capturar los datos CSI.

## Configuración

Antes de compilar el proyecto, asegúrate de ajustar las constantes en el archivo `main/app_config.h` para que coincidan con tu red y receptor:

- `WIFI_SSID` y `WIFI_PASS`: Las credenciales de tu red WiFi.
- `DR_SIMI_RX_IP` y `DR_SIMI_RX_PORT`: La dirección IP y el puerto UDP del dispositivo receptor.
- `UDP_BLAST_INTERVAL_MS`: El intervalo de transmisión en milisegundos (por defecto es 20ms).
- `DISPLAY_BRIGHTNESS_PERCENT`: El nivel de brillo de la pantalla.

## Compilación y Flasheo

Este proyecto requiere el uso del framework ESP-IDF de Espressif.

1. Configura tu entorno de desarrollo ESP-IDF.
2. Compila el proyecto:
   ```bash
   idf.py build
   ```
3. Carga (flashea) el firmware en tu ESP32 y abre el monitor serial:
   ```bash
   idf.py -p (PUERTO) flash monitor
   ```

## Licencia

Este proyecto es de código abierto y está disponible bajo la Licencia MIT.
