#include <stdio.h>
#include <string.h>


#define ARRAY_SIZE(x) (sizeof(x)/sizeof(x[0]))

typedef struct data_struct_type {
   char* data;
   size_t size;
}data_struct_t;

data_struct_t* encryptRotXor(data_struct_t* hex, data_struct_t* key);
data_struct_t* convert64ToHex(data_struct_t* base64);
char findB64Idx(char ch);
data_struct_t* breakRotXor(data_struct_t* hex);
data_struct_t* loadFile(char* path);
void writeFile(char* path, data_struct_t* hex);
char findSingleXorKey(data_struct_t* hex);
void freeData(data_struct_t* data);
data_struct_t* keyInit(char* keySring);
size_t* findProbKeyLength(data_struct_t* hex);
data_struct_t* initData(size_t size);

main(int argc, char** argv)
{
   data_struct_t* key;
   data_struct_t* base64;
   data_struct_t* hex;
   data_struct_t* decrypted;
   int i;
   size_t* probKeyLength;
   probKeyLength = (size_t*)malloc(sizeof(size_t)*3);

   //base64 = loadFile("6.txt");
   //hex = convert64ToHex(base64);
   //key = breakRotXor(hex);
   //decrypted = encryptRotXor(hex, key);
   //writeFile("6de.txt", decrypted);

   hex = loadFile("scratchpad.txt");
   //key = keyInit("CharlieRowerdink");
   //key = breakRotXor(hex);
   probKeyLength = findProbKeyLength(hex);
   printf("%d\t%d\t%d", probKeyLength[0], probKeyLength[1], probKeyLength[2]);
   //decrypted = encryptRotXor(hex, key);
   //writeFile("scratchpad.txt", decrypted);


   //freeData(hex);
   //freeData(decrypted);
   return 0;
}

data_struct_t* encryptRotXor(data_struct_t* hex, data_struct_t* key){
   size_t keyLength;
   int j = 0;
   int i;
   data_struct_t* crypt;


   crypt = (data_struct_t*)malloc(sizeof(data_struct_t));
   crypt->data = (char*)malloc(hex->size);
   crypt->size = hex->size;

   
   for (i = 0; i < hex->size; i++){
      crypt->data[i] = hex->data[i] ^ key->data[j];
      if (++j == key->size){ j = 0;}
   }

   return crypt;
}

data_struct_t* convert64ToHex(data_struct_t* base64){

   int i, j, k;
   int idx = 0;
   char sixbit;
   int buffer;
   int hexBuf;
   data_struct_t* hex;

   hex = (data_struct_t*)malloc(sizeof(data_struct_t*));

   hex->size = (base64->size * 3) / 4;
   hex->data = (char*)malloc(hex->size);
   
   for (i = 0; i < base64->size; i += 4){

      //Load the buffer with 4 six bit numbers
      buffer = 0;
      for (j = 0; j < 4; j++){
         sixbit = findB64Idx(base64->data[i+j]);
         if (j == 3){
            buffer = buffer | sixbit;
         } else {
            buffer = (buffer | sixbit) << 6;
         }
         
      }
      //Extract 3 four bit numbers from the buffer and load them into hex.data
      for(k = 2; k >= 0; k--){
         hexBuf = (buffer >> (8*k)) & 0x00000000000000FF;
         memcpy(&hex->data[idx++], &hexBuf, sizeof(char));
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


data_struct_t* breakRotXor(data_struct_t* hex){

   int i, j, numBlocks, probKeyLength;
   data_struct_t* buffer;
   data_struct_t* key;

   probKeyLength = findProbKeyLength(hex);
   printf("%d\n", probKeyLength);
   numBlocks = hex->size / probKeyLength;

   buffer = initData(numBlocks);
   key = initData(probKeyLength);

   for (i = 0; i < probKeyLength; i++){
      for (j = 0; j < numBlocks; j += probKeyLength){
         memcpy(&buffer->data[j], &hex->data[i+j], sizeof(char));
      }
      key->data[i] = findSingleXorKey(buffer);

   }
   return key;
}

size_t* findProbKeyLength(data_struct_t* hex){
   int keyLength, byte, hamdistA, hamdistB, lowNormHamdist;
   char* str1A;
   char* str2A;
   char* str1B;
   char* str2B;
   float normHamdist, avHamdist;
   size_t probKeyLength[3];
   lowNormHamdist = 0xFFFFFFFF;

   for (keyLength = 2; (keyLength < 40) && (hex->size >= keyLength * 4); keyLength++){
      str1A = (char*)malloc(sizeof(char) * keyLength);
      str2A = (char*)malloc(sizeof(char) * keyLength);
      str1B = (char*)malloc(sizeof(char) * keyLength);
      str2B = (char*)malloc(sizeof(char) * keyLength);

      memcpy(str1A, hex->data, keyLength);
      memcpy(str2A, &hex->data[keyLength], keyLength);
      memcpy(str1B, &hex->data[keyLength * 2], keyLength);
      memcpy(str2B, &hex->data[keyLength * 3], keyLength);

      hamdistA = findHammingDistance(str1A, str2A, keyLength);
      hamdistB = findHammingDistance(str1B, str2B, keyLength);

      avHamdist = (hamdistA + hamdistB) / 2;

      normHamdist = avHamdist / (float)keyLength; 
      printf("%f\t%d\n", normHamdist, keyLength);
      if (normHamdist < lowNormHamdist){
         lowNormHamdist = normHamdist;
         probKeyLength[2] = probKeyLength[1];
         probKeyLength[1] = probKeyLength[0];
         probKeyLength[0] = keyLength;
      }

      free(str1A);
      free(str2A);
      free(str1B);
      free(str2B);
   }
   return probKeyLength;
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

data_struct_t* loadFile(char* path){
   FILE* pFile;
   size_t result;
   long lSize;
   data_struct_t* buffer;

   buffer = (data_struct_t*)malloc(sizeof(data_struct_t));

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

void writeFile(char* path, data_struct_t* hex){
   FILE* pFile;
   pFile = fopen(path, "r+");
   if (pFile==NULL) {fputs ("File error", stderr); exit (1);}
   fwrite(hex->data, 1, hex->size, pFile);
   fclose (pFile);
}

char findSingleXorKey(data_struct_t* hex){
   char topCharactor, xored;
   long byte;
   int charactor;
   int score;
   int topScore = 0;

   for(charactor = 0; charactor < 255; charactor++){
      score = 0;
      for (byte = 0; byte < hex->size; byte++){
         xored = hex->data[byte] ^ charactor;
         if (xored == 'a' || xored == 'e' || xored == 'i' || xored == 'o' || xored == 't' || xored == ' '){
            score++;
         }
      }
      if (score > topScore){
         topScore = score;
         topCharactor = charactor;
      }
   }

   return topCharactor;
}

data_struct_t* initData(size_t size){
   data_struct_t* buffer;
   buffer = (data_struct_t*)malloc(sizeof(data_struct_t));
   buffer->data = (char*)malloc(sizeof(size));
   buffer->size = size;
   return buffer;
}

void freeData(data_struct_t* data){
   free(data->data);
   free(data);
   data = NULL;
}

data_struct_t* keyInit(char* keySring){
   data_struct_t* key;
   key = (data_struct_t*)malloc(sizeof(data_struct_t));
   key->data = (char*)malloc(strlen(keySring));
   key->size = strlen(keySring);
   memcpy(key->data, keySring, key->size);
   return key;

}

