#include <inttypes.h>

typedef unsigned char u_char;

typedef struct {
	// sub keys
	uint32_t key[8];
	// sbox vectors
	unsigned char sb1[16], sb2[16], sb3[16], sb4[16], 
				  sb5[16], sb6[16], sb7[16], sb8[16];
} blk_ctx;

/* Function
 * init -> initialize sbox vectors and generate subkeys
 * enc -> encryptionation
 * dec -> decryptionation
 * destroy -> clean up after yourself
 */

/* Setup and Teardown */
void blk_init(blk_ctx *, uint32_t *);
void blk_destroy(blk_ctx *);

/* Encryption and Decryption */
void blk_dec(blk_ctx *, uint32_t *);
void blk_enc(blk_ctx *, uint32_t *);

/* Internal */
uint32_t blk_scrm(blk_ctx *, uint32_t);
uint32_t blk_sub(blk_ctx *, uint32_t);
