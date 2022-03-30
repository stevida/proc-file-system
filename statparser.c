
/**
 * Steven Vidal
 *
 *  This file holds only one long function definition to collect the data from the stat, statm and cmdline files
 *  in the /proc/PID folders
 *
 *
 */


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <errno.h>
#include <string.h>
#include <sys/stat.h>

#include "tups_header.h"


DATA * stat_parse(int PID){


    DATA * d = d_malloc(PID);

    FILE * fp_stat,* fp_statm, * buf,* fp_cmdline;

    /** ~PID~ assignment to DATA struct**/
    char * pid_str =(void*) malloc(sizeof(char)*64); sprintf(pid_str, "%d", PID); //creates the PID into a string to append to path
    char * path_stat = (void*) malloc(sizeof(char)*512);
    char * path_statm = (void*) malloc(sizeof(char)*512);
    char * path_cmdline= (void*) malloc(sizeof(char)*512);

    strcpy(path_stat,"/proc/");
    strcat(path_stat,pid_str);

    strcpy(path_statm,path_stat); /** using  this for the statm path**/
    strcpy(path_cmdline,path_stat); /** using this for the cmdline path **/


    strcat(path_stat,"/stat");
    fp_stat = fopen(path_stat,"r");

    if(fp_stat == NULL){
        fprintf(stdout,"ERROR: PID %d DOES NOT EXIST | %d",PID,errno);
        exit(1);
    }

    /**creates folder of all the pid_stat files to add to check if values are correct**/
    char str_buf[64] = "./pid_stat/";
    char file_name[64] = "temp_";
    strcat(file_name,pid_str);
    strcat(str_buf,file_name);

    int m;
    char * dir = "pid_stat";
    rmdir(dir);
    m = mkdir(dir, 0777); //create directory
    if (m == errno) {
        perror("error making directory\n");
        exit(1);
    }
    /****************************** PARSING STAT  ********************************************************/

    if(fp_stat!=NULL){
        if(!(buf = fopen(str_buf,"w"))){
            fprintf(stdout,"Error creating file: %s | %d\n",str_buf,errno);
            exit(0);
        }
        char c;
        c = (char) fgetc(fp_stat);
        int content = 1; // using the spaces in the stat files as indicators for what data info im looking at
        char string_data[512] = "\0";
        while(!feof(fp_stat)){
            fputc(c, buf);
            strncat(string_data,&c,1); /**concatenate the c char to string data**/
            if(c == ' '){ //there is a space line
                if(content == 3){                       /** state of the process **/
                     strcpy(d->s_state,string_data);
                }
                if(content == 14){                     /**u_time**/
                    int string_data_int = (int) strtol(string_data,NULL,10);
                    d-> u_time = string_data_int;
                }
                if(content == 15){                     /**s_time**/
                    strcpy(d-> s_time, string_data);
                }
                /**
                 * content keeps track of number in stat file, once it reaches a value im interested in it
                 * frees the string and reallocates space for it, treating it as an empty string
                 */
                content++;
                strcpy(string_data,""); //clears string
                fputs("\n",buf);
            }
            c = (char) fgetc(fp_stat);

        }
        fclose(fp_stat);
    }

     /*****************************************PARSING STATM************************************************************/

    strcat(path_statm,"/statm");
    fp_statm = fopen(path_statm,"r");

    char string_data_statm[256] = "\0";
    if(fp_statm!=NULL){
        char c;
        c = (char) fgetc(fp_statm);
        fputs("\nfirst element of statm: ",buf);
        while(!feof(fp_statm)) {
            strncat(string_data_statm,&c,1);
            if (c == ' '){ /** we only need the first element **/
                strcpy(d->v_mem,string_data_statm);
                fputs(string_data_statm,buf);
                break;
            }
            c = (char) fgetc(fp_statm);
        }

    }
    /*****************************************PARSING CMDLINE************************************************************/


    strcat(path_cmdline,"/cmdline");

    /** THERES NO COMMAND LINE, RETURN NOTHING**/
    fp_cmdline = fopen(path_cmdline,"r");
    if(fp_cmdline == NULL){
        strcpy(d->cmd_line,"[]");
        fclose(buf);
        fclose(fp_statm);
        fclose(fp_cmdline);
        return d;
    }

    char string_data_cmdline[256] = "[";
    char n;
    n = (char) fgetc(fp_cmdline);
    fputs("\nelements of cmdline: \n",buf);
    while(!feof(fp_cmdline)) {
        strncat(string_data_cmdline,&n,1);
        if (n == '\0'){ /** if null-terminating character, continue **/
            n = (char) fgetc(fp_cmdline);
            if(feof(fp_cmdline)){   /** so we dont add the last space before the bracket **/
                break;
            }
            strncat(string_data_cmdline," ",1);
            continue;
        }
        n = (char) fgetc(fp_cmdline);
    }
    strcat(string_data_cmdline,"]");
    fputs(string_data_cmdline,buf);
    strcpy(d->cmd_line,(char *) string_data_cmdline);
    fclose(buf);
    fclose(fp_statm);
    fclose(fp_cmdline);

    return d;
}
