#include <ESP8266WiFi.h>
#define WIFI_SSID ""
#define WIFI_PASSWORD ""
 
void setup() {
  Serial.begin(9600);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);   Serial.print("connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);}
  Serial.println();
  Serial.print("connected: ");
  Serial.println(WiFi.localIP());
  }
 
void loop() {
  int val = analogRead(A0);
  pinMode(D1, INPUT);
  if ( digitalRead(D1) == LOW ) {
    Line_Notify("On");   //เรียกใช้ การแจ้งเตือน
    delay(100);
  }
  if (val > 500){
    Line_Notify("Alert");
    delay(100);
  }
  delay(10);
}


const char* host = "maker.ifttt.com";
const char *privateKey = "bss5ro-Nm-cDFTuqhu9HuB";   //แก้ เป็น key ของตัวเอง
const char *event = "alert";  // แก้เป็น event ของตัวเอง


void Line_Notify(String msg) {
  WiFiClientSecure client;
  client.setInsecure();
  const int httpPort = 443;
  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    return;
  }
  
  // We now create a URI for the request
  String url = "/trigger/";
  url += event;
  url += "/with/key/";
  url += privateKey;
  String message = "value1=" + String(msg);
  client.print(String("POST ") + url + " HTTP/1.1\r\n" +
             "Host: " + host + "\r\n" +
             "Content-Type: application/x-www-form-urlencoded\r\n" +
             "Content-Length: " + message.length() + "\r\n\r\n" +
             message + "\r\n");

  
  while (client.connected())
  {
      if (client.available())
  {
      String line = client.readStringUntil('\r');
      Serial.print(line);
  } else {
      delay(50);
  };
  
  }
}
