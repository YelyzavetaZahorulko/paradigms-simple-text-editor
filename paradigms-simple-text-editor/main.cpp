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

void printHelp(){
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

void freeTextContainer(struct TextContainer* container) {
    free(container->buffer);
    container->buffer = NULL;
    container->current_size = 0;
    container->capacity = 0;
}

void initTextContainer(struct TextContainer* container) {
    container->buffer = (char*)malloc(INITIAL_CAPACITY * sizeof(char));
    if (container->buffer == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }
    container->current_size = 0;
    container->capacity = INITIAL_CAPACITY;
}

void resizeTextContainer(struct TextContainer* container, int new_capacity) {
    container->buffer = (char*)realloc(container->buffer, new_capacity * sizeof(char));
    if (container->buffer == NULL) {
        fprintf(stderr, "Memory reallocation failed\n");
        exit(1);
    }
    container->capacity = new_capacity;
}

void appendText(char* text_to_append) {
    if (current_line < MAX_LINES) {
        struct TextContainer* line = &text[current_line];
        initTextContainer(line);

        int append_length = strlen(text_to_append);
        if (line->current_size + append_length >= line->capacity) {
            resizeTextContainer(line, line->current_size + append_length + 1);
        }
        strcpy(line->buffer, text_to_append);
        line->current_size = append_length;
        current_line++;
    } else {
        printf("Error: Maximum number of lines reached.\n");
    }
}

void handleСommand(int command) {
    char input[256];
    if (command == 1) {
        printf("Enter text to append: ");
        fgets(input, sizeof(input), stdin);
        input[strcspn(input, "\n")] = '\0'; // Remove newline character
        appendText(input);
    }
    else {
        printf("The command is not implemented.\n");
    }
}


int main() {

    int command;
    printHelp();
    while (1) {
        printf("\nChoose the command: \n>");
        if (scanf("%d", &command) != 1) {
            printf("Invalid input. Please enter a number.\n");
            while (getchar() != '\n'); // Clear the input buffer
            continue;
        }
        getchar(); // Clear the newline character from the buffer

        if (command < 1 || command > 9) {
            printf("Invalid command number. Please enter a number between 1 and 9.\n");
            continue;
        }

        handleСommand(command);
    }
    return 0;
}