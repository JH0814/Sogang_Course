#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "list.h"
#include "hash.h"
#include "bitmap.h"
struct list list_array[15];
struct hash hash_list[15];
struct bitmap bitmap_list[15];

unsigned int hash_hash(const struct hash_elem* e, void *aux){ // function for hash
    return hash_int(e->value);
}
bool hash_less(const struct hash_elem* a, const struct hash_elem* b, void *aux){ // fuction for comparing hash value
    return a->value < b->value;
}
void hash_del(struct hash_elem* h, void *aux){ // function for deleting hash element
    free(h);
}
void hash_squ(struct hash_elem *e, void *aux){ // funcion for squaring hash value when hash_apply called
    e->value = e->value * e->value;
}
void hash_tri(struct hash_elem *e, void *aux){// funcion for cubing hash value when hash_apply called
    e->value = e->value * e->value * e->value;
}
void clear_hash(struct hash_elem *e, void *aux){ // function for deleting all hash element
    e->list_elem.prev->next = e->list_elem.next;
    e->list_elem.next->prev = e->list_elem.prev;
    free(e);
}

bool list_val_cmp(const struct list_elem *a, const struct list_elem *b, void *aux){ // funcion for comparing list element's value
    struct list_item *a_item = list_entry(a, struct list_item, elem);
    struct list_item *b_item = list_entry(b, struct list_item, elem);
    return a_item->data < b_item->data;
}

