from machine import Pin, SPI
import time
import st7789
import vga1_bold_16x32 as font_big
import vga1_8x16 as font_small

# ===== COLORES =====
BLACK  = st7789.BLACK
WHITE  = st7789.WHITE
GREEN  = st7789.GREEN
RED    = st7789.RED
YELLOW = st7789.YELLOW
CYAN   = st7789.CYAN
BLUE   = st7789.BLUE
DARK_GREY = st7789.color565(55, 55, 55)

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

screen = 0

# ======================================

def draw_footer():
    tft.fill_rect(0, 112, 240, 23, DARK_GREY)
    tft.text(font_small,
             "SCREEN " + str(screen + 1) + "/3  BOOT=NEXT",
             5, 118, WHITE, DARK_GREY)

# ======================================

def screen1():
    tft.fill(BLACK)
    tft.text(font_big,   "HELLO ESP32",       10, 20, CYAN,  BLACK)
    tft.text(font_small, "Display Widgets OK", 10, 65, WHITE, BLACK)
    draw_footer()

# ======================================

def screen2():
    tft.fill(BLACK)
    tft.text(font_big, "SHAPES", 10, 20, YELLOW, BLACK)

    tft.rect(10, 50, 50, 30, RED)
    tft.fill_rect(70, 50, 50, 30, GREEN)
    tft.circle(175, 65, 15, BLUE)

    draw_footer()

# ======================================

def screen3():
    tft.fill(BLACK)
    tft.text(font_big, "PROGRESS", 10, 20, GREEN, BLACK)

    value = time.ticks_ms() % 100

    tft.rect(10, 60, 200, 20, WHITE)
    tft.fill_rect(12, 62, value * 2, 16, GREEN)

    tft.text(font_small, str(value) + "%", 10, 88, WHITE, BLACK)

    draw_footer()

# ======================================

def render():
    if screen == 0:
        screen1()
    elif screen == 1:
        screen2()
    elif screen == 2:
        screen3()

# ======================================

# ===== SETUP =====
render()

last_btn = 1

# ===== LOOP =====
while True:
    btn = button.value()

    if last_btn == 1 and btn == 0:
        time.sleep_ms(50)
        screen = (screen + 1) % 3
        render()

    last_btn = btn
    time.sleep_ms(10)