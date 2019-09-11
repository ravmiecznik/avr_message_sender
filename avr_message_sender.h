/*
 * avr_message_sender.h
 *
 *  Created on: 11 wrz 2019
 *      Author: rafal
 */

#ifndef AVR_MESSAGE_SENDER_AVR_MESSAGE_SENDER_H_
#define AVR_MESSAGE_SENDER_AVR_MESSAGE_SENDER_H_

#include <stdint.h>
#include "../atm128_usart/usart.h"


namespace MessageSender {


	class Tail{

	};

	class Message{
	private:
		void 	_putchar(char);

	public:
		Usart&	usart;
		Tail tail;
		Message(Usart& usart): usart(usart) {};
		void send(char*, uint16_t);

	};


}

#endif /* AVR_MESSAGE_SENDER_AVR_MESSAGE_SENDER_H_ */
