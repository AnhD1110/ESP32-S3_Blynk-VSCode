#include <Arduino.h>
#include "DHT.h"
#include <Wire.h>
#include <Adafruit_SSD1306.h>


#define BLYNK_TEMPLATE_ID "TMPL6jNe7-ufd"
#define BLYNK_TEMPLATE_NAME "IOT02"
#define BLYNK_AUTH_TOKEN "pfeXGLJ2xCFGJ6S-EMjR7-QdA407SKhp"

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

char ssid[] = "Wokwi-GUEST";
char pass[] = "";

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define SCREEN_ADDRESS 0x3C
Adafruit_SSD1306 display(SCREEN_WIDTH,SCREEN_HEIGHT, &Wire, OLED_RESET);
DHT dht(14,DHT22);

float h,t;

BlynkTimer timer;

void myTimerEvent()
{
  Blynk.virtualWrite(V0, h);
  Blynk.virtualWrite(V1, t);
}

BLYNK_WRITE(V2)
{
int pinValue = param.asInt();
  if(pinValue == 1) // RED
  {
    digitalWrite(4, HIGH);  // Tắt Green
    digitalWrite(5, HIGH); // Bật Red (GPIO 5)
    digitalWrite(6, LOW);  // Tắt Blue
  }
  else if(pinValue == 2) // YELLOW (Cần kết hợp Red + Green)
  {
    digitalWrite(4, HIGH); // Bật Green (GPIO 4)
    digitalWrite(5, LOW); // Bật Red (GPIO 5)
    digitalWrite(6, LOW);  // Tắt Blue
  }
  else if(pinValue == 3) // BLUE
  {
    digitalWrite(4, LOW);  // Tắt Green
    digitalWrite(5, HIGH);  // Tắt Red
    digitalWrite(6, HIGH); // Bật Blue (GPIO 6)
  }
  else
  {
    digitalWrite(4, LOW);
    digitalWrite(5, LOW);
    digitalWrite(6, LOW);
  }
}

void setup()
{
  dht.begin();
  pinMode(4,OUTPUT);
  pinMode(5,OUTPUT);
  pinMode(6,OUTPUT);
  if (!display.begin(SSD1306_SWITCHCAPVCC,SCREEN_ADDRESS))
    for(;;);
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0,0);
    display.println("Connecting to wifi...");
    display.display();


    Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
    timer.setInterval(100L,myTimerEvent);  
  }


void loop()
{
  h = dht.readHumidity();
  t = dht.readTemperature();
  display.clearDisplay();
  display.setCursor(0,0);
  display.println("Humidity.");
  display.setCursor(0,10);
  display.println(h);
  display.setCursor(0,20);
  display.println("Temperature");
  display.setCursor(0,30);
  display.println(t);
  display.display();
 
  Blynk.run();
  timer.run();
}