#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>
#include <SPI.h>

// ===== TFT PINOUT =====
#define TFT_MOSI 23
#define TFT_SCLK 18
#define TFT_CS   15
#define TFT_DC    2
#define TFT_RST   4
#define TFT_BL   32

Adafruit_ST7789 tft =
  Adafruit_ST7789(TFT_CS, TFT_DC, TFT_RST);

int screen = 0;

// gris compatible
#define DARK_GREY 0x39E7

void drawFooter() {

  tft.fillRect(0, 120, 240, 20, DARK_GREY);

  tft.setTextColor(ST77XX_WHITE, DARK_GREY);
  tft.setTextSize(1);
  tft.setCursor(5, 125);

  tft.print("SCREEN ");
  tft.print(screen + 1);
  tft.print("/3  BOOT=NEXT");
}

void screen1() {

  tft.fillScreen(ST77XX_BLACK);

  tft.setTextColor(ST77XX_CYAN);
  tft.setTextSize(2);
  tft.setCursor(10, 20);
  tft.println("HELLO ESP32");

  tft.setTextSize(1);
  tft.setTextColor(ST77XX_WHITE);
  tft.setCursor(10, 60);
  tft.println("Display Widgets OK");

  drawFooter();
}

void screen2() {

  tft.fillScreen(ST77XX_BLACK);

  tft.setTextColor(ST77XX_YELLOW);
  tft.setTextSize(2);
  tft.setCursor(10, 20);
  tft.println("SHAPES");

  tft.drawRect(10, 50, 50, 30, ST77XX_RED);
  tft.fillRect(70, 50, 50, 30, ST77XX_GREEN);
  tft.drawCircle(150, 65, 15, ST77XX_BLUE);

  drawFooter();
}

void screen3() {

  tft.fillScreen(ST77XX_BLACK);

  tft.setTextColor(ST77XX_GREEN);
  tft.setTextSize(2);
  tft.setCursor(10, 20);
  tft.println("PROGRESS");

  int value = millis() % 100;

  tft.drawRect(10, 60, 200, 20, ST77XX_WHITE);
  tft.fillRect(12, 62, value * 2, 16, ST77XX_GREEN);

  tft.setCursor(10, 90);
  tft.setTextSize(1);
  tft.setTextColor(ST77XX_WHITE);
  tft.print(value);
  tft.println("%");

  drawFooter();
}

void render() {

  switch (screen) {

    case 0: screen1(); break;
    case 1: screen2(); break;
    case 2: screen3(); break;
  }
}

void setup() {

  Serial.begin(115200);

  pinMode(TFT_BL, OUTPUT);
  digitalWrite(TFT_BL, HIGH);

  SPI.begin(TFT_SCLK, -1, TFT_MOSI, TFT_CS);

  tft.init(135, 240);
  tft.setRotation(1);

  render();
}

void loop() {

  static bool lastBtn = HIGH;
  bool btn = digitalRead(0); // BOOT

  if (lastBtn == HIGH && btn == LOW) {

    delay(50);

    screen = (screen + 1) % 3;

    render();
  }

  lastBtn = btn;

  delay(10);
}