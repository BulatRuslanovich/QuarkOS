#include "../common.h"
#include "../drivers/screen.h"
#include "../drivers/input.h"
#include "../drivers/print.h"
#include "../drivers/asm_io.h"


s32 kmain() {
    clear_screen();
    print_rick_and_morty();
    printf("Welcome to QuarkOS v1.0\n");

    char username[50];
    char password[50];
    char* os_name = "quark";

    while (1) {
        printf("Username: ");
        scanf(username, sizeof(username));
        printf("Password: ");
        scanf(password, sizeof(password));

        if (strcmp(username, "root") == 0 && strcmp(password, "root") == 0) {
            break;
        } else {
            colored_print(0x04 ,"Wrong Username or Password\n");
        }
    }

    char command[50];

    printf("%s@%s:~$ ", username, os_name);

    while (1) {
        scanf(command, sizeof(command));

        if (strcmp(command, "q") == 0) {
            clear_screen();
            printf("Shutting down...");
            // asm volatile("hlt");
            power_off();
            break;
        }

        if (!strcmp(command, "cow")) {
            printf("\n");
            print_cow();
        } else if (!strcmp(command, "clear")) {
            clear_screen();
        } else if (!strcmp(command, "rimo")) {
            print_rick_and_morty();
        } else if (!strcmp(command, "whoami")) {
            printf(username);
        } else if (!strcmp(command, "help")) {
            clear_screen();
            colored_print(0x0F, " help  | Show this menu\n");
            colored_print(0x0F, " clear | Clear screen\n");
            colored_print(0x0F, " cow   | Show ASCII art cow\n");
            colored_print(0x0F, " rimo  | Rick and Morty art\n");
            colored_print(0x0F, " q     | Shutdown system\n");
        }

        printf("%s@%s:~$ ", username, os_name);
    }

    return 0;
}
