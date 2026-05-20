# Troubleshooting — Solución de Problemas

Árbol de decisión por síntoma. Encuentra el problema más parecido al tuyo y sigue los pasos en orden.

---

## 1. La laptop no detecta el dispositivo (no aparece puerto)

**Verificaciones en orden:**

1. **¿El cable USB-C es de datos o solo de carga?**
   - Prueba con el cable original del kit MisDesarrollos.
   - Los cables de cargadores de celular suelen ser solo de carga.

2. **¿La pantalla del dispositivo enciende al conectar (aunque sea apagada)?**
   - Si el dispositivo no recibe alimentación → cable malo o puerto USB defectuoso.

3. **¿El driver CH340 está instalado?**
   - Ver [`../DRIVERS.md`](../DRIVERS.md) para verificación e instalación por sistema operativo.

4. **Para Linux: ¿tu usuario está en el grupo `dialout`?**
   ```bash
   groups $USER  # debe incluir 'dialout'
   sudo usermod -aG dialout $USER
   # cerrar sesión y volver a entrar
   ```

5. **Para macOS: ¿permitiste el driver en Preferencias de Seguridad?**
   - Después de instalar el driver, ve a Preferencias → Seguridad → Permitir.
   - Reinicia la Mac.

---

## 2. "Failed to connect" al hacer Upload (Arduino IDE)

Esto significa que la laptop ve el puerto, pero el ESP32 no entra en modo programación.

**Solución estándar:**

1. **Mantén presionado el botón BOOT del dispositivo** (es el botón GPIO 0).
2. Sin soltarlo, pulsa el botón **Upload** en Arduino IDE.
3. Espera a que aparezca el mensaje `Connecting...` con puntos.
4. **Suelta el botón** cuando empiece a aparecer `Writing at 0x...`.

**Si sigue fallando:**

- Reduce la velocidad de upload: `Tools → Upload Speed → 115200`.
- Cambia el cable USB-C.
- Reinicia Arduino IDE.
- Verifica que NO tienes el Monitor Serie abierto en otra ventana (bloquea el puerto).
- Verifica que NO tienes Thonny abierto en paralelo (también bloquea el puerto).

---

## 3. La pantalla está en negro (no enciende)

### Si usas Arduino IDE

**Causa #1: La librería instalada no es la correcta.**
Verifica que tienes instalada **Adafruit ST7735 and ST7789 Library** desde el Library Manager.

**Causa #2: Los pines en el código fueron modificados.**
Los ejemplos usan estos pines. Si los modificaste, restaura:
```cpp
#define TFT_MOSI 23
#define TFT_SCLK 18
#define TFT_CS   15
#define TFT_DC    2
#define TFT_RST   4
#define TFT_BL   32
```

**Causa #3: Backlight no encendido.**
Verifica que en `setup()` aparece:
```cpp
pinMode(TFT_BL, OUTPUT);
digitalWrite(TFT_BL, HIGH);
```

### Si usas MicroPython

**Causa #1: Firmware MicroPython estándar (sin driver `st7789`).**
Verifica en el Shell de Thonny:
```python
>>> import st7789
```
Si falla con `ImportError: no module named 'st7789'`, el firmware flasheado no incluye el driver. Debes flashear el firmware `GENERIC-7789` desde el repo de Russ Hughes (ver `SETUP.md` Paso 2).

**Causa #2: Pines modificados en el código.**
Los ejemplos usan:
```python
bl = Pin(32, Pin.OUT)
spi = SPI(2, baudrate=40000000, polarity=1, phase=1,
          sck=Pin(18), mosi=Pin(23))
tft = st7789.ST7789(spi, 135, 240,
      reset=Pin(4, Pin.OUT),
      dc=Pin(2, Pin.OUT),
      cs=Pin(15, Pin.OUT),
      rotation=1)
```

**Causa #3: Backlight no encendido.**
Verifica que `bl.value(1)` está presente al inicio del script.

---

## 4. La pantalla muestra colores raros, invertidos o "lavados"

### Arduino
La librería Adafruit ST7789 maneja la inversión automáticamente para este display. Si los colores se ven mal, posiblemente la inicialización fue incorrecta:

```cpp
tft.init(135, 240);    // dimensiones del panel
tft.setRotation(1);    // horizontal
```

### MicroPython
Si los colores se ven raros, intenta agregar inversión manualmente después del `init()`:
```python
tft.init()
tft.inversion_mode(True)  # algunos paneles lo requieren
```

---

## 5. WiFi no conecta

