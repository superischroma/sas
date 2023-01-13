#ifndef SAS_H
#define SAS_H

#include <stdio.h>
#include <stdbool.h>

/* Macros */

/* Token Type */

#define TT_IDENTIFIER 0
#define TT_NUMBER_LITERAL 1
#define TT_CHAR_LITERAL 2
#define TT_STRING_LITERAL 3
#define TT_KEYWORD 4

/* vector.c Useful Macros */

#define DEFAULT_CAPACITY 10
#define DEFAULT_ALLOC_DELTA 5
#define RETAIN_OLD_CAPACITY -1

#define DEFAULT_VECTOR vector_init(DEFAULT_CAPACITY, DEFAULT_ALLOC_DELTA)

enum {
    KW_LPAREN = '(',
    KW_RPAREN = ')',
    KW_COLON = ':',
    AST_FILE = 256,
    #define keyword(id, name) id,
    #include "keywords.inc"
    #undef keyword 
};

typedef int token_type, ast_node_type;

typedef struct
{
    int offset;
    int row;
    int col;
} location_t;

typedef struct
{
    void** data;
    int size;
    int capacity;
    int alloc_delta;
} vector_t;

typedef struct
{
    FILE* file;
    int row;
    int col;
    int offset;
    int peek;
    vector_t* output;
} lexer_t;

typedef struct
{
    token_type type;
    location_t* loc;
    union
    {
        int id;
        char* content;
    };
} token_t;

typedef struct
{
    char* data;
    int size;
    int capacity;
    int alloc_delta;
} buffer_t;

typedef struct map_t
{
    char** key;
    void** value;
    int size;
    int capacity;
    struct map_t* parent;
} map_t;

typedef struct ast_node_t
{
    ast_node_type type;
    location_t* loc;
    union
    {
        // AST_FILE
        vector_t* items;
    };
} ast_node_t;

typedef struct parser_t
{
    lexer_t* lex;
    map_t* env;
    ast_node_t* file_node;
    int oindex;
} parser_t;

/* sas.c */

extern map_t* keywords;

/* token.c */

token_t* content_token_init(token_type type, char* content, int offset, int row, int col);
token_t* id_token_init(token_type type, int id, int offset, int row, int col);
bool token_has_content(token_t* token);
void token_delete(token_t* token);

/* vector.c */

vector_t* vector_init(int capacity, int alloc_delta);
vector_t* vector_qinit(int count, ...);
void* vector_push(vector_t* vec, void* element);
void* vector_pop(vector_t* vec);
void* vector_get(vector_t* vec, int index);
void* vector_set(vector_t* vec, int index, void* element);
void* vector_top(vector_t* vec);
void vector_clear(vector_t* vec, int capacity);
bool vector_check_bounds(vector_t* vec, int index);
void vector_delete(vector_t* vec);
void vector_concat(vector_t* vec, vector_t* other);

/* buffer.c */

buffer_t* buffer_init(int capacity, int alloc_delta);
char buffer_append(buffer_t* buffer, char c);
char* buffer_nstring(buffer_t* buffer, char* str, int len);
char* buffer_string(buffer_t* buffer, char* str);
long long buffer_int(buffer_t* buffer, long long ll);
char* buffer_export(buffer_t* buffer);
void buffer_delete(buffer_t* buffer);
char buffer_get(buffer_t* buffer, int index);

/* lex.c */

lexer_t* lex_init(FILE* file);
bool lex_eof(lexer_t* lex);
void lex_read_token(lexer_t* lex);
void lex_delete(lexer_t* lex);
token_t* lex_get(lexer_t* lex, int index);

/* log.c */

void errora(char* fmt, ...);
void errorl(lexer_t* lex, char* fmt, ...);
void errorp(int row, int col, char* fmt, ...);

/* map.c */

map_t* map_init(map_t* parent, int capacity);
void* map_put(map_t* map, char* k, void* v);
void* map_get_local(map_t* map, char* k);
void* map_get(map_t* map, char* k);
vector_t* map_keys(map_t* map);
bool map_erase(map_t* map, char* k);
void map_delete(map_t* map);

/* ast.c */

ast_node_t* ast_file_init(location_t* loc);

/* util.c */

bool is_alphanumeric(int c);
int itos(int n, char* buffer);

#endif