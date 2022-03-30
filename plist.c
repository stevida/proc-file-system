/**
 *
 *
 *  Steven Vidal
 *  Project 1
 *
 * plist.c holds all the function definitions but statparse as listed in tups_header.h
 *
 *
 */


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <errno.h>
#include <ctype.h>
#include <string.h>
#include <sys/stat.h>

#include "tups_header.h"

/** using this function to check if the userid matches getuid**/

unsigned int user_id (int pid) {
    /** calling this function in processlist to check if the process owned by the user**/
    unsigned int u_id;
    char str[16];
    sprintf(str, "%d", pid);
    /** int to string check printf("pid: %s",str);  **/

    struct stat * sb = malloc(sizeof(struct stat));
    char * buf = (void *) malloc(sizeof(char) * 512);
    strcpy(buf, "/proc/");
    strcat(buf, str);
    strcat(buf,"/stat");
    stat(buf, sb);

    u_id = sb->st_uid;

    return u_id;
}
int processlist(FLAGS * flags){   /** 0 is specified process, 1 is all processes **/
    /**
     *      return PID to main to process
     *
     *
     */

    struct dirent *dirent;
    DIR *dir;
    int pid;

    /** opening proc and error checking **/
    if(!(dir = opendir("/proc"))){
        fprintf(stderr,"%s: couldn't open /proc/%d","TUps",errno);
        perror(NULL);
        exit(EXIT_FAILURE);
    }

    dirent = readdir(dir);

    int pid_bool = 0; // boolean variable to check to see if the PID is in the /proc folder
    while (dirent!=NULL){

        if(isdigit(*dirent->d_name)){ /**reads only digits because /proc contains non pid stuff **/
            pid = (int)strtol(dirent->d_name,NULL,10);
            if(flags->p_flag){
                if(flags->PID == pid){
                    pid_bool = 1;
                }
            }
            unsigned int user;
            user = user_id(pid);
            if (user == getuid()){ /** IF STATEMENT SEPARATING THE USER PROCESSES **/
                /** PASS TO statparser(int pid) in statparser **/
                DATA * data = d_malloc(pid);
                data = stat_parse(pid);
                print_data(data,flags); /** print data for all of the processes **/
                /** perform data delivery **/
            }
        }
        dirent = readdir(dir);
    }
    closedir(dir);

    if(!pid_bool && flags-> p_flag == 1){ /** quits program if given pid does not exist **/
        fprintf(stdout, "ERROR: %d is not real PID\n", flags->PID);
        exit(1);
    }

    return 0;
}



DATA * d_malloc(int PID){ /** function to allocate space for the struct holding all of the data **/

    struct DATA * d = malloc(sizeof(struct DATA));
    d-> pid = PID;
    d-> s_time = (void *) malloc(sizeof(char)*256);
    d-> s_state = (void *) malloc(sizeof(char)*256);
    d-> v_mem = (void *) malloc(sizeof(char)*256);
    d-> cmd_line = (void *) malloc(sizeof(char)*256);
    d-> u_time = -1;
    /**
     * MUST ALLOCATE SPACE FOR EACH INDIVIDUAL DATA
     */
    return d;
}


FLAGS * cmdline(int argc, char*argv[]){

    int opt;
    FLAGS * flag = malloc(sizeof(struct FLAGS));  // initializes all of the flags to 0

    flag->c_flag = 1;  /* logically flipped */
    flag->u_flag = 1;

    while((opt=getopt(argc,argv,":p:sUSvc")) != -1) {
        switch (opt) {
            case 'p':
                // this means only the specified PID has to display information
                if(!isdigit((int)* optarg)) {
                    if (strcmp(optarg, "-s") == 0) {
                        flag->s_flag = 1;
                        break;
                    } else if (strcmp(optarg, "-U") == 0) {
                        flag->u_flag = 0;
                        break;
                    } else if (strcmp(optarg, "-S") == 0) {
                        flag->S_flag = 1;
                        break;
                    } else if (strcmp(optarg, "-v") == 0) {
                        flag->v_flag = 1;
                        break;
                    } else if (strcmp(optarg, "-c") == 0) {
                        flag->c_flag = 0;
                        break;
                    } else {
                        flag->p_flag = 0; /** list all of the processes **/
                        break;                    }
                }
                flag->PID = (int) strtol(optarg,NULL,10);
                flag->p_flag = 1;
                break;
            case ':':
                optarg = NULL;
                continue;
            case 's':
                flag->s_flag = 1;
                break;
            case 'U':
                flag->u_flag = 0;//logically flipped
                break;
            case 'S':
                flag->S_flag = 1;
                break;
            case 'v':
                flag->v_flag =1;
                break;
            case 'c':
                flag->c_flag = 0; //logically flipped
                break;
            default:
                fprintf(stderr,"Unknown option -%c\n",optopt);
                break;
        }
    }
    return flag;

}

/*** function to print out each data type **/
void print_data(DATA * data, FLAGS * flags){
    printf("%d: ",data->pid);
    if(flags->s_flag) {printf("%s",data->s_state);}
    if(flags->u_flag) {printf("utime= %d ",data-> u_time);}
    if(flags->S_flag) {printf("stime= %s ",data->s_time);}
    if(flags->v_flag) {printf("size= %s ",data->v_mem);}
    if(flags->c_flag){printf("%s ",data->cmd_line);}
    puts(" ");

}