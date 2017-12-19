/*
 * KominekWeb.cpp
 *
 *   Copyright (c) 2016. All rights reserved.
 *
 *      Author: Jan Trzciński  <poljant@post.pl>
 */
//#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPUpdateServer.h>
#include "Kominek.h"
#include "KominekWeb.h"

extern String version;
extern double long timeM;
// login i hasło do sytemu
const char* www_login = "admin";
const char* www_pass = "esp8266";

//const int port = 80;                 // port serwera www
extern ESP8266WebServer server; //(port);
extern ESP8266HTTPUpdateServer httpUpdate;

String WebPage();

Kominek k;


//funkcja oblicza ile minut ma trwać dana procedura
extern int ilM; // ile minut trwa czas manual
unsigned long fminutes( int ile) {
	return (millis()+(1000*60*ile));
}

#ifdef DEBUG
  Serial.println("Server started");
#endif

String HTMLHeader() {           //  nagłówek strony
String  h = "<!DOCTYPE html>\n";
  h += "<html>";
  h += "<head>";
  h += "<title> Kominek</title>";
//  h += "<meta http-equiv=\"Refresh\" content=\"300\" />";  //odświerzaj stronę co 300 sek.
  h += "<meta charset=\"utf-8\">";
  h += "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">";
  h += "<link rel=\"stylesheet\" href=\"https://maxcdn.bootstrapcdn.com/bootstrap/3.3.4/css/bootstrap.min.css\" >";
  h += "</head>";
  h += "<body style=\"text-align: center;color: white; background: black;font-size: 1.5em;\">\n";

  return h;
}

String HTMLFooter() {             //  stopka strony www
String  f = "<p><a href = \"/\"><button class=\"btn btn-info\">Odświerz stronę</button></a></p>";
  f += "<p>Jan Trzciński &copy; 2016</p></td></tr>";
  f += "</body>\n";
  f += "</html>\n";
  return f;
}

String HTMLPage1() {      // pierwsza część strony www
 String t;
 unsigned long sec = millis() / 1000;
 unsigned long min = (sec / 60);
 unsigned long hour = (min / 60);
 unsigned long days = hour / 24;
  sec = sec % 60;
  min = min % 60;
  hour = hour % 24;

  k.readTemp();

  float temperature = k.readTempIn();// (sensors.getTempCByIndex(0));
 // float powersuply = ESP.getVcc()*0.913; //popraw błąd przeliczania napięcia

 t  = "<h1><p>Regulator nawiewu ciepła</p></h1>";
 t += "<p> Wersja oprogramowania ";
 t += (version);
 t += "</p>";
// t += "<p>Napięcie zasilania:";
// t += (powersuply/1000);
// t += "V</p>";
 t += "<p>Czas od uruchomienia dni: ";
 t += (days);
 t += " godz:" ;
 t += ((hour<10) ? "0" : "");
 t += (hour);
 t += ":";
 t += ((min<10) ? "0" : "");
 t += (min);
 t += ":";
 t += ((sec < 10) ? "0" : "");
 t += (sec);
 t += "</p>";
 t += "<p> Temperatura powietrza w komorze: ";
 t += (temperature);
 t += " ºC</p>";
 t += "<p> Temperatura maksymalna: ";
 t += (k.temp_max);
 t += " ºC</p>";

 return t;
}

String HTMLPage2() {            // główna strona www
  String p = "";
  //  display links depending on current state of relay 1,2,3 i 4
  if (!k.bmode) { //gdy ustawiony tryb ręczny
  p += ( (k.relay1.read()) ? "<p><a href = \"/relay1/0\"><button class=\"btn btn-danger\">Przekaźnik 1 ON</button></a></p>\n" \
  : "<p><a href = \"/relay1/1\"><button class=\"btn btn-success\">Przekaźnik 1 OFF</button></a></p>\n");
  p += ( (k.relay2.read()) ? "<p><a href = \"/relay2/0\"><button class=\"btn btn-danger\">Przekaźnik 2 ON</button></a></p>\n" \
  : "<p><a href = \"/relay2/1\"><button class=\"btn btn-success\">Przekaźnik 2 OFF</button></a></p>\n");
  p += ( (k.relay3.read()) ? "<p><a href = \"/relay3/0\"><button class=\"btn btn-danger\">Przekaźnik 3 ON</button></a></p>\n" \
  : "<p><a href = \"/relay3/1\"><button class=\"btn btn-success\">Przekaźnik 3 OFF</button></a></p>\n");
  p += ( (k.relay4.read()) ? "<p><a href = \"/relay4/0\"><button class=\"btn btn-danger\">Przekaźnik 4 ON</button></a></p>\n" \
  : "<p><a href = \"/relay4/1\"><button class=\"btn btn-success\">Przekaźnik 4 OFF</button></a></p>\n");
  // na jakie piny co podłączono
  // stopka strony
  p += ( "<p>Połączenia: 1-Wire na D1, Nadajnik RF na D0 </p>\n");
  p += ( "<p>Odbiornik RF CLARUS gniazdo 2 z serii 0047</p>\n");
  p += ( "<p> relay1 na D5, relay2 na D6, relay3 na D7, relay4 na D8 </p>\n");

  }
  else { // gdy tryb AUTO
  p += ( (k.relay1.read()) ? "<p><button class=\"btn btn-danger\">Przekaźnik 1 ON</button></p>\n" \
  : "<p><button class=\"btn btn-success\">Przekaźnik 1 OFF</button></p>\n");
  p += ( (k.relay2.read()) ? "<p><button class=\"btn btn-danger\">Przekaźnik 2 ON</button></p>\n" \
  : "<p><button class=\"btn btn-success\">Przekaźnik 2 OFF</button></p>\n");
  p += ( (k.relay3.read()) ? "<p><button class=\"btn btn-danger\">Przekaźnik 3 ON</button></p>\n" \
  : "<p><button class=\"btn btn-success\">Przekaźnik 3 OFF</button></p>\n");
  p += ( (k.relay4.read()) ? "<p><button class=\"btn btn-danger\">Przekaźnik 4 ON</button></p>\n" \
  : "<p><button class=\"btn btn-success\">Przekaźnik 4 OFF</button></p>\n");
  }
  // wyświetl jaki tryb wybrany
  p += ( (k.bmode) ? "<p><a href = \"/login\"><button class=\"btn btn-success\">Tryb AUTO</button></a></p>\n" \
  : "<p><a href = \"/auto\"> <button class=\"btn btn-danger\"> Tryb MANUAL</button></a></p>\n");
  return p;
}


 String WebPage() {       // połącz wszystkie części strony www
 return (HTMLHeader() + HTMLPage1() + HTMLPage2() + HTMLFooter());
 }

