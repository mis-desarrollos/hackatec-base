from machine import Pin, SPI
import network
import time
import st7789
import vga1_bold_16x32 as font_big
import vga1_8x16 as font_small

# ===== CONFIG WIFI =====
WIFI_SSID     = "TU_RED_WIFI"
WIFI_PASSWORD = "TU_PASSWORD"
# =======================

WIFI_TIMEOUT_MS    = 20000
RECONNECT_INTERVAL = 30000

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

# ===== COLORES =====
BLACK  = st7789.BLACK
WHITE  = st7789.WHITE
GREEN  = st7789.GREEN
RED    = st7789.RED
YELLOW = st7789.YELLOW
CYAN   = st7789.CYAN

# ===== FUNCION DRAW STATUS =====
def draw_status(line1, line2, color):
    tft.fill(BLACK)
    tft.text(font_big, line1, 10, 20, color, BLACK)
    if line2:
        tft.text(font_small, line2, 10, 70, WHITE, BLACK)

# ===== FUNCION CONECTAR WIFI =====
def connect_wifi():
    print("Conectando a:", WIFI_SSID)
    draw_status("Conectando", WIFI_SSID, YELLOW)

    wifi = network.WLAN(network.STA_IF)
    wifi.active(True)
    wifi.connect(WIFI_SSID, WIFI_PASSWORD)

    start = time.ticks_ms()

    while not wifi.isconnected():
        if time.ticks_diff(time.ticks_ms(), start) > WIFI_TIMEOUT_MS:
            print("Timeout WiFi")
            draw_status("Sin WiFi", "Revisa SSID/pass", RED)
            return False, None
        print(".", end="")
        time.sleep(0.5)

    print()
    print("=== WIFI OK ===")

    ip, _, _, _ = wifi.ifconfig()
    rssi = wifi.status('rssi')
    mac  = ':'.join(['%02x' % b for b in wifi.config('mac')])

    print("IP:", ip)
    print("RSSI:", rssi)

    # ===== PANTALLA CONECTADO =====
    tft.fill(BLACK)
    tft.text(font_big,   "Conectado!",        10, 10,  GREEN,  BLACK)
    tft.text(font_small, "SSID: " + WIFI_SSID, 10, 55,  WHITE,  BLACK)
    tft.text(font_small, "IP: "  + ip,          10, 75,  WHITE,  BLACK)
    tft.text(font_small, "RSSI: "+ str(rssi) + " dBm", 10, 95, WHITE, BLACK)
    tft.text(font_small, "MAC: " + mac,          10, 115, WHITE, BLACK)

    return True, wifi

# ===== SETUP =====
draw_status("HackaTec", "WiFi Setup", CYAN)
time.sleep(1.5)

connected, wifi = connect_wifi()
last_reconnect = time.ticks_ms()

# ===== LOOP =====
while True:
    if not wifi.isconnected():
        if time.ticks_diff(time.ticks_ms(), last_reconnect) > RECONNECT_INTERVAL:
            print("Reconectando WiFi...")
            last_reconnect = time.ticks_ms()
            connected, wifi = connect_wifi()
    time.sleep(1)