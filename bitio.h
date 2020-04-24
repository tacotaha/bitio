#ifndef BITIO_H
#define BITIO_H

#include <stdio.h>
#include <stdint.h>

#define BUF_MAX 8
#define BEOF -1

typedef enum Mode { READ, WRITE } MODE;

typedef struct bitio {
  FILE *fp;
  MODE mode;
  uint8_t buff;
  uint8_t buff_ptr;
  uint8_t buff_max;
  int eof:1;
} bitio_t;

// 1 for little endian, 0 for big
static inline int endianness() {
  uint8_t x = 1;
  return (int) (((char *) &x)[0]);
}

static inline void print_bits(uint8_t bits) {
  for (int i = 7; i >= 0; --i)
    printf("%d", (bits >> i) & 1);
  printf(" ");
}

// open a file for reading or writing
bitio_t *bopen(const char *, MODE);

// write a bit to the file
int bwrite(bitio_t *, int);

// read a bit from the file
int bread(bitio_t *);

// close the file (flush buffer & free memory)
void bclose(bitio_t *);

// flush the buffer
int flush(bitio_t *);

#endif                          /* BITIO_H */
