/*
 * avr_message_sender.h
 *
 *  Created on: 11 wrz 2019
 *      Author: rafal
 */

#ifndef AVR_MESSAGE_SENDER_AVR_TXMESSAGE_SENDER_H_
#define AVR_MESSAGE_SENDER_AVR_TXMESSAGE_SENDER_H_

#include <stdint.h>
#include "../atm128_usart/usart.h"

#define TAIL_START_MARK	'<'
#define TAIL_END_MARK	'>'

Usart& get_main_usart();

//typedef  int (*putchar_p)(char, FILE*);

int	_putchar_f(char, FILE*);
typedef int (*putchar_p)(char, FILE*);


namespace tx_id {
	enum id{
		ack_feedback,
		nak_feedback,
		dtx,
		txt,
		dbg,
		digidiag_frame,
		pin_change_pending,
		banks_info,
	};
}

struct Tail{
	uint8_t 	tail_start=	TAIL_START_MARK;	//'>'
	uint16_t	id=			0;
	uint16_t	context=	0;
	uint16_t 	msg_len=	0;
	uint16_t	msg_crc=	0;
	uint8_t		tail_end=	TAIL_END_MARK;	//'<'

	operator uint8_t* (){
		return &tail_start;
	}
};

class TxMessage{
	/*
	 * Class to create and send message.
	 * |MESSAGE BODY|<TAIL>crc_tail
	 *
	 */
private:
	void 			_putchar(char);
	static uint8_t 	tail_size;
	Tail 			tail;
	bool			tail_was_sent = false;
public:

	TxMessage(uint16_t msg_id, uint16_t context=0);
	TxMessage();
	~TxMessage();
	static 	Usart&	usart;
	void 	send(char*, uint16_t);
	void 	send(char c);
	void 	sends(char*);
	void	fetch_str(char*);
	void 	sends_p(const char *);
	void 	fetch_str_p(const char *);
	void 	fetch_byte(uint8_t c);
	void 	fetch_bytes(uint8_t* c, uint16_t size);
	void	send_tail();
	int 	printf_fetch (const char *format, ...);
	int 	printf (const char *format, ...);
	int 	printf_p_fetch (const char *format, ...);
	int 	printf_p(const char *format, ...);
};




#endif /* AVR_MESSAGE_SENDER_AVR_TXMESSAGE_SENDER_H_ */
