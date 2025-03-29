//
// Created by getname on 29.03.2025.
//

#ifndef ASM_IO_H
#define ASM_IO_H

unsigned char port_byte_in(unsigned short port);

void port_byte_out(unsigned short port, unsigned char data);

unsigned char port_word_in(unsigned short port);

void port_word_out(unsigned short port, unsigned short data);


#endif //ASM_IO_H
