/*
 * RF2260.h
 *
 *  Copyright (c) 2016. All rights reserved.
 *
 *      Author: Jan Trzciński  <poljant@post.pl>
 */

#ifndef RF2260_H_
#define RF2260_H_
#include <Arduino.h>

//namespace RF2260 {
//Klasa zawiera w sobie budowę i obsługę wyłacznika RF z Układem 2260 i podobnym
// czas impulsu wysokiego dla 0 jest 200us
// kod zawiera 24bity
// dla innych typów przełączników RF należy zmienić w/w parametry
class RF2260 {
	char* codOn; //kod załączenia
	char* codOff; //kod wyłączenia
	int lH; // czas trawania (w us) stanu wysokiego na pinie RF
	int RFpin; // pin, do którego podłaczony nadajnik RF
	int RF=0; // 1 - załaczony , 0 - wyłączony
	void send0(void); // wyślij 0
	void send1(void); // wyslij 1
	void sendSync(void); // wyslij sygnał synchronizacji

public:
	RF2260();
	virtual ~RF2260();

	void sendRF(char* scod); // wyslij cały kod on lub off
	void sendOn(void); // wyslij kod załaczenia
	void sendOff(void); // wyslij kod wyłaczenia
	int readRF(void); // czytaj stan wyłącznika RF
	void setCodOn(char*); //ustaw kod załączenia
	void setCodOff(char*); // ustaw kod wyłączenia
	void setRF(int, int, char*, char*); //ustaw parametry RF

};

//} /* namespace RF2260 */

#endif /* RF2260_H_ */
