//
// Created by getname on 29.03.2025.
//

#ifndef COMMON_H
#define COMMON_H

typedef unsigned int u32;
typedef int s32;
typedef unsigned short u16;
typedef short s16;
typedef unsigned char u8;
typedef char s8;

void memcpy(const u8 *src, u8 *dst, u32 len);
int strcmp(const char *s1, const char *s2);
void print_cow();
void print_rick_and_morty();

#endif //COMMON_H
