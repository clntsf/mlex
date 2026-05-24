#include "thompson.h"
#include <stdlib.h>

/* from thompson.h
    typedef struct nfa_expr {
        nfa_node * start;
        nfa_node * end;
    } nfa_expr;

    typedef struct nfa_node {
        nfa_arc * children;
        int nchildren;
        int refcount
    } nfa_node;

    typedef struct nfa_arc {
        nfa_node * to;
        nfa_arc * next_sibling;
        char arc_char;
    } nfa_arc;
*/

// roughly: https://www.scss.tcd.ie/John.Waldron/CSU33071/Lectures/reglang.pdf

nfa_expr * expr_new(char c) {
    nfa_expr * this = malloc(sizeof(nfa_expr));
    this->start = node_new();
    this->end = node_new();
    arc_new(this->start, this->end, c);
    return this;
}

void expr_free(nfa_expr * this) {
    _node_free_rec(this->start);
    free(this);
}

void expr_free(nfa_expr * this) {
    _node_free_rec(this->start);
    free(this);
}

// this is the worst part of the code – I hate mutual recursion
// especially where memory management is concerned – and refcounts
// make it even worse. these two funcs alone make me want to just
// implement the dfa version.

// we have to maintain refcounts in our nodes for this specific reason:
// since the graph can contain nodes with multiple parents we need to prevent
// double-frees
void _node_free_rec(nfa_node * node) {
    _childlist_free(node->children);
    if (node->refcount == 0) {
        free(node);
    }
}

void _childlist_free(nfa_arc * child) {
    if (child == NULL) return;
    _childlist_free(child->next_sibling);
    child->to->refcount--;
    _node_free_rec(child->to);
    free(child);
}

nfa_expr * expr_sequence(nfa_expr * a, nfa_expr * b) {

    // point the end of a towards the children of b's start
    // a->end won't have any children to begin with, which we can
    // guarantee by properly writing our construction algorithm
    a->end->children = b->start->children;
    a->end->nchildren = b->start->nchildren;

    // get rid of now-extraneous expr b (including duplicate start node)
    // and absorb the sequenced nodes into a
    free(b->start);
    a->end = b->end;
    free(b);

    return a;
}

nfa_expr * expr_alternate(nfa_expr * a, nfa_expr * b) {
    nfa_node * nstart = node_new();
    nfa_node * nend = node_new();

    // empty edges to both, see slide 25
    arc_new(nstart, a->start, 0);
    arc_new(nstart, b->start, 0);

    arc_new(a->end, nend, 0);
    arc_new(b->end, nend, 0);

    a->start = nstart;
    a->end = nend;
    free(b);
    return a;
}

nfa_expr * expr_kleene(nfa_expr * this) {

    // slightly modified version of diagram on slide 26
    // we are skipping the added initial node since we don't
    // need it here. We keep the new end node to keep our promise
    // that expr->end will always be childless.
    nfa_node * nend = node_new();

    arc_new(this->end, nend, 0);
    arc_new(this->start, nend, 0);
    arc_new(this->end, this->start, 0);

    this->end = nend;
    return this;
}

nfa_node * node_new() {
    nfa_node * this = malloc(sizeof(nfa_node));
    this->nchildren = 0;
}

nfa_arc * arc_new(nfa_node * from, nfa_node * to, char arc_char);