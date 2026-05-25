#pragma once
#include <stdlib.h>
#include <stdbool.h>
#include "../collections/charset.h"
#include "../collections/bitset.h"

unsigned short _MLEX_NFA_ID_SEED = 0;

typedef struct nfa_arcs { unsigned short arcs[256]; } nfa_arcs;

typedef enum Nodetype_e {
    EMPTY,
    FULL,
} Nodetype;

typedef struct nfa_expr {
    nfa_node * start;
    nfa_node * end;
} nfa_expr;

typedef struct nfa_node {
    union {
        nfa_arcs * arcs;
        bitset * epsilon_closure;
    };
    unsigned short id;
    unsigned short refcount;
    Nodetype type;
    bool accepting;
} nfa_node;

nfa_expr * expr_new(char c);

void expr_free(nfa_expr * this);

nfa_expr * expr_sequence(nfa_expr * a, nfa_expr * b);

nfa_expr * expr_alternate(nfa_expr * a, nfa_expr * b);

nfa_expr * expr_kleene(nfa_expr * this);

nfa_node * node_new();