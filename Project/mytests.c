#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "ProcEntry.h"

int testCreateProcEntry(){
   char testName[] = "Test Create Proc Default/Destroy";
   ProcEntry * test = CreateProcEntry();

   if(test == NULL){
      fprintf(stderr, "%s has failed\n", testName);
      return 1;
   }

   DestroyProcEntry(test);
   fprintf(stderr, "%s has passed\n", testName);

   return 0;

}

int testCreateFromFile(){
   char testName[] = "Test Create Proc From File/Destroy";

   FILE * testFile = fopen("file.txt", "w+");
   fputs("1 (systemd) S 0 1 1 0", testFile);

   rewind(testFile);
   fclose(testFile);

   ProcEntry * newEntry = CreateProcEntryFromFile("file.txt");

   if(newEntry == NULL){
      fprintf(stderr, "%s has failed\n", testName); 
      return 1;
   }

   DestroyProcEntry(newEntry);
   fprintf(stderr, "%s has passed\n", testName);

   return 0;
}

int main(void) {

   int testVals = 0;

   testVals += testCreateProcEntry();
   testVals += testCreateFromFile();


   return testVals;
}