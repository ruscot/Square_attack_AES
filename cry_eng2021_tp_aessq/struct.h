#include <stdint.h>
#define AES_BLOCK_SIZE 16
#define AES_128_KEY_SIZE 16

#ifndef __STRUCT__H__
#define __STRUCT__H__

typedef struct byte_found_node byte_found_node;
typedef struct head_list head_list;

struct byte_found_node
{
    uint8_t byte_found;
    int in_list;
    byte_found_node *next;
};

struct head_list
{
    int found_right_byte[16];
    byte_found_node* index_key_byte[16];
};


#endif // __AES-128_ENC__H__
