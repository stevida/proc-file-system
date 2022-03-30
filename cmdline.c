/**
 * Steven Vidal
 * .c file to parse the command line arguments
 *
 * HAS MAIN!!!!
 * !!Don't include tups_header.h in the compilation for the makefile
 * makefile info:https://www.cs.colostate.edu/~cs157/LectureMakefile.pdf
 *
 */

#include "tups_header.h"

int main(int argc, char *argv[]){
    FLAGS * f = cmdline(argc,argv);
    /**flags for command line and printing**/

    DATA * d = d_malloc(f->PID);
    /** data for the single process**/

    if(f->p_flag){
        /*** if non zero means there is a specified process **/
        /**check if user process**/
        d = stat_parse(f->PID);
        print_data(d,f);
        return 0;
    }
    else { /** theres a list of processes to return **/
        processlist(f);
    }

    return 0;
}