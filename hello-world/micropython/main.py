from machine import Pin, SPI
import st7789
import vga1_bold_16x32 as font

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
tft.fill(st7789.BLACK)

# ===== TEXTO CON FUENTE =====
tft.text(font, "Hello!", 20, 40, st7789.WHITE, st7789.BLACK)
tft.text(font, "HackaTec", 20, 80, st7789.GREEN, st7789.BLACK)
