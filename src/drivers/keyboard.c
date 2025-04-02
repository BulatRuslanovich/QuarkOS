//
// Created by getname on 02.04.2025.
//

#include "keyboard.h"

#include "asm_io.h"

static const char scancode_table[] = {
    0, 0, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0',
    '-', '=', '\b', 0, 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i',
    'o', 'p', '[', ']', '\n', '\b', 'a', 's', 'd', 'f', 'g', 'h',
    'j', 'k', 'l', ';', '\'', '`', 0, '\\', 'z', 'x', 'c', 'v',
    'b', 'n', 'm', ',', '.', '/', 0, '*', 0, ' '
};

u8 shift_pressed = 0;

// Таблица символов с Shift
static const char shifted_scancode_table[] = {
    0,   0,   '!', '@', '#', '$', '%', '^', '&', '*', '(', ')',
    '_', '+', '\b', 0,   'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I',
    'O', 'P', '{', '}', '\n', 0,   'A', 'S', 'D', 'F', 'G', 'H',
    'J', 'K', 'L', ':', '"', '~', 0,   '|', 'Z', 'X', 'C', 'V',
    'B', 'N', 'M', '<', '>', '?', 0,   '*', 0,   ' '
};

char scancode_to_ascii(u8 scancode) {
    u8 base_sc = scancode & 0x7F; // Убираем бит отпускания

    if (base_sc == 0x2A || base_sc == 0x36) { // Left/Right Shift
        shift_pressed = (scancode & 0x80) ? 0 : 1; // 0x80 = отпущено
        return 0; // Не возвращаем символ для Shift
    }

    // Игнорируем отпускания остальных клавиш
    if (scancode & 0x80) return 0;

    // Получаем символ из таблицы
    char c = 0;
    if (scancode >= 0x02 && scancode <= 0x35) {
        c = shift_pressed ? shifted_scancode_table[scancode] : scancode_table[scancode];
    } else if (scancode == 0x39) {
        c = ' ';
    }

    return c;
}

char getchar() {
    u8 scancode;
    while (1) {
        // Ждём любое событие (нажатие или отпускание)
        while ((keyboard_status() & 0x01) == 0);
        scancode = read_scancode();

        // Всегда обрабатываем скан-код для обновления shift_pressed
        char result = scancode_to_ascii(scancode);

        // Возвращаем символ только при нажатии (без 0x80)
        if (!(scancode & 0x80) && result != 0) {
            return result;
        }
    }
}
