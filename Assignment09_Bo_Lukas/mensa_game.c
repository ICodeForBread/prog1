// Author: Boyue, Lukas
/*
Compile: make mensa
Run: ./mensa
Compile & run: make mensa && ./mensa
*/

#include "base.h"

/**
Represents a single list node. The string value is dynamically allocated. 
The node is the owner of the value and has to free it when released.
*/
struct Node {
    char* value; // dynamically allocated string, release memory!
    struct Node* next; // self-reference
};
typedef struct Node Node;

/**
Create a list node. Copies the value (into dynamically allocated storage).
*/
Node* new_node(String value, Node* next) {
    Node* node = xcalloc(1, sizeof(Node));
    node->value = s_copy(value); // s_copy performs dynamic allocation
    node->next = next;
    return node;
}

/**
Prints the components of the given list.
*/
void print_list(Node* list) {
    if (list == NULL) {
        printf("[]");
    } else {
        printf("[%s", list->value);
        for (Node* n = list->next; n != NULL; n = n->next) {
            printf(", %s", n->value);
        }
        printf("]");
    }
}

/**
Print list followed by a newline.
*/
void println_list(Node* list) {
    print_list(list);
    printsln("");
}

/**
Return the number of elements in the list.
*/
int length_list(Node* list) {
    int output = 1;
    for (Node* n = list->next; n != NULL; n = n->next) {
        output++;
    }
    return output;
}

/**
Return the element at position.
*/
String get_list(Node* list, int position) {
    Node* n = list;
    for (int i = 0; i < position; ++i) {
        n = n->next;
    }
    return n->value;
}

/**
Free all nodes of the list, including the values it contains.
*/
void free_list(Node* n) {
    if (n->next != NULL) {
        free_list(n->next);
    }
    free(n->value);
    free(n);
}

/**
Adds an element to the end of the list. Modifies the existing list.
*/
Node* append_list(Node* list, String value) {
    Node* listStart = list;
    while(list->next != NULL) {
        list = list->next;
    }
    list->next = new_node(value, NULL);
    return listStart;
}

// Remove element at position index from list. The element at index has to be deleted.
Node* remove_list(Node* list, int index) {
    Node* listStart = list;
    if (list == NULL || index < 0) return list;
    if (index == 0) { // remove first element of non-empty list
        Node* n = list->next;
        free(list->value);
        free(list);
        return n;
    }
    // return second or later element of non-empty list
    int i = 0;
    for (Node* node = list; node != NULL; node = node->next) {
        if (i + 1 == index) {
            Node* to_remove = node->next;
            if (to_remove == NULL) return list;
            Node* n = to_remove->next;
            free(to_remove->value);
            free(to_remove);
            node->next = n;
            return listStart;
        }
        i++;
    }
    return list;
}

/**
Today's menu.
*/
const String menu[] = { "Currywurst", "Spaghetti", "Vegi", "Eintopf", "Salat" };

/**
The number of items on the menu.
*/
const int menu_count = sizeof(menu) / sizeof(char*);

/**
The list of completed food from the kitchen, waiting to be served. Each time a food item is served, the kitchen prepares a new food item randomly from the menu.
*/
Node* food; // List(String)

/**
The list of students standing in line and waiting for food. Each entry contains a string for the food that this student wants.
*/
Node* students; // List(String)

/**
The reputation of the mensa. The player is a staff member of the mensa. The goal of the player is to maximize the reputation of the mensa. Actually serving a dish that was asked for increases the reputation of the mensa by 1. Serving the wrong dish decreases the reputation by 1. If a student asks for a dish that is on the menu but not ready to be served, the reputation of the mensa decreases by 2.
*/
int reputation = 0;

/**
Example output:
fertige Essen: [Spaghetti, Vegi, Salat, Salat, Vegi]
nächster Essenswunsch: Currywurst (3 hungrige Studierende warten)
Reputation der Mensa: 0
> 
*/
void print_situation(void) {
    printf("fertige Essen: ");
    print_list(food);
    printf("\nnächster Essenswunsch: %s (%d Studenten warten)\n", get_list(students, 0), length_list(students));
    printf("Reputation der Mensa: %d\n", reputation);
    
}

