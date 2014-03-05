#include <SimpleTimer.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <LiquidCrystal.h>

// Data wire is plugged into pin 7 on the Arduino
#define ONE_WIRE_BUS 7

#define LCD_PIN_RS 12
#define LCD_PIN_E 11
#define LCD_PIN_DB4 5
#define LCD_PIN_DB5 4
#define LCD_PIN_DB6 3
#define LCD_PIN_DB7 2

// Setup a oneWire instance to communicate with ANY OneWire devices 
OneWire oneWire(ONE_WIRE_BUS);
// Set oneWire reference to Dallas Temperature sensor. 
DallasTemperature sensors(&oneWire);

LiquidCrystal lcd(LCD_PIN_RS, LCD_PIN_E, LCD_PIN_DB4, LCD_PIN_DB5, LCD_PIN_DB6, LCD_PIN_DB7);

// the timer object
SimpleTimer timer;

// a function to be executed periodically
void repeatMe() 
{
  lcd.setCursor(0, 1);
  lcd.print("Uptime (s): ");
  lcd.print(millis() / 1000);
}

void setup()
{
  sensors.begin();
  lcd.begin(16, 2); //2 rows and 16 columns
  lcd.clear();
  timer.setInterval(1000, repeatMe);
}

void loop()
{
  timer.run();
  sensors.requestTemperatures();
  lcd.setCursor(0, 0);
  lcd.print("Temp: ");
  lcd.print(sensors.getTempCByIndex(0));
  lcd.print(" st.C");
}
