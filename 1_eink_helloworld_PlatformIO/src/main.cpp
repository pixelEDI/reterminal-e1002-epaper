/*
       _          _          _ _
 _ __ (_)_  _____| | ___  __| (_)
| '_ \| \ \/ / _ \ |/ _ \/ _` | |
| |_) | |>  <  __/ |  __/ (_| | |
| .__/|_/_/\_\___|_|\___|\__,_|_|
|_|

https://links.pixeledi.eu
reTerminal E1002 hello world| 11.2025


Wichtig - noch keine SD-Karte einlegen!

*/
#include <Fonts/FreeMonoBold9pt7b.h>
#include <GxEPD2_7C.h>
#include <GxEPD2_BW.h>

// Define ePaper SPI pins
#define EPD_SCK_PIN 7
#define EPD_MOSI_PIN 9
#define EPD_CS_PIN 10
#define EPD_DC_PIN 11
#define EPD_RES_PIN 12
#define EPD_BUSY_PIN 13

// Select the ePaper driver to use
// 0: reTerminal E1001 (7.5'' B&W)
// 1: reTerminal E1002 (7.3'' Color)
#define EPD_SELECT 1

#if (EPD_SELECT == 0)
#define GxEPD2_DISPLAY_CLASS GxEPD2_BW
#define GxEPD2_DRIVER_CLASS GxEPD2_750_GDEY075T7 // 7.5'' B&W driver
#elif (EPD_SELECT == 1)
#define GxEPD2_DISPLAY_CLASS GxEPD2_7C
#define GxEPD2_DRIVER_CLASS GxEPD2_730c_GDEP073E01 // 7.3'' Color driver
#endif

#define MAX_DISPLAY_BUFFER_SIZE 16000

#define MAX_HEIGHT(EPD)                                                        \
  (EPD::HEIGHT <= MAX_DISPLAY_BUFFER_SIZE / (EPD::WIDTH / 8)                   \
       ? EPD::HEIGHT                                                           \
       : MAX_DISPLAY_BUFFER_SIZE / (EPD::WIDTH / 8))

// Initialize display object
GxEPD2_DISPLAY_CLASS<GxEPD2_DRIVER_CLASS, MAX_HEIGHT(GxEPD2_DRIVER_CLASS)>
    display(GxEPD2_DRIVER_CLASS(/*CS=*/EPD_CS_PIN, /*DC=*/EPD_DC_PIN,
                                /*RST=*/EPD_RES_PIN, /*BUSY=*/EPD_BUSY_PIN));

SPIClass hspi(HSPI);

const char HelloWorld[] = "Hello pixeledi!";

void helloWorld() {
  display.setRotation(0);
  display.setFont(&FreeMonoBold9pt7b);
  // For the color screen, you can set different colors, e.g., GxEPD_BLACK,
  // GxEPD_RED
  display.setTextColor(GxEPD_GREEN);
  int16_t tbx, tby;
  uint16_t tbw, tbh;
  display.getTextBounds(HelloWorld, 0, 0, &tbx, &tby, &tbw, &tbh);

  // center the bounding box by transposition of the origin:
  uint16_t x = ((display.width() - tbw) / 2) - tbx;
  uint16_t y = ((display.height() - tbh) / 2) - tby;

  display.setFullWindow();
  display.firstPage();
  do {
    display.fillScreen(GxEPD_WHITE);
    display.setCursor(x, y);
    display.print(HelloWorld);
  } while (display.nextPage());
}

void setup() {
  pinMode(EPD_RES_PIN, OUTPUT);
  pinMode(EPD_DC_PIN, OUTPUT);
  pinMode(EPD_CS_PIN, OUTPUT);

  // Initialize SPI
  hspi.begin(EPD_SCK_PIN, -1, EPD_MOSI_PIN, -1);
  display.epd2.selectSPI(hspi, SPISettings(2000000, MSBFIRST, SPI_MODE0));

  // Initialize display
  display.init(0);
  helloWorld();
}

void loop() {};
