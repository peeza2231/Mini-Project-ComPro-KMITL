#include <TridentTD_LineNotify.h>   //Line Notify Library
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>     // Blynk for ESP8266WiFi Library
#include <SimpleTimer.h>
#include <DHT.h>                    // DHT Sensors Library
#include <LiquidCrystal_I2C.h>      // LCD I2C Library
#define BLYNK_PRINT Serial          // Comment this out to disable prints and save space
#define DHTPIN 12                   // Digital pin D6
#define DHTTYPE DHT11               // DHT 11 Sensor
//#define DHTTYPE DHT21             // DHT 21 (AM2301)
//#define DHTTYPE DHT22             // DHT 22  (AM2302), AM2321

/*----------------------------Setup Internet----------------------------*/
#define WIFI_SSID "PSAIR"                                         //Wifi Name
#define WIFI_PASSWORD "0819054859"                                //Password Wifi 
#define LINE_TOKEN "GL1puVNedgl7Au7glxVuGE7VO00ZmMt9gr1BrRkx0rD"  //Line Token Key
/*------------------------------------------------------------------------------*/

/*----------------------------Setup Blnyk Connection----------------------------*/
    char auth[] = "xXsFom1QgWE5cpj2PdJUV28-hcmOzmnd";                    //Enter the Auth code which was send by Blink. You should get Auth Token in the Blynk App.
                                            //Go to the Project Settings (nut icon).
    char ssid[] = "...";                    //Enter your WIFI Name
    char pass[] = "...";                    //Enter your WIFI Password
/*------------------------------------------------------------------------------*/

DHT dht(DHTPIN, DHTTYPE);               // setting pin to DHTPIN and DHTTYPE 11
//SimpleTimer timer;                      // that you define how often to send data to Blynk App.
LiquidCrystal_I2C lcd(0x27, 20, 4);     //Setting LCD
/*-----------------------------Icon to show on LCD------------------------------*/
byte tmp[8] = {      // Icon Temperature
    0b00100,
    0b01010,
    0b01010,
    0b01110,
    0b01110,
    0b11111,
    0b11111,
    0b01110,
};

byte hum[8] = {     // Icon Humidity
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

void showLCD(){
    int h = dht.readHumidity();
    int t = dht.readTemperature();
    lcd.begin();    // Turn on the blacklight and print a message.
    lcd.createChar(0, tmp);
    lcd.createChar(1, hum);
    lcd.backlight();
    dht.begin();

    lcd.setCursor(0,0);
    lcd.print("Weather my Home");
    lcd.setCursor(1,1);
    lcd.write(byte(0));
    lcd.print(" ");
    lcd.print(t);
    lcd.print((char)223);
    lcd.print("C ");
    lcd.setCursor(9,1);
    lcd.write(byte(1));
    lcd.print(" ");
    lcd.print(h);
    lcd.print("%H");
}

/*-----------------------Function Show in Blynk Application---------------*/
void sendSensor(){
  float h = dht.readHumidity();
  float t = dht.readTemperature(); // or dht.readTemperature(true) for Fahrenheit

  if (isnan(h) || isnan(t)) {
        Serial.println("Failed to read from DHT sensor!");
        return;
    }
  Blynk.virtualWrite(V5, h);  //V5 is for Humidity
  Blynk.virtualWrite(V6, t);  //V6 is for Temperature
}
/*----------------------------------------------------------------------*/

void setup() {
    dht.begin();            
    Blynk.begin(auth, ssid, pass);  //connect blynk app
    timer.setInterval(1000L, sendSensor);   // Setup a function to be called every second
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
    timer.setInterval(1000L, sendSensor);   // Setup a function to be called every second
  }

void loop() {
    Blynk.run();
    timer.run();
    float h = dht.readHumidity();
    float t = dht.readTemperature();
    if (isnan(h) || isnan(t)) {
        lcd.print("Failed to read from DHT sensor!");
        return;
    }
    if (t > 33 && h > 50) {
        String LineText;
        String string1 = "อุณหภูมิ เกินกำหนด ";
        String string2 = " °C";
        LineText = string1 + t + string2;
        //Serial.print("Line ");
        Serial.println(LineText);
        LINE.notify(LineText + "ความชื้น "+String(h)+" %");
    }
    delay(6000);
    if(t > 0 && h > 0){
        Serial.print("Humidity: ");
        Serial.print(h);
        Serial.print("\t");
        Serial.print("Temp: ");
        Serial.println(t);
        Serial.println("..............................."); 
        delay(1000);
  }
  showLCD(); // Call function LCD
}
