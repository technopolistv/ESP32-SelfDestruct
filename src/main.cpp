/*
  https://www.technopolis.tv/PlatformIO-Self-destructing-Arduino-ESP32-Firmware/ 
*/ 

#include <Arduino.h>
#include <SPIFFS.h>
#include <Update.h>

#define NAME "Self Destruct"
#define VERSION "v1.0.0"
#define BUILD NAME " " VERSION
#define GREEN_LED 25
#define BUTTON 32

void perform_update(Stream &updateSource, size_t updateSize) {
  if (Update.begin(updateSize)) {
    size_t written = Update.writeStream(updateSource);

    if (written == updateSize) {
      Serial.println("Written : " + String(written) + " successfully");
    } else {
      Serial.println("Written only : " + String(written) + "/" + String(updateSize) + ". Retry?");
    }

    if (Update.end()) {
      Serial.println("OTA done!");

      if (Update.isFinished()) {
        Serial.println("Update successfully completed. Rebooting.");
        ESP.restart();
      } else {
        Serial.println("Update not finished? Something went wrong!");
      }

    } else {
      Serial.println("Error Occurred. Error #: " + String(Update.getError()));
    }

  } else {
    Serial.println("Not enough space to begin OTA");
  }
}

void init_update() {
  File file = SPIFFS.open("/stub.bin");
  if (!file) {
    Serial.println("Failed to open file for reading");
    return;
  }

  size_t filesize = file.size();

  if (filesize > 0) {
    Serial.println("Try to start update");
    perform_update(file, filesize);
  } else {
    Serial.println("Error, file is empty");
  }

  file.close();

  // remove the payload stub.bin in second stage
  // SPIFFS.remove("/stub.bin");
}

void setup() {
  pinMode(GREEN_LED, OUTPUT);
  pinMode(BUTTON, INPUT_PULLUP);

  // turn on the green LED
  digitalWrite(GREEN_LED, HIGH);

  Serial.begin(115200);
  Serial.println(BUILD);

  if (!SPIFFS.begin(true)) {
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }
}

void loop() {
  if (digitalRead(BUTTON) == LOW) {
    digitalWrite(GREEN_LED, LOW); // turn off the green LED
    delay(500);

    Serial.println("Initializing update...");
    init_update();
  }

  // do something else
}