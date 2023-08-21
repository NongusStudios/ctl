#include <ctlhashmap.h>
#include <stdio.h>
#include <string.h>

typedef struct person_t {
    int age;
    int height;
} person;

int main(void){
    ctl_hashmap map = ctl_hashmap_new(sizeof(person));

    char* keys[] = {
        "Joel",
        "Ryan",
        "Oliver",
        "Van",
        "Sam",
    };

    person* p = ctl_hashmap_insert_str(&map, keys[0]);
    p->age = 18;
    p->height = 185;

    p = ctl_hashmap_insert_str(&map, keys[1]);
    p->age = 19;
    p->height = 165;

    p = ctl_hashmap_insert_str(&map, keys[2]);
    p->age = 19;
    p->height = 178;

    p = ctl_hashmap_insert_str(&map, keys[3]);
    p->age = 20;
    p->height = 170;

    p = ctl_hashmap_insert_str(&map, keys[4]);
    p->age = 19;
    p->height = 132;

    ctl_hashmap_insert_str(&map, "Jongus");
    ctl_hashmap_insert_str(&map, "Shebiles");
    ctl_hashmap_insert_str(&map, "Badlands Chugs");
    ctl_hashmap_insert_str(&map, "SmashingKrunker");
    ctl_hashmap_insert_str(&map, "ryan");

    ctl_vec_foreach(ctl_bucket, bucket, map.buckets){
        ctl_bucket* current = bucket;
        while(current != NULL){
            printf("key: %s, value: %p -> ", (char*)current->value.key, current->value.value);
            current = current->next;
        }
        printf("END\n");
    }
    printf("\n");

    for(size_t i = 0; i < 5; i++){
        p = ctl_hashmap_get_str(&map, keys[i]);
        printf("[%s] {\n.age = %i,\n.height = %i,\n}\n", keys[i], p->age, p->height);
    }
    printf("\n");
    
    ctl_hashmap_erase_str(&map, "Badlands Chugs");
    ctl_hashmap_erase_str(&map, "Ryan");

    ctl_vec_foreach(ctl_bucket, bucket, map.buckets){
        ctl_bucket* current = bucket;
        while(current != NULL){
            printf("key: %s, value: %p -> ", (char*)current->value.key, current->value.value);
            current = current->next;
        }
        printf("END\n");
    }
    printf("\n");
    
    ctl_hashmap_free(&map);
}
