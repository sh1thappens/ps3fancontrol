/*
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "FanControl.h"

#define BAUD 9600
#define DEBUG 1

void(* resetFunc) (void) = 0;		// Softreset

#if DEBUG
LCDLibrary lcd(4, 8, false, BAUD);
#endif

int8_t pwmPin = 9;
volatile uint8_t pwmPower = 85; // Max 90
uint32_t pwmFreq = 25000;

int8_t tmpPin = 0;
volatile float tmpVoltage;
volatile float tmpCelsius;
uint8_t tmpMin = 35; // °C

int8_t statePin = 3;
volatile int8_t state = HIGH;

volatile boolean fanRun = false;

volatile int8_t relay1Pin = 5;	// Power-Line
volatile int8_t relay2Pin = 6;	// PWM-Line

void setup() {
	Serial.begin(BAUD);
	delay(100);

	InitTimersSafe();
	if (SetPinFrequencySafe(pwmPin, pwmFreq))
		Serial.println("sucess");

	attachInterrupt(1, stateChange, CHANGE);
	pinMode(statePin, INPUT);

	pinMode(relay1Pin, OUTPUT);
	digitalWrite(relay1Pin, HIGH);
	pinMode(relay2Pin, OUTPUT);
	digitalWrite(relay2Pin, HIGH);

	#if DEBUG
	lcd.clearScreen();
	#endif

	pwmPower = 30; // Min pwm
}

void loop() {
	tmpVoltage = ((analogRead(tmpPin) * 5.0) / 1024.0);
	tmpCelsius = (tmpVoltage - 0.5) * 100;

	if (state == LOW) {
		if (tmpCelsius > tmpMin) {
			if (pwmPower < 85) {
				if (pwmPower <= 30)
					pwmPower = 30;

				pwmPower = (pwmPower+1) % 86;
			}

			pwmWrite(pwmPin, pwmPower);

			if (!fanRun) {
				digitalWrite(relay1Pin, LOW);
				digitalWrite(relay2Pin, LOW);
				fanRun = true;
				Serial.println("Fan start");
			}
		}

		if (tmpCelsius <= tmpMin && fanRun) {
			ps3On();
			Serial.println("Fan stop");
		}
	}
	#if DEBUG
	infoSerial();
	infoLCD();
	#endif

	delay(500);
}

void stateChange() {
	if(digitalRead(statePin) == LOW) {
		ps3Off();
	} else {
		ps3On();
	}
}

void ps3Off() {
	state = LOW;
	Serial.println(" PS3 OFF ");
}

void ps3On() {
	state = HIGH;
	digitalWrite(relay1Pin, HIGH);
	digitalWrite(relay2Pin, HIGH);
	pwmWrite(pwmPin, 0);
	fanRun = false;
	pwmPower = 30;
	Serial.println(" PS3 ON ");
}

#if DEBUG
void infoLCD() {
	lcd.selectLineOne();
	lcd.print(tmpVoltage);
	lcd.print("V ");
	lcd.print(tmpCelsius);
	lcd.print("*C ");

	lcd.selectLineTwo();
	lcd.print("PS3:");
	lcd.clearLineAtPos(2, 6, 1);
	lcd.goToPos(2, 4);
	state ? lcd.print("ON") : lcd.print("OFF");

	lcd.goToPos(2, 8);
	lcd.print("FAN:");
	lcd.clearLineAtPos(2, 14, 1);
	lcd.goToPos(2, 12);
	fanRun ? lcd.print("ON") : lcd.print("OFF");

	lcd.selectLineThree();
	lcd.print("FAN%:");
	float pwmPer = pwmPower;
	lcd.print((pwmPer/255)*100);

	lcd.goToPos(3, 11);
	lcd.print("RAM:");
	lcd.print(freeRam());
}

void infoSerial() {
	Serial.print(tmpVoltage);
	Serial.print("V ");

	Serial.print(tmpCelsius);
	Serial.print("*C ");

	Serial.print("ps3state:");
	Serial.println(state);

	Serial.print("relay1:");
	Serial.print(digitalRead(relay1Pin));

	Serial.print("relay2:");
	Serial.println(digitalRead(relay2Pin));
}
#endif

int freeRam () {
  extern int __heap_start, *__brkval;
  int v;
  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval);
}
