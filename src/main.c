#include <stdio.h>
#include <stdlib.h>

typedef unsigned long u_long;
typedef unsigned char u_char;

typedef struct {
	// sub keys
	u_long key[8];
	// TODO: sbox vectors
	u_char sb1[8][8], sb2[8][8], sb3[8][8], sb4[8][8];
} blk_ctx;

/* Function
 * init -> initialize sbox vectors and generate subkeys
 * enc -> encryptionation
 * dec -> decryptionation
 * destroy -> clean up after yourself
 */

void blk_init(blk_ctx *, u_long *);
void blk_enc(blk_ctx *, u_long *);
u_long blk_scrm(blk_ctx *, u_long);
void blk_dec(blk_ctx *, u_long *);
void blk_destroy(blk_ctx *);

void blk_init(blk_ctx *ctx, u_long *key){

	int i;
	for(i = 0; i < 8; i++) ctx->key[i] = key[i];
}

u_long blk_scrm(blk_ctx *c, u_long data){

	u_char first = (data & 0xff000000UL) >> 24;
	u_char sec = (data & 0x00ff0000UL) >> 16;
	u_char third = (data & 0x0000ff00UL) >> 8;
	u_char fourth = (data & 0x000000ffUL);

	u_long s1 = first * c->key[0];
	u_long s2 = sec + c->key[1];
	u_long s3 = third + c->key[2];
	u_long s4 = fourth * c->key[3];

	u_long s5 = s1 ^ s3;
	u_long s6 = s2 ^ s4;
	
	u_long s7 = s5 * c->key[4];
	u_long s8 = s6 + s7;
	u_long s9 = s8 * c->key[5];
	
	u_long s10 = s7 + s9;
	
	// I diverge here
	// take a look at this, make sure it works
	
	u_long s11 = (s1 ^ s9) ^ (s2 ^ s10);
	u_long s12 = (s2 ^ s9) ^ (s4 ^ s10);
	
	u_long final = s11 ^ s12;

	return final;
}

void blk_enc(blk_ctx *c, u_long *data){
	register u_long r, l;

	l = data[0];
	r = data[1];
	
	// add more confusion!!
	// and s-boxes!
	r ^= blk_scrm(c, l+c->key[0]); l ^= blk_scrm(c, r+c->key[1]);
	r ^= blk_scrm(c, l+c->key[2]); l ^= blk_scrm(c, r+c->key[3]);
	r ^= blk_scrm(c, l+c->key[4]); l ^= blk_scrm(c, r+c->key[5]);
	r ^= blk_scrm(c, l+c->key[6]); l ^= blk_scrm(c, r+c->key[7]);

	//switch em up so on decryption this 
	//works
	data[0] = r;
	data[1] = l;
}

void blk_dec(blk_ctx *c, u_long *data){
	register u_long r, l;

	l = data[0];
	r = data[1];

	r ^= blk_scrm(c, l+c->key[7]); l ^= blk_scrm(c, r+c->key[6]);
	r ^= blk_scrm(c, l+c->key[5]); l ^= blk_scrm(c, r+c->key[4]);
	r ^= blk_scrm(c, l+c->key[3]); l ^= blk_scrm(c, r+c->key[2]);
	r ^= blk_scrm(c, l+c->key[1]); l ^= blk_scrm(c, r+c->key[0]);

	// right side up now...
	data[0] = r;
	data[1] = l;
}

void blk_destroy(blk_ctx *c){
	int i;
	for(i = 0; i < 8; i++) c->key[i] = 0;
}

int main(){

	//testing...
	
	u_long key[] = {1, 2, 3, 4, 5, 6, 7, 8};
	u_long data[] = {1, 1};
	
	blk_ctx c;

	blk_init(&c, key);
	printf("%lu, %lu\n", data[0], data[1]);
	blk_enc(&c, data);
	printf("%lu, %lu\n", data[0], data[1]);
	blk_dec(&c, data);
	blk_destroy(&c);

	printf("%lu, %lu\n", data[0], data[1]);
	
	return 1;
}
