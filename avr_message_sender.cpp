/*
 * avr_message_sender.cpp
 *
 *  Created on: 11 wrz 2019
 *      Author: rafal
 */

#include "avr_message_sender.h"
#include "../atm128_usart/usart.h"

using namespace MessageSender;

void Message::_putchar(char c){
	usart.Putchar(c);
}

void Message::send(char* buffer, uint16_t amount){
	for(uint16_t i=0; i<amount; i++)
		usart.Putchar(*buffer++);
}
