
//relay setup
const int relayPin  = 2;  // the Arduino pin, which connects to the IN pin of relay
uint32_t lastRelayOnTime = 0;
bool relayOn = false;

//DS1307 setup
#include <Wire.h>
#include <RTClib.h>

RTC_DS1307 rtc;

const uint32_t timeDelay = 259200000UL; //3 day gap between waters


void setup() {
  pinMode(relayPin, OUTPUT);//relay set as OUTPUT
 digitalWrite(relayPin, HIGH);
 
 //DS1307 Startup
 Wire.begin();
 rtc.begin();

  if (!rtc.isrunning()) {
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }
}

void loop() {
  DateTime now = rtc.now();
  if ((millis() - lastRelayOnTime >= timeDelay) && !relayOn) {
    Serial.println("ON");
    digitalWrite(relayPin, LOW);
    lastRelayOnTime = millis();
    relayOn = true;
    delay(2500);
    Serial.println("OFF");
    digitalWrite(relayPin, HIGH);
    relayOn = false;

  }
}
