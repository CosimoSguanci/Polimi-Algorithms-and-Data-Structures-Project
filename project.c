#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define STRING_LENGTH 1000
#define RED '1'
#define BLACK '0'

#define DELETED '1'
#define NOT_DELETED '0'

#define PRINTED '1'
#define NOT_PRINTED '0'

typedef struct entity entity_node_rb;
typedef struct ent_rbtree entity_rbtree;

typedef struct relation rel_node;
typedef struct rel_rbtree rel_rbtree;

typedef struct max_entranti_ent_node max_entranti_ent_node;
typedef struct max_entranti_ent_rbtree max_entranti_ent_rbtree;

typedef struct rel_entranti_inst_node rel_entranti_inst_node;
typedef struct rel_entranti_inst_rbtree rel_entranti_inst_rbtree;

typedef struct ent_rel_node ent_rel_node;
typedef struct ent_rel_rbtree ent_rel_rbtree;

typedef struct entity_data entity_data;

char deleted;
char printed;

struct entity_data {
    char id[STRING_LENGTH];
};

struct entity {
    entity_data *data;
    struct entity *parent;
    struct entity *left;
    struct entity *right;
    char color;
};

struct ent_rbtree {
    entity_node_rb *root;
    entity_node_rb *NIL;
};

struct max_entranti_ent_node {
    entity_data *entity;
    struct max_entranti_ent_node *parent;
    struct max_entranti_ent_node *left;
    struct max_entranti_ent_node *right;
    char color;
};

struct max_entranti_ent_rbtree {
    max_entranti_ent_node *root;
    max_entranti_ent_node *NIL;
};


struct rel_entranti_inst_node {
    entity_data *orig;
    struct rel_entranti_inst_node *parent;
    struct rel_entranti_inst_node *left;
    struct rel_entranti_inst_node *right;
    char color;
};

struct rel_entranti_inst_rbtree {
    rel_entranti_inst_node *root;
    rel_entranti_inst_node *NIL;
};

struct ent_rel_node {
    entity_data *entity;
    struct ent_rel_node *parent;
    struct ent_rel_node *left;
    struct ent_rel_node *right;
    char color;

    unsigned short num_entr;
    rel_entranti_inst_rbtree *entranti_rbtree;
};

struct ent_rel_rbtree {
    ent_rel_node *root;
    ent_rel_node *NIL;
};

struct relation {
    char id[STRING_LENGTH]; // key
    struct relation *parent;
    struct relation *left;
    struct relation *right;
    char color;

    unsigned short max_entranti_num;
    max_entranti_ent_rbtree *max_entranti_rb;
    ent_rel_rbtree *ent_tree;

};

struct rel_rbtree {
    rel_node *root;
    rel_node *NIL;
};




entity_rbtree *rb_entity_create() {
    entity_rbtree *rb = malloc(sizeof(entity_rbtree));
    rb -> NIL = malloc(sizeof(entity_node_rb));
    rb -> NIL -> color = BLACK;
    rb -> root = rb -> NIL;
    return rb;
}

rel_rbtree *rb_relation_create() {
    rel_rbtree *rb = malloc(sizeof(rel_rbtree));
    rb -> NIL = malloc(sizeof(rel_node));
    rb -> NIL -> color = BLACK;
    rb -> root = rb -> NIL;
    return rb;
}

max_entranti_ent_rbtree *rb_max_entranti_create() {
    max_entranti_ent_rbtree *rb = malloc(sizeof(max_entranti_ent_rbtree));
    rb -> NIL = malloc(sizeof(max_entranti_ent_node));
    rb -> NIL -> color = BLACK;
    rb -> root = rb -> NIL;
    return rb;
}

ent_rel_rbtree *rb_ent_rel_create() {
    ent_rel_rbtree *rb = malloc(sizeof(ent_rel_rbtree));
    rb -> NIL = malloc(sizeof(ent_rel_node));
    rb -> NIL -> color = BLACK;
    rb -> root = rb -> NIL;
    return rb;
}

rel_entranti_inst_rbtree *rb_rel_entranti_create() {
    rel_entranti_inst_rbtree *rb = malloc(sizeof(rel_entranti_inst_rbtree));
    rb -> NIL = malloc(sizeof(rel_entranti_inst_node));
    rb -> NIL -> color = BLACK;
    rb -> root = rb -> NIL;
    return rb;
}

void left_rotate_rel_entranti_inst(rel_entranti_inst_rbtree *rb, rel_entranti_inst_node *node) {
    rel_entranti_inst_node *y = node -> right;
    node -> right = y -> left;
    if(y -> left != rb -> NIL) {
        y -> left -> parent = node;
    }
    y -> parent = node -> parent;
    if(node -> parent == rb -> NIL) {
        rb -> root = y;
    }
    else if(node == node -> parent -> left) {
        node -> parent -> left = y;
    }
    else {
        node -> parent -> right = y;
    }
    y -> left = node;
    node -> parent = y;
}

void right_rotate_rel_entranti_inst(rel_entranti_inst_rbtree *rb, rel_entranti_inst_node *node) {
    rel_entranti_inst_node *y = node -> left;
    node -> left = y -> right;
    if(y -> right != rb -> NIL) {
        y -> right -> parent = node;
    }
    y -> parent = node -> parent;
    if(node -> parent == rb -> NIL) {
        rb -> root = y;
    }
    else if(node == node -> parent -> right) {
        node -> parent -> right = y;
    }
    else {
        node -> parent -> left = y;
    }
    y -> right = node;
    node -> parent = y;
}

void rb_insert_fixup_rel_entranti_inst(rel_entranti_inst_rbtree *rb, rel_entranti_inst_node *new) {

    if (new == rb->root){
        rb -> root -> color = BLACK;
    }
    else {
        rel_entranti_inst_node *x = new->parent;
        if (x->color == RED) {
            if (x == x->parent->left) {
                rel_entranti_inst_node *y = x->parent->right;
                if (y->color == RED) {
                    x->color = BLACK;
                    y->color = BLACK;
                    x->parent->color = RED;
                    rb_insert_fixup_rel_entranti_inst(rb, x->parent);
                } else {
                    if (new == x->right) {
                        new = x;
                        left_rotate_rel_entranti_inst(rb, new);
                        x = new -> parent;
                    }

                    x->color = BLACK;
                    x->parent->color = RED;
                    right_rotate_rel_entranti_inst(rb, x->parent);
                }
            } else {
                if (x == x->parent->right) {
                    rel_entranti_inst_node *y = x->parent->left;
                    if (y->color == RED) {
                        x->color = BLACK;
                        y->color = BLACK;
                        x->parent->color = RED;
                        rb_insert_fixup_rel_entranti_inst(rb, x->parent);
                    } else {
                        if (new == x->left) {
                            new = x;
                            right_rotate_rel_entranti_inst(rb, new);
                            x = new -> parent;
                        }

                        x->color = BLACK;
                        x->parent->color = RED;
                        left_rotate_rel_entranti_inst(rb, x->parent);
                    }
                }
            }
        }
    }
}

