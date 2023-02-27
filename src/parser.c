#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "lexer.c"
#include "parse_tree.c"

void initialize_parser(){
    ParseTree = create_parse_tree();    
    ParserStack = create_stack();
}
