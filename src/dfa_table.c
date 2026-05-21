#include <stdbool.h>
#include <stdlib.h>
#include "dfa_table.h"

/*

typedef struct dfa_state {
    unsigned short arcs[256];
} dfa_state;

typedef struct dfa_table {
    dfa_state * states;
    unsigned short size;
} dfa_table;

*/

dfa_table * table_new (unsigned short size) {
    dfa_table * this = malloc(sizeof(dfa_table));
    this->states = malloc(sizeof(dfa_state*) * size);

    this->size = size;
    for (int i=0; i<size; i++) {
        this->states[i] = *state_new();
    }

    return this;

}

dfa_state * state_new () {
    dfa_state * this = calloc(1, sizeof(dfa_state));
    return this;
}

bool add_arc(dfa_state * state_from, char arc, dfa_state * state_to) {
    if (state_from->arcs[arc] != 0) return false;

    state_from->arcs[arc] = state_to->id;
}

void table_free (dfa_table * table) {
    for (int i=0; i<table->size; i++) {
        free(table->states);
    }
}