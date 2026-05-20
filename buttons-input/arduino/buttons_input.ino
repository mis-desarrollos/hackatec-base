#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>
#include <SPI.h>

// ===== TFT =====
#define TFT_MOSI 23
#define TFT_SCLK 18
#define TFT_CS   15
#define TFT_DC    2
#define TFT_RST   4
#define TFT_BL   32

// ===== BUTTON =====
#define BUTTON_BOOT 0

// ===== CONFIG =====
const uint32_t DEBOUNCE_MS   = 50;
const uint32_t LONG_PRESS_MS = 1000;

Adafruit_ST7789 tft =
  Adafruit_ST7789(TFT_CS, TFT_DC, TFT_RST);

// ===== BUTTON STATE =====

bool lastReading       = HIGH;
bool currentState      = HIGH;

uint32_t lastDebounce  = 0;
uint32_t pressStart    = 0;

bool longTriggered     = false;

uint32_t shortCount    = 0;
uint32_t longCount     = 0;

// ======================================

void drawUI() {

  tft.fillScreen(ST77XX_BLACK);

  tft.setTextWrap(false);

  // ===== HEADER =====

  tft.setTextColor(
    ST77XX_CYAN,
    ST77XX_BLACK
  );

  tft.setTextSize(2);

  tft.setCursor(10, 10);
  tft.println("Buttons Input");

  tft.drawFastHLine(
    0,
    35,
    240,
    ST77XX_CYAN
  );

  // ===== LABEL =====

  tft.setTextColor(
    ST77XX_WHITE,
    ST77XX_BLACK
  );

  tft.setTextSize(1);

  tft.setCursor(10, 50);
  tft.println("BOOT Button (GPIO0)");
}

// ======================================

void drawStats(bool pressed) {

  tft.fillRect(
    0,
    65,
    240,
    70,
    ST77XX_BLACK
  );

  // ===== STATUS =====

  tft.setTextSize(3);

  tft.setTextColor(
    pressed
    ? ST77XX_GREEN
    : ST77XX_RED,
    ST77XX_BLACK
  );

  tft.setCursor(10, 70);

  tft.println(
    pressed
    ? "PRESSED"
    : "RELEASED"
  );

  // ===== COUNTS =====

  tft.setTextSize(1);

  tft.setTextColor(
    ST77XX_WHITE,
    ST77XX_BLACK
  );

  tft.setCursor(10, 110);

  tft.print("Short: ");
  tft.println(shortCount);

  tft.setCursor(120, 110);

  tft.setTextColor(
    ST77XX_YELLOW,
    ST77XX_BLACK
  );

  tft.print("Long: ");
  tft.println(longCount);
}

// ======================================

void setup() {

  Serial.begin(115200);

  // ===== BUTTON =====
  pinMode(
    BUTTON_BOOT,
    INPUT_PULLUP
  );

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

  drawUI();

  drawStats(false);

  Serial.println();
  Serial.println(
    "=== Buttons Input ==="
  );

  Serial.println(
    "BOOT = GPIO0"
  );
}

// ======================================

void loop() {

  bool reading =
    digitalRead(BUTTON_BOOT);

  // ===== DEBOUNCE =====

  if (reading != lastReading) {

    lastDebounce = millis();
  }

  if (
    millis() - lastDebounce
    > DEBOUNCE_MS
  ) {

    if (reading != currentState) {

      currentState = reading;

      // ===== PRESSED =====

      if (currentState == LOW) {

        pressStart = millis();

        longTriggered = false;

        Serial.println(
          "BUTTON PRESS"
        );
      }

      // ===== RELEASED =====

      else {

        uint32_t held =
          millis() - pressStart;

        if (!longTriggered) {

          shortCount++;

          Serial.print(
            "SHORT PRESS: "
          );

          Serial.print(held);

          Serial.println(" ms");
        }
      }

      drawStats(
        currentState == LOW
      );
    }
  }

  // ===== LONG PRESS =====

  if (
    currentState == LOW &&
    !longTriggered
  ) {

    if (
      millis() - pressStart
      > LONG_PRESS_MS
    ) {

      longTriggered = true;

      longCount++;

      Serial.println(
        "LONG PRESS"
      );

      drawStats(true);
    }
  }

  lastReading = reading;

  delay(5);
}