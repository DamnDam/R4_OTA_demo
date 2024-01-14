#include <Arduino.h>
#include <WiFiS3.h>
// #define NO_OTA_PORT
#include <ArduinoOTA.h> // Import Wifi BEFORE ArduinoOTA
#include "secret.h"
// secret.h should contain:
// const char * WIFI_SSID = "ssid";
// const char * WIFI_PASS = "password";

const char *OTA_PASS = "password";
bool OTA_mode = false;

void OTA_loop();

void setup()
{
  // status of WiFi connection

  // Initialize serial:
  Serial.begin(9600);

  // initialize the LED pin as an output:
  pinMode(LED_BUILTIN, OUTPUT);

  // initialize the pushbutton pin as an input:
  pinMode(PIN_D8, INPUT_PULLUP);

  // check for the presence of the WiFi module:
  if (WiFi.status() == WL_NO_MODULE)
  {
    Serial.println("WiFi module not present");
    // don't continue:
    while (true)
      ;
  }

  // attempt to connect to Wifi network:

  for (int status = WL_IDLE_STATUS; status != WL_CONNECTED; status = WiFi.begin(WIFI_SSID, WIFI_PASS))
  {
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(WIFI_SSID);
  }

  // Light up the LED if we're connected:
  digitalWrite(LED_BUILTIN, HIGH);

  // you're connected now, so print out the status:
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  // print the received signal strength:
  Serial.print("signal strength (RSSI):");
  Serial.print(WiFi.RSSI());
  Serial.println(" dBm");

  // add normal setup code below ...
}

void loop()
{
  // check for WiFi OTA updates
  OTA_loop();

  // add your normal loop code below ...

}

void OTA_loop()
{
  static bool button_pressed = false;

  // check for WiFi OTA updates
  if (OTA_mode)
  {
    // blink the LED to show OTA mode
    digitalWrite(LED_BUILTIN, millis() % 500 < 150);
    ArduinoOTA.handle();
  }

  // check for button press
  if (digitalRead(PIN_D8) == LOW)
  {
    digitalWrite(LED_BUILTIN, HIGH);
    if (button_pressed)
      return;
    button_pressed = true;
    if (OTA_mode)
    {
      ArduinoOTA.end();
    }
    else
    {
      // start the WiFi OTA library with internal (flash) based storage
      ArduinoOTA.begin(WiFi.localIP(), "Arduino", OTA_PASS, InternalStorage);
    }
    OTA_mode = !OTA_mode;
  }
  else
  {
    button_pressed = false;
  }
}
