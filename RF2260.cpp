/*
 * RF2260.cpp
 *
 * Copyright (c) 2016. All rights reserved.
 *
 *      Author: Jan Trzciński  <poljant@post.pl>
 *
 *      Klasa RF2260 umożliwia nadawanie kodów dla układu z serii 2260.i podobnych.
 *      Układ ten stosowany jest w tanich i popularnych wyłacznikach 220V.
 *      Częstotliwość nadawania to 433MHz  lub 896MHz.
 *      Do uruchomienia układu potrzebny jest nadajnik RF na 433MHz lub 866MHz
 */
#include <Arduino.h>
#include "RF2260.h"

#define pinRF D0	//pin, do którego podłączamy nadajnik RF

char* cOn  = "000101010001010111000011"; //kod załączenia
char* cOff = "000101010001010111001100"; //kod wyłączenia
int lh = 200; // czas trawania (w us) stanu wysokiego na pinie RF
int replay = 6; // ile razy powtórzyc nadawanie kodu

//namespace RF2260 {
 RF2260::RF2260()
 {
	  RF2260::setRF(pinRF, lh, cOn, cOff);
 }

 RF2260::~RF2260() {
	// TODO Auto-generated destructor stub

 }
 void RF2260::setCodOn(char* cod){
	codOn = cod;
 }
 void RF2260::setCodOff(char* cod){
	codOff = cod;
 }
// wyślij 0 na szynę RFpin
//   _
//  | |___
 void RF2260::send0(void) {
	digitalWrite(RFpin, HIGH);
	delayMicroseconds(lH);
	digitalWrite(RFpin, LOW);
	delayMicroseconds(lH * 3);
 }
// wyślij 1 na szynę RFpin
//   ___
//  |   |_
 void RF2260::send1(void) {
	digitalWrite(RFpin, HIGH);
	delayMicroseconds((lH * 3));
	digitalWrite(RFpin, LOW);
	delayMicroseconds(lH);
 }
// wyslij impuls synchronizacji
//   _
// _| |________________
 void RF2260::sendSync(void){
	RF2260::send0();
	delay(5 * lH/1000);
	delay(5 * lH/1000);
 }
 void RF2260::sendRF(char* cod){
	for (int j = 0; j < replay; j++){
		int i = 0;
		while (cod[i] !='\0'){
			switch(cod[i]) {
			case '0':
				RF2260::send0();
				break;
			case '1':
				RF2260::send1();
				break;
			}
			i++;
		}
		RF2260::sendSync();
	}
 }
 void RF2260::sendOff(void){

	 RF2260::sendRF(codOff);
	 RF=0;
 }
 void RF2260::sendOn(void){

	 RF2260::sendRF(codOn);
	 RF=1;
 }
//odczytaj czy RF załaczony
 int RF2260::readRF(){
	return RF;
 }
 void RF2260::setRF(int pin, int l, char* On, char* Off){
	RFpin = pin; // ustaw pin podłączenia RF
	pinMode(RFpin, OUTPUT);
	lH = l; // czas trwania (us) stanu wysokiego na pinie RF
	RF2260::setCodOn(On);
	RF2260::setCodOff(Off);
	RF = 0; // 1 - załaczony , 0 - wyłączony

 }

//} /* namespace RF2260 */
