# Display — Guía del LCD TFT 1.14"

Documento de referencia sobre el display integrado del dispositivo: especificaciones, dibujado en Arduino y MicroPython, y patrones recomendados.

---

## Especificaciones técnicas

| Característica | Valor |
|---|---|
| Tipo | LCD TFT IPS multicolor |
| Tamaño físico | 1.14 pulgadas (~26 × 14 mm) |
| Resolución | 135 × 240 píxeles |
| Profundidad de color | 16 bits por píxel (formato RGB565, 65K colores) |
| Controlador | ST7789 |
| Interfaz | SPI 4-wire |
| Backlight | Controlado por GPIO 32 (PWM disponible) |
| Voltaje de operación | 3.3 V |
| Corriente | ~20 mA |
| Ángulo de visión | Completo (IPS) |
| Temperatura de operación | -20 a 70 °C |

---

## Conexión interna

```
ST7789 ← ESP32
DC     ← GPIO 2
RST    ← GPIO 4
CS     ← GPIO 15
SCLK   ← GPIO 18
MOSI   ← GPIO 23
BL     ← GPIO 32
```

Estos pines están físicamente cableados en la placa. No son configurables.

---

## Sistema de coordenadas

En **rotación 1** (horizontal, la usada por los ejemplos):

- Origen `(0, 0)` en la esquina **superior izquierda**.
- Eje X aumenta hacia la **derecha** (0 a 239).
- Eje Y aumenta hacia **abajo** (0 a 134).
- Centro de la pantalla: `(120, 67)`.

Rotaciones disponibles:

| Rotación | Orientación | Dimensiones (ancho × alto) |
|---|---|---|
| 0 | Vertical | 135 × 240 |
| 1 | Horizontal | 240 × 135 |
| 2 | Vertical invertido | 135 × 240 |
| 3 | Horizontal invertido | 240 × 135 |

---

## Dibujado en Arduino (Adafruit_ST7789)

### Inicialización

```cpp
#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>
#include <SPI.h>

#define TFT_MOSI 23
#define TFT_SCLK 18
#define TFT_CS   15
#define TFT_DC    2
#define TFT_RST   4
#define TFT_BL   32

Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_RST);

void setup() {
  pinMode(TFT_BL, OUTPUT);
  digitalWrite(TFT_BL, HIGH);

  SPI.begin(TFT_SCLK, -1, TFT_MOSI, TFT_CS);
  tft.init(135, 240);
  tft.setRotation(1);
  tft.fillScreen(ST77XX_BLACK);
}
```

### Texto

```cpp
tft.setTextWrap(false);                              // sin wrap automático
tft.setTextColor(ST77XX_WHITE, ST77XX_BLACK);        // color, fondo
tft.setTextSize(2);                                  // 1, 2, 3, 4, 5...
tft.setCursor(10, 20);                               // x, y
tft.println("Hola");
```

Tamaños típicos:
- `setTextSize(1)` → carácter de 6×8 px
- `setTextSize(2)` → 12×16 px
- `setTextSize(3)` → 18×24 px
- `setTextSize(4)` → 24×32 px

### Formas

```cpp
tft.drawPixel(x, y, color);
tft.drawLine(x0, y0, x1, y1, color);
tft.drawFastHLine(x, y, length, color);     // más rápido para horizontales
tft.drawFastVLine(x, y, length, color);     // más rápido para verticales
tft.drawRect(x, y, w, h, color);            // marco
tft.fillRect(x, y, w, h, color);            // relleno
tft.drawRoundRect(x, y, w, h, radius, color);
tft.fillRoundRect(x, y, w, h, radius, color);
tft.drawCircle(cx, cy, r, color);
tft.fillCircle(cx, cy, r, color);
tft.drawTriangle(x0, y0, x1, y1, x2, y2, color);
tft.fillTriangle(x0, y0, x1, y1, x2, y2, color);
```

### Colores predefinidos

```cpp
ST77XX_BLACK, ST77XX_WHITE
ST77XX_RED, ST77XX_GREEN, ST77XX_BLUE
ST77XX_CYAN, ST77XX_MAGENTA, ST77XX_YELLOW
ST77XX_ORANGE
```

### Color personalizado RGB565

```cpp
uint16_t color = tft.color565(R, G, B);  // R, G, B de 0-255
// Ejemplo: naranja
uint16_t naranja = tft.color565(255, 165, 0);
```

---

