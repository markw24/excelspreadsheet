#include "model.h"
#include "interface.h"
#include <string.h>
#include <stddef.h>
#include <stdlib.h>
typedef enum{
    CELL_EMPTY
};
typedef struct HashNode {
    ROW row;
    COL col;
    Cell cell;
    struct HashNode* next;
} HashNode;

// Hash table structure
typedef struct {
    HashNode** table;
    size_t size;
} HashTable;

// Global hash table instance
static HashTable hashTable;

void model_init() {
    // You can initialize the hash table or any other necessary data structure here
    // For simplicity, let's initialize a simple hash table with a fixed size
    hashTable.size = 100;
    hashTable.table = (HashNode**)malloc(sizeof(HashNode*) * hashTable.size);
    memset(hashTable.table, 0, sizeof(HashNode*) * hashTable.size);
}

// Function to set the value of a cell
void set_cell_value(ROW row, COL col, char *text) {
    // Check if the text is NULL or empty
    if (!text || strlen(text) == 0) {
        // Handle invalid input
        return;
    }

    // Find or create a node in the hash table for the specified cell
    HashNode* node = hashTable.table[row % hashTable.size];
    while (node != NULL && node->col != col) {
        node = node->next;
    }

    if (node == NULL) {
        // Create a new node
        node = (HashNode*)malloc(sizeof(HashNode));
        node->row = row;
        node->col = col;
        node->next = hashTable.table[row % hashTable.size];
        hashTable.table[row % hashTable.size] = node;
    }

    // Free the existing text if the cell contains text
    if (node->cell.type == TEXT) {
        free(node->cell.content.text);
    }

    // Set the cell information
    node->cell.type = TEXT;
    node->cell.content.text = strdup(text);

    update_cell_display(row,col,text);
}

// Function to clear the contents of a cell
void clear_cell(ROW row, COL col) {
    // Find the node in the hash table
    HashNode* node = hashTable.table[row % hashTable.size];
    HashNode* prev = NULL;

    while (node != NULL && node->col != col) {
        prev = node;
        node = node->next;
    }

    if (node != NULL) {
        // Free the text content if the cell contains text
        if (node->cell.type == TEXT) {
            free(node->cell.content.text);
        }

        // Remove the node from the linked list
        if (prev == NULL) {
            hashTable.table[row % hashTable.size] = node->next;
        } else {
            prev->next = node->next;
        }

        free(node);
        update_cell_display(row,col,"");
    }

}

// Function to get the textual representation of a cell's value
char *get_textual_value(ROW row, COL col) {
    // Find the node in the hash table
    HashNode* node = hashTable.table[row % hashTable.size];

    while (node != NULL && node->col != col) {
        node = node->next;
    }

    if (node != NULL) {
        // Return a copy of the text content (assuming it's a string)
        char *text = strdup(node->cell.content.text);
        update_cell_display(row, col, text);
    }

    // Return NULL if the cell is not found
    return NULL;
}