rel_entranti_inst_node *rb_insert_rel_entranti_inst(rel_entranti_inst_rbtree *rb, entity_node_rb *orig) {
    rel_entranti_inst_node *y = rb -> NIL;
    rel_entranti_inst_node *x = rb -> root;

    while(x != rb -> NIL) {
        y = x;
        if(strcmp(orig -> data -> id, x -> orig -> id) < 0)
            x = x -> left;
        else
            x = x -> right;
    }

    rel_entranti_inst_node *new = malloc(sizeof(rel_entranti_inst_node));
    new -> orig = orig -> data;
    new -> parent = y;
    if(y == rb -> NIL)
        rb -> root = new;
    else if(strcmp(new->orig->id, y->orig->id) < 0)
        y -> left = new;
    else
        y -> right = new;
    new -> left = rb -> NIL;
    new -> right = rb -> NIL;
    new -> color = RED;
    rb_insert_fixup_rel_entranti_inst(rb, new);
    return new;
}


rel_entranti_inst_node* rb_minimum_rel_entranti_inst(rel_entranti_inst_rbtree *rb, rel_entranti_inst_node *x) {
    while(x -> left != rb -> NIL) {
        x = x -> left;
    }
    return x;
}

void rb_delete_fixup_rel_entranti_inst(rel_entranti_inst_rbtree *rb, rel_entranti_inst_node *x) {

    if(x -> color == RED || x -> parent == rb -> NIL) {
        x -> color = BLACK;
    }
    else if(x == x -> parent -> left) {
        rel_entranti_inst_node *w = x -> parent -> right;
        if(w -> color == RED){
            w -> color = BLACK;
            x -> parent -> color = RED;
            left_rotate_rel_entranti_inst(rb, x -> parent);
            w = x -> parent -> right;
        }
        if(w -> left -> color == BLACK && w -> right -> color == BLACK) {
            w -> color = RED;
            rb_delete_fixup_rel_entranti_inst(rb, x -> parent);
        }
        else {
            if (w->right->color == BLACK) {
                w->left->color = BLACK;
                w->color = RED;
                right_rotate_rel_entranti_inst(rb, w);
                w = x->parent->right;
            }
            w -> color = x -> parent -> color;
            x -> parent -> color = BLACK;
            w -> right -> color = BLACK;
            left_rotate_rel_entranti_inst(rb, x -> parent);
        }
    }
    else {
        rel_entranti_inst_node *w = x -> parent -> left;
        if(w -> color == RED){
            w -> color = BLACK;
            x -> parent -> color = RED;
            right_rotate_rel_entranti_inst(rb, x -> parent);
            w = x -> parent -> left;
        }
        if(w -> right -> color == BLACK && w -> left -> color == BLACK) {
            w -> color = RED;
            rb_delete_fixup_rel_entranti_inst(rb, x -> parent);
        }
        else {
            if (w->left->color == BLACK) {
                w->right->color = BLACK;
                w->color = RED;
                left_rotate_rel_entranti_inst(rb, w);
                w = x->parent->left;
            }
            w -> color = x -> parent -> color;
            x -> parent -> color = BLACK;
            w -> left -> color = BLACK;
            right_rotate_rel_entranti_inst(rb, x -> parent);
        }
    }
}

rel_entranti_inst_node *tree_successor_rel_entranti_inst(rel_entranti_inst_rbtree *rb, rel_entranti_inst_node *x) {
    if(x -> right != rb -> NIL)
        return rb_minimum_rel_entranti_inst(rb, x -> right);
    rel_entranti_inst_node *y = x -> parent;
    while(y != rb -> NIL && x == y -> right) {
        x = y;
        y = y -> parent;
    }
    return y;
}

void rb_delete_rel_entranti_inst(rel_entranti_inst_rbtree *rb, rel_entranti_inst_node *z) {
    rel_entranti_inst_node *y;
    rel_entranti_inst_node *x;
    if(z -> left == rb -> NIL || z -> right == rb -> NIL) {
        y = z;
    }
    else y = tree_successor_rel_entranti_inst(rb, z);
    if(y -> left != rb -> NIL) {
        x = y -> left;
    }
    else x = y -> right;

    x->parent = y->parent;

    if (y -> parent == rb -> NIL) {
        rb -> root = x;
    }
    else if(y == y -> parent -> left) {
        y -> parent -> left = x;
    }
    else y -> parent -> right = x;

    if(y != z) {
        z -> orig = y -> orig;
    }

    if(y -> color == BLACK)
        rb_delete_fixup_rel_entranti_inst(rb, x);

    free(y);
}

rel_entranti_inst_node *search_rel_entranti_inst(rel_entranti_inst_rbtree *rb, rel_entranti_inst_node *node, char *id) {
    if(node == rb -> NIL || strcmp(node -> orig -> id, id) == 0){
        return node;
    }
    if(strcmp(id, node -> orig -> id) < 0) {
        return search_rel_entranti_inst(rb, node -> left, id);
    }
    else {
        return search_rel_entranti_inst(rb, node -> right, id);
    }
}

void search_rel_entranti_inst_and_delete(rel_entranti_inst_rbtree *rb, rel_entranti_inst_node *node, char *id) {
    if(node == rb -> NIL)
        return;
    if(strcmp(node -> orig -> id, id) == 0){
        rb_delete_rel_entranti_inst(rb, node);
        return;
    }
    if(strcmp(id, node -> orig -> id) < 0) {
        search_rel_entranti_inst_and_delete(rb, node -> left, id);
    }
    else {
        search_rel_entranti_inst_and_delete(rb, node -> right, id);
    }
}

void left_rotate_ent_rel(ent_rel_rbtree *rb, ent_rel_node *node) {
    ent_rel_node *y = node -> right;
    node -> right = y -> left;
    if(y -> left != rb -> NIL) {
        y -> left -> parent = node;
    }
    y -> parent = node -> parent;
    if(node -> parent == rb -> NIL) {
        rb -> root = y;
    }
    else if(node == node -> parent -> left) {
        node -> parent -> left = y;
    }
    else {
        node -> parent -> right = y;
    }
    y -> left = node;
    node -> parent = y;
}

