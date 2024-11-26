CC=gcc
CFLAGS=-I.

parallel_min_max : /workspaces/os_lab_2019/lab3/src/utils.o /workspaces/os_lab_2019/lab3/src/find_min_max.o /workspaces/os_lab_2019/lab3/src/utils.h /workspaces/os_lab_2019/lab3/src/find_min_max.h 
	$(CC) -o parallel_min_max /workspaces/os_lab_2019/lab3/src/utils.o /workspaces/os_lab_2019/lab3/src/find_min_max.o /workspaces/os_lab_2019/lab3/src/parallel_min_max.c $(CFLAGS)

process_memory : 
	$(CC) -o process_memory process_memory.c $(CFLAGS)

parallel_sum.o: parallel_sum.c
	$(CC) $^ -o $@

parallel_sum: parallel_sum.o
	$(CC) $^ -o $@ -lpthread

.PHONY: clear

clear:
	rm parallel_sum.o parallel_sum parallel_min_max porc process_memory process_memory.o ta tasc.o sum