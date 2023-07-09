#ifndef DCT_H
#define DCT_H

#include <inttypes.h>
#include <stddef.h>

void dct(size_t m, size_t n, uint8_t *in, float *out);
void idct(size_t m, size_t n, float *in, uint8_t *out);
void imgcompress(size_t m, size_t n, uint8_t *in, int16_t *out);
void imgdecompress(size_t m, size_t n, int16_t *in, uint8_t *out);
int rlencode(int16_t *in, int16_t *out, size_t length);
int rldecode(int16_t *in, int16_t *out, size_t length);

#endif
