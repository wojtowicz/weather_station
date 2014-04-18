#include "etherShield.h"
#include "ETHER_28J60.h"
#include <SimpleTimer.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <LiquidCrystal.h>

// Data wire is plugged into pin 2 on the Arduino
#define ONE_WIRE_BUS 2

#define LCD_PIN_RS 3
#define LCD_PIN_E 4
#define LCD_PIN_DB4 5
#define LCD_PIN_DB5 6
#define LCD_PIN_DB6 7
#define LCD_PIN_DB7 8

// Setup a oneWire instance to communicate with ANY OneWire devices 
OneWire oneWire(ONE_WIRE_BUS);
// Set oneWire reference to Dallas Temperature sensor. 
DallasTemperature sensors(&oneWire);

LiquidCrystal lcd(LCD_PIN_RS, LCD_PIN_E, LCD_PIN_DB4, LCD_PIN_DB5, LCD_PIN_DB6, LCD_PIN_DB7);

float temp=-100,temp2=-100; // wartość początkowa temperatur
 
unsigned long lastTempRequest = 0;
int delayInMillis = 0;

// the timer object
SimpleTimer timer;

static uint8_t mac[6] = {0x54, 0x55, 0x58, 0x10, 0x00, 0x24}; // ustawiamy MAC adres
static uint8_t ip[4] = {192, 168, 0, 15};  // ustalamy adres IP pod jakim ma być dostępny program
static uint16_t port = 80;
ETHER_28J60 e;

// a function to be executed periodically
void repeatMe() 
{
  lcd.setCursor(0, 1);
  lcd.print("Uptime (s): ");
  lcd.print(millis() / 1000);
}

void setup()
{
  lcd.begin(16, 2);  //2 rows and 16 columns
  lcd.clear();
  lcd.setCursor(0, 1);
  lcd.print("Ladowanie");
  e.setup(mac, ip, port);
  
  sensors.begin();
  sensors.setWaitForConversion(false);
  sensors.setResolution(12);
  sensors.requestTemperatures();
  
  delayInMillis = 750 / (1 << (12 - 12));
  lastTempRequest = millis();
  
  timer.setInterval(1000, repeatMe);
}

void loop()
{
  char* params;
  int et,et2;
  
  if (millis() - lastTempRequest >= delayInMillis) // czas jaki upłynął jest wystarczający aby wartości były gotowe
  {
    temp = sensors.getTempCByIndex(0); // pobranie wartości pierwszego termometru
 
    // Poniżej wyświetlenie tych danych na wyświetlaczu //
    lcd.setCursor(0, 0);
    lcd.print("Temp: ");
    lcd.print(temp);
    lcd.print(" st.C");
 
    sensors.requestTemperatures(); // wysłanie kolejnego żądania temperatur
    delayInMillis = 750 / (1 << (12 - 12)); // wpisujemy czas po jakim żądanie temperatury będzie gotowe do odczytu
    lastTempRequest = millis(); // ustalamy aktualny czas jako czas ostatniego żądania temperatur
  }
  
  timer.run();
  
  if (params = e.serviceRequest()) // obsługa połączenia przychodzącego
  {
      et2 = temp*100; // wyświetlenie temperatury bez przecinków
      e.print(et2); // wyplucie wyniku do klienta
    e.respond(); // wysłanie odpowiedzi
  }
}
