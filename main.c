#include <stdlib.h>
#include <assert.h>

#include "bitio.h"

#define NUMBITS 10003

int main(int argc, char **argv) {
  int bit = 0, i = 0;
  uint8_t bits[NUMBITS];

  // open for writing 
  bitio_t *b = bopen("test.bin", WRITE);

  // write some bits
  for (int i = 0; i < NUMBITS; ++i) {
    bits[i] = rand() & 1;
    bwrite(b, bits[i]);
  }

  // close file (flush buffer & free mem)
  bclose(b);

  // open for reading
  b = bopen("test.bin", READ);

  // read some bits
  while ((bit = bread(b)) != BEOF)
    // make sure we read the right bits
    assert(bit == bits[i++]);

  // make sure we read the right number of bits
  assert(i == NUMBITS);

  bclose(b);

  return 0;
}
