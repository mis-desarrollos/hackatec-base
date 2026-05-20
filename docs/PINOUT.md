# Pinout — Mapa de Pines GPIO

Mapa completo de los pines disponibles del ESP32 en este dispositivo.

---

## Pines ocupados por hardware integrado

⚠️ **NO usar para otros propósitos.** Estos pines ya están conectados al display y a las funciones internas de la placa.

| GPIO | Uso | Notas |
|---|---|---|
| 2  | TFT DC (data/command) | SPI del display |
| 4  | TFT RST (reset) | Reset del display |
| 15 | TFT CS (chip select) | SPI del display |
| 18 | TFT SCLK (clock) | SPI del display |
| 23 | TFT MOSI (data) | SPI del display |
| 32 | TFT BL (backlight) | Salida digital, controla encendido de pantalla |
| 0  | Botón BOOT | Único botón programable; también pone ESP32 en modo bootloader si está presionado al arrancar |

El botón **RESET** es físico de hardware y no es un GPIO programable. Resetea el ESP32 directamente.

---

## Pines disponibles para tu solución

### Pines GPIO digitales seguros

Soportan input/output, pull-up/pull-down interno, y son seguros de usar en cualquier momento.

| GPIO | Tipo | Notas especiales |
|---|---|---|
| 12 | I/O | ⚠️ "Strapping pin" — si está HIGH al arrancar, cambia voltaje del flash. Mejor solo como salida. |
| 13 | I/O | Libre |
| 14 | I/O | Libre |
| 17 | I/O | Libre |
| 19 | I/O | Libre (en otras placas es MOSI, aquí queda libre) |
| 21 | I/O | I2C SDA por convención |
| 22 | I/O | I2C SCL por convención |
| 25 | I/O | Soporta DAC (salida analógica) |
| 26 | I/O | Soporta DAC (salida analógica) |
| 27 | I/O | Libre |
| 33 | I/O | Soporta ADC (entrada analógica) |

### Pines input-only

Solo soportan entrada, NO salida. NO tienen pull-up/pull-down internos. La placa puede no exponerlos físicamente, verifica antes de soldar.

| GPIO | Tipo | Notas |
|---|---|---|
| 34 | Input | ADC, sin pull-up interno |
| 35 | Input | ADC, sin pull-up interno |
| 36 | Input | ADC, sin pull-up interno (también llamado SVP) |
| 39 | Input | ADC, sin pull-up interno (también llamado SVN) |

### Pines de alimentación

| Pin | Voltaje | Uso |
|---|---|---|
| 5V / VIN | 5V | Salida directa del USB. Útil para alimentar sensores que requieren 5V. |
| 3V3 | 3.3V | Salida del regulador del ESP32. Hasta ~500 mA. |
| GND | 0V | Tierra. Hay varios pines GND disponibles. |

---

## ⚠️ Restricciones críticas

### Voltajes
- Las entradas GPIO del ESP32 son **3.3V tolerantes**.
- Si conectas un sensor que entrega 5V a un GPIO, **destruyes el pin**.
- Usa divisor de voltaje o convertidor de nivel lógico cuando trabajes con sensores 5V.

### Corriente
- Cada GPIO soporta máximo **40 mA** como salida.
- Para LEDs grandes, motores o relés: usa transistor o módulo driver.

### Strapping pins (afectan el arranque del ESP32)
- **GPIO 0, 2, 5, 12, 15** son strapping pins.
- Si tienen niveles "raros" durante el arranque (entre el power-on y el primer segundo), pueden hacer que el ESP32 entre en modo programación, no arranque desde flash, o cambie la frecuencia del flash.
- **GPIO 0, 2 y 15 ya están en uso** por el display y el botón BOOT — no son problema.
- **GPIO 12** sí está disponible pero úsalo preferentemente como SALIDA.

### GPIO 6-11
**NUNCA uses GPIO 6, 7, 8, 9, 10, 11.** Están conectados internamente al chip de memoria SPI Flash del ESP32. Usarlos hace que el dispositivo no arranque.

En esta placa normalmente no están expuestos al usuario, pero menciónalo si vas a soldar conexiones directas.

---

## Periféricos especiales disponibles

### I2C
- **SDA por convención:** GPIO 21
- **SCL por convención:** GPIO 22
- Soporta múltiples dispositivos en el mismo bus (cada uno con dirección distinta).
- Velocidad estándar: 100 kHz. Hasta 400 kHz con dispositivos rápidos.

### SPI
- El SPI #2 del ESP32 ya está usado por el display (pines 15, 18, 23, 2, 4).
- Si necesitas otro SPI para un sensor, puedes usar el SPI #1 (HSPI) con pines arbitrarios.

### UART
- **UART0:** GPIO 1 (TX) y GPIO 3 (RX) — usados para Serial Monitor / USB. NO usar para otros propósitos.
- **UART2:** GPIO 17 (TX) y GPIO 16 (RX) — disponibles si necesitas comunicación serie con otro dispositivo.

### ADC (entrada analógica)
- **Pines ADC1:** GPIO 32-39 (en este dispositivo GPIO 32 está ocupado; disponibles 33, 34, 35, 36, 39).
- **Pines ADC2:** GPIO 0, 2, 4, 12-15, 25-27. **No se pueden usar si WiFi está activo.**
- Resolución: 12 bits (0-4095).
- Lectura:
  - Arduino: `analogRead(pin)`
  - MicroPython: `ADC(Pin(pin)).read()`

### DAC (salida analógica)
- Solo GPIO 25 y GPIO 26.
- Resolución: 8 bits (0-255).
- Salida: 0 a 3.3V proporcional al valor.

### Touch (capacitivo)
- GPIO 13, 14, 27, 33 (los disponibles en esta placa).
- Útil para botones táctiles sin componentes adicionales.

---

## Resumen práctico para HackaTec

Si vas a agregar componentes externos, estos son tus pines recomendados:

**Salidas digitales (LEDs, relés vía transistor):**
GPIO 13, 14, 17, 19, 25, 26, 27

**Entradas digitales (botones extra, sensores ON/OFF):**
GPIO 13, 14, 17, 19, 21, 22, 25, 26, 27, 33

**I2C (la mayoría de sensores modernos: BMP280, MPU6050, OLED, RTC, etc.):**
GPIO 21 (SDA) + GPIO 22 (SCL)

**ADC (sensores analógicos: potenciómetros, LDR, sensores de humedad/temperatura analógicos):**
GPIO 33, 34, 35, 36, 39

**Botones extra con pull-up interno:**
GPIO 13, 14, 17, 19, 25, 26, 27 (los disponibles que NO son input-only)
