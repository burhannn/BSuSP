/*
1.: Erstelle leeren Heap (1 MB)

      Block at 0x7fc017d00000, size =  1048576 byte, free
2.: A = alloc(1 KB)

      Block at 0x7fc017d00000, size =     1024 byte, allocated
      Block at 0x7fc017d00410, size =  1047536 byte, free
3.: B = alloc(4 KB)

      Block at 0x7fc017d00000, size =     1024 byte, allocated
      Block at 0x7fc017d00410, size =     4096 byte, allocated
      Block at 0x7fc017d01420, size =  1043424 byte, free
4.: C = alloc(2 KB)

      Block at 0x7fc017d00000, size =     1024 byte, allocated
      Block at 0x7fc017d00410, size =     4096 byte, allocated
      Block at 0x7fc017d01420, size =     2048 byte, allocated
      Block at 0x7fc017d01c30, size =  1041360 byte, free
5.: alloc(1 MB) liefert WARNING: Can not allocate memory
0x0

      Block at 0x7fc017d00000, size =     1024 byte, allocated
      Block at 0x7fc017d00410, size =     4096 byte, allocated
      Block at 0x7fc017d01420, size =     2048 byte, allocated
      Block at 0x7fc017d01c30, size =  1041360 byte, free
6.: free(B)

      Block at 0x7fc017d00000, size =     1024 byte, allocated
      Block at 0x7fc017d00410, size =     4096 byte, free
      Block at 0x7fc017d01420, size =     2048 byte, allocated
      Block at 0x7fc017d01c30, size =  1041360 byte, free
7.: free(C)  (sollte mit B verschmelzen)

      Block at 0x7fc017d00000, size =     1024 byte, allocated
      Block at 0x7fc017d00410, size =  1047536 byte, free
8.: free(A)  (sollte zu einem einzigen Speicherblock verschmelzen)

      Block at 0x7fc017d00000, size =  1048576 byte, free
9.: Gebe Heap wieder frei
*/
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
/* Datenstruktur, in der die Verwaltungsdaten des Heaps gespeichert werden,
 * beispielsweise die Groesse des Heaps, Zeiger auf die Liste der Speicherbloecke
 * bzw. auf die Freispeicherliste und auf die der bereits belegten Speicherbloecke.
 */
typedef struct mem_heap {
    bool allocated;
	int size;
	struct mem_heap *next;
} mem_heap_t;

static bool is_initialized = false;
static void* end_addr;
/* Heap anlegen und initialisiereneap
 * -------------------------------
 * Parameter:
 *   size: Groesse des Speichers, der fuer den Heap belegt werden soll und dann
 *         zur Allokation von allozierte Speicherbloecken zur Verfuegung steht
 * Rueckgabewert:
 *   Zeiger auf eine Datenstruktur, die den Heap verwaltet und die allen anderen
 *   Funktionen uebergeben werden muss.
 */
mem_heap_t* mem_init(int size){
    if (is_initialized) {
        printf("WARNUNG: First fit memory manager is already initialized "
            "- ignoring init call\n");
        return NULL;
    }
    // Speicher für Header und size Nutzdaten allozieren
    mem_heap_t* memheap = (mem_heap_t*) malloc(size + sizeof(mem_heap_t));
    memheap->allocated = false;
    memheap->size = size;
    memheap->next = NULL;
    is_initialized = true;
    end_addr = ((char*) memheap) + size + sizeof(mem_heap_t);
    return memheap; 
}

/* Heap freigeben
 * --------------
 * Parameter:
 *   heap: Zeiger auf die Verwaltungsdaten des Heaps, der freigegeben werden soll
 * Anmerkungen:
 *   Diese Funktion gibt allen Speicher wieder frei, der von mem_init belegt wurde.
 */
void mem_cleanup(mem_heap_t* heap){
    free(heap);
    is_initialized = false;
}

/* Speicherblock allozieren
 * --------------------------
 * Parameter:
 *   heap: Zeiger auf die Verwaltungsdaten des Heaps, auf dem der Speicherblock
 *         alloziert werden soll
 *   size: Groesse des nutzbaren Speichers, der alloziert werden soll
 *         (also ohne den Header)
 * Rueckgabewert:
 *   Ein Zeiger auf einen mindestens size Bytes grossen Speicherblock
 *   oder NULL, falls kein Speicherblock alloziert werden konnte.
 */