// funkcja ustawia wszystkie strony www
void setservers(void){

 httpUpdate.setup(&server,"/update", www_login, www_pass); // umożliwia aktualizację poprzez WiFi

 server.on("/", [](){      // odświerz stronę www
    server.send(200, "text/html", WebPage());
  });

 server.on("/login", [](){	//zaloguj się do strony
     if(!server.authenticate(www_login, www_pass))
       return server.requestAuthentication();
     k.setManual(); // = false;
     k.led0.setOff(); ////sygnalizacja załączenia trybu MANUAL (LED ESP8266)
     timeM = fminutes(ilM);  // odnów czas trybu MANUAL
     server.send(200, "text/html", WebPage());
   });

 server.on("/auto", [](){      // ustaw tryb AUTO
     k.bmode = true;
     k.led0.setOn();
     server.send(200, "text/html", WebPage());
  });

 server.on("/relay1/0", [] ()     //  wyłącz przekaźnik 1
  { if (!k.bmode) {
	  k.relay1.setOff();
	  k.rf1.sendOff();
	  timeM = fminutes(ilM);	// odnów czas trybu MANUAL
  }
    server.send(200, "text/html", WebPage());
  });

 server.on("/relay1/1", []()      // załącz przekaźnik 1
  { if (!k.bmode) {
    k.relay1.setOn();
    k.rf1.sendOn();
    timeM = fminutes(ilM); // odnów czas trybu MANUAL
  }
      server.send(200, "text/html", WebPage());
  });

 server.on("/relay2/0", [] ()     // wyłącz przekaźnik 2
  { if (!k.bmode) {
	  k.relay2.setOff();
	  timeM = fminutes(ilM);   // odnów czas trybu MANUAL
  }
   server.send(200, "text/html", WebPage());
  });

 server.on("/relay2/1", []()      // załącz przekaźnik 2
  { if (!k.bmode) {
	  k.relay2.setOn();
	  timeM = fminutes(ilM); // odnów czas trybu MANUAL
  }
    server.send(200, "text/html", WebPage());
  });

 server.on("/relay3/0", [] ()     // wyłącz przekaźnik 3
  { if (!k.bmode) {
	  k.relay3.setOff();
	  timeM = fminutes(ilM);   // odnów czas trybu MANUAL
  }
    server.send(200, "text/html", WebPage());
  });

 server.on("/relay3/1", []()      // załącz przekaźnik 3
  { if (!k.bmode) {
	  k.relay3.setOn();
	  timeM = fminutes(ilM); // odnów czas trybu MANUAL
  }
    server.send(200, "text/html", WebPage());
  });

 server.on("/relay4/0", [] ()     // wyłącz przekaźnik 4
  { if (!k.bmode) {
	  k.relay4.setOff();
	  timeM = fminutes(ilM);   // odnów czas trybu MANUAL
  }
    server.send(200, "text/html", WebPage());
  });

 server.on("/relay4/1", []()      // załącz przekaźnik 4
  { if (!k.bmode) {
	  k.relay4.setOn();
	  timeM = fminutes(ilM); // odnów czas trybu MANUAL
  }
    server.send(200, "text/html", WebPage());
  });

 server.on("/tempmax", []()      // resetuj temp_max
  { if (!k.bmode) {
	  k.temp_max = 20;
	  timeM = fminutes(ilM); // odnów czas trybu MANUAL
  }
    server.send(200, "text/html", WebPage());
  });

 server.begin();                // Start serwera www
 }




