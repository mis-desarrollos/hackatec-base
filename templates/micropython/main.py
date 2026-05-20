from machine import Pin, SPI
import st7789
import vga1_bold_16x32 as font

# ===== TFT PINS =====
TFT_MOSI = 23
TFT_SCLK = 18
TFT_CS   = 15
TFT_DC   = 2
TFT_RST  = 4
TFT_BL   = 32

# ===== BACKLIGHT =====
bl = Pin(TFT_BL, Pin.OUT)
bl.value(1)

# ===== SPI =====
spi = SPI(
    2,
    baudrate=40000000,
    polarity=1,
    phase=1,
    sck=Pin(TFT_SCLK),
    mosi=Pin(TFT_MOSI)
)

# ===== DISPLAY =====
tft = st7789.ST7789(
    spi,
    135,
    240,
    reset=Pin(TFT_RST, Pin.OUT),
    dc=Pin(TFT_DC, Pin.OUT),
    cs=Pin(TFT_CS, Pin.OUT),
    rotation=1
)

tft.init()
tft.fill(st7789.BLACK)

# ===== UI BASE =====
tft.text(font, "ESP32 READY", 10, 20, st7789.CYAN, st7789.BLACK)
tft.text(font, "Template base OK", 10, 60, st7789.WHITE, st7789.BLACK)

# ===== LOOP =====
while True:
    pass