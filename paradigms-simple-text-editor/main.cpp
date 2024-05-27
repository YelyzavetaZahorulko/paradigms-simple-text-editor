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

TextContainer* text_array;
int line_count = 0;

void printHelp(){
    printf("Commands: \n");
    printf("1 - append <text> - append text to the end \n");
    printf("2 - start the new line \n");
    printf("3 - save <filename> - use files to save the information \n");
    printf("4 - load <filename> - use files to load the information \n");
    printf("5 - print the current text to console \n");
    printf("6 - insert the text  by line and symbol index \n");
    printf("7 - search <word> \n");
}

void freeTextContainer(TextContainer* container) {
    free(container->buffer);
    container->buffer = NULL;
    container->current_size = 0;
    container->capacity = 0;
}

void initTextContainer(TextContainer* container) {
    container->buffer = (char*)malloc(INITIAL_CAPACITY * sizeof(char));
    if (container->buffer == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }
    container->current_size = 0;
    container->capacity = INITIAL_CAPACITY;
}

void resizeTextContainer(TextContainer* container, int new_capacity) {
    container->buffer = (char*)realloc(container->buffer, new_capacity * sizeof(char));
    if (container->buffer == NULL) {
        fprintf(stderr, "Memory reallocation failed\n");
        exit(1);
    }
    container->capacity = new_capacity;
}

void appendText(char* text_to_append) {
    line_count++;
    text_array = (TextContainer*)realloc(text_array, line_count * sizeof(TextContainer));
    initTextContainer(&text_array[line_count - 1]);
    TextContainer* line = &text_array[line_count - 1];

    int append_length = strlen(text_to_append);
    if (line->current_size + append_length + 1 > line->capacity) {
        resizeTextContainer(line, line->current_size + append_length + 1);
    }
    strcat(line->buffer, text_to_append);
    line->current_size += append_length;
}

void saveToFile(char* filename) {
    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        printf(">Unable to open file for writing.\n");
        return;
    }
    for (int i = 0; i < line_count; i++) {
        fprintf(file, "%s\n", text_array[i].buffer);
    }
    fclose(file);
    printf(">Text has been saved successfully");
}

void loadFromFile(char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf(">Unable to open file for reading.\n");
        return;
    }
    char buffer[MAX_LINE_LENGTH];
    line_count = 0;
    while (fgets(buffer, MAX_LINE_LENGTH, file) != NULL) {
        buffer[strcspn(buffer, "\n")] = '\0'; // Remove newline character
        line_count++;
        text_array = (TextContainer*)realloc(text_array, line_count * sizeof(TextContainer));
        initTextContainer(&text_array[line_count - 1]);
        int len = strlen(buffer);
        if (text_array[line_count - 1].capacity < len + 1) {
            resizeTextContainer(&text_array[line_count - 1], len + 1);
        }
        strcpy(text_array[line_count - 1].buffer, buffer);
        text_array[line_count - 1].current_size = len;
    }
    fclose(file);
    printf("Text has been loaded successfully");
}

void printText() {
    if (line_count == 0) {
        printf("Text container is empty.\n");
        return;
    }
    printf("Current text:\n>");
    for (int i = 0; i < line_count; i++) {
        printf("%s\n", text_array[i].buffer);
    }
}

void insertText(int line, int index, char* text_to_insert) {
    if (line >= line_count || line < 0) {
        printf("Error: Invalid line number.\n");
        return;
    }
    TextContainer* line_text = &text_array[line];
    int insert_length = strlen(text_to_insert);
    if (index > line_text->current_size || index < 0) {
        printf("Error: Invalid index.\n");
        return;
    }
    if (line_text->current_size + insert_length + 1 > line_text->capacity) {
        resizeTextContainer(line_text, line_text->current_size + insert_length + 1);
    }
    memmove(line_text->buffer + index + insert_length, line_text->buffer + index, line_text->current_size - index + 1);
    memcpy(line_text->buffer + index, text_to_insert, insert_length);
    line_text->current_size += insert_length;
}

void search_word(char* word) {
    for (int i = 0; i < line_count; i++) {
        char* found = strstr(text_array[i].buffer, word);
        if (found) {
            int word_index = (int)(found - text_array[i].buffer);
            printf("Found '%s' at line %d, index %d\n", word, i, word_index);
            return;
        }
    }
    printf("Word '%s' not found.\n", word);
}

void freeMemory() {
    for (int i = 0; i < line_count; i++) {
        freeTextContainer(&text_array[i]);
    }
    free(text_array);
}

void handleСommand(int command) {
    char input[256];
    if (command == 1) {
        printf("Enter text to append: ");
        fgets(input, sizeof(input), stdin);
        input[strcspn(input, "\n")] = '\0'; // Remove newline character
        appendText(input);
    }
    else if (command == 2) {
        line_count++;
        text_array = (TextContainer*)realloc(text_array, line_count * sizeof(TextContainer));
        initTextContainer(&text_array[line_count - 1]);
    }
    else if (command == 3) {
        printf("Enter filename to save: ");
        fgets(input, sizeof(input), stdin);
        input[strcspn(input, "\n")] = '\0'; // Remove newline character
        saveToFile(input);
    } else if (command == 4) {
        printf("Enter filename to load: ");
        fgets(input, sizeof(input), stdin);
        input[strcspn(input, "\n")] = '\0'; // Remove newline character
        loadFromFile(input);
    }
    else if (command == 5) {
        printText();
    } else if (command == 6) {
        printf("Enter line number: ");
        int line;
        scanf("%d", &line);
        printf("Enter index: ");
        int index;
        scanf("%d", &index);
        getchar(); // Clear the newline character from the buffer
        printf("Enter text to insert: ");
        fgets(input, sizeof(input), stdin);
        input[strcspn(input, "\n")] = '\0'; // Remove newline character
        insertText(line, index, input);
    } else if (command == 7) {
        printf("Enter word to search: ");
        fgets(input, sizeof(input), stdin);
        input[strcspn(input, "\n")] = '\0'; // Remove newline character
        search_word(input);
    }
    else {
        printf("The command is not implemented.\n");
    }
}


int main() {

    text_array = (TextContainer*)malloc(INITIAL_CAPACITY * sizeof(TextContainer));
    if (text_array == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }

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