void right_rotate_ent_rel(ent_rel_rbtree *rb, ent_rel_node *node) {
    ent_rel_node *y = node -> left;
    node -> left = y -> right;
    if(y -> right != rb -> NIL) {
        y -> right -> parent = node;
    }
    y -> parent = node -> parent;
    if(node -> parent == rb -> NIL) {
        rb -> root = y;
    }
    else if(node == node -> parent -> right) {
        node -> parent -> right = y;
    }
    else {
        node -> parent -> left = y;
    }
    y -> right = node;
    node -> parent = y;
}

void rb_insert_fixup_ent_rel(ent_rel_rbtree *rb, ent_rel_node *new) {

    if (new == rb->root){
        rb -> root -> color = BLACK;
    }
    else {
        ent_rel_node *x = new->parent;
        if (x->color == RED) {
            if (x == x->parent->left) {
                ent_rel_node *y = x->parent->right;
                if (y->color == RED) {
                    x->color = BLACK;
                    y->color = BLACK;
                    x->parent->color = RED;
                    rb_insert_fixup_ent_rel(rb, x->parent);
                } else {
                    if (new == x->right) {
                        new = x;
                        left_rotate_ent_rel(rb, new);
                        x = new -> parent;
                    }

                    x->color = BLACK;
                    x->parent->color = RED;
                    right_rotate_ent_rel(rb, x->parent);
                }
            } else {
                if (x == x->parent->right) {
                    ent_rel_node *y = x->parent->left;
                    if (y->color == RED) {
                        x->color = BLACK;
                        y->color = BLACK;
                        x->parent->color = RED;
                        rb_insert_fixup_ent_rel(rb, x->parent);
                    } else {
                        if (new == x->left) {
                            new = x;
                            right_rotate_ent_rel(rb, new);
                            x = new -> parent;
                        }

                        x->color = BLACK;
                        x->parent->color = RED;
                        left_rotate_ent_rel(rb, x->parent);
                    }
                }
            }
        }
    }
}

ent_rel_node *rb_insert_ent_rel(ent_rel_rbtree *rb, entity_node_rb *entity) {
    ent_rel_node *y = rb -> NIL;
    ent_rel_node *x = rb -> root;

    while(x != rb -> NIL) {
        y = x;
        if(strcmp(entity -> data -> id, x -> entity -> id) < 0)
            x = x -> left;
        else
            x = x -> right;
    }

    ent_rel_node *new = malloc(sizeof(ent_rel_node));
    new -> entity = entity -> data;
    new -> parent = y;
    if(y == rb -> NIL)
        rb -> root = new;
    else if(strcmp(new->entity->id, y->entity->id) < 0)
        y -> left = new;
    else
        y -> right = new;
    new -> left = rb -> NIL;
    new -> right = rb -> NIL;
    new -> color = RED;
    new -> entranti_rbtree = rb_rel_entranti_create();
    new -> num_entr = 0;
    rb_insert_fixup_ent_rel(rb, new);
    return new;
}


ent_rel_node* rb_minimum_ent_rel(ent_rel_rbtree *rb, ent_rel_node *x) {
    while(x -> left != rb -> NIL) {
        x = x -> left;
    }
    return x;
}

void rb_delete_fixup_ent_rel(ent_rel_rbtree *rb, ent_rel_node *x) {

    if(x -> color == RED || x -> parent == rb -> NIL) {
        x -> color = BLACK;
    }
    else if(x == x -> parent -> left) {
        ent_rel_node *w = x -> parent -> right;
        if(w -> color == RED){
            w -> color = BLACK;
            x -> parent -> color = RED;
            left_rotate_ent_rel(rb, x -> parent);
            w = x -> parent -> right;
        }
        if(w -> left -> color == BLACK && w -> right -> color == BLACK) {
            w -> color = RED;
            rb_delete_fixup_ent_rel(rb, x -> parent);
        }
        else {
            if (w->right->color == BLACK) {
                w->left->color = BLACK;
                w->color = RED;
                right_rotate_ent_rel(rb, w);
                w = x->parent->right;
            }
            w -> color = x -> parent -> color;
            x -> parent -> color = BLACK;
            w -> right -> color = BLACK;
            left_rotate_ent_rel(rb, x -> parent);
        }
    }
    else {
        ent_rel_node *w = x -> parent -> left;
        if(w -> color == RED){
            w -> color = BLACK;
            x -> parent -> color = RED;
            right_rotate_ent_rel(rb, x -> parent);
            w = x -> parent -> left;
        }
        if(w -> right -> color == BLACK && w -> left -> color == BLACK) {
            w -> color = RED;
            rb_delete_fixup_ent_rel(rb, x -> parent);
        }
        else {
            if (w->left->color == BLACK) {
                w->right->color = BLACK;
                w->color = RED;
                left_rotate_ent_rel(rb, w);
                w = x->parent->left;
            }
            w -> color = x -> parent -> color;
            x -> parent -> color = BLACK;
            w -> left -> color = BLACK;
            right_rotate_ent_rel(rb, x -> parent);
        }
    }
}

ent_rel_node *tree_successor_ent_rel(ent_rel_rbtree *rb, ent_rel_node *x) {
    if(x -> right != rb -> NIL)
        return rb_minimum_ent_rel(rb, x -> right);
    ent_rel_node *y = x -> parent;
    while(y != rb -> NIL && x == y -> right) {
        x = y;
        y = y -> parent;
    }
    return y;
}

void rb_delete_ent_rel(ent_rel_rbtree *rb, ent_rel_node *z) {
    ent_rel_node *y;
    ent_rel_node *x;
    if(z -> left == rb -> NIL || z -> right == rb -> NIL) {
        y = z;
    }
    else y = tree_successor_ent_rel(rb, z);
    if(y -> left != rb -> NIL) {
        x = y -> left;
    }
    else x = y -> right;

    x->parent = y->parent;

    if (y -> parent == rb -> NIL) {
        rb -> root = x;
    }
    else if(y == y -> parent -> left) {
        y -> parent -> left = x;
    }
    else y -> parent -> right = x;

    if(y != z) {
        z -> entity = y -> entity;
        z -> entranti_rbtree = y -> entranti_rbtree;
        z -> num_entr = y -> num_entr;
    }

    if(y -> color == BLACK)
        rb_delete_fixup_ent_rel(rb, x);

    free(y);
}

ent_rel_node *search_ent_rel(ent_rel_rbtree *rb, ent_rel_node *node, char *id) {
    if(node == rb -> NIL || strcmp(node -> entity -> id, id) == 0){
        return node;
    }
    if(strcmp(id, node -> entity -> id) < 0) {
        return search_ent_rel(rb, node -> left, id);
    }
    else {
        return search_ent_rel(rb, node -> right, id);
    }
}

