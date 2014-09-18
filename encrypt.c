#include <stdio.h>
#include <string.h>
#include "base64.h"

#define ARRAY_SIZE(x) (sizeof(x)/sizeof(x[0]))

void encryptRotXor(char* path, char* key);
char* convert64ToHex(char* data, int dataLength, char* key);
char findB64Idx(char ch);

main(int argc, char **argv)
{
   char defaultKey[] = "ICE";
   char* key;
/*
   if (argc == 3){
      key = argv[2];
   } else if (argc == 2){
      key = &defaultKey;
   } else {
      fputs ("Incorrect Arguments", stderr); exit (1);
   }

   encryptRotXor(argv[1], key);
*/

   char data[] = "SSdtIGtpbGxpbmcgeW91ciBicmFpbiBsaWtlIGEgcG9pc29ub3VzIG11c2hyb29t";
   //key = (char*)malloc(Base64decode_len(data));
   //Base64decode(key, data);
   convert64ToHex(data, key);
   printf("%x\n", key);

}

void encryptRotXor(char* path, char* key){
   FILE* pFile;
   char* encrypt;
   long lSize;
   char* buffer;
   size_t result;
   int j = 0;
   int i;

   
   
   pFile = fopen(path, "r+b");
   if (pFile==NULL) {fputs ("File error", stderr); exit (1);}

   // obtain file size:
   fseek (pFile , 0 , SEEK_END);
   lSize = ftell (pFile);
   rewind (pFile);

   // allocate memory to contain the whole file:
   buffer = (char*) malloc (sizeof(char)*lSize);
   if (buffer == NULL) {fputs ("Memory error", stderr); exit (2);}

   // allocate memory to contain the encrypted file:
   encrypt = (char*) malloc (sizeof(char)*lSize);
   if (encrypt == NULL) {fputs ("Memory error", stderr); exit (3);}

   // copy the file into the buffer:
   result = fread (buffer, 1, lSize, pFile);
   if (result != lSize) {fputs ("Reading error", stderr); exit (4);}

   /* the whole file is now loaded in the memory buffer. */

   for (i = 0; i < lSize; i++){
      encrypt[i] = buffer[i] ^ key[j];
      if (++j == strlen(key)){ j = 0;}
      
   }

   rewind(pFile);
   fwrite(encrypt, 1, lSize, pFile);
   // terminate
   fclose (pFile);
   free (buffer);
   return 0;
}

char* convert64ToHex(char* data, int dataLength, char* hexData){

   int i, j;
   char sixbit;
   char threebytes[3];
   //char* hexData;
   int buffer = 0;

   hexData = (char*)malloc((sizeof(data) * 3)/4);

   
   for (i = 0; i < sizeof(data); i += 4){

      for (j = 0; j < 4; j++){
         sixbit = findB64Idx(data[j]);
         if (j == 3){
            buffer = buffer | sixbit;
         } else {
            buffer = (buffer | sixbit) << 6;
         }
         
      }
      

      printf("%x\n", buffer);
       
   }

   return 0;
}

char findB64Idx(char ch){
   int i;
   static char encoding_table[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

   for (i = 0; i < 64; i++){
      if (ch == encoding_table[i]){
         return i;
      }
   }
}
