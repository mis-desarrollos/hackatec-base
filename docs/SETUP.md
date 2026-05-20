# Setup — Configuración del entorno de desarrollo

Este documento cubre la configuración inicial para **Arduino IDE** y **MicroPython**. Sigue solo la sección del lenguaje que vayas a usar.

---

## Drivers USB

Antes de cualquier IDE, asegúrate de que tu laptop reconoce el dispositivo.

Conecta el ESP32 por USB-C. Si tu sistema no lo detecta automáticamente, instala el driver **CH340**.

📄 Ver [`DRIVERS.md`](../DRIVERS.md) para detalles e instrucciones por sistema operativo.

---

## Opción A — Arduino IDE

### Paso 1 — Instalar Arduino IDE

Descarga la versión 2.x desde: https://www.arduino.cc/en/software

### Paso 2 — Agregar URL del gestor de placas

Abre Arduino IDE y ve a `File → Preferences` (en macOS: `Arduino IDE → Settings`).

En el campo **Additional Boards Manager URLs**, pega:

```
https://espressif.github.io/arduino-esp32/package_esp32_index.json
```

Click `OK`.

### Paso 3 — Instalar el paquete ESP32

Ve a `Tools → Board → Boards Manager`.

Busca **"esp32"** e instala el paquete de **Espressif Systems**.

La instalación puede tardar varios minutos (descarga ~200 MB).

### Paso 4 — Seleccionar la placa

Ve a `Tools → Board → esp32 → ESP32 Dev Module`.

### Paso 5 — Configurar placa y procesador

En el menú `Tools` configura los siguientes parámetros (los valores son los validados para el dispositivo de este kit):

| Parámetro | Valor |
|---|---|
| CPU Frequency | `240MHz (WiFi/BT)` |
| Core Debug Level | `None` |
| Erase All Flash Before Sketch Upload | `Disabled` |
| Events Run On | `Core 1` |
| Flash Frequency | `80MHz` |
| Flash Mode | `QIO` |
| Flash Size | `4MB (32Mb)` |
| JTAG Adapter | `Disabled` |
| Arduino Runs On | `Core 1` |
| Partition Scheme | `Default 4MB with spiffs (1.2MB APP/1.5MB SPIFFS)` |
| PSRAM | `Disabled` |
| Upload Speed | `512000` |

> Si experimentas errores `Failed to connect` al subir código, prueba bajar **Upload Speed** a `115200`.

### Paso 6 — Instalar la librería del display

Ve a `Sketch → Include Library → Manage Libraries`.

Busca **"Adafruit ST7789"** e instala:

- **Adafruit ST7735 and ST7789 Library** (by Adafruit)

Al instalarla, el IDE te ofrecerá instalar también **Adafruit GFX Library** como dependencia. Acepta.

### Paso 7 — Verificar con Hello World

Abre `hello-world/arduino/hello_world.ino` desde este repositorio. Compila y sube.

Si la pantalla muestra texto, todo está listo.

⚠️ Si al subir aparece `Failed to connect: Timed out waiting for packet header`:
mantén presionado el botón **BOOT** del dispositivo y vuelve a intentar el upload. Suelta el botón cuando empiece a escribir.

---

## Opción B — MicroPython

### Paso 1 — Instalar Thonny IDE

Thonny es el IDE recomendado para MicroPython por su simplicidad y herramientas integradas para gestionar el ESP32.

Descarga desde: https://thonny.org

### Paso 2 — Descargar el firmware MicroPython con driver del display incluido

El dispositivo requiere un firmware MicroPython **personalizado** que incluye el módulo `st7789` precompilado (no viene en el firmware estándar de MicroPython).

Descarga el firmware desde el repositorio oficial de Russ Hughes:

https://github.com/russhughes/st7789_mpy/tree/master/firmware/GENERIC-7789

Descarga el archivo `.bin` más reciente disponible para `GENERIC-7789`. Guárdalo en una ubicación accesible.

### Paso 3 — Flashear el firmware al ESP32

**Desde Thonny (recomendado):**

1. Conecta el dispositivo por USB.
2. Abre Thonny.
3. Ve a `Tools → Options → Interpreter`.
4. En `Which kind of interpreter...` selecciona **`MicroPython (ESP32)`**.
5. En `Port` selecciona el puerto del dispositivo (`COMx` en Windows, `/dev/cu.wchusbserial*` en macOS, `/dev/ttyUSB0` en Linux).
6. Click en `Install or update MicroPython` (esquina inferior derecha).
7. En el diálogo, selecciona la opción `Install MicroPython` con un firmware existente y selecciona el `.bin` que descargaste.
8. Click `Install`. Espera 1-2 minutos.

> Si Thonny no te permite seleccionar firmware custom, usa `esptool.py` desde la terminal:
> ```bash
> pip install esptool
> esptool.py --chip esp32 --port <PUERTO> erase_flash
> esptool.py --chip esp32 --port <PUERTO> --baud 460800 write_flash -z 0x1000 firmware.bin
> ```

### Paso 4 — Verificar que MicroPython responde

1. En Thonny, click en `Stop/Restart backend` (icono rojo).
2. En la ventana inferior `Shell` debe aparecer el prompt `>>>`.
3. Prueba:
   ```python
   >>> import st7789
   >>> print("Driver disponible")
   ```
4. Si `import st7789` falla, el firmware flasheado no es el correcto. Vuelve al Paso 2 con el firmware GENERIC-7789.

### Paso 5 — Subir las fuentes al dispositivo

Los ejemplos MicroPython de este repositorio usan fuentes bitmap que vienen junto con el driver `st7789` del firmware Russ Hughes:

- `vga1_bold_16x32`
- `vga1_8x16`

Estas fuentes **ya están incluidas** en el firmware GENERIC-7789, así que no requieren upload adicional.

Para verificar:
```python
>>> import vga1_bold_16x32
>>> import vga1_8x16
```

Si alguna falla con `ImportError`, descárgala desde el mismo repo de Russ Hughes (carpeta `fonts/`) y súbela a `/` del ESP32 desde Thonny.

### Paso 6 — Verificar con Hello World

1. Abre `hello-world/micropython/main.py` desde este repositorio.
2. En Thonny, ve a `File → Save as...`
3. Selecciona **`MicroPython device`** (no "This computer").
4. Guarda como `main.py` en la raíz.
5. Reinicia el dispositivo (Ctrl+D en el Shell, o desconecta/reconecta USB).

Si la pantalla muestra texto, todo está listo.

---

## Pines del dispositivo (referencia)

Los ejemplos del repositorio ya usan estos pines. No los modifiques a menos que sepas exactamente qué estás haciendo.

| Función | GPIO |
|---|---|
| TFT MOSI | 23 |
| TFT SCLK | 18 |
| TFT CS | 15 |
| TFT DC | 2 |
| TFT RST | 4 |
| TFT BL (backlight) | 32 |
| Botón BOOT (único programable) | 0 |

📄 Ver [`PINOUT.md`](./PINOUT.md) para el mapa completo de pines disponibles para tu solución.

---

## Resumen del primer arranque

1. Conectar dispositivo → laptop lo reconoce (o instalar CH340).
2. Elegir lenguaje (Arduino IDE o MicroPython).
3. Instalar IDE + librerías/firmware según sección.
4. Cargar `hello-world/` correspondiente.
5. Confirmar que el display enciende y muestra texto.
6. Continuar con `wifi-setup/`, `http-client/`, `buttons-input/`, `display-widgets/`.
7. Construir tu solución sobre `templates/`.
