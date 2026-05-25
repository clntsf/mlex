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
// - charset[0] = charset->bits[0] & 1
// - charset[255] = charset->bits[7] >> 31
// 
// for more detail see the implementation of charset_get
typedef struct { uint64_t bits[4]; } charset;

// returns a new empty (all bits low) charset
charset charset_new();

// returns a new full (all bits high) charset
charset charset_high();

// set bit for char c in set: returns whether c was previously set
int charset_setchar(charset * this, unsigned char c);

// check if char c is in set: boolean (0/1)
int charset_getchar(charset * this, unsigned char c);

// returns a charset corresponding to the regex r"."
charset charset_dot();

// if exactly one bit is set in this charset, return its index, else -1
// empty sets will also return -1, but this can be checked with charset_isempty
int charset_issingle(charset * this);

// returns 1 if this charset is empty, 0 otherwise
bool charset_isempty(charset * this);

// take the union of two charsets
charset charset_union(charset * a, charset * b);

// add a char range (i.e. A-Z) to a charset, in-place
charset * charset_add_range(charset * this, unsigned char lower, unsigned char upper);

// complement a charset in place (high bits become low and vice versa, logical NOT)
charset charset_complement(charset * this);