void* mem_alloc(mem_heap_t *heap, int size){

    if (!is_initialized) {
        printf("ERROR: heap is not initialized\n");
        return NULL;
    }
    mem_heap_t* heaptemp = NULL;
    while(heap){
        if((!heap->allocated) && heap->size >= size){
            heaptemp = heap;
            break;
        }
        heap = heap->next;
    }
    
    if (!heaptemp) {
        printf("WARNING: Can not allocate memory\n");
        return NULL;
    }
    int mindestgroesse = size + sizeof(mem_heap_t);
    if (heaptemp->size > mindestgroesse) {
        // Wegen der Pointerarithmetik ist hier ein Cast (*char) erforderlich
        mem_heap_t* zweiterTeil = (mem_heap_t*) ((char*) heaptemp + mindestgroesse);

        zweiterTeil->size = heaptemp->size - mindestgroesse;
        zweiterTeil->allocated = false;
        zweiterTeil->next = heaptemp->next;

        heaptemp->size = size;
        heaptemp->next = zweiterTeil;
    }
    // Falls diese Unterteilung nicht erfolgt, ist heaptemp->size > size.

    heaptemp->allocated = true;
    // Zurueckgegeben wird ein Zeiger auf die size Bytes Speicher. Diese
    // liegen hinter dem Header, der deshalb uebersprungen werden muss. Wegen
    // der Pointerarithmetik ist hier wieder ein Cast (char*) erforderlich.
    return (void*) ((char*) heaptemp + sizeof(mem_heap_t));
}

/* Speicherblock freigeben
 * -------------------------
 * Parameter:
 *   heap: Zeiger auf die Verwaltungsdaten des Heaps, auf dem der freizugebende
 *         Speicherblock liegt
 *   ptr:  Zeiger auf den Speicherblock, der freigegeben werden soll
 * Anmerkungen:
 *   Falls NULL als ptr uebergeben wird, geschieht nichts.
 */
void mem_free(mem_heap_t *heap, void* ptr){
    if (ptr < (void*) heap || ptr >= end_addr || ptr == NULL) {
        printf("ERROR: Address is out of memory bounds or NULL\n");
        return;
    }

    // Zeiger auf Header berechnen und Block freigeben
    mem_heap_t* header = (mem_heap_t*) ((char*) ptr - sizeof(mem_heap_t));
    header->allocated = false;

    // Freie Speicherbloecke miteinander vereinigen: Dazu die gesamte
    // verkettete Liste durchgeben. Falls freie Bloecke aufeinander folgen,
    // diese miteinander vereinigen.
    for (mem_heap_t* node = heap; node; node = node->next)
        while (!node->allocated && node->next && !node->next->allocated) {
            node->size += node->next->size + sizeof(mem_heap_t);
            node->next = node->next->next;
        }
}

/* Alle Speicherbloecke auflisten
 * -------------------------------
 * Parameter:
 *   heap:  Der Heap, dessen Belegung ausgegeben werden soll
 * Anmerkungen:
 *   Ausgegeben wird eine Liste mit allen freien und belegten Speicherbloecken sowie
 *   deren Groesse und die Menge des bereits belegten und des noch freien Speichers.
 */
void mem_dump(mem_heap_t *heap){
    int cnt = 0;

    // Iteriere uber alle Elemente der Liste
    for (mem_heap_t *node = heap; node; node = node->next) {
        printf("      Block at %p, size = %8d byte, ", node, node->size);

        if (node->allocated)
            printf("allocated\n");
        else
            printf("free\n");

        // printe ersten 100
        if (cnt++ > 100) {
            printf("...\n");
            return;
        }
    }
}

int main() {
    printf("1.: Erstelle leeren Heap (1 MB)\n\n");
    mem_heap_t *heap = mem_init(1024*1024);
    mem_dump(heap);

    printf("2.: A = alloc(1 KB)\n\n");
    void* A = mem_alloc(heap, 1024);
    mem_dump(heap);

    printf("3.: B = alloc(4 KB)\n\n");
    void* B = mem_alloc(heap, 4096);
    mem_dump(heap);

    printf("4.: C = alloc(2 KB)\n\n");
    void* C = mem_alloc(heap, 2048);
    mem_dump(heap);

    printf("5.: alloc(1 MB) liefert ");
    void* D = mem_alloc(heap, 1024*1024);
    printf("%p\n\n", D);
    mem_dump(heap);

    printf("6.: free(B)\n\n");
    mem_free(heap, B);
    mem_dump(heap);

    printf("7.: free(C)  (sollte mit B verschmelzen)\n\n");
    mem_free(heap, C);
    mem_dump(heap);

    printf("8.: free(A)  (sollte zu einem einzigen Speicherblock verschmelzen)\n\n");
    mem_free(heap, A);
    mem_dump(heap);

    printf("9.: Gebe Heap wieder frei\n\n");
    mem_cleanup(heap);
}