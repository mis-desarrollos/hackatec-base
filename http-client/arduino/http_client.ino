#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>
#include <SPI.h>

// ===== WIFI =====
const char* WIFI_SSID     = "TU_RED_WIFI";
const char* WIFI_PASSWORD = "TU_PASSWORD";

// IMPORTANTE:
// ESP32 solo soporta redes WiFi 2.4GHz

// ===== ENDPOINTS =====
const char* URL_GET  = "http://httpbin.org/get";
const char* URL_POST = "http://httpbin.org/post";

// ===== TFT =====
#define TFT_MOSI 23
#define TFT_SCLK 18
#define TFT_CS   15
#define TFT_DC    2
#define TFT_RST   4
#define TFT_BL   32

// ===== INTERVALOS =====
const uint32_t GET_INTERVAL_MS  = 10000;
const uint32_t POST_INTERVAL_MS = 30000;

Adafruit_ST7789 tft =
  Adafruit_ST7789(TFT_CS, TFT_DC, TFT_RST);

uint32_t lastGetTime  = 0;
uint32_t lastPostTime = 0;

uint32_t requestCount = 0;

// ======================================

void drawHeader() {

  tft.fillScreen(ST77XX_BLACK);

  tft.setTextWrap(false);

  tft.setTextColor(
    ST77XX_CYAN,
    ST77XX_BLACK
  );

  tft.setTextSize(2);

  tft.setCursor(10, 10);
  tft.println("HTTP Client");

  tft.drawFastHLine(
    0,
    35,
    240,
    ST77XX_CYAN
  );
}

// ======================================

void drawRequestStatus(
  const char* method,
  int code,
  const char* extra
) {

  drawHeader();

  uint16_t color =
    (code >= 200 && code < 300)
    ? ST77XX_GREEN
    : ST77XX_RED;

  tft.setTextColor(
    color,
    ST77XX_BLACK
  );

  tft.setTextSize(2);

  tft.setCursor(10, 50);

  tft.print(method);
  tft.print(": ");
  tft.println(code);

  tft.setTextSize(1);

  tft.setTextColor(
    ST77XX_WHITE,
    ST77XX_BLACK
  );

  tft.setCursor(10, 85);

  tft.print("Requests: ");
  tft.println(requestCount);

  if (extra != nullptr) {

    tft.setCursor(10, 105);

    tft.setTextColor(
      ST77XX_YELLOW,
      ST77XX_BLACK
    );

    tft.println(extra);
  }

  tft.setCursor(10, 125);

  tft.setTextColor(
    ST77XX_WHITE,
    ST77XX_BLACK
  );

  tft.println("Auto GET/POST");
}

// ======================================

bool connectWiFi() {

  drawHeader();

  tft.setTextSize(1);

  tft.setCursor(10, 50);
  tft.println("Conectando WiFi...");

  Serial.print("Conectando a ");
  Serial.println(WIFI_SSID);

  WiFi.mode(WIFI_STA);

  WiFi.begin(
    WIFI_SSID,
    WIFI_PASSWORD
  );

  uint32_t start = millis();

  while (
    WiFi.status() != WL_CONNECTED &&
    millis() - start < 20000
  ) {

    delay(500);
    Serial.print(".");
  }

  if (WiFi.status() != WL_CONNECTED) {

    Serial.println("WiFi FAIL");

    drawRequestStatus(
      "WiFi",
      -1,
      "Sin conexion"
    );

    return false;
  }

  Serial.println();
  Serial.println("WiFi OK");

  Serial.print("IP: ");
  Serial.println(WiFi.localIP());

  return true;
}

// ======================================

void doGet() {

  if (WiFi.status() != WL_CONNECTED) {

    drawRequestStatus(
      "GET",
      -1,
      "No WiFi"
    );

    return;
  }

  Serial.println();
  Serial.println("GET Request");

  HTTPClient http;

  http.begin(URL_GET);

  int code = http.GET();

  requestCount++;

  Serial.print("HTTP Code: ");
  Serial.println(code);

  String body = "";

  if (code > 0) {

    body = http.getString();

    Serial.println(
      body.substring(0, 200)
    );
  }

  char extra[40] = "";

  if (body.length() > 0) {

    JsonDocument doc;

    DeserializationError err =
      deserializeJson(doc, body);

    if (
      !err &&
      doc["origin"].is<const char*>()
    ) {

      snprintf(
        extra,
        sizeof(extra),
        "IP: %s",
        doc["origin"].as<const char*>()
      );
    }
  }

  drawRequestStatus(
    "GET",
    code,
    extra
  );

  http.end();
}

// ======================================

void doPost() {

  if (WiFi.status() != WL_CONNECTED) {

    drawRequestStatus(
      "POST",
      -1,
      "No WiFi"
    );

    return;
  }

  JsonDocument doc;

  doc["device"] = "ESP32";
  doc["event"] = "auto_post";
  doc["request"] = requestCount + 1;

  String payload;

  serializeJson(
    doc,
    payload
  );

  Serial.println();
  Serial.println("POST Request");

  Serial.println(payload);

  HTTPClient http;

  http.begin(URL_POST);

  http.addHeader(
    "Content-Type",
    "application/json"
  );

  int code =
    http.POST(payload);

  requestCount++;

  Serial.print("HTTP Code: ");
  Serial.println(code);

  if (code > 0) {

    Serial.println(
      http.getString().substring(0, 200)
    );
  }

  drawRequestStatus(
    "POST",
    code,
    "auto_post"
  );

  http.end();
}

// ======================================

void setup() {

  Serial.begin(115200);

  // ===== BACKLIGHT =====
  pinMode(TFT_BL, OUTPUT);
  digitalWrite(TFT_BL, HIGH);

  // ===== SPI =====
  SPI.begin(
    TFT_SCLK,
    -1,
    TFT_MOSI,
    TFT_CS
  );

  // ===== TFT =====
  tft.init(135, 240);

  tft.setRotation(1);

  drawHeader();

  if (connectWiFi()) {

    doGet();
  }
}

// ======================================

void loop() {

  // ===== GET =====

  if (
    millis() - lastGetTime
    > GET_INTERVAL_MS
  ) {

    lastGetTime = millis();

    doGet();
  }

  // ===== POST =====

  if (
    millis() - lastPostTime
    > POST_INTERVAL_MS
  ) {

    lastPostTime = millis();

    doPost();
  }

  delay(20);
}