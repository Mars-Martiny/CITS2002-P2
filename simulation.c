// Maryam Saeed 23121354
// Rohma Rehman 23845362
// mac or linux ?????

// Include necessary header files
#include <stdio.h>
#include <stdlib.h>

//key:
//vm = virtual memory
//struct = structure
//defn = definition/ define
//funct = function


//struct to represent page in memory
struct memory {
      int process_id;
      int page_num;
      int last_accessed;
};


//defn ram and vm arrays
struct memory    *ram[16]; //array representing ram (16 locations)
struct memory     *vm[32]; //array representing vm (32 locations)
int      page_table[4][4]; //page table for 4 processes with 4 pages


//Mars
//error things n whatnot
//

//Rohma
//funct 1: initialise vm for 4 processes, each with 4 pages
void init_vm(){
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
   int vm_pg_index = 0;

   for (int process = 0; process<4; process++) {
    for (int page = 0; page<4; page++) {
        struct memory *page_info = (struct memory *)malloc(sizeof(struct memory));

        if (page_info == NULL){
            printf("Memory allocation failed\n");
            return;
        }

        page_info ->process_id = process_id;
        page_info ->page_num = page_num;
        page_info ->last_accessed = 0;

        vm[vm_pg_index] = page_info;
        vm[vm_pg_index + 1] = page_info;

        vm_pg_index += 2;

        page_table[process][page] = 1;
    }
   }
}

//Mars
//funct 2: bring pages from vm to ram when requested
void page_to_ram(int process_id, int current_time){
    /*
    - bring a requested page from the vm to the ram
    - if the ram is full apply LRU algorithm to evict least recently used page for the same process
    - find next page to bring into ram
    - check for free space in ram
    - handle case when ram is full
    - bring page into ram
    - update page table and last accessed time
    */
}

//Rohma
//funct 3: read process requests from input file and simulate page requests
void simulate(const char *input_file){
    /*
    - simulation of paging process by reading input file, containing list of process ids
    - each process id correpsonds to a request to bring a page into the ram
    - open input file
    - read process ids from file
    - call funct 2
    - incrememnt simulation time
    - close input file
    */
   FILE *file = fopen(input_file,"r");
   if (!file) {
    printf("Error in opening file %s\n", input_file);
    return;
   }

   int process_id, current_time = 0;
   
}

//Mars
//funct 4: print page tables and ram content to output file after simulation
void output_simulate(const char *output_file){
    /*
    - outputs results of simulation to specified output file, incl final state of each process's page table and the contents of its ram
    - open output file
    - print page tables for all processes
    - print contents of ram
    - close output file
    */
}


//Rohma
//main funct
int main(int argc, char *argv[]){
    /*
    - check command line args
    - initialise virtual memory
    - simulate paging process
    - output final results
    - return success
    */
   init_vm();
}