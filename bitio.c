#include <stdlib.h>
#include <stdio.h>
#include "bitio.h"

bitio_t *bopen(const char *fp, MODE mode) {
  bitio_t *b = malloc(sizeof(bitio_t));
  if (b) {
    b->fp = NULL;
    if (fp) {
      b->fp = fopen(fp, mode == READ ? "rb" : "wb");
      if (!b->fp) {
        free(b);
        return NULL;
      }
    }
    b->buff = b->eof = 0;
    b->mode = mode;
    b->buff_max = BUF_MAX;
    b->buff_ptr = mode == READ ? b->buff_max : 0;

  }
  return b;
}

bitio_t *bopen_f(FILE * fp, MODE mode) {
  bitio_t *b = bopen(NULL, mode);
  if (b)
    b->fp = fp;
  return b;
}

int flush(bitio_t * b) {
  if (b->buff_ptr == 0)
    return 0;

  int status = fwrite(&(b->buff), 1, 1, b->fp);
  if (!status) {
    perror("flush");
    exit(status);
  }
  b->buff = b->buff_ptr = 0;
  b->buff_max = BUF_MAX;
  return status;
}

int bwrite(bitio_t * b, int bit) {
  if (b->buff_ptr == b->buff_max)
    flush(b);

  b->buff |= ((bit & 1) << b->buff_ptr++);
  return 1;
}

int bread(bitio_t * b) {
  unsigned char pad_len[2];

  if (b->buff_ptr == b->buff_max) {
    if (b->eof)
      return BEOF;

    // read actual data
    fread(&b->buff, 1, 1, b->fp);

    // try reading the pad length + an extra byte
    fread(pad_len, 1, 2, b->fp);

    // it actually was the pad length
    if (feof(b->fp)) {
      b->eof = 1;
      b->buff_max -= pad_len[0];
    }
    // false alarm, put them back
    else {
      ungetc(pad_len[1], b->fp);
      ungetc(pad_len[0], b->fp);
    }

    b->buff_ptr = 0;
  }

  return (b->buff >> b->buff_ptr++) & 1;
}

void bclose(bitio_t * b) {
  if (b) {
    if (b->mode == WRITE) {
      uint8_t pad_len;
      pad_len = b->buff_max - b->buff_ptr;
      flush(b);

      // last byte of file is pad length 
      fwrite(&pad_len, 1, 1, b->fp);
    }
    fclose(b->fp);
    free(b);
  }
}