void delent_orig_inner(rel_entranti_inst_rbtree *tree, rel_entranti_inst_node *node, ent_rel_node *ent_node, char *id_orig) {
    if(node != tree -> NIL) {
        delent_orig_inner(tree, node -> left, ent_node, id_orig);


        if(strcmp(node -> orig -> id, id_orig) == 0) {
            rb_delete_rel_entranti_inst(tree, node);
            ent_node -> num_entr = ent_node -> num_entr - 1;
            deleted = DELETED;

            return;
        }


        delent_orig_inner(tree, node -> right, ent_node, id_orig);




    }

}

void delent_orig(ent_rel_rbtree *tree, ent_rel_node *node, char *id_orig) {
    if(node != tree -> NIL) {
        delent_orig(tree, node -> left, id_orig);


        //  delent_orig_inner(node -> entranti_rbtree, node -> entranti_rbtree -> root, node, id_orig);

        rel_entranti_inst_node *h = search_rel_entranti_inst(node -> entranti_rbtree, node -> entranti_rbtree -> root, id_orig);
        if(h != node -> entranti_rbtree -> NIL){
            rb_delete_rel_entranti_inst(node -> entranti_rbtree, h);
            node -> num_entr = node -> num_entr - 1;
            deleted = DELETED;
        }


        delent_orig(tree, node -> right, id_orig);



    }
}


void search_ent_rel_and_delete(ent_rel_rbtree *rb, ent_rel_node *node, char *id) {
    if(node == rb -> NIL)
        return;
    if(strcmp(node -> entity -> id, id) == 0){
        rb_delete_ent_rel(rb, node);
        return;
    }
    if(strcmp(id, node -> entity -> id) < 0) {
        search_ent_rel_and_delete(rb, node -> left, id);
    }
    else {
        search_ent_rel_and_delete(rb, node -> right, id);
    }
}

void left_rotate_max_entranti_ent(max_entranti_ent_rbtree *rb, max_entranti_ent_node *node) {
    max_entranti_ent_node *y = node -> right;
    node -> right = y -> left;
    if(y -> left != rb -> NIL) {
        y -> left -> parent = node;
    }
    y -> parent = node -> parent;
    if(node -> parent == rb -> NIL) {
        rb -> root = y;
    }
    else if(node == node -> parent -> left) {
        node -> parent -> left = y;
    }
    else {
        node -> parent -> right = y;
    }
    y -> left = node;
    node -> parent = y;
}

void right_rotate_max_entranti_ent(max_entranti_ent_rbtree *rb, max_entranti_ent_node *node) {
    max_entranti_ent_node *y = node -> left;
    node -> left = y -> right;
    if(y -> right != rb -> NIL) {
        y -> right -> parent = node;
    }
    y -> parent = node -> parent;
    if(node -> parent == rb -> NIL) {
        rb -> root = y;
    }
    else if(node == node -> parent -> right) {
        node -> parent -> right = y;
    }
    else {
        node -> parent -> left = y;
    }
    y -> right = node;
    node -> parent = y;
}

void rb_insert_fixup_max_entranti_ent(max_entranti_ent_rbtree *rb, max_entranti_ent_node *new) {

    if (new == rb->root){
        rb -> root -> color = BLACK;
    }
    else {
        max_entranti_ent_node *x = new->parent;
        if (x->color == RED) {
            if (x == x->parent->left) {
                max_entranti_ent_node *y = x->parent->right;
                if (y->color == RED) {
                    x->color = BLACK;
                    y->color = BLACK;
                    x->parent->color = RED;
                    rb_insert_fixup_max_entranti_ent(rb, x->parent);
                } else {
                    if (new == x->right) {
                        new = x;
                        left_rotate_max_entranti_ent(rb, new);
                        x = new -> parent;
                    }

                    x->color = BLACK;
                    x->parent->color = RED;
                    right_rotate_max_entranti_ent(rb, x->parent);
                }
            } else {
                if (x == x->parent->right) {
                    max_entranti_ent_node *y = x->parent->left;
                    if (y->color == RED) {
                        x->color = BLACK;
                        y->color = BLACK;
                        x->parent->color = RED;
                        rb_insert_fixup_max_entranti_ent(rb, x->parent);
                    } else {
                        if (new == x->left) {
                            new = x;
                            right_rotate_max_entranti_ent(rb, new);
                            x = new -> parent;
                        }

                        x->color = BLACK;
                        x->parent->color = RED;
                        left_rotate_max_entranti_ent(rb, x->parent);
                    }
                }
            }
        }
    }
}

void freeMaxTree (max_entranti_ent_rbtree *tree, max_entranti_ent_node *node){
    if(node == tree -> NIL)
        return;
    freeMaxTree(tree, node -> left);
    freeMaxTree(tree, node -> right);
    free(node);
}

void freeEntrantiTree (rel_entranti_inst_rbtree *tree, rel_entranti_inst_node *node){
    if(node == tree -> NIL)
        return;
    freeEntrantiTree(tree, node -> left);
    freeEntrantiTree(tree, node -> right);
    free(node);
}

max_entranti_ent_node *rb_insert_max_entranti_ent(max_entranti_ent_rbtree *rb, entity_data *entity) {
    max_entranti_ent_node *y = rb -> NIL;
    max_entranti_ent_node *x = rb -> root;

    while(x != rb -> NIL) {
        y = x;
        if(strcmp(entity -> id, x -> entity -> id) < 0)
            x = x -> left;
        else
            x = x -> right;
    }

    max_entranti_ent_node *new = malloc(sizeof(max_entranti_ent_node));
    new -> entity = entity;
    new -> parent = y;
    if(y == rb -> NIL)
        rb -> root = new;
    else if(strcmp(new->entity->id, y->entity->id) < 0)
        y -> left = new;
    else
        y -> right = new;
    new -> left = rb -> NIL;
    new -> right = rb -> NIL;
    new -> color = RED;
    rb_insert_fixup_max_entranti_ent(rb, new);
    return new;
}


max_entranti_ent_node* rb_minimum_max_entranti_ent(max_entranti_ent_rbtree *rb, max_entranti_ent_node *x) {
    while(x -> left != rb -> NIL) {
        x = x -> left;
    }
    return x;
}

