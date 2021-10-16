#include "struct.h"
#include <stddef.h>
/**
 * Some functions to check add, remove, check if 
 * we have the value of the correct bytes for the key 
 * we are trying to found
*/

/**
 * After founding each correct bytes of the key
 * put each byte in the key key.
*/
void put_guess_byte_in_key(head_list *list_byte, uint8_t *key){
    int i;
    for(i = 0; i < 16; i++){
        key[i] = list_byte->index_key_byte[i]->byte_found;
    }
}

/**
 * Print each potential byte value for a given node
 * in the linked list
*/
void print_byte_found_node(byte_found_node *byte_found){
    while(byte_found != NULL) {
        printf("%d ", byte_found->byte_found);
        byte_found = byte_found->next;

    }
    printf("\n");
}

/**
 * Print the all the potential bytes found for the key
*/
void print_head_list(head_list *list_byte){
    printf("\nPrint all the byte found for the key guessed\n");
    int i;
    for(i = 0; i < 16; i++){
        printf("Index byte %d :", i);
        print_byte_found_node(list_byte->index_key_byte[i]);
    }
}

/**
 * If we are not in the first turn of the guessed key (we are testing on another A-set)
 * If we found a byte in this turn we don't have to addd it to our linked list. If he's 
 * in the list we just have to remember we have found it. So we set the in_list value
 * of the byte to one for this turn.
*/
void other_turn_set_in_list(uint8_t index_byte, uint8_t byte_value, head_list *list_byte) {
    byte_found_node *current_byte_found_node = list_byte->index_key_byte[index_byte];
    while(current_byte_found_node != NULL) {
        if(current_byte_found_node->byte_found == byte_value) {
            current_byte_found_node->in_list = 1;
            return;
        }
        current_byte_found_node = current_byte_found_node->next;
    }
}

/**
 * For a new turn, on a new A-set, we set all the in_list value 
 * of each byte guessed to 0
*/
void init_new_turn_byte(head_list *list_byte) {
    int i;
    byte_found_node *current_byte_found_node;
    for(i = 0; i < 16; i++){
        current_byte_found_node = list_byte->index_key_byte[i];
        while(current_byte_found_node != NULL){
            current_byte_found_node->in_list = 0;
            current_byte_found_node = current_byte_found_node->next;
        }
    }
}

/**
 * At the end of a turn we removed all the false positive bytes
*/
void remove_all_byte_not_in_turn(uint8_t index_byte, head_list *list_byte) {
    byte_found_node *current_byte_found_node, *to_freed_node;
    while(list_byte->index_key_byte[index_byte]->in_list != 1) {
        to_freed_node = list_byte->index_key_byte[index_byte];
        list_byte->index_key_byte[index_byte] = list_byte->index_key_byte[index_byte]->next;
        free(to_freed_node);
    }
    current_byte_found_node = list_byte->index_key_byte[index_byte];
    while(current_byte_found_node->next != NULL){
        if(current_byte_found_node->next->in_list != 1) {
            to_freed_node = current_byte_found_node->next;
            current_byte_found_node->next = current_byte_found_node->next->next;
            free(to_freed_node);
            continue;
        } 
        current_byte_found_node = current_byte_found_node->next;
    }
}

/**
 * Add a guessed byte in our list if it is not already in.
*/
void add_byte_if_not_in(uint8_t index_byte, uint8_t byte_value, head_list *list_byte){
    if(list_byte->index_key_byte[index_byte] == NULL) {
        byte_found_node *current_byte_found_node = malloc(sizeof(byte_found_node));
        current_byte_found_node->byte_found = byte_value;
        current_byte_found_node->in_list = 1;
        current_byte_found_node->next = NULL;
        list_byte->index_key_byte[index_byte] = current_byte_found_node;
    } else {
        byte_found_node *current_byte_found_node = list_byte->index_key_byte[index_byte];
        if(current_byte_found_node->byte_found == byte_value){
            return;
        }
        while(current_byte_found_node->next != NULL) {
            if(current_byte_found_node->byte_found == byte_value){
                return;
            }
            current_byte_found_node = current_byte_found_node->next;
        }
        byte_found_node *new_byte_found_node = malloc(sizeof(byte_found_node));
        new_byte_found_node->byte_found = byte_value;
        new_byte_found_node->in_list = 1;
        new_byte_found_node->next = NULL;
        current_byte_found_node->next = new_byte_found_node;
    }    
}

/**
 * Initialize our list of guessed bytes (the head of the linked list)
*/
void init_list_byte(head_list *list_byte){
    int i;
    for(i = 0; i < 16; i++){
        list_byte->found_right_byte[i] = 0;
        list_byte->index_key_byte[i] = NULL;
    }
}

/**
 * Go through the list of supposed bytes a given index of the key
 * to know if we have found the correct byte
*/
void check_byte_found_right_value(head_list *list_byte, uint8_t byte_to_check){
    int counter = 1;
    if(list_byte->index_key_byte[byte_to_check] == NULL){
        return ;
    } else {
        byte_found_node *current_byte_found_node = list_byte->index_key_byte[byte_to_check];
        while(current_byte_found_node->next != NULL) {
            current_byte_found_node = current_byte_found_node->next;
            counter++;
        }
        if(counter == 1) {
            list_byte->found_right_byte[byte_to_check] = 1;
        }
    }
}

/**
 * For a given index of the key return 0 if we still have 
 * false positive byte
*/
int check_one_byte_founc(head_list *list_byte, uint8_t byte_to_check){
    return list_byte->found_right_byte[byte_to_check];
}

/**
 * Check if for all of the index of the key we have foudn the right byte.
*/
int check_all_byte_found(head_list *list_byte){
    int i;
    for(i = 0; i < 16; i++){
        if(list_byte->found_right_byte[i] == 0){
            return 0;
        }
    }
    return 1;
}

