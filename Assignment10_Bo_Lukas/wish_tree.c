// Author: Boyue, Lukas
/*
    make wish_tree
    ./wish_tree
    make wish_tree && ./wish_tree
*/

#include "base.h"


typedef struct Element Element;

void free_element(Element* element);


typedef struct Node{
    struct Node* next;
    char* value;
}Node;

/*
    Returns a pointer to a dynamically allocated node.
*/
Node* new_node(char* value, Node* next){
    Node* node = xmalloc(sizeof(Node));
    node->next = next;
    node->value = s_copy(value);
    return node;
}

/* 
    Releases the memory used by node. 
*/
Node* free_node(Node* node){
    Node* next = node->next;
    free(node->value);
    free(node);
    return next;
}

/*
    Returns the length of a list of nodes.
*/
int length(Node* node){
    int count = 0;
    while(node != NULL){
        count++;
        node = node->next;
    }
    return count;
}

/*
    Returns true if the list of nodes contains find.
*/
bool contains(Node* node, char* find){
    while(node != NULL){
        if(strcmp(node->value, find) == 0){
            return true;
        }
        node = node->next;
    }
    return false;
}


/* 
    A TreeNode has a left and right successor of type TreeNode. A successor TreeNode is a TreeNode or NULL
        
            TreeNode root
           /          \
          /            \
    TreeNode left      TreeNode right
    /       \       /           \
   ...      ...    ...          ...
*/

typedef struct TreeNode{
    struct TreeNode* left;
    struct TreeNode* right;
    struct Element* element;
}TreeNode;

/*
    Returns a pointer to a dynamically allocated tree_node.
*/
TreeNode* new_tree_node(Element* element){
    TreeNode* bnode = xmalloc(sizeof(TreeNode));
    bnode->left = NULL;
    bnode->right = NULL;
    bnode->element = element;
    return bnode;
}

/*  todo: g)
    Releases the memory used by a tree_node and also frees the included element.
*/
void free_tree_node(TreeNode* tree){
    if (tree->left != NULL) {
        free_tree_node(tree->left);
    }

    if (tree->right != NULL) {
        free_tree_node(tree->right);
    }

    free_element(tree->element);
    free(tree);
}


//todo: b)
struct Element{
    char* wishText;
    int ammount;
    char* names;
};

//todo: b)
Element* new_element(char* wish, char* child){
    Element* e = xmalloc(sizeof(Element));
    e->wishText = s_copy(wish);
    e->ammount = 1;
    e->names = s_copy(child);
    return e;
}

//todo: g)
void free_element(Element* element){
    free(element->wishText);
    free(element->names);
    free(element);
}

//todo: c)
TreeNode* add_wish(TreeNode* tree, char* wish, char* child){
    // tree empty
    if (tree == NULL) {
        //printf("new TreeNode\n");
        Element* e = new_element(wish, child);
        return new_tree_node(e);
    }

    // wunsch == element im baum
    if (strcmp(wish, tree->element->wishText) == 0) {
        tree->element->ammount++;
        child = s_concat(" ", child);

        char* oldNameList = tree->element->names;
        tree->element->names = s_concat(oldNameList, child);
        free(child);
        free(oldNameList);

        /*for some reason the strcomp() dont seem to work properly (the list is not ordered) but with s_compare() the function works like i want it to*/
    } else if (s_compare(wish, tree->element->wishText) == -1) { // go to the left 
        //printf("go left ");
        tree->left = add_wish(tree->left, wish, child);
    } else { // go to the right
        //printf("go right ");
        tree->right = add_wish(tree->right, wish, child);
    }
    return tree;
}

typedef struct ElementNode{
    struct ElementNode* next;
    Element* element;
}ElementNode;

ElementNode* new_element_node(Element* element, ElementNode* next){
    ElementNode* en = xmalloc(sizeof(ElementNode));
    en->next = next;
    en->element = element;
    return en;
}
//todo: g)
void free_element_list(ElementNode* en){
    if (en->next != NULL) {
        free_element_list(en->next);
    }
    free(en);
}

void print_element_list(ElementNode* list, int n){
    printf("%55s\t%6s\n", "Wunsch", "Anzahl");
    while(n > 0 && list != NULL){
        printf("%55s\t%6d\n", list->element->wishText, list->element->ammount); 
        n--;
        list = list->next;
    }
}

//todo: e)
ElementNode* createList(ElementNode* result, TreeNode* tree){

    // for going down the tree
    if (tree->left != NULL) {
        //printf("going left\n");
        result = createList(result, tree->left);
    }

    if (tree->right != NULL) {
        //printf("going right\n");
        result = createList(result, tree->right);
    }

    // ------
    if (result == NULL) {
        result = new_element_node(tree->element, NULL);
    } else {
        result = new_element_node(tree->element, result);
    }
  
    return result;
}


