#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define INITIAL_CAPACITY 100
#define MAX_LINES 100
#define MAX_LINE_LENGTH 100

struct TextContainer {
    char* buffer; // for dynamic memory allocation
    int current_size;
    int capacity;
};

struct TextContainer text[MAX_LINES];
int current_line = 0;

void print_help(){
    printf("Commands: \n");
    printf("1 - append <text> - append text to the end \n");
    printf("2 - start the new line \n");
    printf("3 - save <filename> - use files to save the information \n");
    printf("4 - load <filename> - use files to save the information \n");
    printf("5 - print the current text to console \n");
    printf("6 - insert the text  by line and symbol index \n");
    printf("7 - search <word> \n");
    printf("8 - clear the console \n");
}

void free_text_container(struct TextContainer* container) {
    free(container->buffer);
    container->buffer = NULL;
    container->current_size = 0;
    container->capacity = 0;
}

void init_text_container(struct TextContainer* container) {
    container->buffer = (char*)malloc(INITIAL_CAPACITY * sizeof(char));
    if (container->buffer == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }
    container->current_size = 0;
    container->capacity = INITIAL_CAPACITY;
}

void resize_text_container(struct TextContainer* container, int new_capacity) {
    container->buffer = (char*)realloc(container->buffer, new_capacity * sizeof(char));
    if (container->buffer == NULL) {
        fprintf(stderr, "Memory reallocation failed\n");
        exit(1);
    }
    container->capacity = new_capacity;
}