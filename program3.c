#include <stdio.h>
#include <string.h>

int main() {
    char buffer[256];
    while (fgets(buffer, sizeof(buffer), stdin) != NULL) {
        int length = strlen(buffer);
        printf("Program 3 received %d characters.\n", length);
    }
    return 0;
}