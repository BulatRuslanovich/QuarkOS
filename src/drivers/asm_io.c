/**
* @file asm_io.c
 * @brief Низкоуровневые операции ввода-вывода через порты
 * @author getname
 * @date 02.04.2025
 * @defgroup io_ports Функции работы с портами ввода-вывода
 * @{
 */

#include "asm_io.h"

/**
 * Чтение байта из порта ввода-вывода (I/O порта)
 * @param port 16-битный адрес порта (0x0000-0xFFFF)
 * @return Считанный байт данных
 *
 * @note Особенности:
 * - Использует ассемблерную инструкцию IN (чтение из порта)
 * - DX хранит адрес порта, результат возвращается через AL
 * - Гарантируется атомарность операции на уровне процессора
 *
 * @warning Не проверяет валидность адреса порта
 */
unsigned char port_byte_in(unsigned short port) {
    unsigned char result;
    __asm__("in %%dx, %%al" : "=a" (result) : "d" (port));
    return result;
}

/**
 * Запись байта в порт ввода-вывода
 * @param port 16-битный адрес порта (0x0000-0xFFFF)
 * @param data 8-битное значение для записи
 *
 * @note Особенности:
 * - Использует ассемблерную инструкцию OUT (запись в порт)
 * - AL содержит данные, DX - адрес порта
 * - Операция выполняется за 1 такт шины
 */
void port_byte_out(unsigned short port, unsigned char data) {
    __asm__("out %%al, %%dx" : : "a" (data), "d" (port));
}

/**
 * Чтение 16-битного слова из порта ввода-вывода
 * @param port 16-битный адрес порта (0x0000-0xFFFF)
 * @return Считанное 16-битное слово
 *
 * @note Особенности:
 * - Использует регистр AX вместо AL для 16-битных операций
 * - Адресация портов осуществляется через DX
 * - Поддерживает операции с 16-битными устройствами (например, PCI)
 */
unsigned char port_word_in(unsigned short port) {
    unsigned short result;
    __asm__("in %%dx, %%ax" : "=a" (result) : "d" (port));
    return result;
}

/**
 * Запись 16-битного слова в порт ввода-вывода
 * @param port 16-битный адрес порта (0x0000-0xFFFF)
 * @param data 16-битное значение для записи
 *
 * @note Особенности:
 * - Использует регистр AX для передачи данных
 * - Совместима с 16-битными контроллерами (PIC, DMA)
 * - Время выполнения зависит от типа устройства
 */
void port_word_out(unsigned short port, unsigned short data) {
    __asm__("out %%ax, %%dx" : : "a" (data), "d" (port));
}

/** @} */ // Конец группы io_ports