ElementNode* insert_ordered_by_count(ElementNode* result, TreeNode* tree){

    result = createList(result, tree);
    ElementNode* listStart = result;
    bool sorted = true;

    // bubble sort because i don't know how to it differently and i can't be botherd to search for a faster sorting algorythem
    do {
        sorted = true;
        result = listStart;
        while(result->next != NULL) {
            if (result->element->ammount < result->next->element->ammount) {
                Element* temp = result->next->element;
                result->next->element = result->element;
                result->element = temp;
                sorted = false;
            }
            result = result->next;
        }
    } while (!sorted);
    
    
    return listStart;
}

//todo: d)
void print_tree_as_list(TreeNode* tree){
    if (tree->left != NULL) {
        print_tree_as_list(tree->left);
    }
    
    printf("%55s %6d %s\n",tree->element->wishText, tree->element->ammount, tree->element->names);
  
    if (tree->right != NULL) {
        print_tree_as_list(tree->right);
    }
}

// Skips the rest of the current line.
char* skip_line(char* s) {
    while (*s != '\n' && *s != '\0') s++;
    if (*s == '\n') s++;
    return s;
}

// Skips the current token.
char* skip_token(char* s) {
    while (*s != ',' && *s != '\n' && *s != '\0') s++;
    return s;
}

// Skips spaces.
char* skip_spaces_and_commas(char* s) {
    while (*s == ' ' || *s == ',') s++;
    return s;
}


// Create a new token from s (inclusive) to t (exclusive).
char* new_token(char* s, char* t) {
    char* token = xmalloc(t - s + 1);
    int i = 0;
    while (s < t) {
        token[i] = *s;
        i++;
        s++;
    }
    token[i]= '\0';
    return token;
}

// Returns a new token by first skipping spaces and commas and then reading the token. 
// Sets *s to the first character after the token.
char* next_token(/*INOUT*/char** s) {
    *s = skip_spaces_and_commas(*s);
    char* t = skip_token(*s);
    char* token = new_token(*s, t);
    *s = t; // set *s to the character after the token
    return token;
}


/* 
    This function reads in one line of the wish text file and adds the wishes of a child to tree.
*/
TreeNode* read_wish_list(char** s, TreeNode* tree){
    char* child = next_token(s);
    
    while(**s != '\0' && **s != '\n'){
        char* wish = next_token(s);
        *s = skip_spaces_and_commas(*s);
        tree = add_wish(tree, wish, child); 
        free(wish);
    }
    
    free(child);
    return tree;
}

/*  
    This function reads the whole file and uses the read_wish_list function to parse a single line.
*/
TreeNode* read_wishes(char* filename, TreeNode* tree){
    char* file = s_read_file(filename);
    char* s = file;
    s = skip_line(s);
    while(*s != '\0'){
        tree = read_wish_list(&s, tree);
        s = skip_line(s);
        
    }
    free(file);
    return tree;
}


int main(int argc, char** argv){
    report_memory_leaks(true);
    
    TreeNode* tree = read_wishes("wishes.txt", NULL);
    
    print_tree_as_list(tree);
    
    ElementNode* sorted_by_count = insert_ordered_by_count(NULL, tree);
    
    print_element_list(sorted_by_count, 11);
    
    
    //todo: f)
    ElementNode* wishList = sorted_by_count;
    char c[] = "";
    char* names = c;
    for (int i = 0; i < 11; i++) {
        char* currentNames = wishList->element->names;
        int nameStart = 0;
        int nameEnd = 0;

        while (*(currentNames + nameEnd) != '\0') {
            if (*(currentNames + nameEnd) == ' ' || *(currentNames + nameEnd) == '\0') {
                char* name = s_sub(currentNames, nameStart, nameEnd);

                if (!s_contains(names, name)) {
                    char* oldname = name;
                    name = s_concat(" ", name);
                    free(oldname);

                    char* oldNames = names;
                    names = s_concat(oldNames, name);

                    if (!(i == 0 && nameStart == 0)) {
                        s_free(oldNames);
                    }
                }

                nameStart = nameEnd + 1;
                free(name);
            }
            nameEnd++;
        }
        wishList = wishList->next;
    }

    printf("kids that get a present:\n");
    printf("%s\n", names);

    int childrenWithPresent = 0;
    char* names2 = names;
    while (*names != '\0') {
        if (*names == ' ') childrenWithPresent++;
        names++;
    }

    if (childrenWithPresent >= 29) {
        printf("Every child gets a present\n");
    } else {
        printf("Unfortunatly not every child gets a present. %d/29!\n", childrenWithPresent);
    }
    
    //todo: g)
    free(names2);
    free_element_list(sorted_by_count);
    free_tree_node(tree);



    return 0;
}