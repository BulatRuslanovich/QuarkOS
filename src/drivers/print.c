//
// Created by getname on 03.04.2025.
//

#include "print.h"
#include "../common.h"
#include "screen.h"
#include <stdarg.h>

/**
 * Преобразует целое число без знака в строку в указанной системе счисления.
 *
 * @param value Число для преобразования
 * @param buf   Буфер для записи результата (должен быть достаточного размера)
 * @param base  Основание системы счисления (10 или 16)
 *
 * @note Для base 16 добавляет префикс "0x" перед числом
 */
static void itoa(u32 value, char *buf, u8 base) {
    static const char digits[] = "0123456789abcdef";
    char *ptr = buf;
    u32 tmp = value;

    // Генерация строки в обратном порядке
    do {
        *ptr++ = digits[tmp % base];
        tmp /= base;
    } while (tmp > 0);

    // Добавление префикса для шестнадцатеричных чисел
    if (base == 16) {
        *ptr++ = 'x';
        *ptr++ = '0';
    }

    *ptr = '\0';

    // Разворот строки для правильного порядка символов
    char *start = buf;
    char *end = ptr - 1;
    while (start < end) {
        char tmp = *start;
        *start = *end;
        *end = tmp;
        start++;
        end--;
    }
}

/**
 * Форматированный вывод строки с поддержкой базовых спецификаторов.
 *
 * @param format Строка формата с плейсхолдерами:
 *               %s - строка
 *               %d - десятичное число
 *               %x - шестнадцатеричное число
 *               %% - символ '%'
 * @param ...    Аргументы, соответствующие плейсхолдерам
 */
void printf(const char *format, ...) {
    va_list args;
    va_start(args, format);

    while (*format) {
        if (*format == '%') {
            format++;
            switch (*format) {
                case 's': { // Обработка строк
                    char *str = va_arg(args, char*);
                    while (*str) {
                        putchar(*str++, GREEN_ON_BLACK);
                    }
                    break;
                }
                case 'd': { // Обработка десятичных чисел
                    u32 num = va_arg(args, u32);
                    char buf[32];
                    itoa(num, buf, 10);
                    char *p = buf;
                    while (*p) {
                        putchar(*p++, GREEN_ON_BLACK);
                    }
                    break;
                }
                case 'x': { // Обработка шестнадцатеричных чисел
                    u32 num = va_arg(args, u32);
                    char buf[32];
                    itoa(num, buf, 16);
                    char *p = buf;
                    while (*p) {
                        putchar(*p++, GREEN_ON_BLACK);
                    }
                    break;
                }
                case '%': { // Вывод символа '%'
                    putchar('%', GREEN_ON_BLACK);
                    break;
                }
            }
        } else {
            putchar(*format, GREEN_ON_BLACK);
        }
        format++;
    }

    va_end(args);
}

/**
 * Цветной форматированный вывод с поддержкой спецификаторов.
 *
 * @param color  Атрибут цвета текста (фоновый цвет | цвет текста)
 * @param format Строка формата (аналогично printf)
 * @param ...    Аргументы для подстановки
 */
void colored_print(u8 color, const char *format, ...) {
    va_list args;
    va_start(args, format);

    while (*format) {
        if (*format == '%') {
            format++;
            switch (*format) {
                case 's': {
                    char *str = va_arg(args, char*);
                    while (*str) {
                        putchar(*str++, color);
                    }
                    break;
                }
                case 'd': {
                    u32 num = va_arg(args, u32);
                    char buf[32];
                    itoa(num, buf, 10);
                    char *p = buf;
                    while (*p) {
                        putchar(*p++, color);
                    }
                    break;
                }
                case 'x': {
                    u32 num = va_arg(args, u32);
                    char buf[32];
                    itoa(num, buf, 16);
                    char *p = buf;
                    while (*p) {
                        putchar(*p++, color);
                    }
                    break;
                }
                case '%': {
                    putchar('%', color);
                    break;
                }
            }
        } else {
            putchar(*format, color);
        }
        format++;
    }

    va_end(args);
}