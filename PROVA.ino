//  Guilherme Maioli
//  Gabriel Oliveira Santos
//  Kit 2
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "DHT.h"
#include <TM74HC595Display.h>
#include <TimerOne.h>

#define DHTPIN A0
#define SOLOPIN A1
#define CHUVAPIN A2
#define LDRPIN A3
#define LCDPIN4 A4
#define LCDPIN5 A5
#define DHTTYPE DHT11

int PINO_SCLK = 7;
int PINO_RCLK = 6;
int PINO_DIO = 5;

int pino_buzzer = 4;
int pino_chuva = 3;
int pino_solo = 2;

int val_chuva_d;
int val_chuva_a;
int val_solo_d;
int val_solo_a;
int val_ldr;
int val_umd;
int luminosidade;

float humidity;
float temperature_celsius;
float temperature_fahrenheit;

int pino_led = 13;

unsigned char LED_0F[10];

DHT dht(DHTPIN, DHTTYPE);
TM74HC595Display disp(PINO_SCLK, PINO_RCLK, PINO_DIO);
LiquidCrystal_I2C lcd(0x3f,2,1,0,4,5,6,7,3, POSITIVE);

void setup() {
  pinMode(pino_chuva, INPUT);
  pinMode(pino_solo, INPUT);
  pinMode(pino_buzzer, OUTPUT);
  pinMode(pino_led, OUTPUT);

  pinMode(SOLOPIN, INPUT);
  pinMode(CHUVAPIN, INPUT);
  pinMode(LDRPIN, INPUT);

  digitalWrite(pino_buzzer, LOW);
  digitalWrite(pino_led, LOW);

  //Definicao dos digitos de 0 a 9
  LED_0F[0] = 0xC0; //0
  LED_0F[1] = 0xF9; //1
  LED_0F[2] = 0xA4; //2
  LED_0F[3] = 0xB0; //3
  LED_0F[4] = 0x99; //4
  LED_0F[5] = 0x92; //5
  LED_0F[6] = 0x82; //6
  LED_0F[7] = 0xF8; //7
  LED_0F[8] = 0x80; //8
  LED_0F[9] = 0x90; //9

  Timer1.initialize(1500); // set a timer of length 1500
  Timer1.attachInterrupt(timerIsr); // attach the service routine here

  disp.clear();
  
  lcd.begin (16,2);
  lcd.clear();
  
  lcd.setBacklight(HIGH);
  
  lcd.setCursor(0,0);
  lcd.print("SISTEMAS");
  lcd.setCursor(0,1);
  lcd.print("EMBARCADOS");
  delay(2000);

  lcd.clear();

  lcd.setCursor(0,0);
  lcd.print("Gabriel Oliveira");
  lcd.setCursor(0,1);
  lcd.print("dos Santos");
  delay(2000);

  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Guilherme");
  lcd.setCursor(0,1);
  lcd.print("Maioli");
  delay(2000);
}

void loop() {
  val_chuva_d = digitalRead(pino_chuva);
  val_chuva_a = analogRead(CHUVAPIN);
  val_solo_d = digitalRead(pino_solo);
  val_solo_a = analogRead(SOLOPIN);

  val_ldr = analogRead(LDRPIN);
  luminosidade = map(val_ldr,0,1023,0,100);

  humidity = dht.readHumidity();
  temperature_celsius = dht.readTemperature();
  temperature_fahrenheit = dht.readTemperature(true);

  verificaBuzzer();
  
  lcd.clear();
  disp.clear();
  
  lcd.setCursor(0,0);
  lcd.print("TEMPERATURA:");
  lcd.setCursor(0,1);
  lcd.print(temperature_celsius);
  disp.digit4(temperature_celsius);
  lcd.print(" C");
  
  delay(2000);

  lcd.clear();
  disp.clear();
  
  lcd.setCursor(0,0);
  lcd.print("TEMPERATURA:");
  lcd.setCursor(0,1);
  lcd.print(temperature_fahrenheit);
  disp.digit4(temperature_fahrenheit);
  lcd.print(" F");
  delay(2000);

  lcd.clear();
  disp.clear();
  
  lcd.setCursor(0,0);
  lcd.print("UMIDADE AR:");
  lcd.setCursor(0,1);
  lcd.print(humidity);
  disp.digit4(humidity);
  lcd.print(" %");
  delay(2000);

  lcd.clear();
  disp.clear();
  
  lcd.setCursor(0,0);
  lcd.print("LUMINOSIDADE:");
  lcd.setCursor(0,1);
  lcd.print(luminosidade);
  disp.digit4(luminosidade);
  delay(2000);

  lcd.clear();
  disp.clear();
  
  lcd.setCursor(0,0);
  lcd.print("UMIDADE SOLO:");
  lcd.setCursor(0,1);
  lcd.print(val_solo_a);
  disp.digit4(val_solo_a);
  delay(2000);

  lcd.clear();
  disp.clear();
  
  lcd.setCursor(0,0);
  lcd.print("INDICE CHUVA:");
  lcd.setCursor(0,1);
  lcd.print(val_chuva_a);
  disp.digit4(val_chuva_a);
  delay(2000);
}

void verificaBuzzer() {
  if(luminosidade > 650) {
    digitalWrite(pino_buzzer, HIGH);
    digitalWrite(pino_led, HIGH);
  } else if(humidity < 30) {
    digitalWrite(pino_buzzer, HIGH);
    digitalWrite(pino_led, HIGH);
  } else if(temperature_celsius < 20) {
    digitalWrite(pino_buzzer, HIGH);
    digitalWrite(pino_led, HIGH);
  } else if(val_solo_a < 650) {
    digitalWrite(pino_buzzer, HIGH);
    digitalWrite(pino_led, HIGH);
  } else if(val_chuva_a < 650) {
    digitalWrite(pino_buzzer, HIGH);
    digitalWrite(pino_led, HIGH);
  } else {
    digitalWrite(pino_buzzer, LOW);
    digitalWrite(pino_led, LOW);
  }
}

void timerIsr()
{
  disp.timerIsr();
}
