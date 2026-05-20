#include <WiFi.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>
#include <SPI.h>

// ===== CONFIG WIFI =====
const char* WIFI_SSID     = "TU_RED_WIFI";
const char* WIFI_PASSWORD = "TU_PASSWORD";
// =======================

// ===== TFT PINS =====
#define TFT_MOSI 23
#define TFT_SCLK 18
#define TFT_CS   15
#define TFT_DC    2
#define TFT_RST   4
#define TFT_BL   32
// =====================

const uint32_t WIFI_TIMEOUT_MS    = 20000;
const uint32_t RECONNECT_INTERVAL = 30000;

Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_RST);

uint32_t lastReconnectAttempt = 0;

void drawStatus(
  const char* line1,
  const char* line2,
  uint16_t color
) {

  tft.fillScreen(ST77XX_BLACK);

  tft.setTextWrap(false);

  tft.setTextColor(color, ST77XX_BLACK);
  tft.setTextSize(2);

  tft.setCursor(10, 20);
  tft.println(line1);

  if (line2 != nullptr) {

    tft.setTextColor(ST77XX_WHITE, ST77XX_BLACK);
    tft.setTextSize(1);

    tft.setCursor(10, 60);
    tft.println(line2);
  }
}

bool connectWiFi() {

  Serial.print("Conectando a: ");
  Serial.println(WIFI_SSID);

  drawStatus("Conectando...", WIFI_SSID, ST77XX_YELLOW);

  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  uint32_t start = millis();

  while (WiFi.status() != WL_CONNECTED) {

    if (millis() - start > WIFI_TIMEOUT_MS) {

      Serial.println("Timeout WiFi");

      drawStatus(
        "Sin WiFi",
        "Revisa SSID/pass",
        ST77XX_RED
      );

      return false;
    }

    Serial.print(".");
    delay(500);
  }

  Serial.println();
  Serial.println("=== WIFI OK ===");

  Serial.print("IP: ");
  Serial.println(WiFi.localIP());

  Serial.print("RSSI: ");
  Serial.println(WiFi.RSSI());

  // ===== DISPLAY =====

  tft.fillScreen(ST77XX_BLACK);

  tft.setTextWrap(false);

  tft.setTextColor(ST77XX_GREEN, ST77XX_BLACK);
  tft.setTextSize(2);

  tft.setCursor(10, 10);
  tft.println("Conectado!");

  tft.setTextColor(ST77XX_WHITE, ST77XX_BLACK);
  tft.setTextSize(1);

  tft.setCursor(10, 45);
  tft.print("SSID: ");
  tft.println(WIFI_SSID);

  tft.setCursor(10, 65);
  tft.print("IP: ");
  tft.println(WiFi.localIP());

  tft.setCursor(10, 85);
  tft.print("RSSI: ");
  tft.print(WiFi.RSSI());
  tft.println(" dBm");

  tft.setCursor(10, 105);
  tft.print("MAC: ");
  tft.println(WiFi.macAddress());

  return true;
}

void setup() {

  Serial.begin(115200);

  // ===== BACKLIGHT =====
  pinMode(TFT_BL, OUTPUT);
  digitalWrite(TFT_BL, HIGH);

  // ===== SPI =====
  SPI.begin(TFT_SCLK, -1, TFT_MOSI, TFT_CS);

  // ===== TFT =====
  tft.init(135, 240);
  tft.setRotation(1);

  drawStatus(
    "HackaTec",
    "WiFi Setup",
    ST77XX_CYAN
  );

  delay(1500);

  connectWiFi();
}

void loop() {

  if (WiFi.status() != WL_CONNECTED) {

    if (
      millis() - lastReconnectAttempt
      > RECONNECT_INTERVAL
    ) {

      Serial.println("Reconectando WiFi...");

      lastReconnectAttempt = millis();

      connectWiFi();
    }
  }

  delay(1000);
}