#ifndef __THOMPSON_H__
#define __THOMPSON_H__

typedef struct nfa_expr {
    nfa_node * start;
    nfa_node * end;
} nfa_expr;

typedef struct nfa_node {
    nfa_arc * children;
    int nchildren;
    int refcount;
} nfa_node;

typedef struct nfa_arc {
    nfa_node * to;
    nfa_arc * next_sibling;
    char arc_char;
} nfa_arc;

nfa_expr * expr_new(char c);

void expr_free(nfa_expr * this);

nfa_expr * expr_sequence(nfa_expr * a, nfa_expr * b);

nfa_expr * expr_alternate(nfa_expr * a, nfa_expr * b);

nfa_expr * expr_kleene(nfa_expr * this);

nfa_node * node_new();

nfa_arc * arc_new(nfa_node * from, nfa_node * to, char arc_char);

#endif