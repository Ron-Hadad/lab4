#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

// Struct for program state
typedef struct {
  char debug_mode;
  char file_name[128];
  int unit_size;
  unsigned char mem_buf[10000];
  size_t mem_count;
  char display_flag;
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

state program_state = {.unit_size = 1}; //Initializing the program_state global variable 

int main() {
    while (true) {
        // Print debug information
        if (program_state.debug_mode) {
            fprintf(stderr, "unit_size: %d, file_name: %s, mem_count: %zu\n", 
                program_state.unit_size, program_state.file_name, program_state.mem_count);
        }

        // Print menu
        int optionCount = 0;
        printf("\nChoose action:\n");
        while (menu[optionCount].name != NULL) {
            printf("%d-%s\n", optionCount, menu[optionCount].name);
            optionCount++;
        }

        // Obtain user choice
        int choice;
        printf("> ");
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

        setbuf(stdin, NULL); // Discard the "\n" character
        
        // Execute chosen function
        menu[choice].MenuFunction(&program_state);
    }
    return 0;
}


    //menu functions:

    // choice = 0
    void toggleDebugMode(state* s) {
        if (!s->debug_mode) {
            s->debug_mode = 1;
            fprintf(stderr, "Debug flag now on\n");
        }
        else {
            s->debug_mode = 0;
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
        unsigned int location;
        unsigned int length;

        printf("Please enter <location> (hexadecimal) <length> (decimal):\n");
        if (fgets(input, sizeof(input), stdin) == NULL) {
            fprintf(stderr, "Error: Failed to read input.\n");
            fclose(file);
            return;
        }

        if (sscanf(input, "%x %u", &location, &length) != 2) {
            fprintf(stderr, "Error: Invalid input format.\n");
            fclose(file);
            return;
        }
      
        // if debug mode is on:
        if (s->debug_mode) {
            fprintf(stderr, "Debug: file_name set to '%s', location: 0x%x, length: %u\n", s->file_name, location, length);
        }

        fseek(file, location * s->unit_size, SEEK_SET);
        size_t x = fread(s->mem_buf, s->unit_size, length, file);
        // Update mem_count
        s->mem_count += x;
        fclose(file);
        printf("Loaded %zu units into memory\n", x);
    }

    // choice = 4
    void toggleDisplayMode(state* s){
        if (s->display_flag) {
            printf("Display flag now off, decimal representation\n");
            s->display_flag = 0; // Turn display flag off
        }
        else {
            printf("Display flag now on, hexadecimal representation\n");
            s->display_flag = 1; // Turn display flag on
        }
}

    // choice = 5
    void memoryDisplay(state* s){
        static char* hex_formats[] = {"%hhX\n", "%hX\n", "No such unit", "%X\n"};
        static char* dec_formats[] = {"%hhd\n", "%hd\n", "No such unit", "%d\n"};
        char input[256];
        int addr = 0;
        int length = 0;
        FILE* file = NULL;
        void *start_addr;

        printf("Enter address and length:\n> ");
        if(fgets(input, 256, stdin) == NULL){
            fprintf(stderr, "Error: Failed to read input.\n");
            return;
        }
        if (sscanf(input, "%x %d", &addr, &length) != 2) {
            fprintf(stderr, "Error: Invalid input format.\n");
            return;
        }
        if(addr == 0)
            start_addr = &(s->mem_buf);
        else
            start_addr = &addr;

        if (s->file_name[0] == '\0') {
            fprintf(stderr, "Error: No file name specified.\n");
            return;
        }

        file = fopen(s->file_name, "rb");
        if (file == NULL) {
            fprintf(stderr, "Error: Failed to open file '%s'.\n", s->file_name);
            return;
        }
        
        if(s->display_flag){
            printf("Hexadecimal\n");
            printf("===========\n");
        }
        else{
            printf("Decimal\n");
            printf("=======\n");
        }
        for (int i = 0; i < length; i++) {
            if(s->display_flag)
                printf(hex_formats[s->unit_size -1], *((int*)start_addr));
            else
                printf(dec_formats[s->unit_size -1], *((int*)start_addr));
            start_addr += s->unit_size;
        }
        fclose(file);
    }

    // choice = 6
    void saveIntoFile(state* s){
        if (s->file_name[0] == '\0') {
            fprintf(stderr, "Error: File name is not set.\n");
            return;
        }
        char input[256];
        unsigned int source_addr;
        unsigned int target_loc;
        unsigned int length;
        void *start_ptr;
        printf("Please enter <source-address> <target-location> <length>\n");
        if(fgets(input, 256, stdin) == NULL){
            fprintf(stderr, "Error: Failed to read input.\n");
            return;
        }
        if (sscanf(input, "%x %x %d", &source_addr, &target_loc, &length) != 3) {
            fprintf(stderr, "Error: Invalid input format.\n");
            return;
        }
        int file = open(s->file_name, O_RDWR);
        if (file == -1) {
            fprintf(stderr, "Error: Failed to open file '%s' for writing.\n", s->file_name);
            return;
        }
        // Get the file size
        off_t file_size = lseek(file, 0, SEEK_END);
         if (file_size == -1) {
            fprintf(stderr, "Error: Failed to get file size.\n");
            close(file);
            return;
        }
        if (target_loc >= file_size) {
            fprintf(stderr, "Error: Target location exceeds file size. No data will be copied.\n");
            close(file);
            return;
        }
        if(source_addr == 0)
            start_ptr = &(s->mem_buf);
        else
            start_ptr = &source_addr;
        
        // Seek to the target location in the file
        if (lseek(file, target_loc, SEEK_SET) == -1) {
            fprintf(stderr, "Error: Failed to seek to target location in the file.\n");
            close(file);
            return;
        }
        // Write the memory content to the file
        ssize_t bytes_written = write(file, start_ptr, s->unit_size * length);
        if (bytes_written == -1) {
            fprintf(stderr, "Error: Failed to write data to file.\n");
        } 
        else {
            printf("Data written to file successfully.\n");
        }
        
        close(file);
    }

    // choice = 7
    void memoryModify(state* s){
        unsigned int location;
        unsigned int val;
        char input[256];
        printf("Please enter <location> <val>\n");
        if(fgets(input, 256, stdin) == NULL){
            fprintf(stderr, "Error: Failed to read input.\n");
            return;
        }
        if (sscanf(input, "%x %x", &location, &val) != 2) {
            fprintf(stderr, "Error: Invalid input format.\n");
            return;
        }

        if (s->debug_mode) {
            printf("Location: 0x%x\n", location);
            printf("Val: 0x%x\n", val);
        }
        if (location + s->unit_size > 10000) {
            fprintf(stderr, "Error: Invalid memory location.\n");
            return;
        }
        // Modify the memory buffer
        unsigned int* memPtr = (unsigned int*)(s->mem_buf + location);
        *memPtr = val;

        printf("Memory modified successfully.\n");
    }

    // choice = 8
    void quit(state* s) {
        if (s->debug_mode)
            fprintf(stderr, "Debug: quitting\n");
        exit(0);
    }

