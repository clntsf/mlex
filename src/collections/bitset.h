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
    size_t size;
} bitset;

// returns a new empty (all bits low) bitset
bitset bitset_new(size_t size);

// returns a new full (all bits high) bitset
bitset bitset_high(size_t size);

// set bit for bit x in set: returns whether x was previously set
int bitset_set(bitset * this, unsigned int c);

// check if bit x is in set: boolean (0/1)
int bitset_get(bitset * this, unsigned int x);

// if exactly one bit is set in this bitset, return its index, else -1
// empty sets will also return -1, but this can be checked with bitset_isempty
int bitset_issingle(bitset * this);

// returns 1 if this bitset is empty, 0 otherwise
bool bitset_isempty(bitset * this);

// take the union of two bitsets
bitset bitset_union(bitset * a, bitset * b);

// add a char range (i.e. A-Z) to a bitset, in-place
bitset * bitset_add_range(bitset * this, unsigned int lower, unsigned int upper);

// complement a bitset in place (high bits become low and vice versa, logical NOT)
bitset bitset_complement(bitset * this);