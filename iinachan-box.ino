#include <lorawan_client.h>

#include <SHT31.h>
#include <Arduino.h>
#include <Wire.h>
#include <math.h>

#define MAX_BYTES 11
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

  while (!Serial);
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
  long lsound = 0;
  for (int i = 0; i < 32; i++) {
    lsound += analogRead(SOUND_SENSOR);
  }
  int sound = (int) lsound >> 5;
  //    Serial.print("sound: ");
  //    Serial.println(sound);

  // read light
  int light = analogRead(LIGHT_SENSOR);
  //    Serial.print("light: ");
  //    Serial.println(light);

  //read pir
  bool pir = digitalRead(PIR_MOTION_SENSOR);
  //    Serial.print("pir: ");
  //    Serial.println(pir);
  //
  // read temp/humidity
  float temp = sht31.getTemperature();
  float hum = sht31.getHumidity();
  //    Serial.print("Temp = ");
  //    Serial.print(temp);
  //    Serial.println(" C"); //The unit for  Celsius because original arduino don't support speical symbols
  //    Serial.print("Hum = ");
  //    Serial.print(hum);
  //    Serial.println("%");
  //    Serial.println();
  delay(500);
  char buf[MAX_BYTES];
  packageDataForLorawan(buf, sound, light, pir, temp, hum);
  debugLorawanData(buf);

//  lorawan.sendData(buf);

  digitalWrite(LED, ledStatus);
  ledStatus = ledStatus == HIGH ? LOW : HIGH;
}

void packageDataForLorawan(char *buf, int sound, int light, bool pir, float temp, float hum) {
  // data  | num_byte
  // sound | 1
  // light | 1
  // pir   | 1
  // temp  | 1
  // humidity 1
  byte bsound = map(sound, 0, 1023, 0, 255);
  byte blight = map(light, 0, 1023, 0, 255);
  byte bpir = pir;
  byte btemp = ceil(temp);
  byte bhum = ceil(hum);

  debug("bsound", bsound);
  debug("blight", blight);
  debug("bpir", bpir);
  debug("btemp", btemp);
  debug("bhumidity", bhum);
  byte data[] = {bsound, blight, bpir, btemp, bhum};
  for (int i = 0; i < MAX_BYTES; ++i) {
    if (i < sizeof(data) / sizeof(byte)) {
      buf[i] = data[i];
    } else {
      buf[i] = 0;
    }
  }
}

void debug(char *key, long val) {
  Serial.print(key);
  Serial.print(": ");
  Serial.println(val);
}


void debugLorawanData(char *buf) {
  Serial.print("RES: ");
  for (int i = 0; i < MAX_BYTES; ++i) {
    Serial.print((byte) buf[i]);
  }
  Serial.println();
}

