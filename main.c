#include <stdio.h>
#include <string.h>

#include "mmemory.h"

int main(void) {
    char* text = malloc(sizeof(char) * 11);
    strcpy(text, "Hello World");
    printf(text);

    printf("\n");

    free(text);
    text = realloc(text, 10);
    strcpy(text, "Hello World!");
    printf(text);

    printf("\nPROGRAM COMPLETE\n");
}