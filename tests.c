#include <string.h>
#include <assert.h>
#include <math.h>
#include <crtdbg.h>
#include <stdio.h>
#include <stdlib.h>

#include "dlist.h"

typedef struct {
    int* array;
    float d_variable;
} Value;

void destroy_value(Value* value) {
    free(value->array);
}

void* test_create() {
    void* new_list = dlist_create(sizeof(Value));
    assert(dlist_count(new_list) == 0);
    return new_list;
}

void test_fill_list(void** dlist, size_t* counter) {
    int* array = (int*)malloc(sizeof(int) * 10);
    for (size_t i = 0; i < 10; i++) {
        array[i] = i;
    }
    Value value = { array, 0.f };
    Value* prepended_value = dlist_prepend(*dlist);
    if (prepended_value) {
        *prepended_value = value;
        assert(dlist_count(*dlist) == ++(*counter));
        Value* item = (Value*)dlist_item(*dlist, *counter - 1);
        for (size_t i = 0; i < 10; i++) {
            assert(item->array[i] == value.array[i]);
        }
        assert(fabsf(item->d_variable - value.d_variable) < 1e-10f);
        assert(dlist_item(*dlist, *counter) == NULL);
    }
    else
        free(array);
}

void test_init(void** dlist) {
    *dlist = dlist_init(*dlist, sizeof(int), destroy_value);
    if(*dlist)
        assert(dlist_count(*dlist) == 0);
    else
        assert(dlist_count(*dlist) == INVALID);
}

void test_item_id_func(void** dlist, int* counter) {
    size_t item_id = dlist_insertAfter(*dlist, dlist_stop(*dlist));
    ++(*counter);
    assert(item_id != 0);
    /*size_t item_id_2 = dlist_insertBefore(*dlist, dlist_stop(*dlist));
    assert(item_id_2 != 0);*/
    size_t current_id = dlist_first(*dlist);
    while (dlist_next(*dlist, current_id) != dlist_stop(*dlist)) {
        current_id = dlist_next(*dlist, current_id);
    }
    assert(current_id == dlist_last(*dlist));

    current_id = dlist_last(*dlist);
    while (dlist_prev(*dlist, current_id) != dlist_stop(*dlist)) {
        current_id = dlist_prev(*dlist, current_id);
    }
    assert(current_id == dlist_first(*dlist));
}

void test_insert_remove(void** dlist, int* counter) {
    void* old_head = dlist_current(*dlist, dlist_first(*dlist));
    void* new_head = dlist_insertBefore(*dlist, dlist_first(*dlist));
    if (new_head) {
        assert(dlist_count(*dlist) == ++(*counter));
        assert(new_head == dlist_current(*dlist, dlist_first(*dlist)));
        dlist_removeFirst(*dlist, NULL);
        assert(dlist_count(*dlist) == --(*counter));
        assert(dlist_current(*dlist, dlist_first(*dlist)) == old_head);
    }

    void* old_tail = dlist_current(*dlist, dlist_last(*dlist));
    void* new_tail = dlist_insertAfter(*dlist, dlist_last(*dlist));
    if (new_tail) {
        assert(dlist_count(*dlist) == ++(*counter));
        assert(new_tail == dlist_current(*dlist, dlist_last(*dlist)));
        dlist_removeLast(*dlist, NULL);
        assert(dlist_count(*dlist) == --(*counter));
        assert(dlist_current(*dlist, dlist_last(*dlist)) == old_tail);
    }
}

void test_append_erase(void** dlist, int* counter) {
    void* old_tail = dlist_current(*dlist, dlist_last(*dlist));
    void* new_tail = dlist_append(*dlist);
    if(*dlist) 
        assert(dlist_count(*dlist) == ++(*counter));
    else 
        assert(dlist_count(*dlist) == INVALID);
    assert(new_tail == dlist_current(*dlist, dlist_last(*dlist)));
    dlist_erase(*dlist, dlist_last(*dlist), NULL);
    assert(dlist_count(*dlist) == --(*counter));
    assert(dlist_current(*dlist, dlist_last(*dlist)) == old_tail);
}

void test_clear_delete(void** dlist) {
    dlist_clear(*dlist, NULL);
    if (*dlist)
        assert(dlist_count(*dlist) == 0);
    else
        assert(dlist_count(*dlist) == INVALID);
    dlist_destroy(*dlist, NULL);
    assert(_CrtDumpMemoryLeaks() == 0);
}

int main(int argc, char* argv[])
{
    void* dlist = NULL;
    dlist = test_create();
    size_t counter = 0;
    test_fill_list(&dlist, &counter);
    test_fill_list(&dlist, &counter);
    test_init(&dlist);
    counter = 0;
    test_item_id_func(&dlist, &counter);
    test_insert_remove(&dlist, &counter);
    test_append_erase(&dlist, &counter);
    test_clear_delete(&dlist);
    return 0;
}