void rb_delete_fixup_max_entranti_ent(max_entranti_ent_rbtree *rb, max_entranti_ent_node *x) {

    if(x -> color == RED || x -> parent == rb -> NIL) {
        x -> color = BLACK;
    }
    else if(x == x -> parent -> left) {
        max_entranti_ent_node *w = x -> parent -> right;
        if(w -> color == RED){
            w -> color = BLACK;
            x -> parent -> color = RED;
            left_rotate_max_entranti_ent(rb, x -> parent);
            w = x -> parent -> right;
        }
        if(w -> left -> color == BLACK && w -> right -> color == BLACK) {
            w -> color = RED;
            rb_delete_fixup_max_entranti_ent(rb, x -> parent);
        }
        else {
            if (w->right->color == BLACK) {
                w->left->color = BLACK;
                w->color = RED;
                right_rotate_max_entranti_ent(rb, w);
                w = x->parent->right;
            }
            w -> color = x -> parent -> color;
            x -> parent -> color = BLACK;
            w -> right -> color = BLACK;
            left_rotate_max_entranti_ent(rb, x -> parent);
        }
    }
    else {
        max_entranti_ent_node *w = x -> parent -> left;
        if(w -> color == RED){
            w -> color = BLACK;
            x -> parent -> color = RED;
            right_rotate_max_entranti_ent(rb, x -> parent);
            w = x -> parent -> left;
        }
        if(w -> right -> color == BLACK && w -> left -> color == BLACK) {
            w -> color = RED;
            rb_delete_fixup_max_entranti_ent(rb, x -> parent);
        }
        else {
            if (w->left->color == BLACK) {
                w->right->color = BLACK;
                w->color = RED;
                left_rotate_max_entranti_ent(rb, w);
                w = x->parent->left;
            }
            w -> color = x -> parent -> color;
            x -> parent -> color = BLACK;
            w -> left -> color = BLACK;
            right_rotate_max_entranti_ent(rb, x -> parent);
        }
    }
}

max_entranti_ent_node *tree_successor_max_entranti_ent(max_entranti_ent_rbtree *rb, max_entranti_ent_node *x) {
    if(x -> right != rb -> NIL)
        return rb_minimum_max_entranti_ent(rb, x -> right);
    max_entranti_ent_node *y = x -> parent;
    while(y != rb -> NIL && x == y -> right) {
        x = y;
        y = y -> parent;
    }
    return y;
}

void rb_delete_max_entranti_ent(max_entranti_ent_rbtree *rb, max_entranti_ent_node *z) {
    max_entranti_ent_node *y;
    max_entranti_ent_node *x;
    if(z -> left == rb -> NIL || z -> right == rb -> NIL) {
        y = z;
    }
    else y = tree_successor_max_entranti_ent(rb, z);
    if(y -> left != rb -> NIL) {
        x = y -> left;
    }
    else x = y -> right;

    x->parent = y->parent;

    if (y -> parent == rb -> NIL) {
        rb -> root = x;
    }
    else if(y == y -> parent -> left) {
        y -> parent -> left = x;
    }
    else y -> parent -> right = x;

    if(y != z) {
        z -> entity = y -> entity;
    }

    if(y -> color == BLACK)
        rb_delete_fixup_max_entranti_ent(rb, x);

    free(y);
}

max_entranti_ent_node *search_max_entranti_ent(max_entranti_ent_rbtree *rb, max_entranti_ent_node *node, char *id) {
    if(node == rb -> NIL || strcmp(node -> entity -> id, id) == 0){
        return node;
    }
    if(strcmp(id, node -> entity -> id) < 0) {
        return search_max_entranti_ent(rb, node -> left, id);
    }
    else {
        return search_max_entranti_ent(rb, node -> right, id);
    }
}

char search_max_entranti_ent_and_delete(max_entranti_ent_rbtree *rb, max_entranti_ent_node *node, char *id) {
    if(node == rb -> NIL)
        return '0';
    if(strcmp(node -> entity -> id, id) == 0){
        rb_delete_max_entranti_ent(rb, node);
        return '1';
    }
    if(strcmp(id, node -> entity -> id) < 0) {
        return search_max_entranti_ent_and_delete(rb, node -> left, id);
    }
    else {
        return search_max_entranti_ent_and_delete(rb, node -> right, id);
    }
}

void left_rotate_relation(rel_rbtree *rb, rel_node *node) {
    rel_node *y = node -> right;
    node -> right = y -> left;
    if(y -> left != rb -> NIL) {
        y -> left -> parent = node;
    }
    y -> parent = node -> parent;
    if(node -> parent == rb -> NIL) {
        rb -> root = y;
    }
    else if(node == node -> parent -> left) {
        node -> parent -> left = y;
    }
    else {
        node -> parent -> right = y;
    }
    y -> left = node;
    node -> parent = y;
}

void right_rotate_relation(rel_rbtree *rb, rel_node *node) {
    rel_node *y = node -> left;
    node -> left = y -> right;
    if(y -> right != rb -> NIL) {
        y -> right -> parent = node;
    }
    y -> parent = node -> parent;
    if(node -> parent == rb -> NIL) {
        rb -> root = y;
    }
    else if(node == node -> parent -> right) {
        node -> parent -> right = y;
    }
    else {
        node -> parent -> left = y;
    }
    y -> right = node;
    node -> parent = y;
}

void rb_insert_fixup_relation(rel_rbtree *rb, rel_node *new) {

    if (new == rb->root){
        rb -> root -> color = BLACK;
    }
    else {
        rel_node *x = new->parent;
        if (x->color == RED) {
            if (x == x->parent->left) {
                rel_node *y = x->parent->right;
                if (y->color == RED) {
                    x->color = BLACK;
                    y->color = BLACK;
                    x->parent->color = RED;
                    rb_insert_fixup_relation(rb, x->parent);
                } else {
                    if (new == x->right) {
                        new = x;
                        left_rotate_relation(rb, new);
                        x = new -> parent;
                    }

                    x->color = BLACK;
                    x->parent->color = RED;
                    right_rotate_relation(rb, x->parent);
                }
            } else {
                if (x == x->parent->right) {
                    rel_node *y = x->parent->left;
                    if (y->color == RED) {
                        x->color = BLACK;
                        y->color = BLACK;
                        x->parent->color = RED;
                        rb_insert_fixup_relation(rb, x->parent);
                    } else {
                        if (new == x->left) {
                            new = x;
                            right_rotate_relation(rb, new);
                            x = new -> parent;
                        }

                        x->color = BLACK;
                        x->parent->color = RED;
                        left_rotate_relation(rb, x->parent);
                    }
                }
            }
        }
    }
}

rel_node *rb_insert_relation(rel_rbtree *rb, char *id_ent) {
    rel_node *y = rb -> NIL;
    rel_node *x = rb -> root;

    while(x != rb -> NIL) {
        y = x;
        if(strcmp(id_ent, x -> id) < 0)
            x = x -> left;
        else
            x = x -> right;
    }

    rel_node *new = malloc(sizeof(rel_node));
    strcpy(new -> id, id_ent);
    new -> parent = y;
    if(y == rb -> NIL)
        rb -> root = new;
    else if(strcmp(new->id, y->id) < 0)
        y -> left = new;
    else
        y -> right = new;
    new -> left = rb -> NIL;
    new -> right = rb -> NIL;
    new -> color = RED;

    new -> ent_tree = rb_ent_rel_create();
    new -> max_entranti_rb = rb_max_entranti_create();
    new -> max_entranti_num = 0;

    rb_insert_fixup_relation(rb, new);
    return new;
}




