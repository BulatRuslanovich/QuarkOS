/**
* @file keyboard.c
 * @brief Обработка клавиатурного ввода и преобразование скан-кодов
 * @author getname
 * @date 02.04.2025
 * @defgroup keyboard Модуль обработки клавиатуры
 * @{
 */

#include "keyboard.h"
#include "asm_io.h"

/**
 * @brief Таблица преобразования базовых скан-кодов в ASCII
 * @details Соответствует US QWERTY раскладке без модификаторов.
 * Индексация начинается со скан-кода 0x02 (соответствует клавише '1').
 *
 * @note Особенности:
 * - Специальные коды:
 *   - '\b' (0x0E) - Backspace
 *   - '\n' (0x1C) - Enter
 * - Нулевые значения обозначают недопустимые/служебные символы
 */
static const char scancode_table[] = {
    0, 0, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0',
    '-', '=', '\b', 0, 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i',
    'o', 'p', '[', ']', '\n', '\b', 'a', 's', 'd', 'f', 'g', 'h',
    'j', 'k', 'l', ';', '\'', '`', 0, '\\', 'z', 'x', 'c', 'v',
    'b', 'n', 'm', ',', '.', '/', 0, '*', 0, ' '
};

/**
 * @brief Флаг состояния клавиш Shift
 * @details Глобальная переменная, обновляемая при обработке скан-кодов:
 * - 1: Shift нажат
 * - 0: Shift не нажат
 *
 * @warning Не является потокобезопасной
 */
u8 shift_pressed = 0;

/**
 * @brief Таблица преобразования скан-кодов с активным Shift
 * @details Содержит модифицированные символы верхнего регистра и
 * специальные символы. Формат аналогичен scancode_table.
 *
 * @note Отличия от базовой таблицы:
 * - Заглавные буквы
 * - Измененные специальные символы (например, '!' вместо '1')
 */
static const char shifted_scancode_table[] = {
    0,   0,   '!', '@', '#', '$', '%', '^', '&', '*', '(', ')',
    '_', '+', '\b', 0,   'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I',
    'O', 'P', '{', '}', '\n', 0,   'A', 'S', 'D', 'F', 'G', 'H',
    'J', 'K', 'L', ':', '"', '~', 0,   '|', 'Z', 'X', 'C', 'V',
    'B', 'N', 'M', '<', '>', '?', 0,   '*', 0,   ' '
};

/**
 * @brief Преобразует скан-код в символ ASCII
 * @param scancode Скан-код клавиши (младшие 7 бит - код, старший бит - флаг отпускания)
 * @return Преобразованный символ или 0 в случаях:
 *         - Клавиша отпущена (старший бит установлен)
 *         - Недопустимый скан-код
 *         - Служебная клавиша (Shift, Caps Lock и др.)
 *
 * @note Особенности работы:
 * - Обновляет глобальный флаг shift_pressed
 * - Поддерживает основной диапазон символов (0x02-0x35)
 * - Обрабатывает специальный случай пробела (0x39)
 * - Игнорирует коды отпускания клавиш
 *
 * @warning Не поддерживает расширенные скан-коды (> 0x80)
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
 * @brief Блокирующее чтение символа с клавиатуры
 * @return Введенный символ ASCII (игнорирует служебные коды)
 *
 * @note Алгоритм работы:
 * 1. Ожидает установку флага доступности данных (бит 0 статуса)
 * 2. Читает скан-код через порт 0x60
 * 3. Преобразует через scancode_to_ascii()
 * 4. Возвращает только валидные символы
 *
 * @warning Функция блокирует выполнение до получения символа
 * @see scancode_to_ascii() Для деталей преобразования кодов
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

/** @} */ // Конец группы keyboard