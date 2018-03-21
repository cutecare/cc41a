/*
Part of http://cutecare.ru project
Author: evgeny.savitsky@gmail.com
*/
#include "cc41a.h"

/**
 * Constructor
 */
CC41A::CC41A(int rxPin, int txPin, int resetPin, long baud)
{
	bleRxPin = rxPin;
	bleTxPin = txPin;
	bleResetPin = resetPin;
	bleBaud = baud;
}

void CC41A::configure(const char * bleName)
{
	pinMode(bleRxPin, OUTPUT);
	
	SoftwareSerial bleSerial(bleTxPin, bleRxPin);
	bleSerial.begin(bleBaud);
	
	sendCommand(&bleSerial, "AT+RENEW");	// restore default settings
	delay(1000);

	char text[32] = "";
	sprintf(text, "AT+NAME%s", bleName);
	sendCommand(&bleSerial, text);

	sendCommand(&bleSerial, "AT+ROLE0");	// slave mode
	sendCommand(&bleSerial, "AT+TYPE0");	// unsecure, no pin required
	sendCommand(&bleSerial, "AT+POWE3");	// max RF power
	sendCommand(&bleSerial, "AT+ADVI3");	// advertising interval 300ms
	sendCommand(&bleSerial, "AT+PWRM0");	// auto-sleep
	sendCommand(&bleSerial, "AT+IBEA1");    // iBeacon mode
	sendCommand(&bleSerial, "AT+MARJ0x0000");    
	sendCommand(&bleSerial, "AT+MINO0x0000");
	sendCommand(&bleSerial, "AT+RESET");    // sleep
	
	pinMode(bleRxPin, INPUT);
}

void CC41A::setData(unsigned int minor, unsigned int major, bool autosleep)
{
	pinMode(bleRxPin, OUTPUT);
	wakeUpBLE();

	SoftwareSerial bleSerial(bleTxPin, bleRxPin);
	bleSerial.begin(bleBaud);

	char buff[32] = "";
	sprintf(buff, "AT+MARJ0x%04X", major);
	sendCommand(&bleSerial, buff);
	sprintf(buff, "AT+MINO0x%04X", minor);
	sendCommand(&bleSerial, buff);
	if (autosleep) {
		sendCommand(&bleSerial, "AT+RESET");
	}

	pinMode(bleRxPin, INPUT);
}

void CC41A::major(unsigned int value, bool autosleep)
{
	pinMode(bleRxPin, OUTPUT);
	wakeUpBLE();

	SoftwareSerial bleSerial(bleTxPin, bleRxPin);
	bleSerial.begin(bleBaud);

	char buff[32] = "";
	sprintf(buff, "AT+MARJ0x%04X", value);
	sendCommand(&bleSerial, buff);
	if (autosleep) {
		sendCommand(&bleSerial, "AT+RESET");
	}

	pinMode(bleRxPin, INPUT);
}

void CC41A::minor(unsigned int value, bool autosleep)
{
	pinMode(bleRxPin, OUTPUT);
	wakeUpBLE();

	SoftwareSerial bleSerial(bleTxPin, bleRxPin);
	bleSerial.begin(bleBaud);

	char buff[32] = "";
	sprintf(buff, "AT+MINO0x%04X", value);
	sendCommand(&bleSerial, buff);
	if (autosleep) {
		sendCommand(&bleSerial, "AT+RESET");
	}

	pinMode(bleRxPin, INPUT);
}

void CC41A::wakeUpBLE()
{
	pinMode(bleResetPin, OUTPUT);
	delay(100);
	digitalWrite(bleResetPin, HIGH);
	delay(100);
	digitalWrite(bleResetPin, LOW);
	delay(100);
	pinMode(bleResetPin, INPUT);
}

void CC41A::sendCommand(SoftwareSerial * bleSerial, const char * data) {
	delay(200);
	bleSerial->println(data);
}
