#include <DHT.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_DotStar.h>
#include <SPI.h>
#include "wiring_private.h"

#define LED_PIN                     13
#define RELAY_PIN                   0
#define DHT_PIN                     1
#define DHT_TYPE                    DHT22
#define DHT_UPDATE_INTERVAL         60000
#define DHT_TIMING                  11
#define DATA_PIN                    7
#define CLOCK_PIN                   8
#define MISO_PIN                    30

DHT dht(DHT_PIN, DHT_TYPE, DHT_TIMING);
Adafruit_DotStar pixel(1, DATA_PIN, CLOCK_PIN, DOTSTAR_BRG);
//SPIClass ledSPI(&sercom1, MISO_PIN, CLOCK_PIN, DATA_PIN, SPI_PAD_0_SCK_1, SERCOM_RX_PAD_2);
//Adafruit_DotStar pixel(1, &ledSPI, DOTSTAR_BRG); // Hardware SPI

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
  pixel.show();

  for (int i = 0; i < 5; i++) {
    digitalWrite(LED_PIN, HIGH);
    delay(300);
    digitalWrite(LED_PIN, LOW);
    delay(300);
  }
}

void loop() {

  static unsigned long last_update = 0;
  if ((millis() - last_update > DHT_UPDATE_INTERVAL) || (last_update == 0)) {
    last_update = millis();

    double h = dht.readHumidity();
    double t = dht.readTemperature();

    if (isnan(h) || isnan(t)) {
      Serial.println("[DHT] Error reading sensor");
    } else {
      Serial.print("[DHT] Temperature: "); Serial.print(t); Serial.println(" ºC");
      Serial.print("[DHT] Humidity: "); Serial.print(h); Serial.println(" %");

      if (t > 22) {
        digitalWrite(RELAY_PIN, HIGH);
        pixel.setPixelColor(1, 0xFF0000); // Red
        pixel.show();
      } else {
        digitalWrite(RELAY_PIN, LOW);
        pixel.setPixelColor(1,  0x0000FF); // Blue
        pixel.show();
      }
    }
  }
}