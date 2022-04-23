// Author: Boyue, Lukas
#include "base.h"

typedef enum {
    NT_DIR,
    NT_FILE,
} NodeType;

typedef struct node_s {
    char name[65];
    NodeType type;
    union {
        struct {
            struct node_s ** nodes;
            int nodes_count;
        } dir;
        struct {
            char * content;
            size_t size;
        } file;
    };
} Node;

Node * make_file(char * name) {
    Node* file = xmalloc(sizeof(Node));

    // setting name
    int i = 0;
    for (; *(name + i) != '\0'; i++) {
        *(file->name + i) = *(name + i);
    }
    *(file->name + i) = '\0';

    file->type = NT_FILE;

    file->file.content = NULL;
    file->file.size = 0;
    
    return file;
}

Node * make_directory(char * name) {
    Node* dir = xmalloc(sizeof(Node));

    // setting name
    int i = 0;
    for (; *(name + i) != '\0'; i++) {
        *(dir->name + i) = *(name + i);
    }
    *(dir->name + i) = '\0';

    dir->type = NT_DIR;

    dir->dir.nodes = NULL;
    dir->dir.nodes_count = 0;
    
    return dir;
}

void insert_into_directory(Node * directory, Node * new_node) {
    // *(directory->dir.nodes) = first node
    // directory->dir.nodes = allocated area
    directory->dir.nodes_count++;
    int dirSize = directory->dir.nodes_count;
    if (dirSize == 1) {
        directory->dir.nodes = xmalloc(sizeof(Node*));
        *(directory->dir.nodes) = new_node;
    } else {
        directory->dir.nodes = xrealloc(directory->dir.nodes, sizeof(Node*) * dirSize + 1); // resizeing
        Node** ptrArr = directory->dir.nodes;
        ptrArr[dirSize-1] = NULL;
        int pos = 0;

        // strcmp() = 1 if first string befor second string
        while (strcmp(ptrArr[pos]->name, new_node->name) < 0 && ptrArr[pos + 1] != NULL) {
            pos++;
        }

        if (strcmp(ptrArr[pos]->name, new_node->name) < 0 && ptrArr[pos + 1] == NULL) {
            pos++;
        } else {
            for (int i = dirSize - pos; i > 0; i--) {
                ptrArr[pos + i] = ptrArr[pos + i - 1];
            }
        }

        ptrArr[pos] = new_node;
    }
}

void print_node(Node * node, char * prefix) {
    Node** ptrArr = node->dir.nodes;

    for (int i = 0; ptrArr[i] != NULL && i < node->dir.nodes_count; ++i) {
        if (ptrArr[i]->type == NT_FILE) {
            if (strcmp(node->name, "") == 0) {
                printf("/");
            } else {
                printf("%s/", prefix);
            }
            
            printf("%s\n", ptrArr[i]->name);
        } else {
            char* path = s_concat(prefix, "/");     
            char* path2 = s_concat(path, ptrArr[i]->name);
            print_node(ptrArr[i], path2);
            free(path);
            free(path2);  
        }
    }
}

Node * find_nodeHelper(Node * node, char * path, char* currentPath) {
    Node** ptrArr = node->dir.nodes;

    if (node->type == NT_FILE && s_equals(path, currentPath)) {
        //printf("|%s|\n", currentPath);
        free(currentPath);
        return node;
    } else if (node->type == NT_FILE) {
        free(currentPath);
        return NULL;
    } else {

        for (int i = 0; ptrArr[i] != NULL && i < node->dir.nodes_count; ++i) {
            //printf("%s\n", currentPath);
            if (s_contains(path, ptrArr[i]->name)) {
                char* oldpath = currentPath;
                currentPath = s_concat(currentPath, "/");
                free(oldpath);
                oldpath = currentPath;
                currentPath = s_concat(currentPath, ptrArr[i]->name);
                free(oldpath);
                // printf("%s\n", currentPath);
                if (ptrArr[i]->type == NT_DIR && s_length(path) - s_length(currentPath) == 1) {
                    free(currentPath);
                    return ptrArr[i];
                }
                return find_nodeHelper(ptrArr[i], path, currentPath);
            } else if (s_equals(path, currentPath)) {
                free(currentPath);
                return node;
            }
        }
    }
    free(currentPath);
    return NULL;
}

Node * find_node(Node * node, char * path) {
    Node** ptrArr = node->dir.nodes;
    Node* output = NULL;
    
    if (s_length(path) == 1) {
        return node;
    }


    for (int i = 0; ptrArr[i] != NULL && i < node->dir.nodes_count; ++i) {
        //printf("%d\n", i);
        char* name = s_concat("/", ptrArr[i]->name);
        Node* output = find_nodeHelper(ptrArr[i], path, name);
        //free(name);
        if (output != NULL) {
            // printf("%p\n", output);
            return output;
        }
    }
    // printf("%p\n", output);
    return output;
}

void free_node(Node * node) {
    Node** ptrArr = node->dir.nodes;
    for (int i = 0; ptrArr[i] != NULL && i < node->dir.nodes_count; ++i) {
        if (ptrArr[i]->type == NT_DIR) {
            free_node(ptrArr[i]);
        } else {
            free(ptrArr[i]);
        }
    }

    if (node->type == NT_DIR) {
        free(ptrArr);
    }
    free(node);
}

int main(void) {
    report_memory_leaks(true);

    // Create a filesystem
    Node * root = make_directory("");

    // Insert some directories and files
    Node * test_file = make_file("test.txt");
    insert_into_directory(root, test_file);
    insert_into_directory(root, make_file("hello.c"));
    insert_into_directory(root, make_file("world.c"));

    Node * home = make_directory("home");

    Node * user = make_directory("user");
    insert_into_directory(user, make_file("image.jpg"));
    Node * config_file = make_file("config.cfg");
    insert_into_directory(user, config_file);
    insert_into_directory(user, make_file("game.exe"));

    insert_into_directory(home, user);
    insert_into_directory(root, home);

    Node * system = make_directory("system");
    insert_into_directory(system, make_file("8_configuration.txt"));
    insert_into_directory(system, make_file("4_processes.txt"));

    Node * secret = make_directory("secret");
    Node * flag_file = make_file("flag.txt");
    insert_into_directory(secret, flag_file);

    insert_into_directory(root, system);
    insert_into_directory(system, secret);

    insert_into_directory(root, make_file("archive.a"));

    // Output the filesystem's content
    printf("\n");
    print_node(root, "");
    printf("\n");

    // Make sure we can extract the contained nodes again:
    // files
    test_equal_i(test_file == find_node(root, "/test.txt"), true);
    test_equal_i(flag_file == find_node(root, "/system/secret/flag.txt"), true);
    test_equal_i(config_file == find_node(root, "/home/user/config.cfg"), true);
    // directory without trailing slash
    test_equal_i(user == find_node(root, "/home/user"), true);
    // directory with trailing slash
    test_equal_i(secret == find_node(root, "/system/secret/"), true);
    // returning root itself
    test_equal_i(root == find_node(root, "/"), true);
    // invalid paths
    test_equal_i(find_node(root, "/test.txt/") == NULL, true);
    test_equal_i(find_node(root, "/doesnot.exist") == NULL, true);
    test_equal_i(find_node(root, "/also/does/not/exist") == NULL, true);
    test_equal_i(find_node(root, "/world.c/invalid") == NULL, true);

    free_node(root);
    return 0;
}
