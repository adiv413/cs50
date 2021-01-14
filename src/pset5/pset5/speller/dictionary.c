/**
 * Implements a dictionary's functionality.
 * Thanks to @chrisdaly and @dpmehta02 for the help
 */

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#include "dictionary.h"

#define HASHTABLE_SIZE 65536

// define a node type
typedef struct node
{
	char word[LENGTH+1];
	struct node* next;

}
node;

// define a variable to count the words loaded from the dictionary
int word_count;

// define a hashtable
node* hashtable[HASHTABLE_SIZE] = {NULL}; 

// hashing function
int hashing_function(char* word)
{
    unsigned int hash = 0;
    for (int i = 0, n = strlen(word); i < n; i++)
        hash = (hash << 2) ^ word[i];
    return hash % HASHTABLE_SIZE;
}
 // create a temporary node
node* new_node;
/**
 * Returns true if word is in dictionary else false.
 */
 
bool check(const char *word)
{
    int l = strlen(word);
    char *copy = malloc(l + 1);
    
    
    // copy the characters and make them lower case
    for (int i = 0; i < l; i++)
        copy[i] = tolower(word[i]);
    
    // add the end of string character
    copy[l] = '\0';

    //  hash the word
    int index = hashing_function(copy);
    node* dictword = hashtable[index];
    if (dictword)
    {
        // point a cursor node to the head node
        node* cursor = dictword;
        

        // loop through the nodes while a next pointer isn't null
        while (cursor->next != NULL)
            {
                if (strcmp(copy, cursor->word) == 0)
                {
                    free(copy);
                    return true;
                };
                cursor = cursor->next;
            };

        if (strcmp(copy, cursor->word) == 0)
            {
            free(copy);
    		return true;
            };
    	free(copy);

		cursor = cursor->next;
	    
    }
    free(copy);
    return false;
}


/**
 * Loads dictionary into memory. Returns true if successful else false.
 */
bool load(const char *dictionary)
{
    
    FILE* file = fopen(dictionary, "r");
    
    int count = 0;
    
    if (file == NULL)
    	{
    		printf("Could not open %s.\n", dictionary);
    		return false;
    	}
    	
    // set a buffer to store an output word of length = LENGTH + 1 for null terminator
    char buffer[LENGTH+2];

    // loop through the dictionary until a null character
    while (fgets(buffer, sizeof(buffer), file))
    {
        
    
        // overwrite the \n with \0
        buffer[strlen(buffer) - 1] = '\0';
        
        node* new_node = malloc(sizeof(node));
        
        count++;
        
        
        if (feof(file))
        {
            // hit end of file
            free(new_node);
            break;
        }

    	// set the node's pointers to the word and to the next node
        // copies buffer's contents into the location of new_node's word pointer
    	strncpy(new_node->word, buffer, LENGTH + 1);
    	new_node->next = NULL;
    	
    	

    	// hash the word to get the index
    	int index = hashing_function(buffer);
    	//printf("buffer:%s index:%i", buffer, index);

    	// if the hashtable contains no value at that index then assign the new_node node there
        // (the entry takes the pointers of new_node)
    	if (hashtable[index] == NULL)
    	{
    		hashtable[index] = new_node;
    	}

    	// otherwise append the node to the end of the linked list
    	else
    	{
    		// point a cursor node to the index, (i.e the first entry in the linked list)
    		node* cursor = hashtable[index];

    		// loop through the nodes while the next pointer isn't null
    		while (cursor->next != NULL)
    		{
    			cursor = cursor->next;
    		}

    	// the end of the linked list has been found, append the node by changing the shared pointer
    		cursor->next = new_node;
    		
    	}
    	
        word_count++;
    }
    printf("%i", count);
    // close the file
    fclose(file);

    return true;
    
}

/**
 * Returns number of words in dictionary if loaded else 0 if not yet loaded.
 */
unsigned int size(void)
{
    return word_count;
}

/**
 * Unloads dictionary from memory. Returns true if successful else false.
 */
bool unload(void)
{
    for (int i = 0; i < HASHTABLE_SIZE; i++)
    {
        // check the table for a node at that index
        node* cursor = hashtable[i];
        while (cursor != NULL)
        {
            // create a temporary node to save the position of the next node
            node* nextnode = cursor->next;

            // free the current node
            free(cursor);
            

            // move the cursor to the next node
            cursor = nextnode;
            
        }
        
        hashtable[i] = NULL;
    }
     
    return true;
}
