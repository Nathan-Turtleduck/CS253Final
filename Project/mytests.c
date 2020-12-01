#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "ProcEntry.h"

int testCreateProcEntry(void){
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

int testCreateFromFile(void){
   char testName[] = "Test Create Proc From File/Destroy";

   char fileName[] = "/proc/1/stat";

   ProcEntry * newEntry = CreateProcEntryFromFile(fileName);

   if(newEntry == NULL){
      fprintf(stderr, "%s has failed\n", testName); 
      return 1;
   }

   DestroyProcEntry(newEntry);
   fprintf(stderr, "%s has passed\n", testName);

   return 0;
}

int testCreatePrintDestroy(void){
   char testName[] = "Test Create Proc From File/Print/Destroy";

   char fileName[] = "/proc/1/stat";

   ProcEntry * newEntry = CreateProcEntryFromFile(fileName);

   if(newEntry == NULL){
      fprintf(stderr, "%s has failed\n", testName); 
      return 1;
   }

   PrintProcEntry(newEntry);
   DestroyProcEntry(newEntry);
   fprintf(stderr, "%s has passed\n", testName);

   return 0;

}

int testCreateNULL(void){
   char testName[] = "Test Create Proc From Null";

   ProcEntry * nullEntry = CreateProcEntryFromFile(NULL);

   if(nullEntry != NULL){
      fprintf(stderr, "%s has failed\n", testName);
      return 1;
   }

   fprintf(stderr, "%s has passed\n", testName);
   return 0;
}

int testCreateFromFileDNE(void){
   char testName[] = "Test Create Proc From File DNE";
   
   ProcEntry * dneEntry = CreateProcEntryFromFile("text.txt");

   if(dneEntry != NULL){
      fprintf(stderr, "%s has failed\n", testName);
      return 1;
   }

   fprintf(stderr, "%s has passed\n", testName);
   return 0;

}

int testCreateFromFileInvalid(void){
   char testName[] = "Test Create Proc From File Invalid Format";

   FILE * invalidFile = fopen("invalidFile.txt", "w+");
   fputs("Invalid file cant get all values", invalidFile);

   rewind(invalidFile);
   fclose(invalidFile);

   ProcEntry * invalidEntry = CreateProcEntryFromFile("invalidFile.txt");

   if(invalidEntry != NULL){
      fprintf(stderr, "%s has failed\n", testName);
      return 1;
   }

   fprintf(stderr, "%s has passed\n", testName);
   remove("invalidFile.txt");
   return 0;

}

int main(void) {

   int testVals = 0;

   testVals += testCreateProcEntry();
   testVals += testCreateFromFile();
   testVals += testCreatePrintDestroy();
   testVals += testCreateNULL();
   testVals += testCreateFromFileDNE();
   testVals += testCreateFromFileInvalid();


   return testVals;
}