/**
* @file input.c
 * @brief Реализация функций ввода с клавиатуры
 * @author getname
 * @date 02.04.2025
 */

#include "input.h"
#include "keyboard.h"
#include "screen.h"

/**
 * @brief Считывает строку с клавиатуры с обработкой специальных символов
 *
 * @param[out] buffer Указатель на буфер для сохранения введенных данных
 * @param[in] max_size Максимальный размер буфера (включая нулевой терминатор)
 *
 * @note Особенности реализации:
 * - Поддерживает обработку Backspace (удаление последнего символа)
 * - Завершает ввод при получении символа новой строки (Enter)
 * - Выводит вводимые символы на экран в реальном времени
 * - Гарантирует нуль-терминацию строки
 *
 * @warning
 * - Не проверяет валидность указателя buffer
 * - При max_size = 1 буфер будет сразу завершен нулем
 *
 * @see getchar() Для получения символов с клавиатуры
 * @see putchar() Для отображения символов на экране
 */
void scanf(char *buffer, u32 max_size) {
    u32 index = 0;
    while (1) {
        char c = getchar();
        if (c == '\n') { // Enter
            buffer[index] = '\0';
            putchar('\n', GREEN_ON_BLACK);
            return;
        }

        if (c == '\b') { // Backspace
            if (index > 0) {
                index--;
                putchar('\b', GREEN_ON_BLACK);
            }
        } else if (c != 0 && index < max_size - 1) {
            buffer[index++] = c;
            putchar(c, GREEN_ON_BLACK);
        }
    }
}

/** @} */