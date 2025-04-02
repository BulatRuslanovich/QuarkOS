//
// Created by getname on 29.03.2025.
//

#include "common.h"
#include "drivers/screen.h"
#include "drivers/print.h"

/**
 * Копирует заданное количество байт из источника в приемник.
 *
 * @param src  Указатель на источник данных (read-only)
 * @param dst  Указатель на приемник данных (куда будет производиться запись)
 * @param len  Количество байт для копирования (в диапазоне [0, U32_MAX])
 *
 * @note Особенности реализации:
 * 1. Выполняет побайтовое копирование через 8-битные указатели
 * 2. Не проверяет валидность указателей (должны быть инициализированы)
 * 3. Не обрабатывает перекрывающиеся области памяти
 */
void memcpy(const u8 *src, u8 *dst, const u32 len) {
    u32 i = 0;
    while (i < len) {
	    dst[i] = src[i];
    }
}

int strcmp(const char *s1, const char *s2) {
    // Пока символы совпадают и не достигнут конец строки
    while (*s1 && (*s1 == *s2)) {
        s1++;
        s2++;
    }

    // Возвращаем разницу между ASCII-кодами символов
    return *(const unsigned char*)s1 - *(const unsigned char*)s2;
}

void kprint_colored(u8 *str, u8 attr) {
    while (*str) {
        if (*str == '_')
            putchar(*str, 0x00);
        else
            putchar(*str, attr);
        str++;
    }
}

void print_cow() {
    const char *cow_art[] = {
        "  ^__^                             \n",
        "  (oo)\\_______                     \n",
        "  (__)\\       )\\/\\                \n",
        "      ||----w |                    \n",
        "      ||     ||                    \n"
    };

    // Печатаем каждую строку ASCII-арта
    for (int i = 0; i < sizeof(cow_art) / sizeof(cow_art[0]); i++) {
        colored_print(0x01, "%s", cow_art[i]);
    }
}

void print_rick_and_morty() {
    u8 *char_map[] = {
        "__#_#_#\n",
        "__#####____", "#####\n",
        "__#", "###", "#____", "#", "###", "#\n",
        "__#", "#", "#", "#", "#____", "#", "#", "#", "#", "#\n",
        "__#", "#", "#", "#", "#____", "#", "#", "#", "#", "#\n",
        "__#####____", "#####\n",
        "___###_____", "_###_\n",
        "__##", "#", "##____", "#####\n",
        "__##", "#", "##____", "#", "###", "#\n",
        "__##", "#", "##____", "#", "###", "#\n",
        "__#", "#", "#", "#", "#____", "#", "###", "#\n",
        "___##", "#______", "###\n",
        "___#_#______#_#\n"
    };
    u8 color_map[] = {
        0xbb,
        0xbb, 0x66,
        0xbb, 0xff, 0xbb, 0x66, 0xff, 0x66,
        0xff, 0x00, 0xff, 0x00, 0xff, 0xff, 0x00, 0xff, 0x00, 0xff,
        0xff, 0x00, 0xff, 0x00, 0xff, 0xff, 0x00, 0xff, 0x00, 0xff,
        0xff, 0xff,
        0xff, 0xff,
        0x77, 0xbb, 0x77, 0xee,
        0x77, 0xbb, 0x77, 0xff, 0xee, 0xff,
        0x77, 0xbb, 0x77, 0xff, 0xee, 0xff,
        0xff, 0x77, 0xbb, 0x77, 0xff, 0xff, 0xee, 0xff,
        0x99, 0x77, 0x99,
        0x99
    };
    u8 i = 0;
    while (i < 61) {
        kprint_colored(char_map[i], color_map[i]);
        i++;
    }
}