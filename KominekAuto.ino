/*  KominekAuto.ino
 *
 *	Copyright (c) 2016. All rights reserved.
 *
 *      Author: Jan Trzciński  <poljant@post.pl>
 *
 * Układ załącza wentylatory obiegu ciepła z kominka
 * Załączenia są zależne od teperatury powietrza w komorze kominka
 * Ustawiane są 3 progi temperatury 30, 60 i 100 stopni Celsjusza
 * Załączenie poszczególnych przekaźników następuje po przekroczeniu
 * poszczególnych progów, a wyłączenie przy temperaturach niższych
 * od progów o wartość ustawioną w zmiennej histereza.
 * Progi:
 * 1. załącza przekazniki 1 i 3 oraz przekaźniki RF1 i RF2
 * 2. załącza przekaźnik 2
 * 3. załącza przekaźnik 4
 * Przekaźniki RF to wyłączniki sterowane pilotem na 433MHz. (np. CLARUS)
 * Wejście w trybu MANUAL po zalogowaniu się http://IP/login lub wciśnięciu tryb AUTO.
 * Tryb "MANUAL" umożliwia ręczne przełączanie przekaźników,
 * Po określomym czasie (timeM), gdy nic nie jest zmieniane -
 * automatycznie przechodzi w stan AUTO.
 * Zmienna ileM podaje ile minut może trwać czas Manual.
 * MANUAL sygnalizowany jest migotaniem LED-a wmontowanego w układ ESP8266 ESP-12E.
 * LED wbudowany świeci się ciągle, gdy brak połączenia z WiFi.
 * Gdy brak połączenia z WiFi układ przechodzi w stan AP_STA.
 * Można sie z nim połączyć poprzez uruchomiony AP.
 * Po połączeniu z AP dostęp przez http://192.168.4.1 login: admin pass: esp8266
 * tryb "AUTO" - automatyczne sterowanie przekaźnikami
 *                zależnie od temperatury
 * Aktualizacja programu poprzez WiFi
 * http://IP/update   login i hasło jak dla logowania
 */
//#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPUpdateServer.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include "RF2260.h"
#include "Relay.h"
#include "Kominek.h"
#include "KominekWeb.h"

String version = "1.0 z dnia 7-11-2016";
extern Kominek k;
double long timeM = 0; //czas rozpoczecia trybu MANUAL
int ilM=3; //ile minut może trwać czas trybu MANUAL
// SSID i pass do WiFi
//const char* ssid = "SSID";   // SSID sieci WiFi
//const char* password = "pass";  // password do WiFi

const char* ssid = "POLJANT_WLAN";   // SSID sieci LAN/WLAN
const char* password = "Marcjzt29newpass";  // password

const int port = 80;                 // port serwera www
ESP8266WebServer server(port);
ESP8266HTTPUpdateServer httpUpdate;

#define DEBUG
#define IP_STATIC
#ifdef IP_STATIC
IPAddress IPadr(10,110,2,120); //stały IP
IPAddress netmask(255,255,0,0);
IPAddress gateway(10,110,0,1);
#endif

//The setup function is called once at startup of the sketch
void setup()
{
// Add your initialization code here
#ifdef DEBUG
Serial.begin(115200);
#endif
#ifdef IP_STATIC
  WiFi.config(IPadr,gateway,netmask);
#endif
  WiFi.mode(WIFI_AP_STA);
  WiFi.begin(ssid, password);
  int i=0;
  while ((WiFi.status() != WL_CONNECTED) && ( i<=15)) {        //  czekaj na połączenie z wifi
    delay(500);
    i +=1;
#ifdef DEBUG
    Serial.print(".");
#endif
  }

 if (WiFi.status() != WL_CONNECTED){
  if (WiFi.getMode() != WIFI_AP_STA){WiFi.mode(WIFI_AP_STA);}  //tryb AP+STATION
  k.led0.setOff();}// włącz LED gdy nie jest połączenie z WiFi
 else {k.led0.setOff();}// wyłącz LED gdy jest połączenie z WiFi
  if (WiFi.getMode() != WIFI_STA){WiFi.mode(WIFI_STA);} //tryb STATION

#ifdef DEBUG
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println(WiFi.localIP());         // Wypisz IP serwera
  Serial.println(WiFi.macAddress());      // wypisz MAC adres
#endif

  setservers(); //uruchom serwery www
  k.begin(); //inicjalizuj parametry kominka
  k.readTemp(); // uruchom odczyt temperatury w kominku

}

// The loop function is called in an endless loop
void loop()
{
	 server.handleClient(); //czekaj na połączenie z klientem

	 if (!k.bmode) {	// gdy trb MANUAL
		if(timeM <= millis()){ k.setAuto();} // sprawdzaj czas trwania MANUAL, gdy minie przełącz na AUTO
	 }
	 k.working(); // uruchom automatykę kominka

	 // załącz LED gdy brak połączenia z WiFi
	 if (WiFi.status() != WL_CONNECTED){
		if (WiFi.getMode() != WIFI_AP_STA){WiFi.mode(WIFI_AP_STA);}  //ustaw tryb AP+STATION
	    k.led0.setOff();
	 }
	 else{ // wyłącz LED gdy jest połączenie z WiFi
	    k.led0.setOn(); // wyłącz LED
	    if (WiFi.getMode() != WIFI_STA){WiFi.mode(WIFI_STA);} //ustaw tryb STATION
	 }

	 if (!k.bmode) {
	   k.led0.setOff(); // włącz sygnalizację trybu MANUAL (LED ESP8266-12E)
	   delay(150);
	   k.led0.setOn(); // wyłącz sygnalizację trybu MANUAL (LED ESP8266-12E)
	 }
}
