/* Code sử dụng Arduino và các API có sẵn cùng websocket và html để tạo giao 
 *  diện cho hệ thống. Vì chưa đáp ứng được nhu cầu realtime nên nhóm chỉ dừng 
 *  ở việc cập nhật các giá trị cảm biến và thao tác nút nhấn.
 *  #Author: Nhom8
*/

#include <ESP8266WiFi.h>
#include <String.h>
#include <Arduino.h>
#include <SHT1x.h>

// Pin SHT10
#define dataPin 4
#define clockPin 5
SHT1x sht1x(dataPin, clockPin);
const char *ssid = "LAOMI3U";
const char *password = "99999999";
// Tao server
WiFiServer server(80);
String lightState;
void setup()
{
  pinMode(D5,OUTPUT);
  digitalWrite(D5,1);
  Serial.begin(115200);
  Serial.println("Starting up");
  delay(10);

  // Ket noi toi mang WiFi
  Serial.println();
  Serial.println();
  Serial.print("Ket noi toi mang ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("Da ket noi WiFi");
  // Khoi dong server
  server.begin();
  Serial.println("Khoi dong Server");
  // In ra dia chi IP
  Serial.println(WiFi.localIP());
   digitalWrite(D5,0);
}

void loop()
{
  // Kiem tra khi co client ket noi
  WiFiClient client = server.available();
  if (!client)
  {
    return;
  }

  // Doi client gui ket noi
  Serial.println("Co mot client moi ket noi xem du lieu");
  while (!client.available())
  {
    delay(1);
  }
  // Doc do am/ nhiet do
  float t;
  float h;
  t= sht1x.readTemperatureC();
  h= sht1x.readHumidity();
  
  // Doc dong dau tien cua yeu cau gui len.
  String req = client.readStringUntil('\r');
  Serial.println(req);
  client.flush();
  String c = (String)client.read();
  // Chuan bi tao web de phan hoi
  String s = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n";
  req = client.readStringUntil('\r');
  client.flush();
  if (req.indexOf("/on") != -1) 
  {
    digitalWrite(D5,1);
  }
  else if (req.indexOf("/off") != -1)
  {
    digitalWrite(D5,0);
  }
  s += "<!DOCTYPE HTML><html>";
  s += "<head> <title>Arduino Home</title>";
  s+="<script type = \"text/JavaScript\">";
  s+="<!--";
  s+="function AutoRefresh( t ) {";
  s+=" setTimeout(\"location.reload(true);\", t);}";
  s+="//--></script>";
  s += "  <meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">";
  s += " <link rel=\"stylesheet\" href=\"https://use.fontawesome.com/releases/v5.7.2/css/all.css\" integrity=\"sha384-fnmOCqbTlWIlj8LyTjo7mOUStjsKC4pOpQbqyi7RrhN7udi9RwhKkMHpvLbHG9Sr\" crossorigin=\"anonymous\">";
  s += " <style>";
  s += " html {";
  s += "    font-family: Arial;";
  s += "    display: inline-block;";
  s += "    margin: 0px auto;";
  s += "     text-align: center;}";
  s += " h2 {";
  s += "  font-size: 3.0rem;";
  s += "  font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif;";
  s += "   animation-name: blur;";
  s += "  animation-duration:5s; }";
  s += " @keyframes blur {";
  s += "  0% {color: rgb(132, 0, 255);  }";
  s += "  25% {color: rgb(255, 0, 157); }";
  s += "50% {color: rgb(0, 255, 119); }";
  s += "100% {color: rgb(102, 0, 128); } }";
  s += " p { font-size: 3.0rem;}";
  s += ".units { font-size: 1.2rem;}";
  s += ".dht-labels {";
  s += "  font-size: 1.5rem;";
  s += "  vertical-align: middle;";
  s += "  padding-bottom: 15px;}";
  s += ".light-labels {";
  s += "font-size: 1.5rem;";
  s += " vertical-align: middle;";
  s += " padding-bottom: 15px; }";
  s += " </style></head>";
  s += "<body onload = \"JavaScript:AutoRefresh(1000);\"><h2>Welcome Home, Buddy!</h2>";
  s += "<p> <i class=\"fas fa-thermometer-half\" style=\"color:#059e8a;\"></i>";
  s += "<span class=\"dht-labels\">Temperature</span>";
  s += "<span id=\"temperature\">" + String(t) + " </span>";
  s += " <sup class=\"units\">&deg;C</sup> </p>";
  s += " <p> <i class=\"fas fa-tint\" style=\"color:#00add6;\"></i>";
  s += "<span class=\"dht-labels\">Humidity</span>";
  s += " <span id=\"humidity\">" + String(h) + "</span>";
  s += " <sup class=\"units\">%</sup> </p>";
  s += "<p> <i class=\"fas fa-lightbulb\" style=\"color:#119203;\" id=\"icon\"></i>";
  s += " <span class=\"light-labels\">Light</span>";
  client.print(s);
  s = "  <a href=\"/on\"><button id=\"btnOn\">ON</button></a>";
  client.print(s);
  s = "  <a href=\"/off\"><button id=\"btnOff\">OFF</button></a> </p></body></html>";
  client.print(s);
  delay(2000);
}
