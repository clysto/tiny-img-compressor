#include <inttypes.h>
#include <stdio.h>

#include "dct.h"

int main(void) {
  uint8_t in[512 * 512] = {0};
  int16_t out[512 * 512] = {0};
  int16_t compress_out[512 * 512] = {0};
  // readfrom lenna.dat
  FILE *fp = fopen("lenna.dat", "rb");
  fread(in, 1, 512 * 512, fp);
  fclose(fp);
  imgcompress(512, 512, in, out);
  // writeto lenna1.dat
  fp = fopen("lenna1.dat", "wb");
  fwrite(out, 2, 512 * 512, fp);
  fclose(fp);

  int l = rlencode(out, compress_out, 512 * 512);
  fp = fopen("lenna_compress.dat", "wb");
  fwrite(compress_out, 2, l, fp);
  fclose(fp);

  imgdecompress(512, 512, out, in);
  // writeto lenna2.dat
  fp = fopen("lenna2.dat", "wb");
  fwrite(in, 1, 512 * 512, fp);
  fclose(fp);

  return 0;
}