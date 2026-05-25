#include "bitset.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/*
typedef struct {
    uint64_t * bits;
    size_t size;
} bitset;
*/

// returns a new empty (all bits low) bitset
bitset * bitset_new(unsigned int size) {
    int size_scaled = ((long)size+63)>>6;
    bitset * this = malloc(sizeof(bitset));

    this->size = size;
    this->nblocks = size_scaled;
    this->bits = calloc(size_scaled, sizeof(uint64_t));
    return this;
}

// check if char c is in set: boolean (0/1)
bool bitset_get(bitset * this, unsigned int c) {
    int block = this->bits[c>>6];
    return (block>>(c&63)) & 1;
}

// set bit for char c in set: returns whether c was previously set
bool bitset_set(bitset * this, unsigned int c) {
    int block = this->bits[c>>6];
    int was_set = (block>>(c&63)) & 1;

    block |= (1u << (c&63));
    this->bits[c>>6] = block;
    return was_set;
}

// if exactly one bit is set in this bitset, return its index, else -1
// empty sets will also return -1, but this can be checked with bitset_isempty
int bitset_issingle(bitset * this) {
    int idx = -1;
    for (int i=0; i<this->nblocks; i++) {
        uint64_t block = this->bits[i];
        if (block == 0u) continue;

        for (int j=0; j<64; j++) {
            if (block&1) {
                if (idx != -1) return -1;
                idx = 64*i+j;
            }
            block>>=1;
        }
    }
    return idx;
}

// returns 1 if this bitset is empty, 0 otherwise
bool bitset_isempty(bitset * this) {
    for (int i=0; i<4; i++) {
        if (this->bits[i] != 0u) return 0;
    }
    return 1;
}

// take the union of two bitsets into a new bitset
bitset * bitset_union(bitset * a, bitset * b) {
    if (a->size != b->size) {
        fprintf(stderr, "Bitset size incompatible: Got sizes %d for bitset a and %d for bitset b\n",
            a->size, b->size);
        exit(1);
    }

    bitset * c = bitset_new(a->size);
    int nblocks = a->nblocks;
    for (int i=0; i<nblocks; i++) {
        c->bits[i] = a->bits[i] | b->bits[i];
    }

    return c;
}

// take the union of two bitsets in-place (mutates a)
bitset * bitset_union_inplace(bitset * a, bitset * b) {
    if (a->size != b->size) {
        fprintf(stderr, "Bitset size incompatible: Got sizes %d for bitset a and %d for bitset b\n",
            a->size, b->size);
        exit(1);
    }

    int nblocks = a->nblocks;
    for (int i=0; i<nblocks; i++) {
        a->bits[i] |= b->bits[i];
    }

    return a;
}