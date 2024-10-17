// Maryam Saeed 23121354
// Rohma Rehman 23845362
// mac or linux ?????

// بِسْمِ اللهِ الرَّحْمٰنِ الرَّحِيْمِ 

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
//init = initialise


// Define given constants 
#define NUM_PROCESSES 4      // Total number of processes
#define PAGES_PER_PROCESS 4  // Pages per process
#define RAM_SIZE 16          // Total RAM size (array length)
#define RAM_PAGES 8          // Max number of pages RAM can hold
#define VM_SIZE 32           // Total Virtual Memory size



//struct to represent page in memory
//typedef used to give shorthand alias to struct (memory)
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
int input_count = 0;     //counts number of process ids listed in in.txt      


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
                if (!isspace(trimmed[i])) { //not whitespace
                    report_error("INCORRECT SYNTAX", line_number);        
                }
                //line-end indicator
                else if (trimmed[i] == '\0' || trimmed[i] == '\n') {    
                    line_count++;
                    continue;
                }
                else if (trimmed[i] != ' ') {//wrong type of whitespace
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
// - allocate memory for each page
// - using malloc?
// - initialise struct fields
memory *creator(int process_id, int page_num){
    memory *page_info = (memory *)malloc(sizeof(memory)); //memory allocaton for a new page

    //successful allocation checking
    if (page_info == NULL){
        fprintf(stderr, "Memory allocation failed.\n");
        exit(1); //if allocation fails
    }

    //init fields of struct
    page_info -> process_id = process_id;
    page_info -> page_num = page_num;
    page_info -> last_accessed = 0; //set last accessed as 0 - not accessed yet

    return page_info;
}


//Rohma
//funct 1: initialise vm for 4 processes, each with 4 pages
    /*
    - set up vm for system by initialising memory pages for each process and setting up page table to indicate all pages are initially in disk (vm)
    - loop over each process
        - for each process loop over each of their 4 pages 
    - store page in vm
    - update page table
    */
void init_vm(){
   int vm_pg_index = 0;
   
   for (int process = 0; process<4; process++) {
    for (int page = 0; page<4; page++) {
        memory *page_info = creator(process, page);

        vm[vm_pg_index] = page_info;
        vm[vm_pg_index + 1] = page_info;

        vm_pg_index += 2; //move to next 2 slots

        page_table[process][page] = 99;
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

// Function to find and replace a page in RAM using LRU
void page_to_ram(memory *page) {
    // Update the page's third value with the global time (assuming third value is `last_used_time`)
    page->last_accessed = time_step;

    // If there is room in RAM, just add the page
    if (ram_page_count < RAM_PAGES) {
        ram[2 * ram_page_count] = page;
        ram[2 * ram_page_count + 1] = page;     //page in 2 contiguous locations
        printf("Loaded page (P%d, %d, %d) into RAM at index %d and %d\n", page->process_id, page->page_num, page->last_accessed, 2*ram_page_count, 2*ram_page_count + 1);
        ram_page_count++;
    } 

    // If RAM is full, remove local LRU if exists otherwise remove global LRU in RAM
    else {
        //position of pages to evict RAM
        int local_LRU_index = 0;
        int global_LRU_index = 0;
        
        //if this value is non-zero then a local LRU exists
        int LRU_index = 0; 
        
        // check for local Least Recently Used (LRU)
        for (int i = 0; i < RAM_PAGES ; i++) {
            if (ram[2*i]->process_id == page->process_id) {
                //check  if LRU index is NOT local
                if (ram[local_LRU_index]->process_id != page->process_id) {
                    local_LRU_index = 2*i;          //assign current index which is local
                }
                else {
                    if (ram[2*i]->last_accessed < ram[local_LRU_index]->last_accessed) {
                        local_LRU_index = 2*i;      //only update index if older (both are local)
                    }
                }
                LRU_index++;      //non-zero if any local pages in RAM
            }

            else if (ram[2*i]->last_accessed < ram[global_LRU_index]->last_accessed) {
                global_LRU_index = 2*i;
            }
        }
        
        
        // LRU eviction: evict the last accessed page 
        if (LRU_index != 0) {
            LRU_index = local_LRU_index; // local LRU eviction
        }
        else{
            LRU_index = global_LRU_index; // global LRU eviction
        }
        
        memory *evicted_page = ram[2 * LRU_index];
        printf("Evicting page (P%d, %d, %d) from RAM (position %d).\n", evicted_page->process_id, evicted_page->page_num, evicted_page->last_accessed, 2*LRU_index);

        // Shift all other pages after the evicted page forward
        for (int i = LRU_index + 1; i < RAM_PAGES; i++) {
            ram[2*(i - 1)] = ram[2*i];
            ram[2*(i - 1) + 1] = ram[2*i + 1];      //shift both locations of page 
        }

        // Load the new page into the last slot of RAM
        ram[2*(RAM_PAGES - 1)] = page;
        ram[2*(RAM_PAGES - 1) +1] = page;
        printf("Loaded page (P%d, %d, %d) into RAM at index %d and %d\n", page->process_id, page->page_num, page->last_accessed, 2*(RAM_PAGES - 1), 2*(RAM_PAGES - 1) + 1);
    }

}


//Rohma
//funct 3: read process requests from input file and simulate page requests
    /*
    - simulation of paging process by reading input array (global array), containing list of process ids
    - each process id corresponds to a request to bring a page into the ram
    - 
    - call funct 2
    - incrememnt simulation time
    - close input file
    */
   //add check if process id and page number exist alr within ram, dont call mars functions, just increment time by one
void simulate(const char *input_file){
    
    int page_num = 0; //initial page num

    //loop through all process ids in input array
    for (int i = 0; i < input_count ; i++){
        int process_id = input[i]; //get p id from input array
        memory *page = vm[process_id * PAGES_PER_PROCESS + page_num]; // fetch page from vm

        int found_in_ram = 0; //flag checking if page is alr in ram
        int ram_index = -1;  // variable to store ram index if page is found
        // set to -1 to represent a situation where the page has not been found in ram

        //searching ram to check if page is alr there
        for (int j = 0; j < ram_page_count*2; j += 2){ //check each page (eac page = 2 locations)
            if (ram[j] != NULL && ram[j]-> process_id == process_id && ram[j] ->page_num == page_num) {
                ram_index = j; //store index where page is found in ram 
                found_in_ram = 1; //page found in ram
                break;
            }
        }

        //if found in ram, update last accessed time
        if (found_in_ram){
            ram[ram_index]->last_accessed = time_step; //update last accessed time
            printf("\n", process_id, page_num, ram_index, time_step);
        }
        //if page is not in ram, bring it into ram
        else {
            printf("\n", process_id, page_num);
            page_to_ram(page);
        }

        //increment page number and wrap around after 3 (0-3 cycle)
        page_num = (page_num + 1) % PAGES_PER_PROCESS;

        time_step++;
    }
}

//Rohma
//funct 4: print page tables and ram content to output file after simulation
    /*
    - outputs results of simulation to specified output file, incl final state of each process's page table and the contents of its ram
    - open output file
    - print page tables for all processes
    - print contents of ram
    - close output file
    */

void output_simulate(const char *output_file){
    // Validate output file extension (out.txt)
    char *out_txt = output_file;
    char *out_extension = strrchr(out_txt, '.');
    if (!out_extension || strcmp(out_extension, ".txt") != 0) {
        fprintf(stderr, "Error: Output file must have .txt extension\n");
        exit(1);
    }

    FILE *out_file = fopen(out_txt, "w");
    if (!out_file) {
        perror("Error opening output file");
        exit(1);
    }

    //output the page tables for all processes
    for (int process = 0; process < NUM_PROCESSES; process++){
        for (int page = 0; page < PAGES_PER_PROCESS; page++){
            fprintf(out_file, "%d", page_table[process][page]);  // Print page table
            if (page < PAGES_PER_PROCESS - 1) {
                fprintf(out_file, ","); //add comma betw page numbers
            }
        }
        fprintf(out_file, "\n"); //new line after each process page table
    }
    
    //output contents of ram
    for (int i = 0; i < RAM_SIZE; i++) {
        if (ram[i] != NULL) {
            fprintf(out_file, "%d,%d,%d", ram[i]->process_id, ram[i]->page_num, ram[i]->last_accessed);
        } else {
            fprintf(out_file, "-,-,-");  // Empty ram slots
        }
        if (i < RAM_SIZE - 1) {
            fprintf(out_file, ";"); //separate ram entries w/ semicolons
        }
    }
    fprintf(out_file, "\n"); //new line after ram contents

    //close file after writing is completed
    fclose(out_file);
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
        fprintf(stderr, "Error: Input file must have .txt extension\n");
        exit(1);
    }
    // Open input file
    FILE *in_file = fopen(in_txt, "r");
    if (!in_file) {
        perror("Error opening input file");
        exit(1);
    }

    // Validate in.txt file contents & puts all info inside globally defined array (input)
    validate_in_contents(in_file); 
    fclose(in_file); 


//    init_vm();

//    simulate();

//    output_simulate();

}