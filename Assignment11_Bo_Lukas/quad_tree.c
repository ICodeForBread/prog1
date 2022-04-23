// Author: Boyue, Lukas
#define STBIW_ASSERT(x)
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#include "base.h"

// The maximum amount of elements a quad tree node can contain before splitting
// into more
const int MAX_ELEMENTS = 4;

// The maximum depth of the quad tree. The root node has a depth of 0. The
// depth of a node is equal to the depth of its parent incremented by 1.
const int MAX_DEPTH = 10;

// The bounds of the area represented by the quad tree.
const int WIDTH = 512;
const int HEIGHT = 512;

// This struct represents an element that is stored in the tree.
typedef struct element_s {
    double x, y;
} Element;

// A node of the quad tree.
typedef struct node_s {
    // The coordinates and size of the node.
    int x, y, w, h;
    // The node's depth.
    int depth;
    // Tagged union, store different data for leaf and non-leaf nodes.
    bool is_leaf;
    union {
        struct {
            // This node's children.
            struct node_s * tl, * tr, * bl, * br;
        } subtree;
        // The elements stored by this node.
        List elements;
    };
} Node;

Element new_element(double x, double y) {
    return (Element) {
        .x = x,
        .y = y
    };
}

Node * new_node(int x, int y, int width, int height, int depth) {
    Node * node = xmalloc(sizeof(Node));
    *node = (Node) {
        .x = x,
        .y = y,
        .w = width,
        .h = height,
        .depth = depth,
        .is_leaf = true,
        .elements = l_create(sizeof(Element)),
    };

    return node;
}

void free_node(Node * node) {
    if (!node->is_leaf) {
        free_node(node->subtree.tl);
        free_node(node->subtree.tr);
        free_node(node->subtree.bl);
        free_node(node->subtree.br);
    } else {
        l_free(node->elements);
    }
    free(node);
}


void insert_element(Node * node, Element e) {

    if (node->is_leaf && l_length(node->elements) < MAX_ELEMENTS) {
        l_append(node->elements, &e);
    } else if (node->is_leaf && l_length(node->elements) >= MAX_ELEMENTS) {
        if (node->depth >= MAX_DEPTH) return;
        // printf("new node\n");
        // create 4 new nodes and put elements in the propper nodes
        // put nodes in a new node (maybe?)
        // change old node pointer to point to new node (depends on line before)
        // free old node (depends on line before)

        // properties of nodes
        int currentX = node->x;
        int currentY = node->y;
        int currentWidth = node->w;
        int currentHeight = node->h;
        int newWidth = currentWidth / 2;
        int newHeight = currentHeight / 2;
        int newDepth = node->depth + 1;

        // pointer to list of elements
        List elements = node->elements;

        node->is_leaf = false;
        // create new nodes
        node->subtree.tl = new_node(currentX,              currentY,               newWidth, newHeight, newDepth);
        node->subtree.tr = new_node(currentX + newWidth,   currentY,               newWidth, newHeight, newDepth);
        node->subtree.bl = new_node(currentX,              currentY + newHeight,   newWidth, newHeight, newDepth);
        node->subtree.br = new_node(currentX + newWidth,   currentY + newHeight,   newWidth, newHeight, newDepth);

        int length = l_length(elements);

        while(length) {
            Element* element = l_get(elements, length - 1);
            insert_element(node, *element);
            l_remove(elements, length - 1);
            length--;
        }

        assert("sub == NULL", node->subtree.tl != NULL);
        assert("sub == NULL", node->subtree.tr != NULL);
        assert("sub == NULL", node->subtree.bl != NULL);
        assert("sub == NULL", node->subtree.br != NULL);

        l_free(elements);


    } else {
        // going down the tree by eliminating alternative paths

        if (node->subtree.br->x <= e.x && node->subtree.br->y <= e.y) { // bottom right
            insert_element(node->subtree.br, e);
        } else if (node->subtree.bl->x <= e.x && node->subtree.bl->y <= e.y) { // bottom left
            insert_element(node->subtree.bl, e);
        } else if (node->subtree.tr->x <= e.x && node->subtree.tr->y <= e.y) { // top right
            insert_element(node->subtree.tr, e);
        } else if (node->subtree.tl->x <= e.x && node->subtree.tl->y <= e.y) { // top left
            insert_element(node->subtree.tl, e);
        } else {
            printf("somthing went wrong\n");
        }
    }
}

void set_pixel(unsigned char * canvas, unsigned char r, unsigned char g, unsigned char b, int x, int y) {
    canvas[(y * WIDTH * 3) + (x * 3)] = r;
    canvas[(y * WIDTH * 3) + (x * 3) + 1] = g;
    canvas[(y * WIDTH * 3) + (x * 3) + 2] = b;
}

void draw_node(Node * root, unsigned char * canvas) {

    int i1 = 0;
    while (i1 < root->w) { // top + bottom line
        set_pixel(canvas, 0, 0, 0, i1 + root->x, root->y);
        //set_pixel(canvas, 0, 0, 0, i1 + root->x, root->h);
        i1++;
    }
    int i2 = 0;
    while (i2 < root->h) { // left + right line
        set_pixel(canvas, 0, 0, 0, root->x, i2 + root->y);
        //set_pixel(canvas, 0, 0, 0, root->w, i2 + root->y);
        i2++;
    }

    if (root->is_leaf) {
        List elements = root->elements;
        int length = l_length(elements);

        while(length) {
            Element* element = l_get(elements, length - 1);
            set_pixel(canvas, 255, 0, 0, element->x, element->y);
            length--;
        }

    } else {
        draw_node(root->subtree.tl, canvas);
        draw_node(root->subtree.tr, canvas);
        draw_node(root->subtree.bl, canvas);
        draw_node(root->subtree.br, canvas);
    }
    
}

// This function generates a random, normally distributed value. Its mean value
// is the middle of min and max. It is ensured that the random value does not
// exceed min or max.
double random_coordinate(double min, double max) {
    double u1 = d_rnd(1.);
    double u2 = d_rnd(1.);
    double r = sqrt(-2.*log(u1))*cos(2.*3.14159*u2);

    double coordinate = (max - min) / 2. + r * 100.;
    if(coordinate < min) return min;
    if(coordinate > max) return max;
    else return coordinate;
}

int main(void) {
    report_memory_leaks(true);

    // Insert randomly placed nodes in the tree.
    Node * root = new_node(0, 0, WIDTH, HEIGHT, 0);
    for(int i = 0; i < 400; i++) {
        double x = random_coordinate(0, WIDTH);
        double y = random_coordinate(0, HEIGHT);
        insert_element(root, new_element(x, y));
    }

    // Allocate a canvas to draw onto with WIDTH * HEIGHT pixels with 3
    // components (red, green and blue) for each pixel.
    unsigned char * canvas = xmalloc(sizeof(unsigned char) * WIDTH * HEIGHT * 3);

    // Set every pixel on the canvas to white
    memset(canvas, 255, sizeof(unsigned char) * WIDTH * HEIGHT * 3);

    draw_node(root, canvas);

    // Create a PNG file from the data in the canvas
    // (you can view this in any image software you like)
    stbi_write_png("quad_tree.png", WIDTH, HEIGHT, 3, canvas, 0);

    free(canvas);
    free_node(root);
    return 0;
}