/**
    Print final message, release all dynamically allocated memory, and exit the program.
    Fertig für heute. Die Mensa schließt.
    Finale Reputation der Mensa: 3
*/
void finish(void) {
    printf("Fertig für heute. Die Mensa schließt.\n");
    printf("Finale Reputation der Mensa: %d\n", reputation);
    free_list(food);
    //free_list(students);
    exit(0);
}

/**
    Run the mensa simulation.

    The mensa person then enters the (0-based) index from the list of food. -1 stands for "not available". -2 ends the program. 

    Here is an example dialog:

    fertige Essen: [Salat, Vegi, Vegi, Spaghetti, Eintopf]
    nächster Essenswunsch: Spaghetti (3 hungrige Studierende warten)
    Reputation der Mensa: 0
    > 3
    Vielen Dank! Ich liebe die Mensa!
    fertige Essen: [Salat, Vegi, Vegi, Eintopf, Eintopf]
    nächster Essenswunsch: Spaghetti (3 hungrige Studierende warten)
    Reputation der Mensa: 1
    > -1
    Spaghetti ist nicht da? Schade.
    fertige Essen: [Salat, Vegi, Vegi, Eintopf, Eintopf]
    nächster Essenswunsch: Salat (2 hungrige Studierende warten)
    Reputation der Mensa: -1
    > 0
    Vielen Dank! Ich liebe die Mensa!
    fertige Essen: [Vegi, Vegi, Eintopf, Eintopf, Eintopf]
    nächster Essenswunsch: Eintopf (2 hungrige Studierende warten)
    Reputation der Mensa: 0
    > 1
    Vegi möchte ich nicht! Ich möchte Eintopf! Dann hole ich mir halt einen Döner!
    fertige Essen: [Vegi, Vegi, Eintopf, Eintopf, Eintopf]
    nächster Essenswunsch: Currywurst (1 hungrige Studierende warten)
    Reputation der Mensa: -1
    > 1
    Vegi möchte ich nicht! Ich möchte Currywurst! Dann hole ich mir halt einen Döner!
    Fertig für heute. Die Mensa schließt.
    Reputation der Mensa: -2
*/
void run_mensa(void) {
    // todo: implement
    // create 5 random food items from the menu
    food = new_node(menu[i_rnd(5)], NULL);
    for (int i = 1; i < 5; ++i) {
        append_list(food, menu[i_rnd(5)]);
    }
    //print_list(food);
    //printf("\n");

    // create 3 random food wishes from the menu (each wish from one student)
    students = new_node(menu[i_rnd(5)], NULL);
    for (int i = 1; i < 3; ++i) {
        append_list(students, menu[i_rnd(5)]);
    }
    //print_list(students);
    //printf("\n");
    
    print_situation();
    
    int i;
    while ((i = i_input()) >= -1) {
        // process input
        // distinguish different cases, update lists
        // use get_list, remove_list, append_list, length_list
        // use i_rnd to select random food items and random food wishes
        // use s_equals or strcmp to compare strings

        int foodAvail = 0;
        for (int j = 0; j < menu_count; ++j) {
            if (s_equals(menu[i], menu[j])) {
                foodAvail = 1;
            }
        }
        if (!foodAvail || i == -1) {
            printf("%s ist nicht da? Schade.\n", get_list(students, 0));
            reputation -= 2;
            students = remove_list(students, 0);
        } else if (s_equals(get_list(food, i), get_list(students, 0))) {
            printf("Vielen Dank! Ich liebe die Mensa!\n");
            reputation++;
            food = remove_list(food, i);
            food = append_list(food, menu[i_rnd(menu_count)]);
            students = append_list(students, menu[i_rnd(menu_count)]);
            students = remove_list(students, 0);
        } else {
            printf("%s möchte ich nicht! Ich möchte %s! Dann hole ich mir halt einen Döner!\n", menu[i], get_list(students, 0));
            reputation--;
            students = remove_list(students, 0);
            food = remove_list(food, i);
            food = append_list(food, menu[i_rnd(menu_count)]);
        }
        if (students == NULL) {
            finish();
        }
        print_situation();
    }
    finish();
}

int main(void) {
    report_memory_leaks(true);
    run_mensa();
    return 0;
}
