#include"dlist.h"
#include<stdlib.h>

typedef struct DList_Node {
    void* data;
    size_t item_id;
    void* prev;
    void* next;
} DList_Node;

typedef struct DList_ {
    DList_Node* head;
    DList_Node* tail;
    size_t itemSize;
} DList;

void* dlist_create(size_t itemSize) {
    DList* new_dlist = (DList*)malloc(sizeof(DList));
    if (!new_dlist) return NULL;
    new_dlist->itemSize = itemSize;
    return new_dlist;
}

void dlist_destroy(void* dlist, void(*destroy)(void*)) {
    dlist_clear(dlist, destroy);
    free(dlist);
}

void* dlist_init(
    void* dlist,
    size_t itemSize,
    void(*destroy)(void*)) {
    if (((DList*)dlist)->head) {
        dlist_clear(dlist, destroy);
    }
    ((DList*)dlist)->itemSize = itemSize;
    return dlist;
}

void dlist_clear(void* dlist, void(*destroy)(void*)) {
    DList_Node* current = ((DList*)dlist)->head;
    while (current) {
        DList_Node* temp = current->next;
        destroy(current);
        current = temp;
    }
    ((DList*)dlist)->head = NULL;
    ((DList*)dlist)->tail = NULL;
}

size_t dlist_count(const void* dlist) {
    if (!dlist) return INVALID;
    DList_Node* current = ((DList*)dlist)->head;
    size_t count = 0;
    for (; current; count++) {
        current = current->next;
    }
    return count;
}
void* dlist_item(void* dlist, size_t i) {
    DList_Node* current = ((DList*)dlist)->head;
    for (size_t count = 0; current; count++) {
        if (count == i) return current;
        current = current->next;
    }
    return NULL;
}
void* dlist_append(void* dlist) {
    DList_Node* new_tail = (DList_Node*)malloc(sizeof(DList_Node));
    if (!new_tail) return NULL;
    new_tail->prev = ((DList*)dlist)->tail;
    (((DList*)dlist)->tail)->next = new_tail;
    ((DList*)dlist)->tail = new_tail;
    return new_tail;
}
void* dlist_prepend(void* dlist) {
    DList_Node* new_head = (DList_Node*)malloc(sizeof(DList_Node));
    if (!new_head) return NULL;
    new_head->next = ((DList*)dlist)->head;
    (((DList*)dlist)->head)->prev = new_head;
    ((DList*)dlist)->head = new_head;
    return new_head;
}
void dlist_removeFirst(void* dlist, void(*destroy)(void*)) {
    DList_Node* temp = ((DList*)dlist)->head->next;
    destroy(((DList*)dlist)->head);
    ((DList*)dlist)->head = temp;
}
void dlist_removeLast(void* dlist, void(*destroy)(void*)) {
    DList_Node* temp = ((DList*)dlist)->tail->prev;
    destroy(((DList*)dlist)->tail);
    ((DList*)dlist)->tail = temp;
}

size_t dlist_first(const void* dlist) {
    if (((DList*)dlist)->head) return ((DList*)dlist)->head->item_id;
    else return INVALID;
}
size_t dlist_last(const void* dlist) {
    if (((DList*)dlist)->tail) return ((DList*)dlist)->tail->item_id;
    else return INVALID;
}
size_t dlist_next(const void* dlist, size_t item_id) {
    DList_Node* current = ((DList*)dlist)->head;
    while (current) {
        if (current->item_id == item_id) return ((DList_Node*)current->next)->item_id;
        else current = current->next;
    }
    return INVALID;
}
size_t dlist_prev(const void* dlist, size_t item_id) {
    DList_Node* current = ((DList*)dlist)->head;
    while (current) {
        if (current->item_id == item_id) return ((DList_Node*)current->prev)->item_id;
        else current = current->next;
    }
    return INVALID;
}
size_t dlist_stop(const void* dlist) {
    return INVALID;
}
void* dlist_current(const void* dlist, size_t item_id) {
    DList_Node* current = ((DList*)dlist)->head;
    while (current) {
        if (current->item_id == item_id) return current;
        else current = current->next;
    }
    return NULL;
}
void* dlist_insertAfter(void* dlist, size_t item_id) {
    if (((DList*)dlist)->head) {
        DList_Node* current = ((DList*)dlist)->head;
        while (current) {
            if (current->item_id == item_id) {
                DList_Node* new_node = malloc(sizeof(DList_Node));
                if (!new_node) return NULL;
                new_node->prev = current;
                new_node->next = current->next;
                current->next = new_node;
                return new_node;
            }
            else current = current->next;
        }
    }
    return NULL;
}
void* dlist_insertBefore(void* dlist, size_t item_id) {
    if (((DList*)dlist)->head) {
        DList_Node* current = ((DList*)dlist)->head;
        while (current) {
            if (current->item_id == item_id) {
                DList_Node* new_node = malloc(sizeof(DList_Node));
                if (!new_node) return NULL;
                new_node->next = current;
                new_node->prev = current->prev;
                current->prev = new_node;
                return new_node;
            }
            else current = current->next;
        }
    }
    return NULL;
}
void dlist_erase(void* dlist, size_t item_id, void(*destroy)(void*)) {
    if (((DList*)dlist)->head) {
        DList_Node* current = ((DList*)dlist)->head;
        while (current) {
            if (current->item_id == item_id) {
                ((DList_Node*)current->prev)->next = current->next;
                destroy(current);
            }
            else current = current->next;
        }
    }
}