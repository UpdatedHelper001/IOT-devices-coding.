#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include <SPI.h>

// ST7735 pins
#define TFT_CS    5
#define TFT_RST   4
#define TFT_DC    2

// Create display object
Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);

void setup() {
  // Start display
  tft.initR(INITR_BLACKTAB);

  // Screen rotation (0 to 3)
  tft.setRotation(1);

  // Fill screen black
  tft.fillScreen(ST77XX_BLACK);

  // Text settings
  tft.setTextColor(ST77XX_WHITE);
  tft.setTextSize(2);

  // Cursor position
  tft.setCursor(20, 50);

  // Print text
  tft.println("Hellow");
}

void loop() {
  // Nothing here
}
