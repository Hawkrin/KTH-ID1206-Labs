#include <stdlib.h>
#include <stdio.h>

#define FRAMES 64

#define SIZE 16

#define Offset(addr) (addr & 0b1111)
#define PageNr(addr) (addr >> 4)


typedef enum available {FREE, TAKEN} available;

int memory[FRAMES*SIZE]; // the memory 

available framemap[FRAMES] = {FREE};

typedef enum pte_status {ALLOCATED, LAZY, SHARED} pte_status;

typedef struct  {
  int frame;
  pte_status status;  
  struct pt_entry *copy;
} pt_entry;
  

typedef struct array {
  int pages;
  pt_entry *pagetable;   
} array;

void frames() {
  printf("frames : ");
  for(int i =0;i < FRAMES; i++) {
    printf(" %d ", framemap[i]);
  }
  printf("\n");
}

void check(array *arr) {

  printf("array %p ", arr);
  printf(" pages  :  %d\n", arr->pages);

  for(int i = 0; i < arr->pages; i++) {
    pt_entry *entry = &arr->pagetable[i];
    printf(" entry : %p \n", entry);
    printf("   frame : %d\n", entry->frame);
    printf("   status :  ");
    switch(entry->status) {
    case ALLOCATED:
      printf("ALLOCATED\n");
      break;
    case LAZY:
      printf("LAZY\n");
      break;
    case SHARED:
      printf("SHARED\n");
      break;
    }
    printf("   copy: %p\n", entry->copy);
  }
}


int find_free() {
  for(int i = 0; i < FRAMES; i++) {
    if( framemap[i] == FREE ) {
      framemap[i] = TAKEN;
      return i;
    }
  }
  return -1;
}

void delete(array *arr) {
  int pages = arr->pages;
  printf("delete array, freeing frames: ");
  for( int i = 0; i < pages; i++) {
    pt_entry *entry =  &arr->pagetable[i];

    switch(entry->status) {
    case ALLOCATED:
      printf(" %d", entry->frame);
      framemap[entry->frame] = FREE;
      break;
    case LAZY:
      break;
    case SHARED: 
      // unlink and update
      pt_entry *prev = entry;
      while(prev->copy != entry)
	prev = prev->copy;

      prev->copy = entry->copy;

      if(prev->copy == prev) {
	prev->status = ALLOCATED;
      }
    }
  }
  printf("\n");
  free(arr->pagetable);
  free(arr);
  return;
}

array *allocate(int size) {
  int pages = size / SIZE;
  int rem = size % SIZE;
  if( rem > 0)
    pages += 1;

  array *new = (array*)malloc(sizeof(array));
  pt_entry *pagetable = (pt_entry*)malloc(sizeof(pt_entry)*pages);  

  new->pages = pages;
  new->pagetable = pagetable;

  for(int i = 0; i < pages; i++) {
    pt_entry *entry = &new->pagetable[i];
    entry->copy = entry;   // a trick
    entry->status = LAZY;
  }
  /*
  printf("allocate array, frames: ");
  for(int i = 0; i < pages; i++ ) {
    int f = find_free();
    if( f == -1 ) {
      delete(new);
      return NULL;
    }
    printf("%d ", f);
    new->pagetable[i] = f;
  }
  printf("\n");
  */
  return new;
}

array *create(int size) {
  array *new = allocate(size);
  if(new == NULL)  {
    printf("out of memory\n");
    exit(-1);
  }
  return new;
}

