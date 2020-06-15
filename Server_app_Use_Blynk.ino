/* Code viết cho BlynkApp để đảm bảo hiệu suất đáp ứng của hệ thống
  Sử dụng các API có sẵn của Arduino và Blynk để tiến hành kết nối các
  cảm biến.
  # Author: Nhom8
*/

#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <SHT1x.h>  

#define dataPin  D2
#define clockPin D1

int ledpin = 14;       
int sensorPin = 12;       

float t;
float h;
SHT1x sht1x(dataPin, clockPin);
char auth[] = "49PZ8IAKF6LHolXL4fCH2bHZaVjxWiFj";
char ssid[] = "LAOMI3U";
char pass[] = "99999999";

void setup()
{
  Serial.begin(115200);
  pinMode(ledpin, OUTPUT);
  pinMode(sensorPin, INPUT);
  Blynk.begin(auth, ssid, pass);
  digitalWrite(ledpin, LOW);
  digitalWrite(2, LOW);
}
void loop()
{
  int sensor_value = digitalRead(sensorPin);                
  if(sensor_value == HIGH){                                         
     digitalWrite(ledpin, HIGH);                                
  }
               
  t= sht1x.readTemperatureC();
  h= sht1x.readHumidity();
  Blynk.virtualWrite(V3,t);
  Blynk.virtualWrite(V4,h);
  Blynk.run();
  
}
