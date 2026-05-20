from machine import Pin, SPI
import time
import st7789
import vga1_bold_16x32 as font_big
import vga1_8x16 as font_small

# ===== CONFIG =====
DEBOUNCE_MS   = 50
LONG_PRESS_MS = 1000

# ===== COLORES =====
BLACK  = st7789.BLACK
WHITE  = st7789.WHITE
GREEN  = st7789.GREEN
RED    = st7789.RED
YELLOW = st7789.YELLOW
CYAN   = st7789.CYAN

# ===== BUTTON =====
button = Pin(0, Pin.IN, Pin.PULL_UP)

# ===== BACKLIGHT =====
bl = Pin(32, Pin.OUT)
bl.value(1)

# ===== SPI =====
spi = SPI(2, baudrate=40000000, polarity=1, phase=1,
          sck=Pin(18), mosi=Pin(23))

# ===== DISPLAY =====
tft = st7789.ST7789(spi, 135, 240,
      reset=Pin(4, Pin.OUT),
      dc=Pin(2, Pin.OUT),
      cs=Pin(15, Pin.OUT),
      rotation=1)

tft.init()
tft.fill(BLACK)

# ===== ESTADO =====
last_reading    = 1
current_state   = 1
last_debounce   = 0
press_start     = 0
long_triggered  = False
short_count     = 0
long_count      = 0

# ======================================

def draw_ui():
    tft.fill(BLACK)
    tft.text(font_big,   "Buttons Input",      10, 10,  CYAN,  BLACK)
    tft.hline(0, 38, 240, CYAN)
    tft.text(font_small, "BOOT Button (GPIO0)", 10, 50, WHITE, BLACK)

# ======================================

def draw_stats(pressed):
    # limpiar area de estado
    tft.fill_rect(0, 65, 240, 70, BLACK)

    # ===== STATUS =====
    if pressed:
        tft.text(font_big, "PRESSED ",  10, 70, GREEN, BLACK)
    else:
        tft.text(font_big, "RELEASED",  10, 70, RED,   BLACK)

    # ===== COUNTS =====
    tft.text(font_small, "Short: " + str(short_count), 10,  110, WHITE,  BLACK)
    tft.text(font_small, "Long:  " + str(long_count),  120, 110, YELLOW, BLACK)

# ======================================

# ===== SETUP =====
draw_ui()
draw_stats(False)
print("\n=== Buttons Input ===")
print("BOOT = GPIO0")

# ===== LOOP =====
while True:
    reading = button.value()
    now     = time.ticks_ms()

    # ===== DEBOUNCE =====
    if reading != last_reading:
        last_debounce = now

    if time.ticks_diff(now, last_debounce) > DEBOUNCE_MS:

        if reading != current_state:
            current_state = reading

            # ===== PRESIONADO =====
            if current_state == 0:
                press_start    = now
                long_triggered = False
                print("BUTTON PRESS")

            # ===== SOLTADO =====
            else:
                held = time.ticks_diff(now, press_start)
                if not long_triggered:
                    short_count += 1
                    print("SHORT PRESS:", held, "ms")

            draw_stats(current_state == 0)

    # ===== LONG PRESS =====
    if current_state == 0 and not long_triggered:
        if time.ticks_diff(now, press_start) > LONG_PRESS_MS:
            long_triggered = True
            long_count += 1
            print("LONG PRESS")
            draw_stats(True)

    last_reading = reading
    time.sleep_ms(5)