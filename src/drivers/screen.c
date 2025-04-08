/**
* @file screen.c
 * @brief Управление выводом на экран и работой с видеопамятью
 * @author getname
 * @date 29.03.2025
 * @defgroup screen Управление экраном
 * @{
 */
#include "screen.h"
#include "../common.h"
#include "asm_io.h"

/**
 * @brief Выводит строку в текущую позицию курсора
 * @param[in] str Указатель на нуль-терминированную ASCII-строку
 *
 * @note Особенности:
 * - Использует цвет GREEN_ON_BLACK по умолчанию
 * - Обрабатывает управляющие символы \n и \b
 * - Не поддерживает Escape-последовательности
 *
 * @warning Не проверяет валидность указателя str
 */
void kprint(u8 *str) {
    while (*str) {
        putchar(*str, GREEN_ON_BLACK);
        str++;
    }
}

/**
 * @brief Выводит символ с указанными атрибутами цвета
 * @param[in] symbol ASCII-код символа
 * @param[in] color  Атрибут цвета (4 бита фона | 4 бита текста)
 *
 * @note Логика работы:
 * - \n - переход на новую строку со скроллингом при необходимости
 * - \b - перемещение курсора назад с удалением символа
 * - При достижении конца экрана инициирует скроллинг
 * - Прямая запись в видеопамять (0xB8000)
 *
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
 * @brief Прокручивает экран на одну строку вверх
 *
 * @note Алгоритм:
 * 1. Копирует строки 1..MAX_ROWS-1 в 0..MAX_ROWS-2
 * 2. Очищает последнюю строку
 * 3. Устанавливает курсор в начало последней строки
 *
 * @warning Использует memcpy для копирования данных
 * @warning Может быть медленной при частом вызове
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
 * @brief Полностью очищает экран
 *
 * @note Заполняет всю видеопамять:
 * - Символы: 0x00
 * - Цвет: GREEN_ON_BLACK
 * - Сбрасывает позицию курсора в начало
 *
 * @see write() Для реализации записи в видеопамять
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
 * @brief Записывает символ в видеопамять
 * @param[in] symbol ASCII-код символа
 * @param[in] color  Атрибут цвета
 * @param[in] offset Смещение в видеопамяти (в байтах)
 *
 * @warning Не проверяет корректность offset
 * @note Каждая позиция на экране занимает 2 байта:
 * [0] - символ, [1] - атрибуты
 */
void write(const u8 symbol, const u8 color, const u16 offset) {
    u8 *vga = (u8 *) VIDEO_ADDRESS;
    vga[offset] = symbol;
    vga[offset + 1] = color;
}

/**
 * @brief Получает текущую позицию курсора
 * @return Текущее смещение курсора в видеопамяти (в байтах)
 *
 * @note Алгоритм:
 * 1. Запрос старшего байта через порт 0x3D4 (14)
 * 2. Запрос младшего байта через порт 0x3D4 (15)
 * 3. Преобразование в смещение: позиция * 2
 *
 * @see port_byte_out() @see port_byte_in()
 */
u16 get_cursor() {
    port_byte_out(REG_SCREEN_CTRL, 14);
    u8 high_byte = port_byte_in(REG_SCREEN_DATA);
    port_byte_out(REG_SCREEN_CTRL, 15);
    u8 low_byte = port_byte_in(REG_SCREEN_DATA);
    return (((high_byte << 8) + low_byte) * 2);
}

/**
 * @brief Устанавливает новую позицию курсора
 * @param[in] offset Смещение в видеопамяти (в байтах)
 *
 * @warning Не проверяет валидность offset
 */
void set_cursor(u16 offset) {
    offset /= 2;

    port_byte_out(REG_SCREEN_CTRL, 14);
    port_byte_out(REG_SCREEN_DATA, (u8) (offset >> 8));
    port_byte_out(REG_SCREEN_CTRL, 15);
    port_byte_out(REG_SCREEN_DATA, (u8) (offset & 0xff));
}

/** @} */ // Конец группы screen