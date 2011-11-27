#include "blockcipher.h"

void blk_init(blk_ctx *ctx, uint32_t *key){
	int i;
	for(i = 0; i < 8; i++) 
		ctx->key[i] = key[i];

	// S-Boxes taken from the GOST algorithm
	u_char sb1[16] = { 4, 10, 9, 2, 13, 8, 0, 14, 6, 11, 1, 12, 7, 15, 5, 3 };
	u_char sb2[16] = { 14, 11, 4, 12, 6, 13, 15, 10, 2, 3, 8, 1, 0, 7, 5, 9 };
	u_char sb3[16] = { 5, 8, 1, 13, 10, 3, 4, 2, 14, 15, 12, 7, 6, 0, 9, 11 };
	u_char sb4[16] = { 7, 13, 10, 1, 0, 8, 9, 15, 14, 4, 6, 12, 11, 2, 5, 3 };
	u_char sb5[16] = { 6, 12, 7, 1, 5, 15, 13, 8, 4, 10, 9, 14, 0, 2, 11, 2 };
	u_char sb6[16] = { 4, 11, 10, 0, 7, 2, 1, 13, 3, 6, 8, 5, 9, 12, 15, 14 };
	u_char sb7[16] = { 13, 11, 4, 1, 3, 15, 5, 9, 0, 10, 14, 7, 6, 8, 2, 12 };
	u_char sb8[16] = { 1, 15, 13, 0, 5, 7, 10, 4, 9, 2, 3, 14, 6, 11, 8, 12 };

	int j;
	for(j = 0; j < 16; j++){
		ctx->sb1[j] = sb1[j];
		ctx->sb2[j] = sb2[j];
		ctx->sb3[j] = sb3[j];
		ctx->sb4[j] = sb4[j];
		ctx->sb5[j] = sb5[j];
		ctx->sb6[j] = sb6[j];
		ctx->sb7[j] = sb7[j];
		ctx->sb8[j] = sb8[j];
	}
}

uint32_t blk_sub(blk_ctx *c, uint32_t data){

	u_char first  =  (data & 0xf0000000UL) >> 28;
	u_char sec    =  (data & 0x0f000000UL) >> 24;
	u_char third  =  (data & 0x00f00000UL) >> 20;
	u_char fourth =  (data & 0x000f0000UL) >> 16;
	u_char fifth  =  (data & 0x0000f000UL) >> 12;
	u_char sixth  =  (data & 0x00000f00UL) >> 8;
	u_char sev    =  (data & 0x000000f0UL) >> 4;
	u_char eight  =  (data & 0x0000000fUL);

	first  = c->sb1[first];
	sec    = c->sb2[sec];
	third  = c->sb3[third];
	fourth = c->sb4[fourth];
	fifth  = c->sb5[fifth];
	sixth  = c->sb6[sixth];
	sev    = c->sb7[sev];
	eight  = c->sb8[eight];

	uint32_t final = 0x00000000UL;
	
	final |= first  << 28;
	final |= sec    << 24;
	final |= third  << 20;
	final |= fourth << 16;
	final |= fifth  << 12;
	final |= sixth  << 8;
	final |= sev    << 4;
	final |= eight;
	
	// divide the long into 4 chars,
	// divide each char into 2 sets of 4 bits
	// use the first set to get a 4 bit num
	// use the second set to get a 4 bit num
	// combine the two 
	// im thinking 8, 1x16 S boxes, like in GOST

	return final;
}

uint32_t blk_scrm(blk_ctx *c, uint32_t data){

	// Optimize this
	u_char first  = (data & 0xff000000UL) >> 24;
	u_char sec    = (data & 0x00ff0000UL) >> 16;
	u_char third  = (data & 0x0000ff00UL) >> 8;
	u_char fourth = (data & 0x000000ffUL);

	uint32_t s1 = first * c->key[0];
	uint32_t s2 = sec + c->key[1];
	uint32_t s3 = third + c->key[2];
	uint32_t s4 = fourth * c->key[3];

	uint32_t s5 = s1 ^ s3;
	uint32_t s6 = s2 ^ s4;
	
	uint32_t s7 = s5 * c->key[4];
	uint32_t s8 = s6 + s7;
	uint32_t s9 = s8 * c->key[5];
	
	uint32_t s10 = s7 + s9;
	
	// take a look at this, make sure it works
	// incorporate the last two keys?
	
	uint32_t s11 = (s1 ^ s9) ^ (s2 ^ s10);
	uint32_t s12 = (s2 ^ s9) ^ (s4 ^ s10);
	
	uint32_t final = (s11 ^ s12);	

	// sbox step here
	uint32_t ret = blk_sub(c, final);
	
	return ret;
	
	// any higher than 2, and it
	// adds to much symmetry...
	//return final << 2 | final >> (32-2);
}

void blk_enc(blk_ctx *c, uint32_t *data){
	register uint32_t r, l;

	l = data[0];
	r = data[1];
	
	// add s-boxes!
	r ^= blk_scrm(c, l+c->key[0]); l ^= blk_scrm(c, r+c->key[1]);
	r ^= blk_scrm(c, l+c->key[2]); l ^= blk_scrm(c, r+c->key[3]);
	r ^= blk_scrm(c, l+c->key[4]); l ^= blk_scrm(c, r+c->key[5]);
	r ^= blk_scrm(c, l+c->key[6]); l ^= blk_scrm(c, r+c->key[7]);

	r ^= blk_scrm(c, l+c->key[7]); l ^= blk_scrm(c, r+c->key[6]);
	r ^= blk_scrm(c, l+c->key[5]); l ^= blk_scrm(c, r+c->key[4]);
	r ^= blk_scrm(c, l+c->key[3]); l ^= blk_scrm(c, r+c->key[2]);
	r ^= blk_scrm(c, l+c->key[1]); l ^= blk_scrm(c, r+c->key[0]);

	//switch em up so on decryption this 
	//works
	data[0] = r;
	data[1] = l;
}

void blk_dec(blk_ctx *c, uint32_t *data){
	register uint32_t r, l;

	l = data[0];
	r = data[1];

	r ^= blk_scrm(c, l+c->key[0]); l ^= blk_scrm(c, r+c->key[1]);
	r ^= blk_scrm(c, l+c->key[2]); l ^= blk_scrm(c, r+c->key[3]);
	r ^= blk_scrm(c, l+c->key[4]); l ^= blk_scrm(c, r+c->key[5]);
	r ^= blk_scrm(c, l+c->key[6]); l ^= blk_scrm(c, r+c->key[7]);

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
	for(i = 0; i < 8; i++) 
		c->key[i] = 0;
}

