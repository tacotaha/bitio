#ifndef BITIO_H
#define BITIO_H

#include <stdio.h>
#include <stdint.h>

#define BUF_MAX 8
#define BEOF -1

typedef enum Mode { READ, WRITE } MODE;

typedef struct bitio {
  FILE *fp;
  uint8_t buff;
  uint8_t buff_len;
} bitio_t;

// 0 for little endian, 1 for big
static inline int endianness() {
  uint8_t x = 1;
  return (int) (((char *) &x)[0]);
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
