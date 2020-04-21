#include <ESP8266WiFi.h>
#define WIFI_SSID ""
#define WIFI_PASSWORD ""
#define BLYNK_PRINT Serial    // Comment this out to disable prints and save space*/
#include <SPI.h>
#include <BlynkSimpleEsp8266.h>
#include <SimpleTimer.h>
#include <DHT.h>
#include <LiquidCrystal_I2C.h>
#define BLYNK_PRINT Serial    // Comment this out to disable prints and save space
#define DHTPIN 12            // Digital pin 6
#define DHTTYPE DHT11       // DHT 11

/*----------------------------Setup Internet----------------------------*/
// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "...";    //Enter the Auth code which was send by Blink
char ssid[] = "....";   //Enter your WIFI Name
char pass[] = "....";   //Enter your WIFI Password
/*----------------------------------------------------------------------*/

DHT dht(DHTPIN, DHTTYPE);   /* setting pin*/
SimpleTimer timer;
// that you define how often to send data to Blynk App.
LiquidCrystal_I2C lcd(0x27, 20, 4); /*Setting LCD*/
/*--------------------------Icon to show on LCD-------------------------*/
byte tmp[8] = {     /* Icon Temperature */
  0b00100,
  0b01010,
  0b01010,
  0b01110,
  0b01110,
  0b11111,
  0b11111,
  0b01110,
};

byte hum[8] = {     /* Icon Humidity */
  0b00100,
  0b00100,
  0b01010,
  0b01010,
  0b10001,
  0b10001,
  0b10001,
  0b01110,
};
/*----------------------------------------------------------------------*/

/*------------------------Function Show in Blynk Application------------------------*/
void sendSensor(){
  float h = dht.readHumidity();
  float t = dht.readTemperature(); // or dht.readTemperature(true) for Fahrenheit

  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  // You can send any value at any time.
  // Please don't send more that 10 values per second.
  Blynk.virtualWrite(V5, h);  //V5 is for Humidity
  Blynk.virtualWrite(V6, t);  //V6 is for Temperature
}
/*----------------------------------------------------------------------*/

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
