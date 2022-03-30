/**
 *
 * Steven Vidal
 * header file holding data struct, flag struct and all
 * of the function declerations
 *

 *
 */
#ifndef PROJECT_1_TUPS_HEADER_H
#define PROJECT_1_TUPS_HEADER_H

/*
 * returning a struct that contains the flags for what to print out
 */

/** int p_flag,s_flag,U_flag,S_flag,v_flag,c_flag;  // 0 is specified process, 1 is all processes **/

typedef struct FLAGS {
    int p_flag;
    int s_flag;
    int u_flag;
    int S_flag;
    int v_flag;
    int c_flag;
    int PID;
} FLAGS ;

typedef struct DATA {

    int pid;
    char * s_state;
    int u_time;
    char * s_time;
    char * v_mem;
    char * cmd_line;

} DATA ;
DATA * stat_parse(int PID);
DATA * d_malloc(int PID);
FLAGS * cmdline(int argc, char*argv[]);
unsigned int user_id (int pid);
int processlist(FLAGS * flags);
void print_data(DATA * data, FLAGS * flags);


#endif //PROJECT_1_TUPS_HEADER_H