array *copy(array *orig) {

  
  array *copy = (array*)malloc(sizeof(array));
  pt_entry *pagetable = (pt_entry*)malloc(sizeof(pt_entry) * orig->pages);  

  int pages = orig->pages;
  
  copy->pages = pages;
  copy->pagetable = pagetable;  

  for(int i = 0; i < pages; i++) {
    pt_entry *orig_entry = &orig->pagetable[i];
    pt_entry *copy_entry = &copy->pagetable[i];
    printf("copy entry %p to %p \n", orig_entry, copy_entry); 

    switch(orig_entry->status) {
    case LAZY:
      copy_entry->status = LAZY;
      copy_entry->copy = copy_entry;  // circular
      break;
    case ALLOCATED:
    case SHARED:
      copy_entry->frame = orig_entry->frame;
      orig_entry->status = SHARED;
      copy_entry->status = SHARED;
      // linking in the circular structure
      copy_entry->copy = orig_entry->copy;
      orig_entry->copy = copy_entry;
      break;
    }
  }

  return copy;
}


void set(array *arr, int pos, int val) {
  printf("set %p  pos %d  val %d\n", arr, pos, val);
  int offset = Offset(pos); 
  int page = PageNr(pos);

  if( page >= arr->pages || page < 0){
    printf("segmentation fault\n");
    exit(1);
  }
  pt_entry *entry = &arr->pagetable[page];

  if( entry->status == LAZY ) {
    printf("page fault ... ");
    int f = find_free();
    if( f == -1 ) {
      delete(arr);
      exit(-1);
    }
    printf("ok\n");
    entry->status = ALLOCATED;
    entry->copy = entry;
    entry->frame = f;
  }

  if( entry->status == SHARED ) {
    printf("page copy ... ");
    int f = find_free();
    printf(" found %d ", f);
    if( f == -1 ) {
      delete(arr);
      exit(-1);
    }    
    // copy content
    for(int i = 0; i < SIZE; i++) {
      memory[f*SIZE + i] = memory[entry->copy->frame*SIZE + i];
    }
    entry->frame = f; 
    entry->status = ALLOCATED;
    
    // unlink and update
    pt_entry *prev = entry;
    while(prev->copy != entry) {
      prev = prev->copy;
    }
    prev->copy = entry->copy;

    if(prev->copy == prev) {
      prev->status = ALLOCATED;
    }
    printf("ok\n");
  }
  int frame = entry->frame;

  printf("page %d,  offset %d, frame = %d\n", page, offset, frame);

  memory[frame*SIZE + offset] = val;
  return;
}

int get(array *arr, int pos) {
  int offset = Offset(pos);
  int page = PageNr(pos);

  if( page >= arr->pages || page < 0 ){
    printf("segmentation fault\n");
    exit(1);
  }

  pt_entry *entry = &arr->pagetable[page];

  if ( entry->status == LAZY ) {
    return 0;
  }

  int frame = entry->frame;

  printf("get %p pos %d  page %d offset %d frame %d\n", arr, pos, page, offset, frame);

  return memory[frame*SIZE + offset];
}

void  bench1() {

    array *a = create(20);
    check(a);

    array *b = create(40);
    check(b);
    
    set(a, 10, 110);
    check(a);
    set(a, 18, 118);
    check(a);

    set(b, 8, 208);
    check(b);
    set(b, 36, 212);
    check(b);
    // set(b, 40, 212);    

    // set(b, 52, 212);    

    printf(" a[10] + a[18] = %d\n", get(a,10) + get(a, 18));
    printf(" b[8] + b[36] = %d\n", get(b,8) + get(b, 36));    
    
    delete(a);
    delete(b);

}

void  bench2() {

    array *a = create(20);

    set(a, 10, 100);

    printf("a[1] = %d\n", get(a, 1));
    
    array *b =copy(a);

    // set(b, 18, 5);
    
    array *c =copy(b);

    printf("c[18] = %d\n", get(c, 18));
    
    set(a, 15, 115);
    set(b, 15, 215);
    set(c, 15, 315);

    
    printf(" a[15] = %d\n", get(a, 15));
    printf(" b[15] = %d\n", get(b, 15));
    printf(" c[15] = %d\n", get(c, 15));        
    
    delete(a);
    delete(b);
    delete(c);    
}


int main() {

  bench1();
  bench2();

  return 0;
  
}
