#include <DS3231.h>  // relogio
#include <Servo.h>	// servo
#include <LiquidCrystal.h> // lcd
#include <Keypad.h> // teclado
#include <Ultrasonic.h> // ultrassom
#include <EEPROM.h> // pode comentar pq não está usando EEPROM por enquanto

#define LED 10
#define pino_buzzer 10
Servo pino_servo;


void setup(){


	// linhas
	pinMode(2, OUTPUT);
	pinMode(3, OUTPUT);
	pinMode(4, OUTPUT);
	pinMode(5, OUTPUT);
	// buzzer
	pinMode(10, OUTPUT);
	// servo
	pino_servo.attach(11);

	//colunas
	pinMode(6, INPUT);
	pinMode(7, INPUT);
	pinMode(8, INPUT);
	pinMode(9, INPUT);

	Serial.begin(9600);
	Serial.println("PetFeeder");
	Serial.println();



}


void loop(){

	// lógica do botão

	pino_servo.write(0);
	delay(500);
	digitalWrite(LED, HIGH);
	tone(pino_buzzer, 1500);
	pino_servo.write(180);
	delay(500);

	noTone(pino_buzzer);
	digitalWrite(LED, LOW);
	delay(500);


	// Se funcionar coloca o LCD aqui


}