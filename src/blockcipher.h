typedef unsigned long u_long;
typedef unsigned char u_char;

typedef struct {
	// sub keys
	u_long key[8];
	// sbox vectors
	u_char sb1[16], sb2[16], sb3[16], sb4[16], 
		   sb5[16], sb6[16], sb7[16], sb8[16];
} blk_ctx;

/* Function
 * init -> initialize sbox vectors and generate subkeys
 * enc -> encryptionation
 * dec -> decryptionation
 * destroy -> clean up after yourself
 */

/* Setup and Teardown */
void blk_init(blk_ctx *, u_long *);
void blk_destroy(blk_ctx *);

/* Encryption and Decryption */
void blk_dec(blk_ctx *, u_long *);
void blk_enc(blk_ctx *, u_long *);

/* Private */
u_long blk_scrm(blk_ctx *, u_long);
u_long blk_sub(blk_ctx *, u_long);

