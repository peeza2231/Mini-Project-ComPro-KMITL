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
  dht.begin();
  lcd.begin();    // initialize the LCD

  lcd.createChar(0, tmp);
  lcd.createChar(1, hum);
  
  // Turn on the blacklight and print a message.
  lcd.backlight();
  //Blynk.begin(auth, ssid, pass);  //connect blynk app
  Serial.begin(115200);
  Serial.println(LINE.getVersion());
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);   Serial.print("connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("connected: ");
  Serial.println(WiFi.localIP());
  LINE.setToken(LINE_TOKEN); // กำหนด Line Token
}
 
void loop() {
  delay(1000);
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  if (isnan(h) || isnan(t)) {
    lcd.print("Failed to read from DHT sensor!");
    return;
   }
   delay(1500);
}
