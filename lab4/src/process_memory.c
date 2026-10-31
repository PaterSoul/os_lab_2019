#include <stdio.h>
#include <stdlib.h>

extern char etext, edata, end;

int global_init = 42;
int global_uninit;

int main() {
    int local_var = 100;
    int *heap_var = malloc(sizeof(int));

    printf("Memory layout:\n\n");
    printf("etext (end of text/code):   %16p\n", (void *)&etext);
    printf("edata (end of data):        %16p\n", (void *)&edata);
    printf("end   (end of bss):         %16p\n", (void *)&end);
    printf("\n");
    printf("main (text segment):        %16p\n", (void *)main);
    printf("global_init (data):         %16p\n", (void *)&global_init);
    printf("global_uninit (bss):        %16p\n", (void *)&global_uninit);
    printf("heap_var (heap):            %16p\n", (void *)heap_var);
    printf("local_var (stack):          %16p\n", (void *)&local_var);

    free(heap_var);
    return 0;
}