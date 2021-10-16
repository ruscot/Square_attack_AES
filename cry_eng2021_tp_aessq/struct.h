#include <stdint.h>
#define AES_BLOCK_SIZE 16
#define AES_128_KEY_SIZE 16

#ifndef __STRUCT__H__
#define __STRUCT__H__

/***
 * This structure is a linked list 
 * We are using it to determine the false positive bytes we have 
 * found during the square attack.
*/
typedef struct byte_found_node byte_found_node;
typedef struct head_list head_list;

struct byte_found_node
{
    //Potential bytes for a given index 
    uint8_t byte_found;
    //Boolean to determine if the value of the byte of index i is
    //in the new round (on new A-set, yes if it's equal to 1). 
    int in_list;
    //Next byte potential value
    byte_found_node *next;
};

struct head_list
{
    //Boolean table for byte of index i if found_right_byte[i] == 1 we have found 
    //the right byte value 
    int found_right_byte[16];
    //One linked per index of bytes
    byte_found_node* index_key_byte[16];
};


#endif // __AES-128_ENC__H__
