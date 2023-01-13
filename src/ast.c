#include <stdlib.h>

#include "sas.h"

ast_node_t* ast_node_init(ast_node_type type, location_t* loc, ast_node_t* base)
{
    ast_node_t* node = calloc(1, sizeof *node);
    *node = *base;
    node->type = type;
    node->loc = loc;
    return node;
}

ast_node_t* ast_file_init(location_t* loc)
{
    return ast_node_init(AST_FILE, loc, &(ast_node_t){
        .items = DEFAULT_VECTOR
    });
}