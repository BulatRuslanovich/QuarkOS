#include "../common.h"
#include "../drivers/screen.h"
#include "../drivers/input.h"
#include "../drivers/print.h"
#include "../drivers/asm_io.h"


s32 kmain() {
    clear_screen();
    print_rick_and_morty();
    printf("QuarkOS v1.0\n");
    printf("Memory: %d MB\n", 1024);
    printf("Address: 0x%x\n", 0x100000);
    printf("> ");

    char input[50];

    while (1) {
        scanf(input, sizeof(input));

        // printf("You entered: ");
        // printf(input);

        if (strcmp(input, "q") == 0) {
            clear_screen();
            printf("Shutting down...");
            // asm volatile("hlt");
            power_off();
            break;
        } else if (!strcmp(input, "cow")) {
            printf("\n");
            print_cow();
        } else if (!strcmp(input, "clear")) {
            clear_screen();
        } else if (!strcmp(input, "rimo")) {
            print_rick_and_morty();

        } else if (!strcmp(input, "help")) {
            clear_screen();
            colored_print(0x0F, " help  | Show this menu\n");
            colored_print(0x0F, " clear | Clear screen\n");
            colored_print(0x0F, " cow   | Show ASCII art cow\n");
            colored_print(0x0F, " rimo  | Rick and Morty art\n");
            colored_print(0x0F, " q     | Shutdown system\n");
        } else {
            colored_print(0x04, "Invalid input!");
        }

        printf("\n> ");
    }

    while (1) {}
    return 0;
}
