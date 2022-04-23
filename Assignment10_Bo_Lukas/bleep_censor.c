// Author: Boyue, Lukas
/*
    make bleep_censor
    ./bleep_censor
    make bleep_censor && ./bleep_censor

*/

#include "base.h"

typedef struct Token{
    char* start;
    char* end;
}Token;

typedef struct TreeNode{
    struct TreeNode* left;
    struct TreeNode* right;
    Token* token;
}TreeNode;

Token* new_token(char* start, char* end){
    Token* t = xmalloc(sizeof(Token));
    t->start = start;
    t->end = end;
    return t;
}

void print_token(Token* t, bool censored) {
    bool firstItearation = true;
    char* temp = t->start; // <- very important!!
    for (; t->start != t->end; t->start++) {
        if (censored && !firstItearation) {
            printf("*");
            firstItearation = false;
        } else {
            printf("%c", *(t->start));
            firstItearation = false;
        }
    }
    t->start = temp;
}

void print_tokenln(Token* t, bool censored) {
    print_token(t, censored);
    printc('\n');
}

TreeNode* new_tree_node(TreeNode* left, Token* token, TreeNode* right){
    TreeNode* tree_node = xmalloc(sizeof(TreeNode));
    tree_node->token = token;
    tree_node->left = left;
    tree_node->right = right;
    return tree_node;
}

void free_tree(TreeNode* tree){
    if (tree->left  != NULL) free_tree(tree->left);
    if (tree->right != NULL) free_tree(tree->right);
    free(tree->token);
    free(tree);
}

TreeNode* new_leaf(Token* token){
    return new_tree_node(NULL, token, NULL);
}

void print_tree(TreeNode* tree){
    if(tree == NULL){
        return;
    }
    print_tree(tree->left);
    printc(' ');
    print_token(tree->token, false);
    printc(' ');
    print_tree(tree->right);
}

int compare_token(Token* t1, Token* t2){
    // t1 befor t2 = -1
    // t1 == t2 = 0
    // t1 after t2 = 0
    int t1Length = t1->end - t1->start;
    int t2Length = t2->end - t2->start;
    char* word1 = s_sub(t1->start, 0, t1Length);
    char* word2 = s_sub(t2->start, 0, t2Length);
    //printf("word 1: %s, \t", word1);
    //printf("word 2: %s \t", word2);
    int out = s_compare_ignore_case(word1, word2);
    free(word1); free(word2);
    return out;
}

void test_token_compare(){
    char* s = "hallo huhu hall HALLO";
    
    Token hallo = {s + 0, s + 5};
    //print_tokenln(&hallo, false);
    Token huhu = {s + 6, s + 10};
    //print_tokenln(&huhu, false);
    Token hall = {s + 11, s + 15};
    //print_tokenln(&hall, true);
    Token hallo_2 = {s + 16, s + 21};
    //print_tokenln(&hallo_2, true);
    
    test_equal_i(compare_token(&hallo, &hallo), 0);
    test_equal_i(compare_token(&huhu, &huhu), 0);
    test_equal_i(compare_token(&huhu, &hallo), 1);
    test_equal_i(compare_token(&hallo, &huhu), -1);
    test_equal_i(compare_token(&hall, &hallo), -1);
    test_equal_i(compare_token(&hallo, &hall), 1);
    test_equal_i(compare_token(&hallo, &hallo_2), 0);
    test_equal_i(compare_token(&hallo_2, &hallo), 0); 
}

bool contains(TreeNode* tree, Token* token){
    if(tree == NULL){
        return false;
    }
    
    int cmp_result = compare_token(token, tree->token);
    if(cmp_result == 0){
        return true;
    }
    else if(cmp_result < 0){
        return contains(tree->left, token);
    }else{
        return contains(tree->right, token);
    }  
}

enum dirEnum {
    LEFT = -1,
    MIDDLE = 0,
    RIGHT = 1,
};

