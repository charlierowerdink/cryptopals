#include <stdio.h>
#include <string.h>
#include "base64.h"
#include "breakRotXor.h"


#define ARRAY_SIZE(x) (sizeof(x)/sizeof(x[0]))


data_t* encryptRotXor(data_t* hex, data_t* key);

data_t* loadFile(char* path);
void writeFile(char* path, data_t* hex);

void freeData(data_t* data);
data_t* keyInit(char* keySring);


main(int argc, char** argv)
{
   data_t* key;
   data_t* base64;
   data_t* hex;
   data_t* decrypted;
   int i;
   size_t* probKeyLength;

   base64 = loadFile("../6.txt");
   hex = convert64ToHex(base64);
   //key = breakRotXor(hex);
   //decrypted = encryptRotXor(hex, key);
   //writeFile("6de.txt", decrypted);

   //hex = loadFile("../scratchpad.txt");
   //key = keyInit("CharlieRowerdink");
   //key = breakRotXor(hex);
   //probKeyLength = findProbKeyLength(hex);
   //printf("%d\t%d\t%d", probKeyLength[0], probKeyLength[1], probKeyLength[2]);
   //decrypted = encryptRotXor(hex, key);
   //writeFile("scratchpad.txt", decrypted);


   //freeData(hex);
   //freeData(decrypted);
   return 0;
}

data_t* encryptRotXor(data_t* hex, data_t* key){
   size_t keyLength;
   int j = 0;
   int i;
   data_t* crypt;


   crypt = (data_t*)malloc(sizeof(data_t));
   crypt->data = (char*)malloc(hex->size);
   crypt->size = hex->size;

   
   for (i = 0; i < hex->size; i++){
      crypt->data[i] = hex->data[i] ^ key->data[j];
      if (++j == key->size){ j = 0;}
   }

   return crypt;
}

data_t* loadFile(char* path){
   FILE* pFile;
   size_t result;
   long lSize;
   data_t* buffer;

   buffer = (data_t*)malloc(sizeof(data_t));

   //File Ops
   pFile = fopen(path, "rb");
   if (pFile==NULL) {fputs ("File error", stderr); exit (1);}

   // obtain file size:
   fseek (pFile , 0 , SEEK_END);
   lSize = ftell (pFile);
   rewind (pFile);

   // allocate memory to contain the whole file:
   buffer->data = (char*) malloc (sizeof(char)*lSize);
   if (buffer->data == NULL) {fputs ("Memory error", stderr); exit (2);}

   // copy the file into the buffer:
   result = fread (buffer->data, 1, lSize, pFile);
   if (result != lSize) {fputs ("Reading error", stderr); exit (4);}

   fclose(pFile);

   buffer->size = lSize;
   return buffer;
}

void writeFile(char* path, data_t* hex){
   FILE* pFile;
   pFile = fopen(path, "r+");
   if (pFile==NULL) {fputs ("File error", stderr); exit (1);}
   fwrite(hex->data, 1, hex->size, pFile);
   fclose (pFile);
}


data_t* initData(size_t size){
   data_t* buffer;
   buffer = (data_t*)malloc(sizeof(data_t));
   buffer->data = (char*)malloc(sizeof(size));
   buffer->size = size;
   return buffer;
}

void freeData(data_t* data){
   free(data->data);
   free(data);
}

data_t* keyInit(char* keySring){
   data_t* key;
   key = (data_t*)malloc(sizeof(data_t));
   key->data = (char*)malloc(strlen(keySring));
   key->size = strlen(keySring);
   memcpy(key->data, keySring, key->size);
   return key;

}

