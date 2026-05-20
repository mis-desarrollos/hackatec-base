#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>
#include <SPI.h>

#define TFT_MOSI 23
#define TFT_SCLK 18
#define TFT_CS   15
#define TFT_DC    2
#define TFT_RST   4
#define TFT_BL   32

Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_RST);

void setup() {

  // Encender backlight
  pinMode(TFT_BL, OUTPUT);
  digitalWrite(TFT_BL, HIGH);

  // Inicializar SPI
  SPI.begin(TFT_SCLK, -1, TFT_MOSI, TFT_CS);

  // Inicializar TFT
  tft.init(135, 240);

  // Rotación
  tft.setRotation(1);

  // Limpiar pantalla COMPLETA
  tft.fillScreen(ST77XX_BLACK);

  // Configuración texto
  tft.setTextWrap(false);
  tft.setTextColor(ST77XX_WHITE, ST77XX_BLACK);
  tft.setTextSize(2);

  // Cursor
  tft.setCursor(20, 40);

  // Texto
  tft.println("Hello");
  tft.println("HackaTec");
}

void loop() {

}