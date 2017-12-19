/*
 * Kominek.cpp
 *
 *   Copyright (c) 2016. All rights reserved.
 *
 *      Author: Jan Trzciński  <poljant@post.pl>
 */

#include <Arduino.h>
#include <DallasTemperature.h>
#include <OneWire.h>
#include "Kominek.h"
#include "Relay.h"
#include "RF2260.h"

// piny do, których podłączono przekaźniki
#define pin_relay1 D5
#define pin_relay2 D6
#define pin_relay3 D7
#define pin_relay4 D8
// pin podłączenia nadajnika RF
#define RFpin D0
// pin podłączenia diody LED wbudowanej
#define pin_LED0 BUILTIN_LED
char* c1On =  "000101010001010111000011"; //kod 1 załączenia
char* c1Off = "000101010001010111001100"; //kod 1 wyłączenia
//char* c2On =  "000101010001010111000011"; //kod 2 załączenia
//char* c2Off = "000101010001010111001100"; //kod 2 wyłączenia
unsigned int lH = 200; // czas trawania (us) stanu wysokiego na pinie RF
// pin z szyną OneWire
#define pinOW D1
// Konfigurowanie 1-Wire do komunikacji z utządzeniami 1-Wire
OneWire oneWire(pinOW);
// Ustaw 1-Wire dla czujników Dallas Temperature.
DallasTemperature sensors(&oneWire);

Kominek::Kominek() {
	led0.setPin(pin_LED0);
	led0.begin();
	relay1.setPin(pin_relay1);
	relay1.begin();
	relay2.setPin(pin_relay2);
	relay2.begin();
	relay3.setPin(pin_relay3);
	relay3.begin();
	relay4.setPin(pin_relay4);
	relay4.begin();
	rf1.setRF(RFpin,lH, c1On, c1Off);
//	rf2.setRF(RFpin,lH, c2On, c2Off);
}

Kominek::~Kominek() {
	// TODO Auto-generated destructor stub
}

void Kominek::setAuto(void){
	bmode = true;
}
void Kominek::setManual(void){
	bmode = false;
}
void Kominek::begin(void){
	Kominek();
}

void Kominek::working(void){
	readTemp();
	if(bmode){
		if (temp_in_box >= temp_on1){
			relay1.setOn();
			if (rf1.readRF()==0){ // nie nadawaj sygnału RF gdy rf1 załączony
			rf1.sendOn();
			};
	//		rf2.sendOn();
			relay3.setOn();
		}
		else if (temp_in_box < temp_off1) {
			relay1.setOff();
			if(rf1.readRF()==1){ // nie nadawaj sygnału RF gdy rf1 wyłącony
			rf1.sendOff();
			}
			relay3.setOff();
		}
		if (temp_in_box >= temp_on2){
			relay4.setOn();
		}
		else if (temp_in_box < temp_off2) {
			relay4.setOff();
		}
		if (temp_in_box >= temp_on3){
			relay2.setOn();
		}
		else if (temp_in_box < temp_off3) {
			relay2.setOff();
		}
	}
}
void Kominek::readTemp(void){
	 sensors.requestTemperatures();  // uruchom odczyt czujników temperatury
	 delay(1000);
	 temp_in_box = sensors.getTempCByIndex(0); //czytaj temperaturę w ºC
	 if (temp_in_box>temp_max) temp_max=temp_in_box;
}

float Kominek::readTempIn(void){
	return temp_in_box;
}
bool Kominek::readMode(void){
	return bmode;
}
