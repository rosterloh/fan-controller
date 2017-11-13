#include <DHT.h>
#include <Adafruit_DotStar.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>
#include <SPI.h>
#include <Wire.h>
#include "wiring_private.h"

#define LED_PIN                     13
#define RELAY_PIN                   3
#define DHT_PIN                     4
#define DHT_TYPE                    DHT22
#define DHT_UPDATE_INTERVAL         60000
#define DHT_TIMING                  11
#define DATA_PIN                    7
#define CLOCK_PIN                   8
#define MISO_PIN                    30

#if (SSD1306_LCDHEIGHT != 64)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif

DHT dht(DHT_PIN, DHT_TYPE, DHT_TIMING);
Adafruit_DotStar pixel(1, DATA_PIN, CLOCK_PIN, DOTSTAR_BRG);
//SPIClass ledSPI(&sercom1, MISO_PIN, CLOCK_PIN, DATA_PIN, SPI_PAD_0_SCK_1, SERCOM_RX_PAD_2);
//Adafruit_DotStar pixel(1, &ledSPI, DOTSTAR_BRG); // Hardware SPI
Adafruit_SSD1306 display(-1);

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  pinMode(RELAY_PIN, OUTPUT);
  dht.begin();

  //ledSPI.begin();
  //pinPeripheral(DATA_PIN, PIO_SERCOM);
  //pinPeripheral(CLOCK_PIN, PIO_SERCOM);
  //pinPeripheral(MISO_PIN, PIO_SERCOM);
  pixel.begin();
  pixel.setBrightness(128); // 0 - 255
  pixel.setPixelColor(1, 255, 255, 255);
  pixel.show();

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C, false);
  display.setTextSize(2);
  display.setTextColor(WHITE);

  for (int i = 0; i < 5; i++) {
    digitalWrite(LED_PIN, HIGH);
    delay(300);
    digitalWrite(LED_PIN, LOW);
    delay(300);
  }
}

void loop() {
  static unsigned long last_update = 0;
  bool fan_on = false;

  if ((millis() - last_update > DHT_UPDATE_INTERVAL) || (last_update == 0)) {
    last_update = millis();

    double h = dht.readHumidity();
    double t = dht.readTemperature();

    if (isnan(h) || isnan(t)) {
      Serial.println("[DHT] Error reading sensor");
    } else {
      Serial.print("[DHT] Temperature: "); Serial.print(t); Serial.println(" ºC");
      Serial.print("[DHT] Humidity: "); Serial.print(h); Serial.println(" %");

      display.clearDisplay();
      display.setCursor(0, 0);
      display.print("T: "); display.print(t); display.println(" C");
      display.print("H: "); display.print(h); display.println(" %");
      display.display();

      if (t > 22) {
        if (!fan_on) {
          digitalWrite(RELAY_PIN, HIGH);
          fan_on = true;
          pixel.setPixelColor(1, 0xFF0000); // Red
          pixel.show();
        }
      } else {
        if (fan_on) {
          digitalWrite(RELAY_PIN, LOW);
          fan_on = false;
          pixel.setPixelColor(1,  0x0000FF); // Blue
          pixel.show();
        }
      }
    }
  }
}