int main(){
    char command[100]; // variable for input
    while(1){
        scanf("%s", command);
        if(strcmp(command, "quit") == 0){ // if "quit" inputed, exit program
            break;
        }
        if(strcmp(command, "create") == 0){ // create part
            scanf("%s", command);
            if(strcmp(command, "list") == 0){
                scanf("%s", command);
                if(strncmp(command, "list", 4) == 0){ // creating list
                    int num = command[strlen(command) - 1] - '0';
                    list_init(&list_array[num]);
                }
                else{
                    printf("Wrong Input!\n");
                }
            }
            else if(strcmp(command, "hashtable") == 0){ // creating hash table
                scanf("%s", command);
                if(strncmp(command, "hash", 4) == 0){
                    int num = command[strlen(command) - 1] - '0';
                    hash_init(&hash_list[num], hash_hash, hash_less, NULL);
                }
                else{
                    printf("Wrong Input!\n");
                }
            }
            else if(strcmp(command, "bitmap") == 0){ // creating bitmap
                scanf("%s", command);
                if(strncmp(command, "bm", 2) == 0){
                    int num = command[strlen(command) - 1] - '0';
                    size_t size;
                    scanf("%zu", &size);
                    bitmap_list[num] = *bitmap_create(size);
                }
                else{
                    printf("Wrong Input!\n");
                }
            }
            else{
                printf("Wrong Input!\n");
            }
        }
        if(strcmp(command, "delete") == 0){ //delete part
            scanf("%s", command);
            if(strncmp(command, "list", 4) == 0){ // deleting list
                int num = command[strlen(command) - 1] - '0';
                while(!list_empty(&list_array[num])){ // free all elements
                    struct list_elem *back = list_pop_back(&list_array[num]);
                    free(back);
                }
                list_init(&list_array[num]);
            }
            else if(strncmp(command, "hash", 4) == 0){ // deleting hash table
                int num = command[strlen(command) - 1] - '0';
                hash_destroy(&hash_list[num], hash_del);
            }
            else if(strncmp(command, "bm", 2) == 0){ // deleting bitmap
                int num = command[strlen(command) - 1] - '0';
                bitmap_list[num].bit_cnt = 0;
                free(bitmap_list[num].bits);
            }
            else{
                printf("Wrong Input!\n");
            }
        }
        if(strcmp(command, "dumpdata") == 0){ // printing data to stdout
            scanf("%s", command);
            if(strncmp(command, "list", 4) == 0){ // print list
                int num = command[strlen(command) - 1] - '0';
                if(!list_empty(&list_array[num])){ // if it is not empty
                    struct list_elem *ptr = list_array[num].head.next;
                    for(;ptr != &(list_array[num].tail); ptr = ptr->next){ // print all value of list
                        struct list_item *cur = list_entry(ptr, struct list_item, elem);
                        printf("%d ", cur->data);
                    }
                    printf("\n");
                }
            }
            else if(strncmp(command, "hash", 4) == 0){ // print hash table
                int num = command[strlen(command) - 1] - '0';
                if(!hash_empty(&hash_list[num])){ // if it is not empty
                    for(size_t i = 0; i<hash_list[num].bucket_cnt; i++){
                        struct list_elem *ptr;
                        struct list* bucket = &hash_list[num].buckets[i];
                        for(ptr = bucket->head.next; ptr != &(bucket->tail); ptr = ptr->next){ // print all value of hash
                            struct hash_elem *cur = list_elem_to_hash_elem(ptr);
                            printf("%d ", cur->value);
                        }
                    }
                    printf("\n");
                }
            }
            else if(strncmp(command, "bm", 2) == 0){ // print bitmap
                int num = command[strlen(command) - 1] - '0';
                size_t bit_cnt = bitmap_list[num].bit_cnt;
                if(bit_cnt){
                    for(size_t i = 0; i<bit_cnt; i++){
                        printf("%d", bitmap_test(&bitmap_list[num], i));
                    }
                    printf("\n");
                }
            }
            else{
                printf("Wrong Input!\n");
            }
        }
        
        // list commands part
        if(strcmp(command, "list_front") == 0){ // print first element in list
            scanf("%s", command);
            if(strncmp(command, "list", 4) == 0){
                int num = command[strlen(command) - 1] - '0';
                struct list_elem* res = list_front(&list_array[num]);
                struct list_item* res_item = list_entry(res, struct list_item, elem);
                printf("%d\n", res_item->data);
            }
            else{
                printf("Wrong Input!\n");
            }
        }
        else if(strcmp(command, "list_back") == 0){ //print last element in list
            scanf("%s", command);
            if(strncmp(command, "list", 4) == 0){
                int num = command[strlen(command) - 1] - '0';
                struct list_elem* res = list_back(&list_array[num]);
                struct list_item* res_item = list_entry(res, struct list_item, elem);
                printf("%d\n", res_item->data);
            }
            else{
                printf("Wrong Input!\n");
            }
        }
        else if(strcmp(command, "list_insert") == 0){ // insert element in list
            scanf("%s", command);
            if(strncmp(command, "list", 4) == 0){
                int num = command[strlen(command) - 1] - '0';
                int idx, val;
                scanf("%d %d", &idx, &val);
                struct list_elem *target = list_array[num].head.next; // search for proper index element
                for(int i = 0; i<idx && target != list_end(&list_array[num]); i++){
                    target = target->next;
                }
                struct list_item *target_item = (struct list_item*)malloc(sizeof(struct list_item));
                target_item->data = val;
                list_insert(target, &target_item->elem);
            }
            else{
                printf("Wrong Input!\n");
            }
        }
        else if(strcmp(command, "list_push_back") == 0){ // insert element to last position of list
            scanf("%s", command);
            if(strncmp(command, "list", 4) == 0){
                int num = command[strlen(command) - 1] - '0';
                int val;
                scanf("%d", &val);
                struct list_item *target_item = (struct list_item*)malloc(sizeof(struct list_item));
                target_item->data = val;
                list_push_back(&list_array[num], &target_item->elem);
            }
            else{
                printf("Wrong Input!\n");
            }
        }
        else if(strcmp(command, "list_push_front") == 0){ // insert element to first position of list
            scanf("%s", command);
            if(strncmp(command, "list", 4) == 0){
                int num = command[strlen(command) - 1] - '0';
                int val;
                scanf("%d", &val);
                struct list_item *target_item = (struct list_item*)malloc(sizeof(struct list_item));
                target_item->data = val;
                list_push_front(&list_array[num], &target_item->elem);
            }
            else{
                printf("Wrong Input!\n");
            }
        }
        else if(strcmp(command, "list_pop_front") == 0){ // remove first element from list
            scanf("%s", command);
            if(strncmp(command, "list", 4) == 0){
                int num = command[strlen(command) - 1] - '0';
                struct list_elem *target = list_pop_front(&list_array[num]);
                if(target) free(target);
            }
            else{
                printf("Wrong Input!\n");
            }
        }
        else if(strcmp(command, "list_pop_back") == 0){ // remove last element from list
            scanf("%s", command);
            if(strncmp(command, "list", 4) == 0){
                int num = command[strlen(command) - 1] - '0';
                struct list_elem *target = list_pop_back(&list_array[num]);
                if(target) free(target);
            }
            else{
                printf("Wrong Input!\n");
            }
        }
        else if(strcmp(command, "list_insert_ordered") == 0){ // insert item to ordered position
            scanf("%s", command);
            if(strncmp(command, "list", 4) == 0){
                int num = command[strlen(command) - 1] - '0';
                int val;
                scanf("%d", &val);
                struct list_item *target_item = (struct list_item*)malloc(sizeof(struct list_item));
                target_item->data = val;
                list_insert_ordered(&list_array[num], &target_item->elem, list_val_cmp, NULL);
            }
            else{
                printf("Wrong Input!\n");
            }
        }
        else if(strcmp(command, "list_empty") == 0){ // print true false of empty list
            scanf("%s", command);
            if(strncmp(command, "list", 4) == 0){
                int num = command[strlen(command) - 1] - '0';
                if(list_empty(&list_array[num])){
                    printf("true\n");
                }
                else{
                    printf("false\n");
                }
            }
            else{
                printf("Wrong Input!\n");
            }
        }
        else if(strcmp(command, "list_size") == 0){ // print number of elements of list
            scanf("%s", command);
            if(strncmp(command, "list", 4) == 0){
                int num = command[strlen(command) - 1] - '0';
                printf("%zu\n", list_size(&list_array[num]));
            }
            else{
                printf("Wrong Input!\n");
            }
        }
        else if(strcmp(command, "list_max") == 0){ // print maximum of list
            scanf("%s", command);
            if(strncmp(command, "list", 4) == 0){
                int num = command[strlen(command) - 1] - '0';
                struct list_elem *max = list_max(&list_array[num], list_val_cmp, NULL);
                struct list_item *max_item = list_entry(max, struct list_item, elem);
                printf("%d\n", max_item->data);
            }
            else{
                printf("Wrong Input!\n");
            }
        }
        else if(strcmp(command, "list_min") == 0){ // print minimum of list
            scanf("%s", command);
            if(strncmp(command, "list", 4) == 0){
                int num = command[strlen(command) - 1] - '0';
                struct list_elem *min = list_min(&list_array[num], list_val_cmp, NULL);
                struct list_item *min_item = list_entry(min, struct list_item, elem);
                printf("%d\n", min_item->data);
            }
            else{
                printf("Wrong Input!\n");
            }
        }
        else if(strcmp(command, "list_remove") == 0){ // remove element by index
            scanf("%s", command);
            if(strncmp(command, "list", 4) == 0){
                int num = command[strlen(command) - 1] - '0';
                int idx;
                scanf("%d", &idx);
                int check = 0; // check for non-existing idx in list
                struct list_elem *target = list_array[num].head.next; // find target
                for(int i = 0; i<idx && target != list_end(&list_array[num]); i++){
                    target = target->next;
                    check++;
                }
                if(check == idx){
                    list_remove(target);
                }
            }
            else{
                printf("Wrong Input!\n");
            }
        }
        else if(strcmp(command, "list_reverse") == 0){ // reverse list
            scanf("%s", command);
            if(strncmp(command, "list", 4) == 0){
                int num = command[strlen(command) - 1] - '0';
                list_reverse(&list_array[num]);
            }
            else{
                printf("Wrong Input!\n");
            }
        }
        else if(strcmp(command, "list_shuffle") == 0){ // shuffle list
            scanf("%s", command);
            if(strncmp(command, "list", 4) == 0){
                int num = command[strlen(command) - 1] - '0';
                list_shuffle(&list_array[num]);
            }
            else{
                printf("Wrong Input!\n");
            }
        }
        else if(strcmp(command, "list_sort") == 0){ // sort list
            scanf("%s", command);
            if(strncmp(command, "list", 4) == 0){
                int num = command[strlen(command) - 1] - '0';
                list_sort(&list_array[num], list_val_cmp, NULL);
            }
            else{
                printf("Wrong Input!\n");
            }
        }
        else if(strcmp(command, "list_splice") == 0){ // splice list
            scanf("%s", command);
            if(strncmp(command, "list", 4) == 0){
                int num1 = command[strlen(command) - 1] - '0';
                int idx;
                scanf("%d", &idx);
                struct list_elem *before = list_array[num1].head.next; // find target elemnet
                for(int i = 0; i<idx && before != list_end(&list_array[num1]); i++){
                    before = before->next;
                }
                scanf("%s", command);
                if(strncmp(command, "list", 4) == 0){ // move from fir to lst of list to target
                    int num2 = command[strlen(command) - 1] - '0';
                    int fir, lst;
                    scanf("%d %d", &fir, &lst);
                    // find each element
                    struct list_elem *fir_elem = list_array[num2].head.next;
                    for(int i = 0; i<fir && fir_elem != list_end(&list_array[num2]); i++){
                        fir_elem = fir_elem->next;
                    }
                    struct list_elem *lst_elem = list_array[num2].head.next;
                    for(int i = 0; i<lst && lst_elem != list_end(&list_array[num2]); i++){
                        lst_elem = lst_elem->next;
                    }
                   list_splice(before, fir_elem, lst_elem);
                }
                else{
                    printf("Wrong Input!\n");
                }
            }
            else{
                printf("Wrong Input!\n");
            }
        }
        else if(strcmp(command, "list_swap") == 0){ // swap elements by index
            scanf("%s", command);
            if(strncmp(command, "list", 4) == 0){
                int num = command[strlen(command) - 1] - '0';
                int idx1, idx2;
                scanf("%d %d", &idx1, &idx2);
                struct list_elem *a = list_array[num].head.next;
                struct list_elem *b = list_array[num].head.next;
                for(int i = 0; i<idx1 && a != list_end(&list_array[num]); i++){
                    a = a->next;
                }
                for(int i = 0; i<idx2 && b != list_end(&list_array[num]); i++){
                    b = b->next;
                }
                list_swap(a, b);
            }
            else{
                printf("Wrong Input!\n");
            }
        }
        else if(strcmp(command, "list_unique") == 0){ // remove other same value element and if other list are given, then remove element appended to it.
            char c = getchar();
            fgets(command, sizeof(command), stdin);
            command[strlen(command) - 1] = '\0';
            if(strncmp(command, "list", 4) == 0){
                int num1 = command[4] - '0';
                if(strlen(command) > 6){
                    if(command[6] == 'l' && command[7] == 'i' && command[8] == 's' && command[9] == 't'){
                        int num2 = command[10] - '0';
                        list_unique(&list_array[num1], &list_array[num2], list_val_cmp, NULL);
                    }
                    else{
                        printf("Wrong Input!\n");
                    }
                }
                else{
                    list_unique(&list_array[num1], NULL, list_val_cmp, NULL);
                }
            }
            else{
                printf("Wrong Input!\n");
            }
        }

        // hash commands part
        if(strcmp(command, "hash_insert") == 0){ // insert hash element
            scanf("%s", command);
            if(strncmp(command, "hash", 4) == 0){
                int num = command[strlen(command) - 1] - '0';
                int val;
                scanf("%d", &val);
                struct hash_elem *target;
                target = (struct hash_elem *)malloc(sizeof(struct hash_elem));
                target->value = val;
                hash_insert(&hash_list[num], target);
            }
            else{
                printf("Wrong Input!\n");
            }
        }
        else if(strcmp(command, "hash_apply") == 0){ // calculate each hash to square or cube
            scanf("%s", command);
            if(strncmp(command, "hash", 4) == 0){
                int num = command[strlen(command) - 1] - '0';
                scanf("%s", command);
                if(strcmp(command, "square") == 0){
                    hash_apply(&hash_list[num], hash_squ);
                }
                else if(strcmp(command, "triple") == 0){
                    hash_apply(&hash_list[num], hash_tri);
                }
                else{
                    printf("Wrong Input!\n");
                }
            }
            else{
                printf("Wrong Input!\n");
            }
        }
        else if(strcmp(command, "hash_delete") == 0){ // find element by value and delete it
            scanf("%s", command);
            if(strncmp(command, "hash", 4) == 0){
                int num = command[strlen(command) - 1] - '0';
                int val;
                scanf("%d", &val);
                int flag = 0; // for checking element found
                struct list_elem *tar;
                struct hash_elem *cur;
                for(size_t i = 0; i<hash_list[num].bucket_cnt; i++){
                    struct list* bucket = &hash_list[num].buckets[i];
                    for(tar = bucket->head.next; tar != &(bucket->tail); tar = tar->next){
                        cur = list_elem_to_hash_elem(tar);
                        if(cur->value == val){
                            flag = 1;
                            break;
                        }
                    }
                    if(flag == 1){
                        break;
                    }
                }
                if(flag){
                    hash_delete(&hash_list[num], cur);
                }
            }
            else{
                printf("Wrong Input!\n");
            }
        }
        else if(strcmp(command, "hash_empty") == 0){ // print true false of hash empty
            scanf("%s", command);
            if(strncmp(command, "hash", 4) == 0){
                int num = command[strlen(command) - 1] - '0';
                if(hash_empty(&hash_list[num])){
                    printf("true\n");
                }
                else{
                    printf("false\n");
                }
            }
            else{
                printf("Wrong Input!\n");
            }
        }
        else if(strcmp(command, "hash_size") == 0){ // print number of hash elements
            scanf("%s", command);
            if(strncmp(command, "hash", 4) == 0){
                int num = command[strlen(command) - 1] - '0';
                printf("%zu\n", hash_size(&hash_list[num]));
            }
            else{
                printf("Wrong Input!\n");
            }
        }
        else if(strcmp(command, "hash_clear") == 0){ // remove all elements of hash
            scanf("%s", command);
            if(strncmp(command, "hash", 4) == 0){
                int num = command[strlen(command) - 1] - '0';
                hash_clear(&hash_list[num], clear_hash);
            }
            else{
                printf("Wrong Input!\n");
            }
        }
        else if(strcmp(command, "hash_find") == 0){ // find element by value and if exist, print it
            scanf("%s", command);
            if(strncmp(command, "hash", 4) == 0){
                int num = command[strlen(command) - 1] - '0';
                int val;
                scanf("%d", &val);
                struct hash_elem *tar = (struct hash_elem *)malloc(sizeof(struct hash_elem));
                tar->value = val;
                if(hash_find(&hash_list[num], tar)){
                    printf("%d\n", val);
                }
                free(tar);
            }
            else{
                printf("Wrong Input!\n");
            }
        }
        else if(strcmp(command, "hash_replace") == 0){ // replace old element to new one
            scanf("%s", command);
            if(strncmp(command, "hash", 4) == 0){
                int num = command[strlen(command) - 1] - '0';
                int val;
                scanf("%d", &val);
                struct hash_elem *tar = (struct hash_elem *)malloc(sizeof(struct hash_elem));
                tar->value = val;
                struct hash_elem *old = hash_replace(&hash_list[num], tar);
                free(old);
            }
            else{
                printf("Wrong Input!\n");
            }
        }

        // bitmap commands part
        if(strcmp(command, "bitmap_mark") == 0){ // change idxth element to true
            scanf("%s", command);
            if(strncmp(command, "bm", 2) == 0){
                int num = command[strlen(command) - 1] - '0';
                size_t idx;
                scanf("%zu", &idx);
                bitmap_mark(&bitmap_list[num], idx);
            }
            else{
                printf("Wrong Input!\n");
            }
        }
        else if(strcmp(command, "bitmap_all") == 0){ // check bitmap from start to cnt number bit and if all true, print true
            scanf("%s", command);
            if(strncmp(command, "bm", 2) == 0){
                int num = command[strlen(command) - 1] - '0';
                size_t start;
                scanf("%zu", &start);
                size_t cnt;
                scanf("%zu", &cnt);
                if(bitmap_all(&bitmap_list[num], start, cnt)){
                    printf("true\n");
                }
                else{
                    printf("false\n");
                }
            }
            else{
                printf("Wrong Input!\n");
            }
        }
        else if(strcmp(command, "bitmap_any") == 0){ // check bitmap from start to cnt number bit and if any one of them is true, print true
            scanf("%s", command);
            if(strncmp(command, "bm", 2) == 0){
                int num = command[strlen(command) - 1] - '0';
                size_t start;
                scanf("%zu", &start);
                size_t cnt;
                scanf("%zu", &cnt);
                if(bitmap_any(&bitmap_list[num], start, cnt)){
                    printf("true\n");
                }
                else{
                    printf("false\n");
                }
            }
            else{
                printf("Wrong Input!\n");
            }
        }
        else if(strcmp(command, "bitmap_contains") == 0){ // check bitmap contain true or false and print it
            scanf("%s", command);
            if(strncmp(command, "bm", 2) == 0){
                int num = command[strlen(command) - 1] - '0';
                size_t start;
                scanf("%zu", &start);
                size_t cnt;
                scanf("%zu", &cnt);
                scanf("%s", command);
                bool val;
                if(strcmp(command, "true") == 0){
                    val = true;
                }
                else if(strcmp(command, "false") == 0){
                    val = false;
                }
                else{
                    printf("Wrong Input!\n");
                    continue;
                }
                if(bitmap_contains(&bitmap_list[num], start, cnt, val)){
                    printf("true\n");
                }
                else{
                    printf("false\n");
                }
            }
            else{
                printf("Wrong Input!\n");
            }
        }
        else if(strcmp(command, "bitmap_count") == 0){ // print number of bits that are true or false from start to cnt bits 
            scanf("%s", command);
            if(strncmp(command, "bm", 2) == 0){
                int num = command[strlen(command) - 1] - '0';
                size_t start;
                scanf("%zu", &start);
                size_t cnt;
                scanf("%zu", &cnt);
                scanf("%s", command);
                bool val;
                if(strcmp(command, "true") == 0){
                    val = true;
                }
                else if(strcmp(command, "false") == 0){
                    val = false;
                }
                printf("%zu\n", bitmap_count(&bitmap_list[num], start, cnt, val));
            }
            else{
                printf("Wrong Input!\n");
            }
        }
        else if(strcmp(command, "bitmap_dump") == 0){ // change hex and print it
            scanf("%s", command);
            if(strncmp(command, "bm", 2) == 0){
                int num = command[strlen(command) - 1] - '0';
                bitmap_dump(&bitmap_list[num]);
            }
            else{
                printf("Wrong Input!\n");
            }
        }
        else if(strcmp(command, "bitmap_expand") == 0){ // expand number of bits of bitmap and maintain its value
            scanf("%s", command);
            if(strncmp(command, "bm", 2) == 0){
                int num = command[strlen(command) - 1] - '0';
                int size;
                scanf("%d", &size);
                bitmap_list[num] = *bitmap_expand(&bitmap_list[num], size);
            }
            else{
                printf("Wrong Input!\n");
            }
        }
        else if(strcmp(command, "bitmap_set_all") == 0){ // set all the value of bit same
            scanf("%s", command);
            if(strncmp(command, "bm", 2) == 0){
                int num = command[strlen(command) - 1] - '0';
                scanf("%s", command);
                bool val;
                if(strcmp(command, "true") == 0){
                    val = true;
                }
                else if(strcmp(command, "false") == 0){
                    val = false;
                }
                bitmap_set_all(&bitmap_list[num], val);
            }
            else{
                printf("Wrong Input!\n");
            }
        }
        else if(strcmp(command, "bitmap_flip") == 0){ // change value of bits to opposite
            scanf("%s", command);
            if(strncmp(command, "bm", 2) == 0){
                int num = command[strlen(command) - 1] - '0';
                int idx;
                scanf("%d", &idx);
                bitmap_flip(&bitmap_list[num], idx);
            }
            else{
                printf("Wrong Input!\n");
            }
        }
        else if(strcmp(command, "bitmap_none") == 0){ // print true if there is no true from start to start+cnt
            scanf("%s", command);
            if(strncmp(command, "bm", 2) == 0){
                int num = command[strlen(command) - 1] - '0';
                int start;
                scanf("%d", &start);
                int cnt;
                scanf("%d", &cnt);
                if(bitmap_none(&bitmap_list[num], start, cnt)){
                    printf("true\n");
                }
                else{
                    printf("false\n");
                }
            }
            else{
                printf("Wrong Input!\n");
            }
        }
        else if(strcmp(command, "bitmap_reset") == 0){ // set idx index bit to false
            scanf("%s", command);
            if(strncmp(command, "bm", 2) == 0){
                int num = command[strlen(command) - 1] - '0';
                int idx;
                scanf("%d", &idx);
                bitmap_reset(&bitmap_list[num], idx);
            }
            else{
                printf("Wrong Input!\n");
            }
        }
        else if(strcmp(command, "bitmap_scan") == 0){ // find group of bit that has same with val
            scanf("%s", command);
            if(strncmp(command, "bm", 2) == 0){
                int num = command[strlen(command) - 1] - '0';
                int start;
                scanf("%d", &start);
                int cnt;
                scanf("%d", &cnt);
                scanf("%s", command);
                bool val;
                if(strcmp(command, "true") == 0){
                    val = true;
                }
                else if(strcmp(command, "false") == 0){
                    val = false;
                }
                printf("%zu\n", bitmap_scan(&bitmap_list[num], start, cnt, val));
            }
            else{
                printf("Wrong Input!\n");
            }
        }
        else if(strcmp(command, "bitmap_scan_and_flip") == 0){ // find group of bit that has same with val and flip them
            scanf("%s", command);
            if(strncmp(command, "bm", 2) == 0){
                int num = command[strlen(command) - 1] - '0';
                int start;
                scanf("%d", &start);
                int cnt;
                scanf("%d", &cnt);
                scanf("%s", command);
                bool val;
                if(strcmp(command, "true") == 0){
                    val = true;
                }
                else if(strcmp(command, "false") == 0){
                    val = false;
                }
                printf("%zu\n", bitmap_scan_and_flip(&bitmap_list[num], start, cnt, val));
            }
            else{
                printf("Wrong Input!\n");
            }
        }
        else if(strcmp(command, "bitmap_set") == 0){ // set idx bit to val
            scanf("%s", command);
            if(strncmp(command, "bm", 2) == 0){
                int num = command[strlen(command) - 1] - '0';
                int idx;
                scanf("%d", &idx);
                scanf("%s", command);
                bool val;
                if(strcmp(command, "true") == 0){
                    val = true;
                }
                else if(strcmp(command, "false") == 0){
                    val = false;
                }
                bitmap_set(&bitmap_list[num], idx, val);
            }
            else{
                printf("Wrong Input!\n");
            }
        }
        else if(strcmp(command, "bitmap_set_multiple") == 0){ // set cnt bits from start to val 
            scanf("%s", command);
            if(strncmp(command, "bm", 2) == 0){
                int num = command[strlen(command) - 1] - '0';
                int start;
                scanf("%d", &start);
                int cnt;
                scanf("%d", &cnt);
                scanf("%s", command);
                bool val;
                if(strcmp(command, "true") == 0){
                    val = true;
                }
                else if(strcmp(command, "false") == 0){
                    val = false;
                }
                bitmap_set_multiple(&bitmap_list[num], start, cnt, val);
            }
            else{
                printf("Wrong Input!\n");
            }
        }
        else if(strcmp(command, "bitmap_size") == 0){ // print number of bits
            scanf("%s", command);
            if(strncmp(command, "bm", 2) == 0){
                int num = command[strlen(command) - 1] - '0';
                printf("%zu\n", bitmap_size(&bitmap_list[num]));
            }
            else{
                printf("Wrong Input!\n");
            }
        }
        else if(strcmp(command, "bitmap_test") == 0){ // print value of idx bit
            scanf("%s", command);
            if(strncmp(command, "bm", 2) == 0){
                int num = command[strlen(command) - 1] - '0';
                int idx;
                scanf("%d", &idx);
                if(bitmap_test(&bitmap_list[num], idx)){
                    printf("true\n");
                }
                else{
                    printf("false\n");
                }
            }
            else{
                printf("Wrong Input!\n");
            }
        }
    }
}