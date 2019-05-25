#include <ESP8266WiFi.h> 
#include <Wire.h>// Include main library
const char* ssid     = "YoDeV";    // Set your SSID
const char* password = "0867454630";  // Set your password
const char* thing = "Nattaphon";   // Set Thing name
const char* host = "dweet.io";  
// -----------------
#include <stdio.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#define ANALOG_PIN A0 
#define OLED_RESET -1
#define OLED_ADDR   0x3C
Adafruit_SSD1306 OLED(OLED_RESET);

// -----------------
int sensorValue;
// -----------------
const int ledPinGreen = D4;
const int ledPinRed = D5;
// -----------------
int analogPin = 0;
int Value = 0;
float volt;
float rs;
// -----------------
const int buzzer = D8;
// -----------------
String result;
int count =0;
const int sw1 = D6;
const int sw2 = D7;
int st_sw1 = 0;
int st_sw2 = 0;
int toggle = 0;

// -----------------

void setup()
{
  Serial.begin(9600);      // sets the serial port to 9600
  delay(10);
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(500);
    Serial.print(".");                // Print WiFi status
  }
  // -----------------
  pinMode(sw1, INPUT);
  pinMode(sw2, INPUT);
  // -----------------
  Wire.begin();
  OLED.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR);
  Serial.println("WiFi connected");   // Print connect status
  Serial.println("IP address: ");     
  Serial.println(WiFi.localIP());     // Print IP address
  pinMode(ledPinGreen, OUTPUT);
  pinMode(ledPinRed, OUTPUT);
}
void text (String result){
  OLED.clearDisplay();
  OLED.setTextColor(WHITE);
  OLED.setCursor(35, 20);
  OLED.setTextSize(2);
  OLED.print(result);
  OLED.display();
}
void loop()
{
  
  
  delay(300);
  ++count;                            // Count value
  WiFiClient client;
  const int httpPort = 80;            // Set HTTP port
  if (!client.connect(host, httpPort))// Test connection 
  {
    Serial.println("connection failed"); // Print connection fail message
    digitalWrite(ledPinGreen, 0);
    return;
  }
  else{
    Serial.println(toggle);
    digitalWrite(ledPinRed, 0);
    text("Ready");
    digitalWrite(ledPinGreen, 1);
    st_sw1 = digitalRead(sw1);
    st_sw2 = digitalRead(sw2);
    if(st_sw1 == 0) {
    toggle = not(toggle);
    }
    if(st_sw2 == 0){
      toggle = 0;
    }
    if(toggle == 1) {
      text("Detect Smoke");
      float smoke = MQ5();
      String url =  String("/dweet/for/") + thing + "?"; // Set message
      url += "count=";
      url += count;
      url += "&smoke=";
      url += smoke;
      client.print(String("GET ") + url + " HTTP/1.1\r\n" +
                   "Host: " + host + "\r\n" +
                   "Connection: keep-alive\r\n" +
                   "Cache-Control: max-age=0\r\n\r\n");
      Serial.println("URL : ");         
      Serial.println(url);
    }if(toggle == 0){
      text("Detect Fire");
      float fire = infared();
      String url =  String("/dweet/for/") + thing + "?"; // Set message
      url += "count=";
      url += count;
      url += "&fire=";
      url += fire;
      client.print(String("GET ") + url + " HTTP/1.1\r\n" +
                   "Host: " + host + "\r\n" +
                   "Connection: keep-alive\r\n" +
                   "Cache-Control: max-age=0\r\n\r\n");
      Serial.println("URL : ");         
      Serial.println(url); 
    }
  }                    
}
 float MQ5(){
      Value = analogRead(analogPin);

      volt = Value*(5.00/1024);

      rs=(100-(20.00*volt))/volt;

      Serial.print("volt : ");       

      Serial.print(volt);

      Serial.print("\t  RS=");       

      Serial.println(rs);
      if(rs<300.0){
        tone(buzzer, 1000);
        digitalWrite(ledPinRed, 1);
        text("Detection Smoke");
        return rs;
      }else{
        digitalWrite(ledPinRed, 0);
        noTone(buzzer);
        return rs; 
      }
 }
 float infared(){
    sensorValue = analogRead(0);       // read analog input pin 0
    Serial.println(sensorValue);  // prints the value read
    if(sensorValue<60){
      tone(buzzer, 1000);
      text("Detection Fire");
      digitalWrite(ledPinRed, 1);
      return sensorValue;
    }else{
      digitalWrite(ledPinRed, 0);
      noTone(buzzer); 
      return sensorValue; 
    }
 }
// ไฟ 60  rs 20
