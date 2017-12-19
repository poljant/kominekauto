/*
 * KominekWeb.h
 *
 *   Copyright (c) 2016. All rights reserved.
 *
 *      Author: Jan Trzciński  <poljant@post.pl>
 */

#ifndef KOMINEKWEB_H_
#define KOMINEKWEB_H_
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPUpdateServer.h>
#include "Kominek.h"
#include "KominekWeb.h"

String HTMLHeader();
String HTMLFooter();
String HTMLPage();
String HTMLPage1();
String HTMLPage2();
void setservers(void);
bool is_authentified(void);
void handleLogin(void);
void handleRoot(void);


#endif /* KOMINEKWEB_H_ */
