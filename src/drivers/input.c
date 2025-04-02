//
// Created by getname on 02.04.2025.
//

#include "input.h"
#include "keyboard.h"
#include "screen.h"

void scanf(char *buffer, u32 max_size) {
    u32 index = 0;
    while (1) {
        char c = getchar();
        if (c == '\n') { // Enter
            buffer[index] = '\0';
            putchar('\n', GREEN_ON_BLACK);
            return;
        } else if (c == '\b') { // Backspace
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
