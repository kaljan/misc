#include <stdint.h>
#include <string.h>

#define ROTR(a,b) (((a) >> (b)) | ((a) << (32-(b))))

#define CH(x,y,z) (((x) & (y)) ^ (~(x) & (z)))
#define MAJ(x,y,z) (((x) & (y)) ^ ((x) & (z)) ^ ((y) & (z)))
#define EP0(x) (ROTR(x,2) ^ ROTR(x,13) ^ ROTR(x,22))
#define EP1(x) (ROTR(x,6) ^ ROTR(x,11) ^ ROTR(x,25))
#define SIG0(x) (ROTR(x,7) ^ ROTR(x,18) ^ ((x) >> 3))
#define SIG1(x) (ROTR(x,17) ^ ROTR(x,19) ^ ((x) >> 10))

static const uint32_t k[64] = {
	0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 
	0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
	0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 
	0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
	0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 
	0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
	0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 
	0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
	0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13,
	0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
	0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3,
	0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
	0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5,
	0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
	0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208,
	0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
};

static const uint32_t initial_state[8] = {
	0x6A09E667,	0xBB67AE85, 0x3C6EF372, 0xA54FF53A, 
	0x510E527F, 0x9B05688C, 0x1F83D9AB, 0x5BE0CD19  
};

static uint32_t tr_state[8];
static uint8_t tr_data[64];
static size_t datalen = 0;
static size_t dataleft = 0;

static inline void transform(void)
{
	uint32_t a, b, c, d, e, f, g, h, i, t1, t2, m[64];
	
	uint32_t x;
	for (i = 0; i < 16; ++i) {
		x = i << 2;
		m[i] = ((tr_data[x]) << 24) | ((tr_data[x | 1]) << 16) |
			((tr_data[x | 2]) << 8) | ((tr_data[x | 3]));
	}

	
	for (; i < 64; ++i) {
		m[i] = SIG1(m[i - 2]) + m[i - 7] + 
			SIG0(m[i - 15]) + m[i - 16];
	}
	
	a = tr_state[0];
	b = tr_state[1];
	c = tr_state[2];
	d = tr_state[3];
	e = tr_state[4];
	f = tr_state[5];
	g = tr_state[6];
	h = tr_state[7];
	
	for (i = 0; i < 64; i++) {
		t1 = h + EP1(e) + CH(e, f, g) + k[i] + m[i];
		t2 = EP0(a) + MAJ(a, b, c);
		h = g;
		g = f;
		f = e;
		e = d + t1;
		d = c;
		c = b;
		b = a;
		a = t1 + t2;
	}

	tr_state[0] += a;
	tr_state[1] += b;
	tr_state[2] += c;
	tr_state[3] += d;
	tr_state[4] += e;
	tr_state[5] += f;
	tr_state[6] += g;
	tr_state[7] += h;
}

void sha256(const uint8_t *data, size_t size, uint8_t *md) {
	size_t bl, i;
	
	if (!data) {
		dataleft = 0;
		datalen = 0;
	}

	if ((dataleft == 0) && (datalen == 0)) {
		for (i = 0; i < 8; i++) {
			tr_state[i] = initial_state[i];
		}
	}

	for (i = 0; i < size; i++) {
		tr_data[datalen++] = data[i];
		if (datalen == 64) {
			transform();
			datalen = 0;
			dataleft += 64;
		}
	}
	
	if (md == NULL) {
		return;
	}

	i = datalen;
	
	
	if (datalen < 56) {
		tr_data[i++] = 0x80;
		while (i < 56) {
			tr_data[i++] = 0;
		}
	} else {
		tr_data[i++] = 0x80;
		while (i < 64) {
			tr_data[i++] = 0;
		}
		transform();
		memset(tr_data, 0, 56);
	}

	bl = (datalen + dataleft) * 8;

	i = 63;
	while (i > 55) {
		tr_data[i--] = ((uint8_t)(bl & 0x000000ff));
		bl >>= 8;
	}
	
	transform();
	
	for (i = 0; i < 4; ++i) {
		md[i]      = (tr_state[0] >> (24 - i * 8)) & 0x000000ff;
		md[i + 4]  = (tr_state[1] >> (24 - i * 8)) & 0x000000ff;
		md[i + 8]  = (tr_state[2] >> (24 - i * 8)) & 0x000000ff;
		md[i + 12] = (tr_state[3] >> (24 - i * 8)) & 0x000000ff;
		md[i + 16] = (tr_state[4] >> (24 - i * 8)) & 0x000000ff;
		md[i + 20] = (tr_state[5] >> (24 - i * 8)) & 0x000000ff;
		md[i + 24] = (tr_state[6] >> (24 - i * 8)) & 0x000000ff;
		md[i + 28] = (tr_state[7] >> (24 - i * 8)) & 0x000000ff;
	}
	
	datalen = 0;
	dataleft = 0;
}
