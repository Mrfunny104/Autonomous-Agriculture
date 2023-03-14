//DHT11 Setup
#include <dht_nonblocking.h>
#define DHT_SENSOR_TYPE DHT_TYPE_11
static const int DHT_SENSOR_PIN = 7;
DHT_nonblocking dht_sensor(DHT_SENSOR_PIN, DHT_SENSOR_TYPE);

//Raspberry Pi is connected to Serial 0
#define serialPi Serial

//soil moisture setup
int val = 0; //value for storing moisture value 
int soilPin = A0;//Declare a variable for the soil moisture sensor 
int soilPower = 4;//Variable for Soil moisture Power

//relay setup
const int relayPin = 2;  // the Arduino pin, which connects to the IN pin of relay
bool relayOn = false;
unsigned long lastRelayOnTime = 0;

//DS1307 setup
#include <Wire.h>
#include <RTClib.h>

RTC_DS1307 rtc;

const int ledPin = 13;
const uint32_t timeDelay = 60000UL;//1800000UL; // 30 * 60 * 1000 milliseconds for half hour gap
const uint32_t relayDelay = 120000UL;//259200000UL; // 3 * 24 * 60 * 60 * 1000 milliseconds for 3 day gap

void setup() {
  serialPi.begin(9600);  //Arduino to serial monitor
  Serial.begin(9600);

  pinMode(soilPower, OUTPUT);
  digitalWrite(soilPower, LOW);//Set to LOW so no power is flowing through the sensor 
  pinMode(relayPin, OUTPUT);//relay set as OUTPUT
  digitalWrite(relayPin, HIGH);

  //DS1307 Startup
  Wire.begin();
  rtc.begin();

  if (!rtc.isrunning()) {
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }
}

//Temp & Humidity extra setup
static bool measure_environment(float *temperature, float *humidity)
{
  static unsigned long measurement_timestamp = millis();

  /* Measure once every four seconds. */
  if (millis() - measurement_timestamp > 3000ul)
  {
    if (dht_sensor.measure(temperature, humidity) == true)
    {
      measurement_timestamp = millis();
      return true;
    }
  }

  return false;
}

void loop() {
  DateTime now = rtc.now();
  
  if (now.second() == 0 && now.minute() % 30 == 0) {
    // Measure temperature and humidity. If the functions return true, then a measurement is available.
    float temperature;
    float humidity;
    int moisture = readSoil();

    if (measure_environment(&temperature, &humidity) == true) {
      // Send temperature and humidity data to Raspberry Pi
      serialPi.print("<");
      serialPi.print(temperature);
      serialPi.print(",");
      serialPi.print(humidity);
      serialPi.print(",");
      serialPi.print(moisture);
      serialPi.println(">");
    }
  }
  
  if (now.hour() % 24 == 0 && now.minute() == 0 && now.second() == 0 && (millis() - lastRelayOnTime >= relayDelay) && !relayOn) {
    digitalWrite(relayPin, LOW);
    relayOn = true;
    delay(5000);
    digitalWrite(relayPin, HIGH);
    relayOn = false;
    lastRelayOnTime = millis();
  }
}

// This is a function used to get the soil moisture content
int readSoil() {
digitalWrite(soilPower, HIGH);//turn D4 "On"
delay(10);//wait 10 milliseconds
val = analogRead(soilPin);//Read the SIG value form sensor
digitalWrite(soilPower, LOW);//turn D4 "Off"
return val;
}
