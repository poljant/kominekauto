/*
 * Kominek.h
 *
 *  Copyright (c) 2016. All rights reserved.
 *
 *      Author: Jan Trzciński  <poljant@post.pl>
 */

#ifndef KOMINEK_H_
#define KOMINEK_H_
#include "Relay.h"
#include "RF2260.h"
 /* Układ załącza wentylatory obiegu ciepła z kominka
 * Załączenia są zależne od teperatury powietrza w komorze kominka
 * Ustawiane są 3 progi temperatury 30, 60 i 100 stopni Celsjusza
 * Załączenie poszczególnych przekaźników następuje po przekroczeniu
 * poszczególnych progów, a wyłączenie przy temperaturach niższych
 * od progów o wartość ustawioną w zmiennej hyster.
 * Progi:
 * 1. załącza przekaźniki 1 i 3 oraz przekaźniki RF1 i RF2
 * 2. załącza przekaźnik 2
 * 3. załącza przekaźnik 4
 * Przekaźniki RF to wyłączniki sterowane pilotem na 433MHz. (np. CLARUS)
 * Tryb "MANUAL" umożliwia ręczne sterowanie przekaźników,
 * Tryb "AUTO" - automatyczne sterowanie przekaźnikami
 *                zależnie od temperatury
 */

// klasa kominka
class Kominek {
public:

	float temp_in_box = 20.0; //temperatura mierzona w komorze kominka
	float temp_on1 = 30.0; //próg pierwszy załaczenia nawiewu
	float temp_on2 = 60.0; //próg drugi załączenia nawiewu
	float temp_on3 = 100.0; // próg trzeci załączenia nawiewu
	float hyster = 3; // różnica pomiedzy temperatura załączenia, a wyłączenia
	float temp_off1 = temp_on1 - hyster; //próg pierwszy wyłączenia nawiewu
	float temp_off2 = temp_on2 - hyster; //próg drugi wyłączenia nawiewu
	float temp_off3 = temp_on3 - hyster; //próg trzeci wyłączenia nawiewu
	float temp_max = 20.0; //temperatura maksymalna osiągnięta podczas pomiarów

	bool bmode = true; // true tryb AUTO, false tryb MANUAL

	RF2260 rf1;   // wyłącznik RF1
	RF2260 rf2;   // wyłącznik RF2
	Relay relay1; // przekaźnik 1
	Relay relay2; // przekaźnik 2
	Relay relay3; // przekaźnik 3
	Relay relay4; // przekaźnik 4
	Relay led0;   // led wbudowany

	Kominek();
	virtual ~Kominek();

	void setAuto(void);
	void setManual(void);
	void begin(void);
	void setTemp(float);
	void readTemp(void);
	float readTempIn(void);
	void working(void);
	bool readMode(void);

};

#endif /* KOMINEK_H_ */
