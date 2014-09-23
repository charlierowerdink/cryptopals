#include <stdio.h>
#include <string.h>
#include "base64.h"

#define ARRAY_SIZE(x) (sizeof(x)/sizeof(x[0]))

void encryptRotXor(char* path, char* key);
void convert64ToHex(char* data, int dataLength, char* key);
char findB64Idx(char ch);
void breakRotXor(char* path, char* decryptedData);

main(int argc, char** argv)
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
   convert64ToHex(data, strlen(data) , key);

   
   //breakRotXor("6.txt", key);

   //printf("\\n", key);

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

void convert64ToHex(char* data, int dataLength, char* hexData){

   int i, j, k;
   int idx = 0;
   char sixbit;
   int buffer;
   int hexBuf;

   hexData = (char*)malloc((dataLength * 3)/4);
   
   for (i = 0; i < dataLength; i += 4){

      buffer = 0;
      for (j = 0; j < 4; j++){
         sixbit = findB64Idx(data[i+j]);
         if (j == 3){
            buffer = buffer | sixbit;
         } else {
            buffer = (buffer | sixbit) << 6;
         }
         
      }
      //TODO: get the buffer to the hexData
      
      if (idx < ((dataLength*3)/4)){
         hexBuf = (buffer >> 16) & 0x00000000000000FF;
         memcpy(&hexData[idx++], &hexBuf, sizeof(char));
         hexBuf = (buffer >> 8) & 0x00000000000000FF;
         memcpy(&hexData[idx++], &hexBuf, sizeof(char));
         hexBuf = buffer & 0x00000000000000FF;
         memcpy(&hexData[idx++], &hexBuf, sizeof(char));
      }


      

      printf("%x", buffer);
      
       
   }
   printf("\nhexData\n");
   for (i = 0; i < dataLength; i++){
      printf("%x", hexData[i]);
   }
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


int findHammingDistance(char* str1, char* str2, int length){

   int i;
   int count = 0;
   int buffer = 0;

   for (i = 0; i < length; i++){
      count += NumberOfSetBits(str1[i] ^ str2[i]);
   }

   return count;

}

int NumberOfSetBits(int i)
{
     i = i - ((i >> 1) & 0x55555555);
     i = (i & 0x33333333) + ((i >> 2) & 0x33333333);
     return (((i + (i >> 4)) & 0x0F0F0F0F) * 0x01010101) >> 24;
}

void breakRotXor(char* path, char* decryptedData){

   FILE* pFile;
   char* encrypt;
   long lSize;
   char* buffer;
   size_t result;
   int i, j, hamdist, normHamdist;
   char* str1, str2, hexData;

   //File Ops
   pFile = fopen(path, "rb");
   if (pFile==NULL) {fputs ("File error", stderr); exit (1);}

   // obtain file size:
   fseek (pFile , 0 , SEEK_END);
   lSize = ftell (pFile);
   rewind (pFile);

   // allocate memory to contain the whole file:
   buffer = (char*) malloc (sizeof(char)*lSize);
   if (buffer == NULL) {fputs ("Memory error", stderr); exit (2);}

   // copy the file into the buffer:
   result = fread (buffer, 1, lSize, pFile);
   if (result != lSize) {fputs ("Reading error", stderr); exit (4);}

   /* the whole file is now loaded in the memory buffer. */

   convert64ToHex(buffer, lSize, hexData);

   //main for loop that loops through Key Sizes
   for (i = 2; i < 40; i++){
      str1 = (char*)malloc(sizeof(char) * i);
      str2 = (char*)malloc(sizeof(char) * i);

     

      //memcpy(str1, hexData[0], sizeof(char) * i);
      //memcpy(str2, buffer[i], sizeof(char) * i);
   

      //hamdist = findHammingDistance(str1, str2, i);
      normHamdist = hamdist / i; 
      //printf("%d\n", normHamdist);

   }
}