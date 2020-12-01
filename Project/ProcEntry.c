/**
 * File: ProcEntry.c
 * Author: Nathan Maroko
 * Date:
 * Description: Implements all methods for a ProcEntry struct
 **/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "ProcEntry.h"

ProcEntry * CreateProcEntry(void){

    ProcEntry * retEntry = (ProcEntry *) malloc(sizeof(ProcEntry));

    if(retEntry == NULL){
        free(retEntry);
        return NULL;
    }else{
        return retEntry;
    }

}

ProcEntry * CreateProcEntryFromFile(const char statFile[]){

    if(statFile == NULL){
        return NULL;
    }

    // Create file and check if its NULL
    FILE * userFile = fopen(statFile, "r");

    if(userFile == NULL){
        fprintf(stderr, "CreateProcEntryFromFile: No such file or directory\n");
        return NULL;
    }

    // Create ProcEntry and check if its NULL
    ProcEntry * newEntry = (ProcEntry *) malloc(sizeof(ProcEntry));
    newEntry->comm = (char *) malloc(sizeof(char));
    newEntry->path = (char *) malloc(sizeof(char));
    strcpy(newEntry->path, statFile);

    if(newEntry == NULL){
        return NULL;
    }

    int numScanned = 0;

    numScanned = fscanf(userFile, "%d %s %c %lu %lu %d",
        &newEntry->pid,
        newEntry->comm,
        &newEntry->state,
        &newEntry->utime,
        &newEntry->stime,
        &newEntry->proc);

    if(numScanned != 6){
        fprintf(stderr, "Error: Loaded %d out of 6\n", numScanned);
        free(newEntry->path);
        free(newEntry->comm);
        free(newEntry);
        fclose(userFile);
        return NULL;
    }

    fclose(userFile);

    return newEntry;

}

void PrintProcEntry(ProcEntry *entry)
{
     unsigned long int utime = entry->utime / sysconf(_SC_CLK_TCK);
     unsigned long int stime = entry->stime / sysconf(_SC_CLK_TCK);
     fprintf(stdout, "%7d %5c %5lu %5lu %4d %-25s %-20s\n",
             entry->pid,
             entry->state,
             utime,
             stime,
             entry->proc,
             entry->comm,
             entry->path);
}

void DestroyProcEntry(ProcEntry * entry){

    if(entry == NULL){
        return;
    }

    free(entry->comm);
    free(entry->path);
    free(entry);

}