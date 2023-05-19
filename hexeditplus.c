#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
# include <string.h>

// Struct for program state
typedef struct {
  char debug_mode;
  char file_name[128];
  int unit_size;
  unsigned char mem_buf[10000];
  size_t mem_count;
  /*
   .
   .
   Any additional fields you deem necessary
  */
} state;

// Function prototypes
void toggleDebugMode(state* s);
void setFileName(state* s);
void setUnitSize(state* s);
void loadIntoMemory(state* s);
void toggleDisplayMode(state* s);
void memoryDisplay(state* s);
void saveIntoFile(state* s);
void memoryModify(state* s);
void quit(state* s);

// Struct for menu options
struct fun_desc {
    char* name;
    void (*MenuFunction)(state*);
};

// Menu options and corresponding function pointers
struct fun_desc menu[] = {
    {"Toggle Debug Mode", toggleDebugMode},
    {"Set File Name", setFileName},
    {"Set Unit Size", setUnitSize},
    {"Load Into Memory", loadIntoMemory},
    {"Toggle Display Mode", toggleDisplayMode},
    {"Memory Display", memoryDisplay},
    {"Save Into File", saveIntoFile},
    {"Memory Modify", memoryModify},
    {"Quit", quit},
    {NULL, NULL}
};

state program_state = {0}; //Initializing the program_state variable with debug mode turned off.

int main() {
    while (true) {
        // Print debug information
        if (program_state.debug_mode) {
            fprintf(stderr, "unit_size: %d, file_name: %s, mem_count: %zu\n", 
                program_state.unit_size, program_state.file_name, program_state.mem_count);
        }

        // Print menu
        int optionCount = 0;
        printf("Choose action:\n");
        while (menu[optionCount].name != NULL) {
            printf("%d-%s\n", optionCount, menu[optionCount].name);
            optionCount++;
        }

        // Obtain user choice
        int choice;
        scanf("%d", &choice);

        // Check if choice is "quit"
        if (choice == 8) {
            quit(&program_state);
            break;
        }

        // Validate choice
        if (choice < 0 || choice >= optionCount) {
            printf("Invalid choice. Please try again.\n");
            continue;
        }

        // Execute chosen function
        int c;
        while ((c = getchar()) != '\n' && c != EOF) {}
        // Discard characters until a newline or EOF is encountered
        menu[choice].MenuFunction(&program_state);
    }
    return 0;
}


    //menu functions:

    // choice = 0
    void toggleDebugMode(state* s) {
        if (!s->debug_mode) {
            s->debug_mode = true;
            fprintf(stderr, "Debug flag now on\n");
        }
        else {
            s->debug_mode = false;
            fprintf(stderr, "Debug flag now off\n");
        }
    }

    // choice = 1
    void setFileName(state* s) {
        printf("Enter file name: ");
        scanf("%127s", s->file_name); //127 is the maximum number of characters to read from the input

        if (s->debug_mode) { //if debug is on
            fprintf(stderr, "Debug: file name set to '%s'\n", s->file_name);

        }
    }

    // choice = 2
    void setUnitSize(state* s) {
        int newSize;
        printf("Enter unit size (1, 2, or 4): ");
        scanf("%d", &newSize);
        if (newSize == 1 || newSize == 2 || newSize == 4) {
            s->unit_size = newSize;
            if (s->debug_mode)
                fprintf(stderr, "Debug: set size to %d\n", s->unit_size);
        }
        else {
            printf("Invalid unit size. Size unchanged.\n");
        }
    }

    // choice = 3
    void loadIntoMemory(state* s){
         if (s->file_name[0] == '\0') {
            fprintf(stderr, "Error: No file name specified.\n");
            return;
        }

        FILE* file = fopen(s->file_name, "rb");
        if (file == NULL) {
            fprintf(stderr, "Error: Failed to open file '%s'.\n", s->file_name);
            return;
        }

        char input[100];
        int location;
        int length;

        printf("Please enter <location> (hexadecimal) <length> (decimal):\n");
        if (fgets(input, sizeof(input), stdin) == NULL) {
            fprintf(stderr, "Error: Failed to read input.\n");
            return;
        }

        if (sscanf(input, "%x %u", &location, &length) != 2) {
            fprintf(stderr, "Error: Invalid input format.\n");
            return;
        }
      
        // if debug mode is on:
        if (s->debug_mode) {
            fprintf(stderr, "Debug: file_name set to '%s', location: 0x%x, length: %u\n", s->file_name, location, length);
        }

        fseek(file, location * s->unit_size, SEEK_SET);
        size_t bytesRead = fread(s->mem_buf, s->unit_size, length, file);
        fclose(file);

        printf("Loaded %zu units into memory\n", bytesRead);
    }

    // choice = 4
    void toggleDisplayMode(state* s){
        printf("Not implemented yet..\n");
    }

    // choice = 5
    void memoryDisplay(state* s){
        printf("Not implemented yet..\n");
    }

    // choice = 6
    void saveIntoFile(state* s){
        printf("Not implemented yet..\n");
    }

    // choice = 7
    void memoryModify(state* s){
        printf("Not implemented yet..\n");
    }

    // choice = 8
    void quit(state* s) {
        if (s->debug_mode)
            fprintf(stderr, "Debug: quitting\n");
        exit(0);
    }

