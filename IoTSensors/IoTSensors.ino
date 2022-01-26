#include <WiFiNINA.h>
#include <ThingSpeak.h>
#include <SPI.h>
#include <dht_nonblocking.h>
#include <LiquidCrystal.h>

char ssid[] = "NOWTVGSZRG2.4";
char password[] = "NathanAndrea";
int status = WL_IDLE_STATUS;
const unsigned long channel_id = 1634429;
const char write_api_key[] = "671XNOTKTV364VIX";


LiquidCrystal lcd(7, 8, 9, 10, 11, 12);


#define DHT_SENSOR_TYPE DHT_TYPE_11


static const int DHT_SENSOR_PIN = 2;
DHT_nonblocking dht_sensor( DHT_SENSOR_PIN, DHT_SENSOR_TYPE );
WiFiClient client;



void setup( )
{
  lcd.begin(16, 2);
  Serial.begin( 9600);
  delay(100);
  Serial.print("Connecting to ");
  Serial.print(ssid);
  WiFi.begin(ssid,password);
  Serial.println("Connected");

  ThingSpeak.begin(client);
}




static bool measure_environment( float *temperature, float *humidity )
{
  static unsigned long measurement_timestamp = millis( );

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




void loop( )
{
  lcd.setCursor(0, 1);
  float temperature;
  float humidity;
  int light = analogRead(A0);
  


  if( measure_environment( &temperature, &humidity ) == true )
  {
    Serial.print( "T = " );
    Serial.print( temperature, 1 );
    
    lcd.print("T = " );
    lcd.print(temperature, 1 );
    Serial.print( " deg. C, H = " );
    Serial.print( humidity, 1 );
    lcd.print("H = " );
    lcd.print(humidity, 1 );
    ThingSpeak.setField(1, String(temperature, 1 )); 
    ThingSpeak.setField(2, String(humidity, 1)); 
    ThingSpeak.setField(3, String(light));
    ThingSpeak.writeFields(channel_id, write_api_key);
    
    Serial.println( "%" );
  }
}
