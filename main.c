#include <stdlib.h>
#include <assert.h>

#include "bitio.h"

#define NUMBITS 10000

int main(int argc, char **argv) {
  uint8_t bit = 0;

  uint8_t bits[NUMBITS];

  // Open for writing 
  bitio_t *b = bopen("test.bin", WRITE);

  // Write bits
  for (int i = 0; i < NUMBITS; ++i) {
    bits[i] = rand() & 1;
    bwrite(b, bits[i]);
  }

  // Close file (flush buffer)
  bclose(b);

  // Open for reading
  b = bopen("test.bin", READ);

  // Read bits
  for (int i = 0; i < NUMBITS; ++i) {
    bit = bread(b);
    assert(bit == bits[i]);
  }

  bclose(b);

  return 0;
}