**Verifica primero:**

- [ ] La red es **2.4 GHz** (no 5 GHz). Ver [`WIFI.md`](./WIFI.md).
- [ ] SSID y password están escritos exactamente, respetando mayúsculas/minúsculas.
- [ ] No es una red corporativa con portal cautivo o WPA-Enterprise.
- [ ] El RSSI (si llega a conectar) es mejor que -75 dBm. Si es -80 o peor, acércate al router.

**Prueba alternativa:** usa el hotspot de tu celular en modo 2.4 GHz. Esto descarta problemas de la red del venue.

---

## 6. El dispositivo se reinicia solo cada pocos segundos (bootloop)

**Causa #1: Watchdog timeout (Arduino).**
Tu `loop()` tarda más de 5 segundos sin ceder control. Soluciones:
- Reemplaza `delay(5000)` por la técnica de `millis()`:
  ```cpp
  static uint32_t last = 0;
  if (millis() - last > 5000) {
    last = millis();
    // acción
  }
  ```
- Si tienes un loop interno largo, agrega `yield();` periódicamente.

**Causa #2: Stack overflow.**
Variables locales muy grandes (arrays grandes dentro de funciones). Solución: declarar como `static` o globales.

**Causa #3: Out of memory en MicroPython.**
Mensaje `MemoryError`. Soluciones:
- `import gc; gc.collect()` después de operaciones que crean muchos objetos.
- Cierra todas las respuestas HTTP con `resp.close()`.
- Usa `del variable` para liberar referencias grandes.

**Causa #4: Voltaje insuficiente.**
- Cable USB malo o puerto USB de la laptop con poca corriente.
- Solución: cambia el cable, prueba un puerto USB diferente, o usa una powerbank.

**Causa #5: Hardware defectuoso.**
Si nada de lo anterior aplica y el dispositivo arranca, parpadea, reinicia, repite — solicita cambio de unidad al soporte técnico.

---

## 7. Compilation errors en Arduino IDE

### `'WiFi.h' was not found`
Falta soporte ESP32 en Board Manager. Ver [`SETUP.md`](./SETUP.md) paso 3 de Arduino.

### `'Adafruit_ST7789.h' was not found` o `'Adafruit_GFX.h' was not found`
Falta instalar la librería. Library Manager → buscar "Adafruit ST7789" → instalar "Adafruit ST7735 and ST7789 Library" (acepta también instalar GFX como dependencia).

### `Sketch too big` o `Not enough memory`
Tu código + librerías supera el espacio disponible. Soluciones:
- Cambia el Partition Scheme: `Tools → Partition Scheme → "Huge APP (3MB No OTA)"`.
- Quita librerías no usadas.

---

## 8. Errores en MicroPython

### `ImportError: no module named 'st7789'`
El firmware flasheado no incluye el driver del display. Debes usar el firmware `GENERIC-7789` de Russ Hughes, no el firmware MicroPython estándar de micropython.org. Ver [`SETUP.md`](./SETUP.md) Paso 2 de MicroPython.

### `ImportError: no module named 'vga1_bold_16x32'`
Las fuentes vienen con el firmware GENERIC-7789. Si no están disponibles, descarga los `.py` desde la carpeta `fonts/` del repo de Russ Hughes y súbelos a `/` del ESP32 desde Thonny.

### `MemoryError`
Falta de RAM. Soluciones:
- `import gc; gc.collect()` antes de la operación que falla.
- Reducir tamaño de strings y listas en memoria.
- Cerrar respuestas HTTP con `resp.close()`.

### El script funciona desde Thonny pero no al desconectar y reconectar
Solo arranca solo si lo guardaste como `main.py` en la raíz del ESP32 (no en una subcarpeta, no con otro nombre).

---

## 9. Botón BOOT se comporta mal

### El botón cuenta varias pulsaciones por una sola
Necesitas debounce. Los ejemplos `buttons-input/` ya lo implementan. Si copiaste código tuyo, agrega:
```cpp
const uint32_t DEBOUNCE_MS = 50;
```
Y compara contra `millis()` antes de registrar el evento.

### El dispositivo entra en modo programación al arrancar
Si dejas el botón BOOT presionado al energizar el dispositivo, el ESP32 entra en modo bootloader. Suelta el botón antes de conectar USB.

### Necesito más botones para mi solución
Esta placa solo tiene el botón BOOT (GPIO 0) como botón programable. Si tu solución necesita más, agrégalos externamente a los GPIOs libres (ver [`PINOUT.md`](./PINOUT.md)).

---