## Dibujado en MicroPython (russhughes/st7789_mpy)

### Inicialización

```python
from machine import Pin, SPI
import st7789
import vga1_bold_16x32 as font

bl = Pin(32, Pin.OUT)
bl.value(1)

spi = SPI(2, baudrate=40000000, polarity=1, phase=1,
          sck=Pin(18), mosi=Pin(23))

tft = st7789.ST7789(spi, 135, 240,
      reset=Pin(4, Pin.OUT),
      dc=Pin(2, Pin.OUT),
      cs=Pin(15, Pin.OUT),
      rotation=1)

tft.init()
tft.fill(st7789.BLACK)
```

### Texto

```python
tft.text(font, "Hola", x, y, color, bg_color)
```

Las fuentes son módulos importables incluidos en el firmware. Tamaños típicos disponibles:

- `vga1_8x8` — 8×8 px (la más pequeña)
- `vga1_8x16` — 8×16 px
- `vga1_bold_16x16` — 16×16 px negrita
- `vga1_bold_16x32` — 16×32 px negrita (recomendada para títulos)

### Formas

```python
tft.pixel(x, y, color)
tft.line(x0, y0, x1, y1, color)
tft.hline(x, y, length, color)
tft.vline(x, y, length, color)
tft.rect(x, y, w, h, color)                  # marco
tft.fill_rect(x, y, w, h, color)             # relleno
tft.circle(cx, cy, r, color)                 # marco
tft.fill_circle(cx, cy, r, color)            # relleno
tft.fill(color)                              # toda la pantalla
```

### Colores predefinidos

```python
st7789.BLACK, st7789.WHITE
st7789.RED, st7789.GREEN, st7789.BLUE
st7789.CYAN, st7789.MAGENTA, st7789.YELLOW
```

### Color personalizado RGB565

```python
color = st7789.color565(R, G, B)
naranja = st7789.color565(255, 165, 0)
```

---

## Patrones recomendados

### Actualizar sin parpadear

Si tienes un valor que cambia mucho (temperatura, contador, indicador), **NO repintes toda la pantalla**. Solo repinta el área afectada:

**Arduino:**
```cpp
tft.fillRect(x, y, w, h, ST77XX_BLACK);  // borra el área anterior
tft.setCursor(x, y);
tft.print(nuevoValor);
```

**MicroPython:**
```python
tft.fill_rect(x, y, w, h, st7789.BLACK)  # borra
tft.text(font, str(nuevo_valor), x, y, st7789.WHITE, st7789.BLACK)
```

### Centrar texto horizontalmente

**Arduino:**
```cpp
int16_t x1, y1;
uint16_t w, h;
tft.getTextBounds("Mi texto", 0, 0, &x1, &y1, &w, &h);
tft.setCursor((240 - w) / 2, y);
tft.print("Mi texto");
```

**MicroPython:**
```python
# Calcula manualmente: cada caracter de vga1_bold_16x32 mide 16 px de ancho
text = "Hola"
text_width = len(text) * 16
x = (240 - text_width) // 2
tft.text(font, text, x, y, st7789.WHITE)
```

### Control de brillo del backlight (PWM)

**Arduino:**
```cpp
// En setup
ledcSetup(0, 5000, 8);          // canal 0, 5 kHz, 8 bits (0-255)
ledcAttachPin(TFT_BL, 0);
ledcWrite(0, 128);              // 50% de brillo
```

**MicroPython:**
```python
from machine import PWM
bl_pwm = PWM(Pin(32), freq=5000, duty=512)  # 50% (duty 0-1023)
```

---

## Performance

Velocidades aproximadas para operaciones comunes:

| Operación | Arduino | MicroPython |
|---|---|---|
| `fillScreen` | ~30 ms | ~150 ms |
| `drawLine` corta | <1 ms | ~5 ms |
| `fillRect` mediano | ~5 ms | ~30 ms |
| Dibujar 1 carácter | <1 ms | ~3 ms |
| FPS práctico (animación simple) | ~30-60 | ~5-15 |

Si tu solución necesita animaciones complejas o dibujado muy frecuente, Arduino es la mejor opción.

---

## Galería de widgets

Ver el ejemplo `display-widgets/` (Arduino y MicroPython) para ver implementadas:

- Tarjetas tipo dashboard
- Barras de progreso
- Indicadores ON/OFF con color
- Iconos básicos construidos con primitivas
- Layouts en grilla

Copia los patrones y adáptalos a tu solución.
