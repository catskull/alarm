#include "wifi_credentials.h"

#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <HTTPClient.h>

// Replace with your HTTPS URL
const char* serverName = "https://alarm.degraw.workers.dev";

#define LED_BUILTIN 13;

// Built-in LED pin
const int ledPin = LED_BUILTIN;

void setup() {
  Serial.begin(115200);

  // Set the built-in LED as an output
  pinMode(ledPin, OUTPUT);

  // Ensure the LED is off initially
  digitalWrite(ledPin, LOW);

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    WiFiClientSecure client;
    client.setInsecure(); // Skips certificate verification

    HTTPClient https;

    // Start the HTTPS connection
    https.begin(client, serverName);

    // Send the request
    int httpResponseCode = https.GET();

    // Handle the response
    if (httpResponseCode > 0) {
      // Serial.print("HTTP Response code: ");
      // Serial.println(httpResponseCode);

      String response = https.getString();
      Serial.println("Response:");
      Serial.println(response);
      if (response == "1") {
        digitalWrite(ledPin, HIGH); // Turn LED on
        Serial.println("ACTIVE SHOOTER");
      } else {
        digitalWrite(ledPin, LOW); // Turn LED off
      }
    } else {
      Serial.print("Error code: ");
      Serial.println(httpResponseCode);
      digitalWrite(ledPin, LOW); // Turn LED off in case of error
    }

    // End the connection
    https.end();
  } else {
    Serial.println("WiFi disconnected. Attempting to reconnect...");
    WiFi.begin(ssid, password);
    digitalWrite(ledPin, LOW); // Turn LED off in case of error
  }

  // Wait before sending the next request
  delay(1000);
}