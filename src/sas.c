#include <stdio.h>

#include "sas.h"

map_t* keywords;

void set_up_keywords(void)
{
    keywords = map_init(NULL, 200);
    #define keyword(id, name) map_put(keywords, name, (void*) id);
    #include "keywords.inc"
    #undef keyword
}

void assemble(FILE* file)
{
    lexer_t* lexer = lex_init(file);
    while (!lex_eof(lexer))
        lex_read_token(lexer);
    for (int i = 0; i < lexer->output->size; i++)
    {
        token_t* token = vector_get(lexer->output, i);
        if (token_has_content(token))
            printf("%s\n", token->content);
        else
            printf("%c (id: %i)\n", token->id, token->id);
    }
    lex_delete(lexer);
}

int main(int argc, char** argv)
{
    if (argc != 2)
        errora("only 1 input file allowed");
    set_up_keywords();
    FILE* file = fopen(argv[1], "r");
    assemble(file);
    fclose(file);
}