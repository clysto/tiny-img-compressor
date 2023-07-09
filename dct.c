#include <math.h>
#include <stdio.h>
#include <stdlib.h>

uint8_t QUANT[8][8] = {{16, 11, 10, 16, 24, 40, 51, 61},     {12, 12, 14, 19, 26, 58, 60, 55},
                       {14, 13, 16, 24, 40, 57, 69, 56},     {14, 17, 22, 29, 51, 87, 80, 62},
                       {18, 22, 37, 56, 68, 109, 103, 77},   {24, 35, 55, 64, 81, 104, 113, 92},
                       {49, 64, 78, 87, 103, 121, 120, 101}, {72, 92, 95, 98, 112, 100, 103, 99}};

void dct(size_t m, size_t n, uint8_t *in, float *out) {
  int i, j, u, v;
  float k, csum = 0;
  for (i = 0; i < m; i++) {
    for (j = 0; j < n; j++) {
      for (u = 0; u < m; u++)
        for (v = 0; v < n; v++)
          csum += ((float)in[u * m + v] - 128) * cos((2 * u + 1) * i * M_PI / (2 * m)) *
                  cos((2 * v + 1) * j * M_PI / (2 * n));
      if (!i && !j)
        k = 0.5;
      else if ((!i && j) || (i && !j))
        k = 1 / sqrt(2);
      else
        k = 1;
      out[i * m + j] = 2 * csum * k / n;
      csum = 0;
    }
  }
}

void idct(size_t m, size_t n, float *in, uint8_t *out) {
  int i, j, u, v;
  float k, csum = 0;
  for (i = 0; i < m; i++) {
    for (j = 0; j < n; j++) {
      for (u = 0; u < m; u++)
        for (v = 0; v < n; v++) {
          if (!u && !v)
            k = 0.5;
          else if ((!u && v) || (u && !v))
            k = 1 / sqrt(2);
          else
            k = 1;
          csum += k * in[u * m + v] * cos((2 * i + 1) * M_PI * u / (2 * m)) * cos((2 * j + 1) * M_PI * v / (2 * n));
        }
      out[i * m + j] = round(csum * 2 / n) + 128;
      csum = 0;
    }
  }
}

void imgcompress(size_t m, size_t n, uint8_t *in, int16_t *out) {
  int w = ceil(m / 8.0), h = ceil(n / 8.0);
  int u, v;
  uint8_t blk[64];
  float dctblk[64];
  for (u = 0; u < w; u++) {
    for (v = 0; v < h; v++) {
      for (int i = 0; i < 8; i++)
        for (int j = 0; j < 8; j++) blk[i * 8 + j] = in[(u * 8 + i) * m + (v * 8 + j)];
      dct(8, 8, blk, dctblk);
      for (int i = 0; i < 8; i++)
        for (int j = 0; j < 8; j++) out[(u * 8 + i) * m + (v * 8 + j)] = round(dctblk[i * 8 + j] / QUANT[i][j]);
    }
  }
}

void imgdecompress(size_t m, size_t n, int16_t *in, uint8_t *out) {
  int w = ceil(m / 8.0), h = ceil(n / 8.0);
  int u, v;
  float dctblk[64];
  uint8_t blk[64];
  for (u = 0; u < w; u++) {
    for (v = 0; v < h; v++) {
      for (int i = 0; i < 8; i++)
        for (int j = 0; j < 8; j++) dctblk[i * 8 + j] = in[(u * 8 + i) * m + (v * 8 + j)] * QUANT[i][j];
      idct(8, 8, dctblk, blk);
      for (int i = 0; i < 8; i++)
        for (int j = 0; j < 8; j++) out[(u * 8 + i) * m + (v * 8 + j)] = blk[i * 8 + j];
    }
  }
}

int rlencode(int16_t *in, int16_t *out, size_t length) {
  int i, j, k, l, cnt = 0;
  for (i = 0; i < length; i++) {
    if (in[i] == 0) {
      for (j = i + 1; j < length; j++) {
        if (in[j] != 0) break;
      }
      out[cnt++] = 0;
      out[cnt++] = j - i;
      i = j - 1;
    } else {
      out[cnt++] = in[i];
    }
  }
  return cnt;
}

int rldecode(int16_t *in, int16_t *out, size_t length) {
  int i, j, k, l, cnt = 0;
  for (i = 0; i < length; i++) {
    if (in[i] == 0) {
      for (j = 0; j < in[i + 1]; j++) {
        out[cnt++] = 0;
      }
      i++;
    } else {
      out[cnt++] = in[i];
    }
  }
  return cnt;
}
