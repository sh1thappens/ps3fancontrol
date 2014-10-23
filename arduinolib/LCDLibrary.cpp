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

#include <LCDLibrary.h>

LCDLibrary::LCDLibrary(uint8_t receivePin,
		uint8_t transmitPin,
		bool inverse_logic /* = false */,
		long baudr):SoftwareSerial(receivePin, transmitPin) {

	this->receivePin = receivePin;
	this->transmitPin = transmitPin;

	pinMode(transmitPin, OUTPUT);
	begin(baudr);
}

void LCDLibrary::clearScreen() {
	write(0xFE);
	write(0x01);
	delay(5);
}

void LCDLibrary::selectLine(uint8_t line) {

	if(line > 0 && line <= 4){
		switch (line) {
			case 1:
				selectLineOne();

			break;
			case 2:
				selectLineTwo();

			break;
			case 3:
				selectLineThree();

			break;
			case 4:
				selectLineFour();

			break;

			default:
				selectLineOne();
		}
	}
}

void LCDLibrary::selectLineOne() {
	write(0xFE);
	write(128);
	delay(5);
}

void LCDLibrary::selectLineTwo() {
	write(0xFE);
	write(192);
	delay(5);
}

// TODO
void LCDLibrary::selectLineThree() {
	write(0xFE);
	write(148);
	delay(5);
}

// TODO
void LCDLibrary::selectLineFour() {
	write(0xFE);
	write(212);
	delay(5);
}

void LCDLibrary::clearLine(uint8_t num) {
	if(num > 0 && num <= 4){
		selectLine(num);
		print("                    ");
		selectLine(num);
	}
}

void LCDLibrary::goToPos(uint8_t line, uint8_t pos) {
	if (pos >= 0 && pos <= 19) {
		if(line >= 1 && line <= 4){
			uint8_t tmp = 0;

			switch (line) {
			case 1:
				tmp = 0 + pos;

			break;
			case 2:
				tmp = 64 + pos;

			break;
			case 3:
				tmp = 20 + pos;

			break;
			case 4:
				tmp = 84 + pos;

			break;

			default:
				tmp = 0 + pos;
			}

			tmp += 128;
			write(0xFE);
			write(tmp);
			delay(5);
		}
	}
}

void LCDLibrary::clearLineAtPos(uint8_t line, uint8_t pos, uint8_t length) {
	if (length<=19){
		goToPos(line, pos);

		String buf = "";
		for (int i = 0; i < length; ++i) {
			buf += " ";
		}
		print(buf);
	}
}

// Moves the Cursor one step to the right
void LCDLibrary::moveCursorRight() {
	write(0xFE);
	write(20);
	delay(5);
}

void LCDLibrary::moveCursorLeft() {
	write(0xFE);
	write(16);
	delay(5);
}

void LCDLibrary::scrollRight() {
	write(0xFE);
	write(20);
	delay(5);
}

void LCDLibrary::scrollLeft() {
	write(0xFE);
	write(24);
	delay(5);
}

void LCDLibrary::turnDisplayOff() {
	write(0xFE);
	write(8);
	delay(5);
}

void LCDLibrary::turnDisplayOn() {
	write(0xFE);
	write(12);
	delay(5);
}

void LCDLibrary::underLineCursorOff() {
	write(0xFE);
	write(12);
	delay(5);
}

void LCDLibrary::underLineCursorOn() {
	write(0xFE);
	write(14);
	delay(5);
}

void LCDLibrary::boxCursorOff() {
	write(0xFE);
	write(12);
	delay(5);
}

void LCDLibrary::boxCursorOn() {
	write(0xFE);
	write(13);
	delay(5);
}

void LCDLibrary::toggleSplashScreen() {
	write(0x7C);
	write(9);
	delay(5);
}

// 128 = OFF, 157 = Fully ON
void LCDLibrary::setBacklight(uint8_t value) {
	write(0x7C);
	write(value);
	delay(5);
}
