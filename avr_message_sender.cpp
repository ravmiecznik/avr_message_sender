/*
 * avr_message_sender.cpp
 *
 *  Created on: 11 wrz 2019
 *      Author: rafal
 */


#include "avr_message_sender.h"
#include "../atm128_usart/usart.h"
#include "../common.h"
#include <avr/crc16.h>
#include <string.h>


//extern Usart bt_usart;

using namespace MessageSender;


//int PUTCHAR(char c, FILE* f){
//	get_main_usart().Putchar(c);
//	return 0;
//}



//***CONSTRUCTORS***
Message::Message(uint16_t msg_id, uint16_t context){
	tail.id = msg_id;
	tail.context = context;
	//_stdout.put	= &PUTCHAR;
}

Message::~Message(){
	send_tail();
}

//***METHODS***
void Message::_putchar(char c){
	usart.Putchar(c);
}

//int Message::_putchar_f(char c, FILE* f){
//	usart.Putchar(c);
//	return 1;
//}

void Message::send(char* buffer, uint16_t amount){
	for(uint16_t i=0; i<amount; i++){
		tail.msg_len++;
		tail.crc = _crc_xmodem_update(tail.crc, *buffer);
		usart.Putchar(*buffer);
		buffer++;				//pointer increment
	}
	send_tail();
}


void Message::fetch_str(char* buffer){
	while(*buffer){
		tail.msg_len++;
		tail.crc = _crc_xmodem_update(tail.crc, *buffer);
		usart.Putchar(*buffer);
		buffer++;				//pointer increment
	}
}

void Message::sends(char* buffer){
	fetch_str(buffer);
	send_tail();
}


void Message::fetch_str_p(const char* str_p){
	char c=pgm_read_byte(str_p++);
	while(c){
		tail.msg_len++;
		tail.crc = _crc_xmodem_update(tail.crc, c);
		usart.Putchar(c);
		c=pgm_read_byte(str_p++);
	}
}

void Message::sends_p(const char* buffer){
	fetch_str_p(buffer);
	send_tail();
}

void Message::fetch_byte(uint8_t c){
	usart.Putchar(c);
	tail.msg_len++;
	tail.crc = _crc_xmodem_update(tail.crc, c);
}

void Message::send_tail(){
	/*
	 * Method to close the message by sending its tail
	 */
	if(not tail_was_sent){
		tail_was_sent = true;
		uint8_t* p_tail = (uint8_t*)tail;
		for(uint8_t i=0; i<tail_size; i++)
			_putchar(*p_tail++);
	}
}

int Message::printf_fetch(const char* format, ...){
	/*
	 * Uses formated print to attach string to message
	 */
	if(strlen(format) < 70){
		char buff[50];
		va_list arg;
		int done;
		va_start (arg, format);
		done = vsprintf(buff, format, arg);
		va_end (arg);
		fetch_str(buff);
		return done;
	}
	return 0;
}

int Message::printf(const char* format, ...){
	/*
	 * Sends formatted message
	 */
	if(strlen(format) < 70){
		char buff[50];
		va_list arg;
		int done;
		va_start (arg, format);
		done = vsprintf(buff, format, arg);
		va_end (arg);
		sends(buff);
		return done;
	}
	return 0;
}


int Message::printf_p_fetch(const char* format, ...){
	/*
	 * Uses formated print to attach pgmspace string to message
	 */
	if(strlen_P(format) <70){
		char buff[50];
		va_list arg;
		int done;
		va_start (arg, format);
		done = vsprintf_P(buff, format, arg);
		va_end (arg);
		fetch_str(buff);
		return done;
	}
	return 0;
}

int Message::printf_p(const char* format, ...){
	/*
	 * Sends formatted string from pgmspace
	 */
	if(strlen_P(format) < 70){
		char buff[50];
		va_list arg;
		int done;
		va_start (arg, format);
		done = vsprintf_P(buff, format, arg);
		va_end (arg);
		sends(buff);
		return done;
		return 0;
	}
	return 0;
}
//***STATIC VARIABLES***
Usart & Message::usart = get_main_usart();
uint8_t Message::tail_size = sizeof(Tail);