rel_node* rb_minimum_relation(rel_rbtree *rb, rel_node *x) {
    while(x -> left != rb -> NIL) {
        x = x -> left;
    }
    return x;
}

void rb_delete_fixup_relation(rel_rbtree *rb, rel_node *x) {

    if(x -> color == RED || x -> parent == rb -> NIL) {
        x -> color = BLACK;
    }
    else if(x == x -> parent -> left) {
        rel_node *w = x -> parent -> right;
        if(w -> color == RED){
            w -> color = BLACK;
            x -> parent -> color = RED;
            left_rotate_relation(rb, x -> parent);
            w = x -> parent -> right;
        }
        if(w -> left -> color == BLACK && w -> right -> color == BLACK) {
            w -> color = RED;
            rb_delete_fixup_relation(rb, x -> parent);
        }
        else {
            if (w->right->color == BLACK) {
                w->left->color = BLACK;
                w->color = RED;
                right_rotate_relation(rb, w);
                w = x->parent->right;
            }
            w -> color = x -> parent -> color;
            x -> parent -> color = BLACK;
            w -> right -> color = BLACK;
            left_rotate_relation(rb, x -> parent);
        }
    }
    else {
        rel_node *w = x -> parent -> left;
        if(w -> color == RED){
            w -> color = BLACK;
            x -> parent -> color = RED;
            right_rotate_relation(rb, x -> parent);
            w = x -> parent -> left;
        }
        if(w -> right -> color == BLACK && w -> left -> color == BLACK) {
            w -> color = RED;
            rb_delete_fixup_relation(rb, x -> parent);
        }
        else {
            if (w->left->color == BLACK) {
                w->right->color = BLACK;
                w->color = RED;
                left_rotate_relation(rb, w);
                w = x->parent->left;
            }
            w -> color = x -> parent -> color;
            x -> parent -> color = BLACK;
            w -> left -> color = BLACK;
            right_rotate_relation(rb, x -> parent);
        }
    }
}

rel_node *tree_successor_relation(rel_rbtree *rb, rel_node *x) {
    if(x -> right != rb -> NIL)
        return rb_minimum_relation(rb, x -> right);
    rel_node *y = x -> parent;
    while(y != rb -> NIL && x == y -> right) {
        x = y;
        y = y -> parent;
    }
    return y;
}

void rb_delete_relation(rel_rbtree *rb, rel_node *z) {
    rel_node *y;
    rel_node *x;
    if(z -> left == rb -> NIL || z -> right == rb -> NIL) {
        y = z;
    }
    else y = tree_successor_relation(rb, z);
    if(y -> left != rb -> NIL) {
        x = y -> left;
    }
    else x = y -> right;

    x->parent = y->parent;

    if (y -> parent == rb -> NIL) {
        rb -> root = x;
    }
    else if(y == y -> parent -> left) {
        y -> parent -> left = x;
    }
    else y -> parent -> right = x;

    if(y != z) {
        strcpy(z -> id, y -> id);
    }

    if(y -> color == BLACK)
        rb_delete_fixup_relation(rb, x);

    free(y);
}

rel_node *search_relation(rel_rbtree *rb, rel_node *node, char *id) {
    if(node == rb -> NIL || strcmp(node -> id, id) == 0){
        return node;
    }
    if(strcmp(id, node -> id) < 0) {
        return search_relation(rb, node -> left, id);
    }
    else {
        return search_relation(rb, node -> right, id);
    }
}

void search_relation_and_delete(rel_rbtree *rb, rel_node *node, char *id) {
    if(node == rb -> NIL)
        return;
    if(strcmp(node -> id, id) == 0){
        rb_delete_relation(rb, node);
        return;
    }
    if(strcmp(id, node -> id) < 0) {
        search_relation_and_delete(rb, node -> left, id);
    }
    else {
        search_relation_and_delete(rb, node -> right, id);
    }
}



void left_rotate_entity(entity_rbtree *rb, entity_node_rb *node) {
    entity_node_rb *y = node -> right;
    node -> right = y -> left;
    if(y -> left != rb -> NIL) {
        y -> left -> parent = node;
    }
    y -> parent = node -> parent;
    if(node -> parent == rb -> NIL) {
        rb -> root = y;
    }
    else if(node == node -> parent -> left) {
        node -> parent -> left = y;
    }
    else {
        node -> parent -> right = y;
    }
    y -> left = node;
    node -> parent = y;
}

void right_rotate_entity(entity_rbtree *rb, entity_node_rb *node) {
    entity_node_rb *y = node -> left;
    node -> left = y -> right;
    if(y -> right != rb -> NIL) {
        y -> right -> parent = node;
    }
    y -> parent = node -> parent;
    if(node -> parent == rb -> NIL) {
        rb -> root = y;
    }
    else if(node == node -> parent -> right) {
        node -> parent -> right = y;
    }
    else {
        node -> parent -> left = y;
    }
    y -> right = node;
    node -> parent = y;
}

void rb_insert_fixup_entity(entity_rbtree *rb, entity_node_rb *new) {

    if (new == rb->root){
        rb -> root -> color = BLACK;
    }
    else {
        entity_node_rb *x = new->parent;
        if (x->color == RED) {
            if (x == x->parent->left) {
                entity_node_rb *y = x->parent->right;
                if (y->color == RED) {
                    x->color = BLACK;
                    y->color = BLACK;
                    x->parent->color = RED;
                    rb_insert_fixup_entity(rb, x->parent);
                } else {
                    if (new == x->right) {
                        new = x;
                        left_rotate_entity(rb, new);
                        x = new -> parent;
                    }

                    x->color = BLACK;
                    x->parent->color = RED;
                    right_rotate_entity(rb, x->parent);
                }
            } else {
                if (x == x->parent->right) {
                    entity_node_rb *y = x->parent->left;
                    if (y->color == RED) {
                        x->color = BLACK;
                        y->color = BLACK;
                        x->parent->color = RED;
                        rb_insert_fixup_entity(rb, x->parent);
                    } else {
                        if (new == x->left) {
                            new = x;
                            right_rotate_entity(rb, new);
                            x = new -> parent;
                        }

                        x->color = BLACK;
                        x->parent->color = RED;
                        left_rotate_entity(rb, x->parent);
                    }
                }
            }
        }
    }
}

