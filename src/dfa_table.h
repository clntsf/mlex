#pragma once
#include <stdbool.h>

typedef struct dfa_state {
    unsigned short arcs[256];
} dfa_state;

typedef struct dfa_table {
    dfa_state * states;
    unsigned short size;
} dfa_table;

dfa_table * table_new (unsigned short size);

dfa_table * table_from_states(dfa_state * states, unsigned short size);

void table_free (dfa_table * table);

dfa_state * state_new ();

bool add_arc(dfa_state * state_from, char arc, dfa_state * state_to);