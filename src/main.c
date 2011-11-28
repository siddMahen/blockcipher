#include <stdio.h>
#include "blockcipher.h"

int main(){

    // Tests:
    // This is a though key
    uint32_t key[] = { 0x00000001UL, // 1 
    				   0x00000010UL, // 2
    				   0x00000011UL, // 3
    				   0x00000100UL, // 4
    				   0x00000101UL, // 5
    				   0x00000110UL, // 6
    				   0x00000111UL, // 7
    				   0x00001000UL  // 8
    				 };

    uint32_t data[] = { 1, 1 };
    
    printf("Key: ");
    int i;
    for(i = 0; i < 8; i++)
    	printf("%x", key[i]);
    printf("\n");
    
    blk_ctx c;
    blk_init(&c, key);
    printf("Decrypted data: %u, %u\n", data[0], data[1]);
    blk_enc(&c, data);
    printf("Encrypted data: %u, %u\n", data[0], data[1]);
    blk_dec(&c, data);
    printf("Decrypted data: %u, %u\n", data[0], data[1]);
    blk_destroy(&c);
    
    return 1;
}
