// Maryam Saeed 23121354
// Rohma Rehman 23845362
// mac or linux ?????

//BISMILLAH

// Include necessary header files
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

//system dependent 
#ifdef _WIN32
#include <io.h>
#define access _access

#else
#include <unistd.h>
#endif

#define access _access      //for the <io.h> header file ,, instead of <unistd.h>

//key:
//vm = virtual memory
//struct = structure
//defn = definition/ define
//funct = function


// Define given constants 
#define NUM_PROCESSES 4      // Total number of processes
#define PAGES_PER_PROCESS 4  // Pages per process
#define RAM_SIZE 16          // Total RAM size (array length)
#define VM_SIZE 32           // Total Virtual Memory size


//struct to represent page in memory
typedef struct{
      int process_id;
      int page_num;
      int last_accessed;
}memory;


//defn ram and vm arrays
memory    *ram[RAM_SIZE];    //array representing ram (16 locations)
memory    *vm[VM_SIZE];      //array representing vm (32 locations)
int       page_table[4][4];  //page table for 4 processes with 4 pages each

//global defn of time step
int time_step = 0;       // Simulation time 
int ram_page_count = 0;  // track no. of pages in RAM

//defn array to hold process_id integers from in.txt 
int input[100];
int input_count = 0;    //initialist input count      


//Mars
// Function to print usage error information
void print_usage(const char *program_name) {
    fprintf(stderr, "Usage: %s in.txt out.txt\n", program_name);
    exit(1);
}
// Function to report errors with line numbers
void report_error(const char *message, int line_number) {
    fprintf(stderr, "! Error on line %d: %s\n", line_number, message);
    exit(1);
}


// Function to validate the contents of the in.txt file 
    // Expectss a single line of integers separated by blanks (aka spaces ' ')
void validate_in_contents(FILE *in_file) {
    char line[1000];
    int line_number = 0;
    int line_count = 0;


    while (fgets(line, sizeof(line), in_file)) {
        line_number++;
        
        // Trim leading whitespace
        char *trimmed = line;
        while (*trimmed && isspace(*trimmed)) {
            trimmed++;
        }
        if (*trimmed == '\0') {     //skip over empty line
            continue;
        }

        //in.txt should contain a single line of integers separated by blanks
        for (int i = 0 ; i < strlen(trimmed) ; i++) {
            if (i%2 == 0) {
                //integer
                if (isdigit(trimmed[i])) {
                    int process_id = atoi(&trimmed[i]);
                    
                    // process_id is a number between 0 and 3
                    if (process_id==0 || process_id==1 || process_id==2 || process_id==3) {
                        input[input_count] = process_id;
                        input_count++;
                    }
                    else {  //wrong digit
                        report_error("INVALID PROCESS ID", line_number);
                    }
                }
                else {  //not a digit
                    report_error("INVALID PROCESS ID", line_number);
                }
            }

            //white space
            else {
                if (!isspace(trimmed[i])) {
                    report_error("INCORRECT SYNTAX", line_number);        
                }
                else if (trimmed[i] == '\0') {
                    line_count++;
                    continue;
                }
                else if (trimmed[i] != ' ') {
                    report_error("INCORRECT SYNTAX", line_number);
                }
            }
        }

        //more than one line of process ids
        if (line_count == 2 ) {
            report_error("MORE THAN ONE LINE", line_number);
        }
    }
}


//HELPER FUNCT for funct 1 helps create and initialise page
memory *helper(int process_id, int page_num){
    memory *page_info = (memory *)malloc(sizeof(memory)); //memory allocaton for a new page

    //successful allocation checking
    if (page_info == NULL){
        printf("Memory allocation failed.\n");

        exit(1); //if allocation fails
    }
}


//Rohma
//funct 1: initialise vm for 4 processes, each with 4 pages
    /*
    - set up vm for system by initialising memory pages for each process and setting up page table to indicate all pages are initially in disk (vm)
    - loop over each process
        - for each process loop over each of their 4 pages 
    - allocate memory for each page
        - using malloc?
    - initialise struct fields
    - store page in vm
    - update page table
    */
void init_vm(){
   int vm_pg_index = 0;

   for (int process = 0; process<4; process++) {
    for (int page = 0; page<4; page++) {
        memory *page_info = (memory *)malloc(sizeof(memory));

        if (page_info == NULL){
            printf("Memory allocation failed\n");
            return;
        }

        page_info ->last_accessed = 0;  //initialise last accessed time to 0

        vm[vm_pg_index] = page_info;
        vm[vm_pg_index + 1] = page_info;

        vm_pg_index += 2;

        page_table[process][page] = 1;
    }
   }
}


//Mars
//funct 2: bring pages from vm to ram when requested
    /*
    - bring a requested page from the vm to the ram
    - if the ram is full apply LRU algorithm to evict least recently used page for the same process
    - find next page to bring into ram
    - check for free space in ram
    - handle case when ram is full
    - bring page into ram
    - update page table and last accessed time
    */
void page_to_ram(int process_id, int current_time){
    //insert code
}


//Rohma
//funct 3: read process requests from input file and simulate page requests
    /*
    - simulation of paging process by reading input file, containing list of process ids
    - each process id correpsonds to a request to bring a page into the ram
    - open input file
    - read process ids from file
    - call funct 2
    - incrememnt simulation time
    - close input file
    */
void simulate(const char *input_file){
   FILE *file = fopen(input_file,"r");
   if (!file) {
    printf("Error in opening file %s\n", input_file);
    return;
   }

   int process_id, current_time = 0;

}

//Mars
//funct 4: print page tables and ram content to output file after simulation
    /*
    - outputs results of simulation to specified output file, incl final state of each process's page table and the contents of its ram
    - open output file
    - print page tables for all processes
    - print contents of ram
    - close output file
    */
void output_simulate(const char *output_file){
    //insert code
}


//Mars & Rohma xoxo
//main funct
    /*
    - check command line args
    - initialise virtual memory
    - simulate paging process
    - output final results
    - return success
    */
int main(int argc, char *argv[]){
    // Check for correct usage      ...ie. simulation in.txt out.txt
    if (argc != 3) {
        print_usage(argv[0]);
    }
    
    // Validate input file extension (in.txt)
    char *in_txt = argv[1];
    char *in_extension = strrchr(in_txt, '.');
    if (!in_extension || strcmp(in_extension, ".txt") != 0) {
        fprintf(stderr, "! Error: Input file must have .txt extension\n");
        exit(1);
    }

    // Open input file
    FILE *in_file = fopen(in_txt, "r");
    if (!in_file) {
        perror("Error opening input file");
        exit(1);
    }

    // Validate in.txt file contents 
    validate_in_contents(in_file);


    // Validate input file extension (in.txt)
    char *out_txt = argv[2];
    char *out_extension = strrchr(out_txt, '.');
    if (!out_extension || strcmp(out_extension, ".txt") != 0) {
        fprintf(stderr, "! Error: Output file must have .txt extension\n");
        exit(1);
    }

    // Open output file
    FILE *out_file = fopen(out_txt, "w");
    if (!out_file) {
        perror("Error opening output file");
        exit(1);
    }


    //
   init_vm();


    //close files after simulation running completed
   fclose(in_file);
   fclose(out_file);

}