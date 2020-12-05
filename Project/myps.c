/* 
 * Author: Nathan Maroko
 * Date: Fri 04 Dec 2020 04:09:14 PM PST
 * Description: ./myps has the same functionality
 * that the ps command has on linux
 */

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>
#include "ProcEntry.h"

#define MAX_PATH_LENGTH 4096
#define UNUSED(x) (void)x

/** Functions used for scandir **/

// Sort function that does not sort
int noSort(const struct dirent ** entryA, const struct dirent ** entryB){
    UNUSED(entryA);
    UNUSED(entryB);
    return 0;
}

// Filter function that filters in PID directories
static int defaultFilter (const struct dirent *current){

    /** Returns 1 if dirent is directory and if the first char
    is a number **/
    if(current->d_type == DT_DIR && isdigit(current->d_name[0])){
        // fprintf(stdout, "%s\n",current->d_name);
        return 1;
    }else{
        return 0;
    }

}

/** Functions used for qsort **/

// Sort function that sorts by process id
static int pidSort(const void *a, const void *b)
{
    ProcEntry *f = *(ProcEntry **)a;
    ProcEntry *s = *(ProcEntry **)b;
    int rval = f->pid - s->pid;
    return rval;
}

// Sort function that sorts by command
static int commSort(const void *a, const void *b)
{
    ProcEntry *f = *(ProcEntry **)a;
    ProcEntry *s = *(ProcEntry **)b;

    int result = strcmp(f->comm, s->comm);
    return result;
}


int main (int argc, char * argv[]) {
    struct dirent **eps;
    int n;
    int opt;
    char dirPath[MAX_PATH_LENGTH];
    int PIDSORTFLAG;
    int COMMANDSORTFLAG = 0;
    int ZOMBIESORTFLAG = 0;

    //Checks to see if custom directory is inputted
    int CUSTOMDIRFLAG = 0;
    
    //Set Default Path
    strcpy(dirPath, "/proc"); 

    // Declare filter function
    int (*filterFunction)(const struct dirent *);
    filterFunction = defaultFilter;

    // Declare sort function
    int (*sortFunction)(const struct dirent **, const struct dirent **);
    sortFunction = noSort;

    while( (opt = getopt(argc, argv, "d:pczh")) != -1){
        switch (opt)
        {

        case 'd':
            strncpy(dirPath, optarg, MAX_PATH_LENGTH);
            CUSTOMDIRFLAG = 1;
            break;
        
        case 'p':
            PIDSORTFLAG = 1;
            break;

        case 'c':
            COMMANDSORTFLAG = 1;
            PIDSORTFLAG = 0;
            break;
        
        case 'z':
            ZOMBIESORTFLAG = 1;
            break;

        case 'h':
            fprintf(stderr, "Usage: %s [-d <path>] [-p] [-c] [-z] [-h]\n", argv[0]);
            fprintf(stderr, "       -d <path> Directory containing proc entries (default: /proc)\n");
            fprintf(stderr, "       -p        Display proc entries sorted by pid (default)\n");
            fprintf(stderr, "       -c        Display proc entries sorted by command lexicographically\n");
            fprintf(stderr, "       -z        Display ONLY proc entries in the zombie state\n");
            fprintf(stderr, "       -h        Display this help message\n");
            exit(0);
            break; 

        default:
            fprintf(stderr, "Error: Invalid option specified\n");
            fprintf(stderr, "Usage: %s [-d <path>] [-p] [-c] [-z] [-h]\n", argv[0]);
            fprintf(stderr, "       -d <path> Directory containing proc entries (default: /proc)\n");
            fprintf(stderr, "       -p        Display proc entries sorted by pid (default)\n");
            fprintf(stderr, "       -c        Display proc entries sorted by command lexicographically\n");
            fprintf(stderr, "       -z        Display ONLY proc entries in the zombie state\n");
            fprintf(stderr, "       -h        Display this help message\n");
            exit(1);
            break;
        }
    }

    //Check to see Command flag
    if(COMMANDSORTFLAG == 0){
        PIDSORTFLAG = 1;
    }

    errno = 0;

    n = scandir(dirPath, &eps, filterFunction, sortFunction);

    if (n < 0) {
        perror("scandir: ");
        exit(1);
    }

    /** Allocate memory for every ProcEntry and load each proc in**/
    ProcEntry ** myprocs = (ProcEntry **) (malloc(sizeof(ProcEntry *) * n));

    for(int i = 0; i < n; ++i){
        char filePath[MAX_PATH_LENGTH]; 

        if(CUSTOMDIRFLAG == 0){
            strcpy(filePath, "/proc/");
            strcat(filePath, eps[i]->d_name);
            strcat(filePath, "/stat");
            // fprintf(stdout, "%s\n", filePath); 
        }else{
            strcpy(filePath, dirPath);
            strcat(filePath, "/");
            strcat(filePath, eps[i]->d_name);
            strcat(filePath, "/stat");
            // fprintf(stdout, "%s\n", filePath);
        }

        myprocs[i] = CreateProcEntryFromFile(filePath);

        //Checks to see if Proc Entry is valid
        if(myprocs[i] == NULL){
            fprintf(stderr, "Unable to load in Proc Entry\n");
            exit(1);
        }
    }

    if(PIDSORTFLAG == 1){
        qsort(myprocs, n, sizeof(ProcEntry *), pidSort);
    }

    if(COMMANDSORTFLAG == 1){
        qsort(myprocs, n, sizeof(ProcEntry *), commSort);
    }

    fprintf(stdout,"%7s %5s %5s %5s %4s %-25s %-20s\n","PID","STATE","UTIME","STIME","PROC","CMD","STAT_FILE");

    /** Display the stats **/
    for(int i = 0; i < n; ++i){

        if(ZOMBIESORTFLAG == 1){
            if(myprocs[i]->state == 'Z'){
                PrintProcEntry(myprocs[i]);
            }
        }else{
            PrintProcEntry(myprocs[i]);
        }
    }


    /** Free up memory **/
    for(int i = 0; i < n; ++i){
        DestroyProcEntry(myprocs[i]);
    }

    for(int i = 0; i < n; ++i){
        free(eps[i]);
    }

    free(eps);
    free(myprocs);

    return 0;
}
