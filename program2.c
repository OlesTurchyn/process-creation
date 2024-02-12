#include <stdio.h>
#include <ctype.h>

int main() {
    char buffer[256];
    while (fgets(buffer, sizeof(buffer), stdin) != NULL) {
        for (int i = 0; buffer[i] != '\0'; i++) {
            buffer[i] = toupper(buffer[i]);
        }
        printf("Program 2 received: %s", buffer);
    }
    return 0;
}