void insert_in_tree(TreeNode** tree, Token* token) {
    /*Nutzen Sie für das sortierte Einfügen die Funktion compare_token. Ist das Ergebnis negativ muss
    es im linken Teilbaum eingefügt werden, ist es 0 muss nichts geschehen und ist es größer als 0
    muss token im rechten Teilbaum eingefügt werden.*/
    if (*tree == NULL) {
        TreeNode* node = new_leaf(token);
        *tree = node;
        return;
    } else {
        int dir = compare_token((*tree)->token, token) * -1;
        if (dir == LEFT) {
            insert_in_tree(&((*tree)->left), token);
        } else if (dir == RIGHT) {
            insert_in_tree(&((*tree)->right), token);
        }
    }
}

void test_insert_token(){
    char* s = "hallo huhu hall HALLO";
    
    Token hallo = {s + 0, s + 5};
    //print_tokenln(&hallo);
    Token huhu = {s + 6, s + 10};
    //print_tokenln(&huhu);
    Token hall = {s + 11, s + 15};
    //print_tokenln(&hall);
    
    TreeNode* tree = NULL;
    
    insert_in_tree(&tree, &hallo);
    
    test_equal_i(compare_token(tree->token, &hallo), 0);

    insert_in_tree(&tree, &hallo);
    test_equal_i(compare_token(tree->token, &hallo), 0);

    insert_in_tree(&tree, &hall);
    test_equal_i(compare_token(tree->left->token, &hall), 0);
    
    insert_in_tree(&tree, &huhu);
    test_equal_i(compare_token(tree->right->token, &huhu), 0);
    
    free(tree->left);
    free(tree->right);
    free(tree);
    
}


TreeNode* create_bleep_tree(char* bleep_words){
    char* pos = bleep_words;
    char* wordBegin = bleep_words;
    TreeNode* tree = NULL; 

    for (; *wordBegin != '\0'; pos++) {
        if (*pos == ' ' || *(pos + 1) == '\0') {
            Token* t = new_token(wordBegin, pos);
            if (!contains(tree, t)) {
                insert_in_tree(&tree, t);
            } else {
                free(t);
            }
            wordBegin = pos + 1;
        }
    }

    return tree;
}

void test_create_bleep_tree(){
    char* s = "hallo huhu hall HALLO";
    
    Token hallo = {s + 0, s + 5};
    //print_tokenln(&hallo);
    Token huhu = {s + 6, s + 10};
    //print_tokenln(&huhu);
    Token hall = {s + 11, s + 15};
    //print_tokenln(&hall);
    
    TreeNode* tree = create_bleep_tree(s);
   
    test_equal_i(compare_token(tree->token, &hallo), 0);  
    test_equal_i(compare_token(tree->left->token, &hall), 0); 
    test_equal_i(compare_token(tree->right->token, &huhu), 0);
    
    free_tree(tree);
}

int main(void){    
    report_memory_leaks(true);
    test_token_compare();
    test_insert_token(); //<--- uncomment when implemented
    test_create_bleep_tree(); // <--- uncomment when implemented
    
    char* bleep_words = "schwein affe dumm bloed bloedmann idiot ";
    TreeNode* tree = create_bleep_tree(bleep_words);
    
    print_tree(tree);
    printc('\n');
    
    char buffer[256];
    get_line(buffer, 256);
    printf("Original String: %s\n", buffer);
    
    char* wordStart = buffer;
    char* wordEnd = buffer;
    
    while (*(wordEnd + 1) != '\0') {
        wordEnd++;
        if (*wordEnd == ' ') {
            // create token for every word
            Token* t = new_token(wordStart, wordEnd);
            /*printf("Token is: \"");
            print_token(t, false);
            printf("\".\n");*/
            // censor token in output if in tree
            if (contains(tree, t)) {
                for (char* c = t->start + 1; c < t->end; c++) {
                    *c = '*';
                }
            }
            // set new beginning of word to old word end and free the token
            wordStart = wordEnd + 1;
            free(t);
        }
    }


    printf("Bleeped String: %s\n", buffer);

    free_tree(tree);
    return 0;
}
