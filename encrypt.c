#include <stdio.h>
#include <string.h>
#include "base64.h"

#define ARRAY_SIZE(x) (sizeof(x)/sizeof(x[0]))

typedef struct data_struct_type {
   char* data;
   size_t size;
}data_struct_t;

data_struct_t encryptRotXor(data_struct_t hex, char* key);
data_struct_t convert64ToHex(data_struct_t base64);
char findB64Idx(char ch);
data_struct_t breakRotXor(data_struct_t hex);
data_struct_t loadFile(char* path);
void writeFile(char* path, data_struct_t hex);

main(int argc, char** argv)
{
   char defaultKey[] = "ICE";
   char* key;
   data_struct_t base64;
   data_struct_t hex;
   data_struct_t decrypted;
   int i;
   

   //base64.data = (char*)malloc(64);
   //base64.size = 64;
   //memcpy(base64.data, "SSdtIGtpbGxpbmcgeW91ciBicmFpbiBsaWtlIGEgcG9pc29ub3VzIG11c2hyb29t", base64.size);
   //convert64ToHex(base64);

   //base64 = loadFile("6.txt");
   //hex = convert64ToHex(base64);
   //decrypted = breakRotXor(hex);

   hex = loadFile("5.txt");
   for (i = 0; i < decrypted.size; i++){
      printf("%s", hex.data[i]);
   }
   decrypted = encryptRotXor(hex, "ICE");
   writeFile("5.txt", decrypted);

   for (i = 0; i < decrypted.size; i++){
      printf("%s", decrypted.data[i]);
   }
   return 0;
}

data_struct_t encryptRotXor(data_struct_t hex, char* key){
   size_t keyLength;
   int j = 0;
   int i;
   data_struct_t crypt;

   crypt.data = (char*)malloc(hex.size);
   crypt.size = hex.size;

   keyLength = strlen(key);
   for (i = 0; i < hex.size; i++){
      crypt.data[i] = hex.data[i] ^ key[j];
      if (++j == keyLength){ j = 0;}
   }

   return crypt;
}

data_struct_t convert64ToHex(data_struct_t base64){

   int i, j, k;
   int idx = 0;
   char sixbit;
   int buffer;
   int hexBuf;
   data_struct_t hex;

   hex.size = (base64.size * 3) / 4;
   hex.data = (char*)malloc(hex.size);
   
   for (i = 0; i < base64.size; i += 4){

      //Load the buffer with 4 six bit numbers
      buffer = 0;
      for (j = 0; j < 4; j++){
         sixbit = findB64Idx(base64.data[i+j]);
         if (j == 3){
            buffer = buffer | sixbit;
         } else {
            buffer = (buffer | sixbit) << 6;
         }
         
      }
      //Extract 3 four bit numbers from the buffer and load them into hex.data
      for(k = 2; k >= 0; k--){
         hexBuf = (buffer >> (8*k)) & 0x00000000000000FF;
         memcpy(&hex.data[idx++], &hexBuf, sizeof(char));
      }
   
      //printf("%x", buffer);
      
   }
   //printf("\nhexData\n");
   //for (i = 0; i < hex.size; i++){
   //   printf("%x", hex.data[i]);
   //}
   return hex;
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


int findHammingDistance(char* str1, char* str2, int size){

   int i;
   size_t count = 0;

   for (i = 0; i < size; i++){
      count += NumberOfSetBits(str1[i] ^ str2[i]); 
   }

   return count;
}

int NumberOfSetBits(int num){
   int dist = 0;

   while(num){
      ++dist; 
      num &= num - 1;
   }
 
   return dist;
}

data_struct_t breakRotXor(data_struct_t hex){

   int keyLength, probKeyLength, byte, hamdist, normHamdist;
   char* str1;
   char* str2;
   data_struct_t decrypted;

   probKeyLength = 0xFF;

   //main for loop that loops through Key Sizes and finds probably key length
   for (keyLength = 2; keyLength < 40; keyLength++){
      str1 = (char*)malloc(sizeof(char) * keyLength);
      str2 = (char*)malloc(sizeof(char) * keyLength);

      printf("%d\t", keyLength);
      memcpy(str1, hex.data, keyLength);
      memcpy(str2, &hex.data[keyLength], keyLength);

      hamdist = findHammingDistance(str1, str2, keyLength);
      normHamdist = hamdist / keyLength; 

      if (normHamdist < probKeyLength){
         probKeyLength = normHamdist;
      }

      printf("%d\n", normHamdist);

      free(str1);
      free(str2);
   }
   //We now have the probable key length



   
   return decrypted;
}


data_struct_t loadFile(char* path){
   FILE* pFile;
   size_t result;
   long lSize;
   data_struct_t buffer;

   //File Ops
   pFile = fopen(path, "rb");
   if (pFile==NULL) {fputs ("File error", stderr); exit (1);}

   // obtain file size:
   fseek (pFile , 0 , SEEK_END);
   lSize = ftell (pFile);
   rewind (pFile);

   // allocate memory to contain the whole file:
   buffer.data = (char*) malloc (sizeof(char)*lSize);
   if (buffer.data == NULL) {fputs ("Memory error", stderr); exit (2);}

   // copy the file into the buffer:
   result = fread (buffer.data, 1, lSize, pFile);
   if (result != lSize) {fputs ("Reading error", stderr); exit (4);}

   fclose(pFile);

   buffer.size = lSize;
   return buffer;
}

void writeFile(char* path, data_struct_t hex){
   FILE* pFile;
   pFile = fopen(path, 'w');
   fwrite(hex.data, 1, hex.size, pFile);
   fclose (pFile);
}

data_struct_t breakSingleXor(data_struct_t hex){
   char byte, charactor, topCharactor, key, xored;
   int score;
   int topScore = 0;
   data_struct_t decrypted;

   for(charactor = 0; charactor < 255; charactor++){
      score = 0;
      for (byte = 0; byte < hex.size; byte++){
         xored = hex.data[byte] ^ charactor;
         if (xored == 'a' || xored == 'e' || xored == 'i' || xored == 'o' || xored == 't' || xored == ' '){
            score++;
         }
      }
      if (score > topScore){
         topScore = score;
         topCharactor = charactor;

      }
   }





}