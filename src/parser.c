#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#ifndef PARSE_TREE_LINKED_LIST_INCLUDED
#define PARSE_TREE_LINKED_LIST_INCLUDED
#include "parser_table.c"
#endif

FILE *program_fp;
LinkedList *Parser_Stack;
TreeNode *Parse_Tree_Root;

void initialize_parser(){
    printf("Building Grammar (Populating symbols)...\n");
    build_grammar();
    printf("Creating parser tree and stack...\n");
    Parse_Tree_Root = create_parse_tree();
    Parser_Stack = create_stack();
    // inserting S initially
    Symbol *base = symbols[0];
    Parser_Stack = push_stack(Parser_Stack, base);
    printf("Generating parse table...\n");
    generate_parse_table();
    parse_table_make();
}

void start_parsing(){
    while (is_empty_stack(Parser_Stack) == 0){
        Symbol *Top_Symbol = top_stack(Parser_Stack);    
        TOKEN Curr_Token = eval_token(program_fp);
        LinkedList *Aux_Stack = create_stack();
        if (Top_Symbol->is_terminal){
            if (Top_Symbol->terminal == Curr_Token.name){
                printf("DEBUG: Matched Terminal: %d\n", Curr_Token.name);
                pop_stack(Parser_Stack);
            } else {
                printf("ERROR: Terminals Don't Match! (STACK TERM: %d, TOKEN: %d)\n", Top_Symbol->terminal, Curr_Token.name);
            }
        } else{
            Symbol* Grammar_Rule = parser_table[Top_Symbol->non_terminal][Curr_Token.name];
            if (Grammar_Rule != NULL){
                printf("DEBUG: Found Rule: \n");
                print_symbol_details(Grammar_Rule);
                // popping from stack
                pop_stack(Parser_Stack);
                Symbol *curr = Grammar_Rule->right;
                while (curr != NULL){
                    push_stack(Aux_Stack, curr);
                    curr = curr->right;
                }
                while (!is_empty_stack(Aux_Stack)){
                    curr = top_stack(Aux_Stack);
                    push_stack(Parser_Stack, curr);
                    pop_stack(Aux_Stack);
                }
                int rule_length = get_length(Grammar_Rule);
                printf("Rule Length: %d\n", rule_length);
                printf("Printing Stack: \n");
                print_stack(Parser_Stack);
                break;
            } else {
                printf("ERROR: No rule found in parser table! (STACK NT: %d, TOKEN: %d)\n", Top_Symbol->non_terminal, Curr_Token.name);
            }
        }
    }
}

int main(){
    initialize_parser();
    // starting lexer
    printf("\n\nStarting lexer...\n");
    char *program_file = "../tests/test_lexer_1.txt";
    program_fp = fopen(program_file, "r");
    lexer_reset(program_fp);
    printf("Starting parsing...\n");
    start_parsing();
}
