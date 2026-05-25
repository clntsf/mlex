#include "bitset.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// returns a new empty (all bits low) bitset
bitset * bitset_new() {
    bitset c = ;    // TIL about zero-initialization of arrays
    return c;
}

// returns a new full (all bits high) bitset
bitset bitset_high() {
    bitset this = 
    return this;
}   

// check if char c is in set: boolean (0/1)
bool bitset_getchar(bitset * this, unsigned char c) {
    int block = this->bits[c>>6];
    return (block>>(c&63)) & 1;
}

// set bit for char c in set: returns whether c was previously set
bool bitset_setchar(bitset * this, unsigned char c) {
    int block = this->bits[c>>6];
    int was_set = (block>>(c&63)) & 1;

    block |= (1u << (c&63));
    this->bits[c>>6] = block;
    return was_set;
}

// returns a bitset corresponding to the regex r"."
bitset bitset_dot() {
    // magic: 0x00000200 has bit in pos 9 set for 0x09 (horizontal tab)
    // which is recognized by r".". No other character below 0x20 is matched,
    // (LF and CR are specifically NOT matched, and the others are control)
    // and all characters above 0x20 are matched, hence this pattern

    bitset this = {0x00000200, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF};
    return this;
}

// if exactly one bit is set in this bitset, return its index, else -1
// empty sets will also return -1, but this can be checked with bitset_isempty
int bitset_issingle(bitset * this) {
    int idx = -1;
    for (int i=0; i<4; i++) {
        uint64_t block = this->bits[i];
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

// take the union of two bitsets
bitset bitset_union(bitset * a, bitset * b) {
    bitset c = {
        a->bits[0] | b->bits[0],
        a->bits[1] | b->bits[1],
        a->bits[2] | b->bits[2],
        a->bits[3] | b->bits[3]
    };
    return c;
}

// add a char range (i.e. A-Z) inclusive to a bitset, in-place
bitset * bitset_add_range(bitset * this, unsigned char lower, unsigned char upper) {
    if (lower > upper) {
        fprintf(stderr, "Incorrect char bounds (lower>upper): lower %c (%d) > upper %c (%d)\n",
            lower, lower, upper, upper);
        exit(1);
    }

    // iterating with int so we don't have issues with overflow and infinite looping
    // if upper == 255. We could fix this with a guard clause but that's awfully slow
    for (int c = lower; c <= upper; c++) {
        bitset_setchar(this, c);
    }

    return this;
}

// complement a bitset in place (high bits become low and vice versa, logical NOT)
bitset bitset_complement(bitset * this) {
    bitset bitset_compl = {~this->bits[0], ~this->bits[1], ~this->bits[2], ~this->bits[3]};
    return bitset_compl;
}