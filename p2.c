// Maryam Saeed 23121354
// Rohma Rehman 23845362

// Include necessary header files
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>

// Define constants for maximum sizes
#define MAX_LINE_LENGTH 1000       // Max length of a line in the ML file
#define MAX_IDENTIFIERS 50         // Max number of identifiers (variables and functions)
#define MAX_IDENTIFIER_LENGTH 13   // Max length of an identifier (12 chars+null terminator)

typedef struct {
    int line_number;
    int line_type;                                      // 1->comment; 2->function; 3->variable; 0->other

    char name[MAX_IDENTIFIER_LENGTH];                   // name of function or variable
    
    float variable_value;                               // only used if id is a variable

    int in_function; 
    char parent_function[MAX_IDENTIFIER_LENGTH];       // only used if in function; name of parent function
} Line;

Line lines[200];

// Prints the correct usage of the program to stderr if invoked incorrectly
void print_usage(const char *program_name) {
    fprintf(stderr, "Usage: %s <ml_file> [arg0 arg1 ...]\n", program_name);
    exit(1);
}

// Prints an error message along with the line number where the error occurred
void report_error(const char *message, int line_number) {
    fprintf(stderr, "! Error on line %d: %s\n", line_number, message);
    exit(1);
}

// https://stackoverflow.com/a/15515276
int starts_with(char *a, char *b) {
   if (strncmp(a, b, strlen(b)) == 0) {
        return 1;
   }
   return 0;
}

int is_variable(char *string) {
    for (int i = 0; i < strlen(string); i++) {
        if (string[i] == '<') {
            return 1;
        }
    }
    return 0;
}

void process_ml_lines(FILE *ml_file) {
    int line_num = 0;
    char line[MAX_LINE_LENGTH];
    char last_function[MAX_IDENTIFIER_LENGTH];
    last_function[0] = '\0';

    while (fgets(line, sizeof(line), ml_file)) {
        printf("%s\n", line);

        if (strlen(line) > MAX_LINE_LENGTH) {
            report_error("LINE TOO LONG", line_num);
        }

        Line new_line = {
                .line_number = line_num,
                .line_type = -1,
                .in_function = 0
            };

        if (line[0] == '\t') {
            new_line.in_function = 1;
            if (last_function[0] == '\0') {
                report_error("INCORRECT INDENTATION; NO FUNCTION DEFINED", line_num);
            }
            strcpy(new_line.parent_function, last_function);
        }

        if (line[0] == '#') {
            new_line.line_type = 1;
            continue;
        }
        else if (starts_with(line, "function")) {
            new_line.line_type = 2;
            
            char buffer[strlen("function")];
            char temp_name[MAX_LINE_LENGTH];

            sscanf(line, "%s %s", buffer, temp_name);

            if (strlen(temp_name) > MAX_IDENTIFIER_LENGTH) {
                report_error("IDENTIFIER LENGTH TOO LONG", new_line.line_number);
            }
            strcpy(new_line.name, temp_name);
            strcpy(last_function, new_line.name);
            // printf("F:%s\n", new_line.name);
        }
        else if (is_variable(line)) {
            // will possibly need to trim whitespace beforehand
            new_line.line_type = 3;
            
            char temp_name[MAX_LINE_LENGTH];
            sscanf(line, "%s <- %f", temp_name, &new_line.variable_value);
            
            if (strlen(temp_name) > MAX_IDENTIFIER_LENGTH) {
                report_error("IDENTIFIER LENGTH TOO LONG", new_line.line_number);
            }
            strcpy(new_line.name, temp_name);
        }

        lines[line_num] = new_line;
        line_num++;
    }
}

void process_c_lines(FILE *c_file) {
    fprintf(c_file, "#include <stdio.h>\n");
    fprintf(c_file, "#include <stdlib.h>\n");
    fprintf(c_file, "#include <math.h>\n");
    
    for (int i = 0; i < sizeof(lines) / sizeof(lines[0]); i++) {

    }
}

int main(int argc, char *argv[]) {
    char *ml_filename = argv[1];
    FILE *ml_file = fopen(ml_filename, "r");
    if (!ml_file) {
        perror("Error opening input file");
        exit(1);
    }

    char c_filename[20];
    snprintf(c_filename, sizeof(c_filename), "ml-%d.c", getpid());  // Create a unique filename using process ID
    FILE *c_file = fopen(c_filename, "wr");
    if (!c_file) {
        perror("Error opening input file");
        exit(1);
    }

    process_ml_lines(ml_file);
    process_c_lines(c_file);

    fclose(ml_file);
    fclose(c_file);

    remove(c_filename);

    return 0;
}


