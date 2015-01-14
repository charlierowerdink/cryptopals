
#include "base64.h"


data_t* convert64ToHex(data_t* base64){

   int i, j, k;
   int idx = 0;
   char sixbit;
   int buffer;
   int hexBuf;
   data_t* hex;

   hex = (data_t*)malloc(sizeof(data_t*));

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
         hexBuf = (buffer >> (8 * k)) & 0x00000000000000FF;
         memcpy(&hex->data[idx++], &hexBuf, sizeof(char));
      }
   }
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