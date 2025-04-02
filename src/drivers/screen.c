// Created by getname on 29.03.2025.

#include "screen.h"
#include "../common.h"
#include "asm_io.h"

/**
 * Выводит строку в текущую позицию курсора
 * @param str Указатель на нуль-терминированную строку (кодировка ASCII)
 *
 * @note Особенности:
 * - Использует зелёный текст на чёрном фоне по умолчанию
 * - Поддерживает управляющие символы (например, \n)
 * - Не поддерживает escape-последовательности
 */
void kprint(u8 *str) {
    while (*str) {
        putchar(*str, GREEN_ON_BLACK);
        str++;
    }
}

/**
 * Выводит символ с указанными атрибутами цвета
 * @param symbol ASCII-код символа
 * @param color  Атрибут цвета (формат: 0xKF, где K-фон, F-передний план)
 *
 * @note Логика обработки:
 * - Символ \n вызывает переход на следующую строку
 * - При достижении конца экрана инициирует прокрутку
 * - Записывает данные напрямую в видеопамять (0xB8000)
 */
void putchar(u8 symbol, u8 color) {
    const u16 offset = get_cursor();

    if (symbol == '\n') {
        if (offset / 2 / MAX_COLS == MAX_ROWS - 1) {
            scroll_line();
        } else {
            set_cursor(offset - offset % (MAX_COLS * 2) + MAX_COLS * 2);
        }
        return;
    }

    if (symbol == '\b') {
        // Обработка Backspace
        if (offset >= 2) {
            set_cursor(offset - 2);
            write(' ', color, offset - 2);
            set_cursor(offset - 2);
        }
        return;
    }


    if (offset == MAX_ROWS * MAX_COLS * 2) {
        scroll_line();
    }

    write(symbol, color, offset);
    set_cursor(offset + 2);
}

/**
 * Прокручивает экран на одну строку вверх
 * @note Алгоритм:
 * 1. Копирует строки 1-24 в 0-23 через memcpy
 * 2. Очищает последнюю строку (24-я)
 * 3. Устанавливает курсор в начало последней строки
 */
void scroll_line() {
    u8 i = 1;

    while (i < MAX_ROWS) {
        memcpy(
            (u8 *) (VIDEO_ADDRESS + MAX_COLS * i * 2),
            (u8 *) (VIDEO_ADDRESS + MAX_COLS * (i - 1) * 2),
            MAX_COLS * 2
        );
        i++;
    }

    const u16 last_line = MAX_COLS * MAX_ROWS * 2 - MAX_COLS * 2;
    i = 0;
    while (i < MAX_COLS) {
        write('\0', GREEN_ON_BLACK, last_line + i * 2);
        i++;
    }
    set_cursor(last_line);
}

/**
 * Очищает экран и сбрасывает курсор в начало
 * @note Заполняет всю видеопамять нулевыми символами
 */
void clear_screen() {
    u16 offset = 0;

    while (offset < MAX_ROWS * MAX_COLS * 2) {
        write('\0', GREEN_ON_BLACK, offset);
        offset += 2;
    }

    set_cursor(0);
}

/**
 * Записывает символ с цветом в указанную позицию видеопамяти
 * @param symbol Код символа ASCII
 * @param color  Атрибут цвета
 * @param offset Смещение в видеопамяти (в байтах)
 */
void write(const u8 symbol, const u8 color, const u16 offset) {
    u8 *vga = (u8 *) VIDEO_ADDRESS;
    vga[offset] = symbol;
    vga[offset + 1] = color;
}

/**
 * Получает текущую позицию курсора
 * @return Смещение курсора в видеопамяти (в байтах)
 *
 * @note Алгоритм:
 * 1. Запрашивает старший байт позиции через порт 0x3D4:14
 * 2. Запрашивает младший байт через порт 0x3D4:15
 * 3. Конвертирует позицию в смещение (1 позиция = 2 байта)
 */
u16 get_cursor() {
    port_byte_out(REG_SCREEN_CTRL, 14);
    u8 high_byte = port_byte_in(REG_SCREEN_DATA);
    port_byte_out(REG_SCREEN_CTRL, 15);
    u8 low_byte = port_byte_in(REG_SCREEN_DATA);
    return (((high_byte << 8) + low_byte) * 2);
}

/**
 * Устанавливает новую позицию курсора
 * @param offset Смещение в видеопамяти (в байтах)
 *
 * @note Преобразует смещение в позицию курсора (offset/2)
 * и записывает в контроллер через порты 0x3D4/0x3D5
 */
void set_cursor(u16 offset) {
    offset /= 2;

    port_byte_out(REG_SCREEN_CTRL, 14);
    port_byte_out(REG_SCREEN_DATA, (u8) (offset >> 8));
    port_byte_out(REG_SCREEN_CTRL, 15);
    port_byte_out(REG_SCREEN_DATA, (u8) (offset & 0xff));
}
