#pragma once

typedef enum Nodetype_e {
    SEQUENCE,
    ALTERNATION,
    CLASS,
    KLEENE
} Nodetype;

typedef struct dfa_tree_node {
    Nodetype type;
    union {
        dfa_tree_node * kleene_child;
        struct branch {
            dfa_tree_node * a;
            dfa_tree_node * b;
        } branch;
        char * chars;
    };
} dfa_tree_node;