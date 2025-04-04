//
// Created by getname on 02.04.2025.
//

#include "keyboard.h"
#include "asm_io.h"

/**
 * Таблица преобразования скан-кодов в символы ASCII
 * (без нажатого Shift). Соответствует клавиатурной раскладке US QWERTY.
 * Индекс массива соответствует скан-коду клавиши минус 0x02.
 * Специальные значения:
 *  0 - недопустимый символ
 * '\b' - Backspace (0x0E)
 * '\n' - Enter (0x1C)
 */
static const char scancode_table[] = {
    0, 0, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0',
    '-', '=', '\b', 0, 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i',
    'o', 'p', '[', ']', '\n', '\b', 'a', 's', 'd', 'f', 'g', 'h',
    'j', 'k', 'l', ';', '\'', '`', 0, '\\', 'z', 'x', 'c', 'v',
    'b', 'n', 'm', ',', '.', '/', 0, '*', 0, ' '
};

/** Глобальный флаг состояния клавиш Shift */
u8 shift_pressed = 0;

/**
 * Таблица преобразования скан-кодов в символы ASCII
 * при нажатом Shift. Содержит модифицированные символы:
 * - Заглавные буквы
 * - Специальные символы верхнего регистра
 * Формат аналогичен scancode_table
 */
static const char shifted_scancode_table[] = {
    0,   0,   '!', '@', '#', '$', '%', '^', '&', '*', '(', ')',
    '_', '+', '\b', 0,   'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I',
    'O', 'P', '{', '}', '\n', 0,   'A', 'S', 'D', 'F', 'G', 'H',
    'J', 'K', 'L', ':', '"', '~', 0,   '|', 'Z', 'X', 'C', 'V',
    'B', 'N', 'M', '<', '>', '?', 0,   '*', 0,   ' '
};

/**
 * Преобразует скан-код клавиши в символ ASCII с учётом состояния Shift
 * @param scancode Скан-код клавиши (младшие 7 бит - код, старший бит - флаг отпускания)
 * @return char - Символ ASCII или 0, если:
 *                - Клавиша отпущена
 *                - Недопустимый скан-код
 *                - Служебная клавиша (Shift и др.)
 */
char scancode_to_ascii(u8 scancode) {
    // Выделяем базовый скан-код (без флага отпускания)
    u8 base_sc = scancode & 0x7F;

    // Обработка клавиш Shift
    if (base_sc == 0x2A || base_sc == 0x36) { // Left/Right Shift
        shift_pressed = (scancode & 0x80) ? 0 : 1; // Обновляем флаг состояния
        return 0; // Shift не генерирует символы
    }

    // Игнорируем события отпускания клавиш
    if (scancode & 0x80) return 0;

    // Преобразование скан-кода в символ
    char c = 0;
    if (scancode >= 0x02 && scancode <= 0x35) { // Основной диапазон символов
        c = shift_pressed
            ? shifted_scancode_table[scancode]
            : scancode_table[scancode];
    } else if (scancode == 0x39) { // Пробел
        c = ' ';
    }

    return c;
}

/**
 * Ожидает и возвращает символ с клавиатуры
 * @return char - Введённый символ ASCII (игнорирует служебные коды)
 * @note Функция блокирующая - ожидает нажатие клавиши
 */
char getchar() {
    u8 scancode;
    while (1) {
        // Ожидаем активное состояние клавиатуры
        while ((keyboard_status() & 0x01) == 0);

        // Читаем скан-код
        scancode = read_scancode();

        // Обрабатываем скан-код (включая обновление shift_pressed)
        char result = scancode_to_ascii(scancode);

        // Возвращаем результат только для валидных нажатий
        if (!(scancode & 0x80) && result != 0) {
            return result;
        }
    }
}