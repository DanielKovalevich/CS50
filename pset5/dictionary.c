/**
 * dictionary.c
 *
 * Computer Science 50
 * Problem Set 5
 *
 * Implements a dictionary's functionality.
 * 
 * Will attempt to use a trie in order to decrease time in checking.
 * It has been noted that this may be a bit bigger and will take longer to load
 * than other data structures like a hash table.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "dictionary.h"

// Create the root node
node *root;

// Global variable because cannot manipulate parameters of functions
// Used in size for quicker computation
long numOfWords;

/**
 * Returns true if word is in dictionary else false.
 */
bool check(const char* word)
{
    // Used to traverse the trie
    node *travNode = root;
    
    // Iterates through each character to see if word exists
    for (int i = 0; i < strlen(word); i++){
        
        int index = 0;
        // If the word is uppercase, then it makes sure
        // the index will be for corresponding lower case letter
        if (word[i] == 39)
            index = 26;
        else if (word[i] < 'a')
            index = tolower(word[i]) - 'a';
        else
            index = word[i] - 'a';
            
        if (travNode->child[index] == NULL)
            return false;
        else
            travNode = travNode->child[index];
    }
    
    return travNode->isWord;
}

/**
 * Loads dictionary into memory.  Returns true if successful else false.
 */
bool load(const char* dictionary)
{
    numOfWords = 0;
    // Allocates enough space for each word, which in this case is 46
    char word[LENGTH + 2];
    root = calloc(1, sizeof(node));
    
    // Opens file for use
    FILE* dicto = fopen(dictionary, "r");
    
    // Checks to see if could open file
    if (dicto == NULL){
        printf("Couldn't open dictionary!");
        return 1;
    }
    
    // Goes through the whole dictionary until EOF
    while (fgets(word, LENGTH + 2, dicto) != NULL){
        // Used to traverse the trie
        node *travNode = root;
        
        // Goes through each letter of the word
        for (int i = 0; i < strlen(word); i++) {
            if (word[i] == '\n'){
                travNode->isWord = true;
                numOfWords++;
            }
            else{
                int index = (word[i] == '\'') ? 26 : word[i] - 'a';
                
                // Creates a node for child of node
                if (travNode->child[index] == 0){
                    travNode->child[index] = calloc(1, sizeof(node));
                    travNode = travNode->child[index];
                }
                // If node already exists, go to next node
                else
                    travNode = travNode->child[index];
            }
        }
    }

    fclose(dicto);
    
    return true;
}

/**
 * Returns number of words in dictionary if loaded else 0 if not yet loaded.
 */
unsigned int size(void)
{
    return numOfWords;
}

/**
 * Unloads dictionary from memory.  Returns true if successful else false.
 */
bool unload(void)
{
    for (short i = 0; i < 27; i++){
        if (root->child[i] == NULL){
            if (i + 1 >= 27){
                //for (short j = 0; j < 27; j++)
                //    free(root->child[j]);
                free(root);
                return true;
            }
        }
        // When root is not NULL
        else{
            unloadChild(root->child[i]);
        }
    }
    
    return false;
}

// Unloads the children of the nodes
void unloadChild(node* node){
    // Goes through each node of each child
    for (short i = 0; i < 27; i++){
        if (node->child[i] != 0){
            unloadChild(node->child[i]);
        }
    }
    
    free(node);
}
