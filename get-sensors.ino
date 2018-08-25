#include <SHT31.h>
#include <Arduino.h>
#include <Wire.h>

#define SOUND_SENSOR A0
#define LIGHT_SENSOR A1
#define PIR_MOTION_SENSOR 8
SHT31 sht31 = SHT31();
void setup()
{
    Serial.begin(9600);
    pinMode(PIR_MOTION_SENSOR, INPUT);
    while(!Serial);
    Serial.println("begin...");  
    sht31.begin();  
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

    
}

