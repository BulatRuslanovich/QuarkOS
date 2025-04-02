//
// Created by getname on 29.03.2025.
//

#ifndef SCREEN_H
#define SCREEN_H

#include "../common.h"

#define VIDEO_ADDRESS 0xb8000
#define MAX_ROWS 25
#define MAX_COLS 80

#define GREEN_ON_BLACK 0x02

#define REG_SCREEN_CTRL 0x3d4
#define REG_SCREEN_DATA 0x3d5

void kprint(u8 *str);
void putchar(u8 symbol, u8 color);
void scroll_line();
void clear_screen();
void write(u8 symbol, u8 color, u16 offset);
u16 get_cursor();
void set_cursor(u16 offset);


#endif //SCREEN_H
