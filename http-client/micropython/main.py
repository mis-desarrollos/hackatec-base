from machine import Pin, SPI
import network
import time
import urequests
import ujson
import st7789
import vga1_bold_16x32 as font_big
import vga1_8x16 as font_small

# ===== CONFIG WIFI =====
WIFI_SSID     = "TU_RED_WIFI"
WIFI_PASSWORD = "TU_PASSWORD"

# ===== ENDPOINTS =====
URL_GET  = "http://httpbin.org/get"
URL_POST = "http://httpbin.org/post"

# ===== INTERVALOS =====
GET_INTERVAL_MS  = 10000
POST_INTERVAL_MS = 30000

# ===== COLORES =====
BLACK  = st7789.BLACK
WHITE  = st7789.WHITE
GREEN  = st7789.GREEN
RED    = st7789.RED
YELLOW = st7789.YELLOW
CYAN   = st7789.CYAN

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

request_count = 0

# ======================================

def draw_header():
    tft.fill(BLACK)
    tft.text(font_big, "HTTP Client", 10, 10, CYAN, BLACK)
    tft.hline(0, 38, 240, CYAN)

# ======================================

def draw_request_status(method, code, extra=None):
    draw_header()

    color = GREEN if 200 <= code < 300 else RED

    tft.text(font_big, method + ": " + str(code), 10, 50, color, BLACK)
    tft.text(font_small, "Requests: " + str(request_count), 10, 90, WHITE, BLACK)

    if extra:
        tft.text(font_small, extra, 10, 108, YELLOW, BLACK)

    tft.text(font_small, "Auto GET/POST", 10, 126, WHITE, BLACK)

# ======================================

def connect_wifi():
    draw_header()
    tft.text(font_small, "Conectando WiFi...", 10, 50, WHITE, BLACK)

    wifi = network.WLAN(network.STA_IF)
    wifi.active(True)
    wifi.connect(WIFI_SSID, WIFI_PASSWORD)

    start = time.ticks_ms()

    while not wifi.isconnected():
        if time.ticks_diff(time.ticks_ms(), start) > 20000:
            print("WiFi FAIL")
            draw_request_status("WiFi", -1, "Sin conexion")
            return False, None
        print(".", end="")
        time.sleep(0.5)

    print()
    print("WiFi OK")
    print("IP:", wifi.ifconfig()[0])
    return True, wifi

# ======================================

def do_get(wifi):
    global request_count

    if not wifi.isconnected():
        draw_request_status("GET", -1, "No WiFi")
        return

    print("\nGET Request")

    try:
        r = urequests.get(URL_GET)
        code = r.status_code
        request_count += 1
        print("HTTP Code:", code)

        extra = ""
        try:
            data = r.json()
            if "origin" in data:
                extra = "IP: " + data["origin"][:25]
        except:
            pass

        r.close()
        draw_request_status("GET", code, extra)

    except Exception as e:
        print("GET Error:", e)
        draw_request_status("GET", -1, "Error")

# ======================================

def do_post(wifi):
    global request_count

    if not wifi.isconnected():
        draw_request_status("POST", -1, "No WiFi")
        return

    payload = ujson.dumps({
        "device":  "ESP32",
        "event":   "auto_post",
        "request": request_count + 1
    })

    print("\nPOST Request")
    print(payload)

    try:
        r = urequests.post(
            URL_POST,
            data=payload,
            headers={"Content-Type": "application/json"}
        )
        code = r.status_code
        request_count += 1
        print("HTTP Code:", code)
        r.close()
        draw_request_status("POST", code, "auto_post")

    except Exception as e:
        print("POST Error:", e)
        draw_request_status("POST", -1, "Error")

# ======================================

# ===== SETUP =====
draw_header()
connected, wifi = connect_wifi()

last_get_time  = time.ticks_ms()
last_post_time = time.ticks_ms()

if connected:
    do_get(wifi)

# ===== LOOP =====
while True:
    now = time.ticks_ms()

    if time.ticks_diff(now, last_get_time) > GET_INTERVAL_MS:
        last_get_time = time.ticks_ms()
        do_get(wifi)

    if time.ticks_diff(now, last_post_time) > POST_INTERVAL_MS:
        last_post_time = time.ticks_ms()
        do_post(wifi)

    time.sleep(0.02)