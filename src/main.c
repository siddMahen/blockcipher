#include <stdio.h>
#include "blockcipher.h"

int main(){

    // Tests:
    uint32_t key[] = { 1, 2, 3, 4, 5, 6, 7, 8 };
    uint32_t data[] = { 1, 1 };
    
    printf("Key: ");
    int i;
    for(i = 0; i < 8; i++)
    	printf("%u", key[i]);
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
