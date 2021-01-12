#include <fcntl.h>   // For open
#include <stdlib.h>  // For free, malloc
#include <stdio.h>   // For printf, fscanf
#include <unistd.h>  // For close, read

#define BUFF_SIZE  1024

int main() {
    char* buffer = malloc(BUFF_SIZE * sizeof(*buffer));
    if (buffer == NULL) {
        printf("ERROR: Allokation mit malloc fehlgeschlagen.\n");
        exit(EXIT_FAILURE);
    }

    char unit1[3], unit2[3], unit3[3];
    int mem_total, mem_free, cached;

    FILE* fd = fopen("/proc/meminfo", "r");

	fscanf(fd, "%*s %d %2s"
			   "%*s %d %2s"
			   "%*s %*d %*s"
			   "%*s %d %s",
			&mem_total, unit1,
			&mem_free, unit2,
			&cached, unit3);

	fclose(fd);
	
	printf("Groesse des gesamten Speichers: %d %s\n", mem_total, unit1);
	printf("Groesse des freien Speichers:   %d %s\n", mem_free, unit2);
	printf("Groesse des Page-Cache:         %d %s\n", cached, unit3);
	return 0;
}
