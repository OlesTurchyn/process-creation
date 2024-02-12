#include <stdio.h>

int main() {
    char buffer[256];
    while (fgets(buffer, sizeof(buffer), stdin) != NULL) {
        printf("Program 1 received: %s", buffer);
    }
    return 0;
}
