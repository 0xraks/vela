#include <WiFi.h>
#include <ESPmDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include "motorcontrol.h"
#include "BluetoothSerial.h"


#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

BluetoothSerial SerialBT;

char incomingchar;
const char* ssid     = "Raks";
const char* password = "samba@raks6369";


unsigned long intervals[] = {75, 40, 70, 40, 70, 40, 70, 40, 1000, 0};
const byte NUM_OF_INTERVALS = sizeof(intervals) / sizeof(unsigned long);
byte currentInterval = 0;
unsigned long previousMillis = 0;
unsigned long prevmillis = 0;


void setup() {

  // Setup needed for OTA
  Serial.begin(115200);
  SerialBT.begin("VelaBot"); //Bluetooth device name
  Serial.println("The device started, now you can pair it with bluetooth!");
  Serial.println("Booting");
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("Connection Failed! Rebooting...");
    delay(5000);
    ESP.restart();
    ArduinoOTA.setHostname("Vela_2WD_Bot");
  }
  // Port defaults to 3232                            // ArduinoOTA.setPort(3232);
  // Hostname defaults to esp3232-[MAC]               // ArduinoOTA.setHostname("myesp32");
  // No authentication by default                     // ArduinoOTA.setPassword("admin");
  // Password can be set with it's md5 value as well  // MD5(admin) = 21232f297a57a5a743894a0e4a801fc3
  // ArduinoOTA.setPasswordHash("21232f297a57a5a743894a0e4a801fc3");
  ArduinoOTA.onStart([]() {
    String type;
    if (ArduinoOTA.getCommand() == U_FLASH)
      type = "sketch";
    else // U_SPIFFS
      type = "filesystem";
    // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
    Serial.println("Start updating " + type);
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("\nEnd");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR)         Serial.println("Auth Failed");
    else if (error == OTA_BEGIN_ERROR)   Serial.println("Begin Failed");
    else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
    else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
    else if (error == OTA_END_ERROR)     Serial.println("End Failed");
  });
  ArduinoOTA.begin();
  Serial.println("Ready");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  //  My setup onwards
  pinMode(2, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(18, OUTPUT);
  pinMode(19, OUTPUT);
  pinMode(21, OUTPUT);
}

void ledblink()
{
  unsigned long currentMillis = millis();          //get current value of millisecond counter
  if (currentMillis - previousMillis >= intervals[currentInterval])  //if the time interval has elapsed
  {
    currentInterval = currentInterval + 1;     // select the next interval in the list
    if (currentInterval >= NUM_OF_INTERVALS)
    {
      currentInterval = 0;
    }
    digitalWrite(2, not digitalRead(2));    //change state of the LED
    previousMillis = currentMillis;                //save the time of change
  }
}
void loop() {
  ArduinoOTA.handle();
  yield();
  ledblink();
  if (SerialBT.available())
  { incomingchar = SerialBT.read();
    if (incomingchar == 'F')
    {
      forward();
    }
    if (incomingchar == 'B')
    {
      backward();
    }
    if (incomingchar == 'S')
    {
      stopbot();
    }
        if (incomingchar == 'R')
    {
      right();
    }
        if (incomingchar == 'L')
    {
      left();
    }
  }
}
