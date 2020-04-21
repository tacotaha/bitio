#include <stdlib.h>
#include <stdio.h>
#include "bitio.h"

bitio_t *bopen(const char *fp, MODE mode) {
  bitio_t *b = malloc(sizeof(bitio_t));
  if (b) {
    b->fp = fopen(fp, mode == READ ? "rb" : "wb");
    if (!b->fp) {
      free(b);
      return NULL;
    }
    b->buff = b->buff_len = 0;
  }
  return b;
}

int flush(bitio_t * b) {
  if (b->buff_len == 0)
    return 0;

  int status = fwrite(&(b->buff), 1, 1, b->fp);
  if (!status) {
    perror("Failed writing to file\n");
    exit(status);
  }
  b->buff = b->buff_len = 0;
  return status;
}

int bwrite(bitio_t * b, int bit) {
  if (b->buff_len == BUF_MAX)
    flush(b);

  b->buff |= ((bit & 1) << b->buff_len++);

  return 0;
}

int bread(bitio_t * b) {
  uint8_t sf;

  // buffer empty -> fill it
  if (b->buff_len == 0) {
    if (feof(b->fp))
      return BEOF;
    fread(&b->buff, 1, 1, b->fp);
    b->buff_len = BUF_MAX;
  }

  --b->buff_len;

  // read in reverse if little endian
  sf = endianness()? BUF_MAX - b->buff_len - 1 : b->buff_len;

  return (b->buff & (1 << sf)) >> sf;
}

void bclose(bitio_t * b) {
  if (b) {
    flush(b);
    fclose(b->fp);
    free(b);
  }
}
