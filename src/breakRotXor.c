#include "breakRotXor.h"

char findSingleXorKey(data_t* hex);
size_t* findProbKeyLength(data_t* hex);
float findHighestLowHamdist(float* lowNormHamdist);
int findHammingDistance(char* str1, char* str2, int size);
int NumberOfSetBits(int num);
int scoreEnglish(data_t* hex);

data_t* breakRotXor(data_t* hex){

   int i, j, k, numBlocks, score, topScore;
   data_t* buffer;
   data_t* key;
   data_t* decrypted;
   size_t* probKeyLength;
   topScore = 0;
   
   for (k = 0; k < 3; k++){

      probKeyLength = findProbKeyLength(hex);
      printf("%d\n", probKeyLength[k]);
      numBlocks = hex->size / probKeyLength[k];

      buffer = initData(numBlocks);
      key = initData(probKeyLength[k]);

      for (i = 0; i < probKeyLength[k]; i++){
         for (j = 0; j < numBlocks; j += probKeyLength){
            memcpy(&buffer->data[j], &hex->data[i+j], sizeof(char));
         }
         key->data[i] = findSingleXorKey(buffer);

      } 
      decrypted = encryptRotXor(hex, key);
      score = scoreEnglish(decrypted); 
      if (score > topScore){
         topScore = score;
      }
      printf("score: %d", score);
   }
       
   return key;
}

char findSingleXorKey(data_t* hex){
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

int scoreEnglish(data_t* hex){
   long byte;
   int score;
   char xored, charactor;
   for (byte = 0; byte < hex->size; byte++){
      xored = hex->data[byte] ^ charactor;
      if (xored == 'a' || xored == 'e' || xored == 'i' || xored == 'o' || xored == 't' || xored == ' '){
         score++;
         }
      }
   return score;
}

size_t* findProbKeyLength(data_t* hex){
   size_t keyLength, byte, hamdistA, hamdistB;
   char* str1A;
   char* str2A;
   char* str1B;
   char* str2B;
   float normHamdist, avHamdist;
   size_t* probKeyLength;
   float* lowNormHamdist;
   int i;

   probKeyLength = (size_t*)malloc(sizeof(size_t)*3);
   lowNormHamdist = (float*)malloc(sizeof(float)*3);

   memset(probKeyLength, 0x00, 3);

   for (i = 0; i < 3; i++){
      lowNormHamdist[i] = 1000000000.0;
   }

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
//      printf("%f\t%d\n", normHamdist, keyLength);
      if(findHighestLowHamdist(lowNormHamdist) > normHamdist){
         lowNormHamdist[2] = lowNormHamdist[1];
         probKeyLength[2] = probKeyLength[1];
         lowNormHamdist[1] = lowNormHamdist[0];
         probKeyLength[1] = probKeyLength[0];
         lowNormHamdist[0] = normHamdist;
         probKeyLength[0] = keyLength;
      }

      free(str1A);
      free(str2A);
      free(str1B);
      free(str2B);
     
   }
   printf("%zu\t%zu\t%zu\n", probKeyLength[0], probKeyLength[1], probKeyLength[2]);
   return probKeyLength;
}

float findHighestLowHamdist(float* lowNormHamdist){
   int i;
   float high = 0;
   for (i = 0; i < 3; i++){
      if (lowNormHamdist[i] > high){
         high = lowNormHamdist[i];
      }
   }
   return high;
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