void rb_insert_entity(entity_rbtree *rb, char *id_ent) {
    entity_node_rb *y = rb -> NIL;
    entity_node_rb *x = rb -> root;

    while(x != rb -> NIL) {
        y = x;
        if(strcmp(id_ent, x -> data -> id) < 0)
            x = x -> left;
        else
            x = x -> right;
    }

    entity_node_rb *new = malloc(sizeof(entity_node_rb));
    new -> data = malloc(sizeof(entity_data));
    strcpy(new -> data -> id, id_ent);
    new -> parent = y;
    if(y == rb -> NIL)
        rb -> root = new;
    else if(strcmp(new->data->id, y->data->id) < 0)
        y -> left = new;
    else
        y -> right = new;
    new -> left = rb -> NIL;
    new -> right = rb -> NIL;
    new -> color = RED;
    rb_insert_fixup_entity(rb, new);
}


entity_node_rb* rb_minimum_entity(entity_rbtree *rb, entity_node_rb *x) {
    while(x -> left != rb -> NIL) {
        x = x -> left;
    }
    return x;
}

void rb_delete_fixup_entity(entity_rbtree *rb, entity_node_rb *x) {

    if(x -> color == RED || x -> parent == rb -> NIL) {
        x -> color = BLACK;
    }
    else if(x == x -> parent -> left) {
        entity_node_rb *w = x -> parent -> right;
        if(w -> color == RED){
            w -> color = BLACK;
            x -> parent -> color = RED;
            left_rotate_entity(rb, x -> parent);
            w = x -> parent -> right;
        }
        if(w -> left -> color == BLACK && w -> right -> color == BLACK) {
            w -> color = RED;
            rb_delete_fixup_entity(rb, x -> parent);
        }
        else {
            if (w->right->color == BLACK) {
                w->left->color = BLACK;
                w->color = RED;
                right_rotate_entity(rb, w);
                w = x->parent->right;
            }
            w -> color = x -> parent -> color;
            x -> parent -> color = BLACK;
            w -> right -> color = BLACK;
            left_rotate_entity(rb, x -> parent);
        }
    }
    else {
        entity_node_rb *w = x -> parent -> left;
        if(w -> color == RED){
            w -> color = BLACK;
            x -> parent -> color = RED;
            right_rotate_entity(rb, x -> parent);
            w = x -> parent -> left;
        }
        if(w -> right -> color == BLACK && w -> left -> color == BLACK) {
            w -> color = RED;
            rb_delete_fixup_entity(rb, x -> parent);
        }
        else {
            if (w->left->color == BLACK) {
                w->right->color = BLACK;
                w->color = RED;
                left_rotate_entity(rb, w);
                w = x->parent->left;
            }
            w -> color = x -> parent -> color;
            x -> parent -> color = BLACK;
            w -> left -> color = BLACK;
            right_rotate_entity(rb, x -> parent);
        }
    }
}

entity_node_rb *tree_successor_entity(entity_rbtree *rb, entity_node_rb *x) {
    if(x -> right != rb -> NIL)
        return rb_minimum_entity(rb, x -> right);
    entity_node_rb *y = x -> parent;
    while(y != rb -> NIL && x == y -> right) {
        x = y;
        y = y -> parent;
    }
    return y;
}

void rb_delete_entity(entity_rbtree *rb, entity_node_rb *z) {
    entity_node_rb *y;
    entity_node_rb *x;
    if(z -> left == rb -> NIL || z -> right == rb -> NIL) {
        y = z;
    }
    else y = tree_successor_entity(rb, z);
    if(y -> left != rb -> NIL) {
        x = y -> left;
    }
    else x = y -> right;

    x->parent = y->parent;
    if (y -> parent == rb -> NIL) {
        rb -> root = x;
    }
    else if(y == y -> parent -> left) {
        y -> parent -> left = x;
    }
    else y -> parent -> right = x;

    if(y != z) {
        z -> data = y -> data;
    }

    if(y -> color == BLACK)
        rb_delete_fixup_entity(rb, x);

    free(y);
}

entity_node_rb *search_entity(entity_rbtree *rb, entity_node_rb *node, char *id) {
    if(node == rb -> NIL || strcmp(node -> data->id, id) == 0){
        return node;
    }
    if(strcmp(id, node -> data -> id) < 0) {
        return search_entity(rb, node -> left, id);
    }
    else {
        return search_entity(rb, node -> right, id);
    }
}

void search_entity_and_delete(entity_rbtree *rb, entity_node_rb *node, char *id) {
    if(node == rb -> NIL)
        return;
    if(strcmp(node -> data -> id, id) == 0){
        rb_delete_entity(rb, node);
        return;
    }
    if(strcmp(id, node -> data -> id) < 0) {
        search_entity_and_delete(rb, node -> left, id);
    }
    else {
        search_entity_and_delete(rb, node -> right, id);
    }
}


void printmax(max_entranti_ent_rbtree *tree, max_entranti_ent_node *node) {
    if(node != tree ->NIL) {
        printmax(tree, node -> left);

        fputs(node -> entity -> id, stdout);
        fputs(" ", stdout);

        printmax(tree, node -> right);

    }
}



void report(rel_rbtree *rb_rel, rel_node *rel, entity_rbtree *rb_entity) {
    if(rel != rb_rel ->NIL) {
        report(rb_rel, rel -> left, rb_entity);

        if(rel -> max_entranti_rb -> root != rel -> max_entranti_rb -> NIL) {
            printed = PRINTED;
            fputs(rel -> id, stdout);
            fputs(" ", stdout);
            printmax(rel -> max_entranti_rb, rel -> max_entranti_rb -> root);
            printf("%hu; ", rel -> max_entranti_num);
        }

        report(rb_rel, rel -> right, rb_entity);
    }
}


unsigned short find_max(ent_rel_rbtree *tree, ent_rel_node *node) {
    
    if (node == tree -> NIL)
        return 0;

    int res = node -> num_entr;
    int lres = find_max(tree, node -> left);
    int rres = find_max(tree, node ->right);
    if (lres > res)
        res = lres;
    if (rres > res)
        res = rres;
    return res;
}

void add_to_max_tree(max_entranti_ent_rbtree *max_tree, ent_rel_rbtree *ent_tree, ent_rel_node *node, unsigned short max) {
    if(node != ent_tree -> NIL) {
        add_to_max_tree(max_tree, ent_tree, node -> left, max);

        if(node -> num_entr == max)
            rb_insert_max_entranti_ent(max_tree, node -> entity);


        add_to_max_tree(max_tree, ent_tree, node -> right, max);

    }
}







