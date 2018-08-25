#include <lorawan_client.h>

#include <SHT31.h>
#include <Arduino.h>
#include <Wire.h>

#define SOUND_SENSOR A0
#define LIGHT_SENSOR A1
#define PIR_MOTION_SENSOR 8
#define LED 4


SHT31 sht31 = SHT31();
LoRaWANClient lorawan;
bool ledStatus = LOW;

void setup()
{
    Serial.begin(9600);
    pinMode(PIR_MOTION_SENSOR, INPUT);
    pinMode(LED, OUTPUT);
    
    while(!Serial);
    Serial.println("begin...");  
    sht31.begin();
    Serial.println("LoRaWAN module init");

    // LoRaデバイスへ接続
    /*
    if (!lorawan.connect(true)) {
      Serial.println(" failed to connect. Halt...");
      for (;;) {};
    }
    */
    Serial.println("\nConnected.\n");
}

void loop() {
    // read sound
    long int sound = 0;
    for(int i=0; i<32; i++) {
        sound += analogRead(SOUND_SENSOR);
    }
    sound >>= 5;
    Serial.print("sound: ");
    Serial.println(sound);
    
    // read light
    int light = analogRead(LIGHT_SENSOR);
    Serial.print("light: ");
    Serial.println(light);
    
    //read pir
    bool pir = digitalRead(PIR_MOTION_SENSOR);
    Serial.print("pir: ");
    Serial.println(pir);
    
    // read temp/humidity
    float temp = sht31.getTemperature();
    float hum = sht31.getHumidity();
    Serial.print("Temp = "); 
    Serial.print(temp);
    Serial.println(" C"); //The unit for  Celsius because original arduino don't support speical symbols
    Serial.print("Hum = "); 
    Serial.print(hum);
    Serial.println("%"); 
    Serial.println();
    delay(500);
    
    digitalWrite(LED, ledStatus);
    ledStatus = ledStatus == HIGH ? LOW : HIGH;

//    lorawan.sendData("test");
}

