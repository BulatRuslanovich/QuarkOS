/**
* @file print.c
 * @brief Реализация функций форматированного вывода
 * @author getname
 * @date 03.04.2025
 * @defgroup print Функции вывода
 * @{
 */

#include "print.h"
#include "../common.h"
#include "screen.h"
#include <stdarg.h>

/**
 * @brief Преобразует беззнаковое целое в строку с указанным основанием
 * @param value Число для преобразования
 * @param[out] buf Буфер для результата (минимальный размер: 32 байта для base=16)
 * @param base Основание системы счисления (поддерживается 10 или 16)
 *
 * @note Особенности:
 * - Для base=16 добавляет префикс "0x"
 * - Буфер заполняется в обратном порядке с последующим разворотом
 * - Не поддерживает отрицательные числа
 *
 * @warning Не проверяет размер буфера! Для base=10 требуется минимум 10 символов,
 *          для base=16 - 10 символов + 2 символа префикса
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
 * @brief Форматированный вывод с фиксированным цветом (GREEN_ON_BLACK)
 * @param format Строка формата со спецификаторами:
 *              - %s: строка (char*)
 *              - %d: десятичное число (u32)
 *              - %x: шестнадцатеричное число (u32)
 *              - %%: символ '%'
 * @param ... Аргументы для подстановки
 *
 * @note Особенности:
 * - Использует статический буфер размером 32 символа для чисел
 * - Не поддерживает выравнивание и форматирование чисел
 * - Цвет фиксирован: зеленый текст на черном фоне
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
 * @brief Форматированный вывод с произвольным цветом
 * @param color Атрибут цвета (фоновый 4 бита | текстовый 4 бита)
 * @param format Строка формата (аналогично printf)
 * @param ... Аргументы для подстановки
 *
 * @note Цвет задается комбинацией фона и текста:
 * @code
 * colored_print(RED_ON_WHITE, "Alert: %d", 42);
 * @endcode
 *
 * @see printf() Для версии с фиксированным цветом
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

/** @} */ // Конец группы print