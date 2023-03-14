//DHT11 Setup
#include <dht_nonblocking.h>
#define DHT_SENSOR_TYPE DHT_TYPE_11

static const int DHT_SENSOR_PIN = 7;
DHT_nonblocking dht_sensor( DHT_SENSOR_PIN, DHT_SENSOR_TYPE );

//Raspberry Pi is connected to Serial 0
#define serialPi Serial

int val = 0; //value for storing moisture value 
int soilPin = A0;//Declare a variable for the soil moisture sensor 
int soilPower = 4;//Variable for Soil moisture Power

void setup() {
 serialPi.begin(9600);  //Arduino to serial monitor
 Serial.begin( 9600);

 pinMode(soilPower, OUTPUT);//Set D6 as an OUTPUT
 digitalWrite(soilPower, LOW);//Set to LOW so no power is flowing through the sensor 
}
 static bool measure_environment( float *temperature, float *humidity )
{
  static unsigned long measurement_timestamp = millis( );

  /* Measure once every four seconds. */
  if( millis( ) - measurement_timestamp > 3000ul )
  {
    if( dht_sensor.measure( temperature, humidity ) == true )
    {
      measurement_timestamp = millis( );
      return( true );
    }
  }

  return( false );
}

void loop() {
 float temperature;
 float humidity;
 int moisture = readSoil();

  /* Measure temperature and humidity.  If the functions returns
     true, then a measurement is available. */
  if( measure_environment( &temperature, &humidity ) == true )
  {
  //Send temperature and humidity and moisture data to Raspberry Pi
  serialPi.print("<");
  serialPi.print(temperature);
  serialPi.print(",");
  serialPi.print(humidity);
  serialPi.println(">");
  //Wait for 30 minutes 
  delay(1800000);
  }
}
