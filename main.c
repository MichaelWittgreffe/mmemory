#include <stdio.h>
#include <string.h>

#include "mmemory.h"

int main(void) {
    char* text = malloc(sizeof(char) * 11);
    strcpy(text, "Hello World");
    printf("%s\n", text);

    free(text);
    text = realloc(text, 10);
    strcpy(text, "Hello World!");
    printf("%s\n", text);

    printf("PROGRAM COMPLETE\n");
}