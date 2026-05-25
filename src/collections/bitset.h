#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

// bitset for chars for use in our lexer generator.
// 
// data is stored with the LSB of each int being the
// LSB in its range and vice versa, i.e. data is in order
// when each int is read LSB-MSB
// - bitset[0] = bitset->bits[0] & 1
// - bitset[255] = bitset->bits[7] >> 31
// 
// for more detail see the implementation of bitset_get
typedef struct {
    uint64_t * bits;
    unsigned int size;
    unsigned int nblocks;
} bitset;

// returns a new empty (all bits low) bitset
bitset * bitset_new(unsigned int size);

// set bit for bit x in set: returns whether x was previously set
bool bitset_set(bitset * this, unsigned int x);

// check if bit x is in set: boolean (0/1)
bool bitset_get(bitset * this, unsigned int x);

// if exactly one bit is set in this bitset, return its index, else -1
// empty sets will also return -1, but this can be checked with bitset_isempty
int bitset_issingle(bitset * this);

// returns 1 if this bitset is empty, 0 otherwise
bool bitset_isempty(bitset * this);

// take the union of two bitsets
bitset * bitset_union(bitset * a, bitset * b);

// take the union of two bitsets in-place (mutates a)
bitset * bitset_union_inplace(bitset * a, bitset * b);