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
        return NULL;
    }

    // Create ProcEntry and check if its NULL
    ProcEntry * newEntry = (ProcEntry *) malloc(sizeof(ProcEntry));

    if(newEntry == NULL){
        return NULL;
    }

    int numScanned = 0;

    numScanned = fscanf(userFile, "%d, %s[^()], %c, %lu, %lu, %d",
        newEntry->pid,
        newEntry->comm,
        newEntry->state,
        newEntry->utime,
        newEntry->stime,
        newEntry->proc);
    
    if(numScanned != 6){
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

    free(entry);

}