//
// Created by getname on 29.03.2025.
//

#ifndef ASM_IO_H
#define ASM_IO_H

#include "../common.h"

unsigned char port_byte_in(unsigned short port);

void port_byte_out(unsigned short port, unsigned char data);

unsigned char port_word_in(unsigned short port);

void port_word_out(unsigned short port, unsigned short data);

static u8 keyboard_status() {
    return port_byte_in(0x64);
}

static u8 read_scancode() {
    return port_byte_in(0x60);
}


#endif //ASM_IO_H
