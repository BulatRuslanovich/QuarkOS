//
// Created by getname on 29.03.2025.
//

#ifndef ASM_IO_H
#define ASM_IO_H

#include "../common.h"

#define SHUTDOWN_PORT 0x604


unsigned char port_byte_in(unsigned short port);

void port_byte_out(unsigned short port, unsigned char data);

unsigned char port_word_in(unsigned short port);

void port_word_out(unsigned short port, unsigned short data);


/**
 * Чтение статуса контроллера клавиатуры.
 *
 * @return u8 - Байт состояния клавиатуры (биты указывают на статус буферов):
 *              Бит 0: 1 = Данные в выходном буфере (можно читать порт 0x60)
 *              Бит 1: 1 = Данные во входном буфере (запись в порт 0x60/0x64 запрещена)
 */
static u8 keyboard_status() {
    return port_byte_in(0x64); // Чтение из порта состояния клавиатуры (0x64)
}

/**
 * Чтение скан-кода клавиши из буфера клавиатуры.
 *
 * @return u8 - Скан-код последней нажатой клавиши.
 *              Для отпускания клавиш старший бит установлен (0x80).
 */
static u8 read_scancode() {
    return port_byte_in(0x60); // Чтение из порта данных клавиатуры (0x60)
}

/**
 * Выключение системы через ACPI (работает в QEMU и некоторых эмуляторах).
 *
 * @note Требует предварительной настройки ACPI в системе.
 *       Для реального железа используйте специфичные для платформы методы.
 */
static void power_off() {
    // Отправка магического значения 0x2000 в порт выключения QEMU (0x604)
    port_word_out(SHUTDOWN_PORT, 0x2000);
    // Альтернатива для некоторых систем: 0x0FED1C00 с 0x0000
}


#endif //ASM_IO_H