void delent_from_rel(rel_rbtree *rel_tree, rel_node *rel, char *id_ent) {
    if(rel != rel_tree -> NIL) {
        delent_from_rel(rel_tree, rel->left, id_ent);


        ent_rel_node *a = search_ent_rel(rel->ent_tree, rel->ent_tree->root, id_ent);


        if (a != rel->ent_tree->NIL) {
            freeEntrantiTree(a -> entranti_rbtree, a -> entranti_rbtree -> root);
            free(a -> entranti_rbtree -> NIL);
            rb_delete_ent_rel(rel->ent_tree, a);
        }


        deleted = NOT_DELETED;

        delent_orig(rel->ent_tree, rel->ent_tree->root, id_ent);

        /*deleted = NOT_DELETED;
        search_ent_rel_and_delete_dest_and_orig(rel -> ent_tree, rel -> ent_tree -> root, id_ent, id_ent);*/




        if ((deleted == DELETED || a != rel->ent_tree->NIL)) {
            freeMaxTree(rel -> max_entranti_rb, rel -> max_entranti_rb -> root);
            free(rel -> max_entranti_rb -> NIL);
            free(rel->max_entranti_rb);

            rel->max_entranti_rb = rb_max_entranti_create();
            unsigned short max = find_max(rel->ent_tree, rel->ent_tree->root);
            if (max != 0) {
                add_to_max_tree(rel->max_entranti_rb, rel->ent_tree, rel->ent_tree->root, max);
            }

            rel->max_entranti_num = max;
        }

        delent_from_rel(rel_tree, rel -> right, id_ent);
    }
}


void delrel_inner(rel_entranti_inst_rbtree *tree, rel_entranti_inst_node *node, ent_rel_node *ent_node, char *id_orig) {
    if(node != tree -> NIL) {
        delrel_inner(tree, node -> left, ent_node, id_orig);


        if(strcmp(node -> orig -> id, id_orig) == 0) {
            rb_delete_rel_entranti_inst(tree, node);
            ent_node -> num_entr = ent_node -> num_entr - 1;
            return;
        }


        delrel_inner(tree, node -> right, ent_node, id_orig);

    }
}


int main() {
    //freopen("1.txt","r",stdin);
    entity_rbtree *entity_tree = rb_entity_create();
    rel_rbtree *relation_tree = rb_relation_create();

    char command_entire [STRING_LENGTH];
    char *command;
    char *id_read;
    while(1) {
        fgets (command_entire, STRING_LENGTH, stdin);
        command = strtok (command_entire," \n");
        if(strcmp(command, "addent") == 0) {
            id_read = strtok (NULL," \n");
            if(search_entity(entity_tree, entity_tree -> root, id_read) == entity_tree -> NIL)
                rb_insert_entity(entity_tree, id_read);
        }
        else if(strcmp(command, "delent") == 0) {
            id_read = strtok (NULL," \n");
            entity_node_rb *node = search_entity(entity_tree, entity_tree -> root, id_read);

            if(node != entity_tree -> NIL) {
                delent_from_rel(relation_tree, relation_tree -> root, id_read);
                free(node -> data);
                rb_delete_entity(entity_tree, node);

            }

        }
        else if(strcmp(command, "addrel") == 0) {


            char *id_orig = strtok (NULL," ");
            char *id_dest = strtok (NULL," ");
            char *id_rel = strtok (NULL," \n");

            // check if all entities involved exist
            entity_node_rb *dest;
            entity_node_rb *orig = search_entity(entity_tree, entity_tree -> root, id_orig);

            if(orig != entity_tree -> NIL) {
                dest = search_entity(entity_tree, entity_tree -> root, id_dest);

                if(dest != entity_tree -> NIL) {

                    rel_node *rel = search_relation(relation_tree, relation_tree->root, id_rel);
                    if(rel == relation_tree -> NIL)
                        rel = rb_insert_relation(relation_tree, id_rel);

                    ent_rel_node *a = search_ent_rel(rel -> ent_tree, rel -> ent_tree -> root, id_dest);

                    if(a == NULL || a == rel -> ent_tree -> NIL) {
                        a = rb_insert_ent_rel(rel -> ent_tree, dest);
                    }

                    rel_entranti_inst_node *b = search_rel_entranti_inst(a -> entranti_rbtree, a -> entranti_rbtree -> root, id_orig);

                    if(b == NULL || b == a -> entranti_rbtree -> NIL) {
                        rb_insert_rel_entranti_inst(a -> entranti_rbtree, orig);
                        a -> num_entr = a -> num_entr + 1;

                        if(a -> num_entr > rel -> max_entranti_num) {
                            rel -> max_entranti_num = a -> num_entr;
                            free(rel -> max_entranti_rb);
                            rel -> max_entranti_rb = rb_max_entranti_create();
                            rb_insert_max_entranti_ent(rel -> max_entranti_rb, dest -> data);
                        }
                        else if(a -> num_entr == rel -> max_entranti_num) {
                            rb_insert_max_entranti_ent(rel -> max_entranti_rb, dest -> data);
                        }
                    }

                }
            }



        }
        else if(strcmp(command, "delrel") == 0) {

            char *id_orig = strtok (NULL," ");
            char *id_dest = strtok (NULL," ");
            char *id_rel = strtok (NULL," \n");

            rel_node *rel = search_relation(relation_tree, relation_tree -> root, id_rel);

            if(rel != relation_tree -> NIL) {
                ent_rel_node *a = search_ent_rel(rel -> ent_tree, rel -> ent_tree -> root, id_dest);
                if(a != rel -> ent_tree -> NIL) {
                    rel_entranti_inst_node *b = search_rel_entranti_inst(a -> entranti_rbtree, a -> entranti_rbtree -> root, id_orig);

                    if(b != a -> entranti_rbtree -> NIL) {
                        rb_delete_rel_entranti_inst(a -> entranti_rbtree, b);
                        a -> num_entr = a -> num_entr - 1;

                        if(a -> num_entr + 1 == rel -> max_entranti_num){
                            unsigned short max = find_max(rel -> ent_tree, rel -> ent_tree -> root);
                            rel -> max_entranti_num = max;
                            freeMaxTree(rel -> max_entranti_rb, rel -> max_entranti_rb -> root);
                            free(rel -> max_entranti_rb -> NIL);
                            free(rel -> max_entranti_rb);
                            rel -> max_entranti_rb = rb_max_entranti_create();
                            if(max != 0)
                                add_to_max_tree(rel -> max_entranti_rb, rel -> ent_tree, rel -> ent_tree -> root, max);
                        }



                    }
                }

            }

        }
        else if(strcmp(command, "end") == 0)
            break;
        else if(strcmp(command, "report") == 0) {
            if(relation_tree -> root == relation_tree -> NIL)
                fputs("none\n", stdout);
            else{
                printed = NOT_PRINTED;
                report(relation_tree, relation_tree -> root, entity_tree);
                if(printed == NOT_PRINTED)
                    fputs("none", stdout);
                fputs("\n", stdout);
            }

        }

    }
}

