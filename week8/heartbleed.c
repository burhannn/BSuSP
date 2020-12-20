/* Musterlösung zur Aufgabe 'Heartbleed'
 * Florian Dittrich 2019/12/05
 *
 * Ziel dieser Aufgabe ist es, eine Methode zur Speicherreservierung zu implementieren, bei dem die Daten immun gegen
 * das Auslesen per Buffer-Overrun sind. Dies kostet jedoch zusätzlich Speicherplatz.
 *
 * a) Schreiben Sie dazu eine Funktion
 *       void* secure_malloc(size_t nbytes);
 *    Die gewünschte Speichergröße muss dort auf ein Vielfaches der Kachelgröße des Systems aufgerundet werden. Die
 *    Kachelgröße lässt sich mit getpagesize() abfragen. Zusätzlich wird vor und hinter den benötigten Kacheln jeweils
 *    eine weitere Kachel belegt, die gegen das Auslesen und Schreiben geschützt ist. Der gesamte Bereich lässt
 *    sich mit mmap belegen. Danach können die Zugriffsrechte für einzelne Kacheln in diesem Bereich mit mprotect
 *    entsprechend geändert werden.
 *
 * b) Schreiben Sie eine Funktion
 *      void secure_free(void* ptr);
      die den von secure_malloc erhaltenen Zeiger übergeben bekommt und den Speicherbereich mit munmap wieder freigibt.
 *
 * c) Schreiben Sie ein kleines Testprogramm, das eine Anzahl von Bytes einliest (als Parameter oder über die Konsole),
 *    die dann mit secure_malloc reserviert werden sollen. Es sollen die Anzahl der reservierten Bytes und die Speicher-
 *    adresse ausgegeben werden, die secure_malloc liefert.
 *    Außerdem soll es mit getpid() seine PID abfragen und ausgeben. Überprüfen Sie nun in der Shell mit dem Kommando
 *    pmap <PID>, wieviel Speicher reserviert wurde und welche Zugriffsrechte dort bestehen.
 *
 * Hinweise: - munmap erwartet als Parameter die Größe des Speicherbereichs. Sie müssen diesen also mitspeichern.
 *           - Hilfe zu mmap und mprotect finden sie in den man-pages.
 */

#include <stdio.h>    // Für printf und scanf
#include <unistd.h>   // Für getpagesize und getpid
#include <sys/mman.h> // Für mmap, mprotect und munmap

void* secure_malloc(size_t nbytes) {
    int page_size = getpagesize();

    // nbytes + 2*page_size auf ein Vielfaches von page_size aufrunden
    size_t aufgerundet = nbytes + (page_size - nbytes % page_size);

    // Größe der angeforderten Speicherseiten
    size_t length = aufgerundet + page_size*2;

    // Speicherseiten allozieren
    void* addr = mmap(
            NULL,                       // Das Betriebssystem wählt einen Speicherbereich aus
            length,                     // Größe des angeforderten Speicherbereichs
            PROT_READ | PROT_WRITE    , // Auf den gesamten Speicherbereich darf zugegriffen werden
            MAP_PRIVATE                 // Der Speicherbereich ist nur für diesen Prozess bestimmt
             | MAP_ANONYMOUS, 0, 0);    // fd und offset werden nicht verwendet

    if (addr == MAP_FAILED) {
        printf("secure_malloc: Fehler beim Aufruf von mmap\n");
        return NULL;
    }

    // Größe der Allokation in die erste Speicherseite schreiben.
    // Da diese Seite gleich gesperrt wird, ist diese Information dort geschützt
    *((size_t*) addr) = length;

    // Speicherbereich am Anfang sperrem
    int prot_change = mprotect(
        addr,                           // Die erste Speicherseite
        page_size,                      // Nur eine Speicherseite sperren
        PROT_NONE);                     // Keinen Zugriff mehr gestatten

    if (prot_change) {
        printf("secure_malloc: Fehler %d beim ersten Aufruf von mprotect\n", prot_change);
        return NULL;
    }

    prot_change = mprotect(
        addr + aufgerundet + page_size, // Die letzte Speicherseite
        page_size,                      // Nur eine Speicherseite sperren
        PROT_NONE);                     // Keinen Zugriff mehr gestatten

    if (prot_change) {
        printf("secure_malloc: Fehler %d beim zweiten Aufruf von mprotect\n", prot_change);
        return NULL;
    }

    printf("secure_malloc: %ld Bytes Speicher erfolgreich alloziert\n", length);
    return addr + page_size;
}

void secure_free(void* ptr) {
    int page_size = getpagesize();

    // Zeiger auf erste Speicherseite
    void* addr = ptr - page_size;

    // Erste Speicherseite entsperren, um Größe auslesen zu können
    int prot_change = mprotect(
        addr,                           // Die erste Speicherseite
        page_size,                      // Nur eine Speicherseite entsperren
        PROT_READ);                     // Lesezugriff gestatten

    if (prot_change) {
        printf("secure_free: Fehler %d beim Aufruf von mprotect\n", prot_change);
        return;
    }

    // Größe der Allokation auslesen
    size_t length = *((size_t*) addr);

    // Gesamten Speicherbereich freigeben
    int result = munmap(addr, length);
    if (prot_change)
        printf("secure_free: Fehler %d beim Aufruf von munmap\n", result);
}


int main() {
    printf("Die Seitengröße beträgt %d\n\n", getpagesize());
    printf("Dieser Prozess hat die PID %d\n\n", getpid());

    // Speichergröße von Konsole einlesen
    int mem_size = 0;
    printf("Wie viel Speicher soll reserviert werden? ");
    scanf("%d", &mem_size);

    // Speicher anfordern
    void* secure_mem = secure_malloc(mem_size);
    printf("Es wurden %d Bytes ab der Adresse %p reserviert\n\n", mem_size, secure_mem);

    while (1) {
        // Adresse zum Zugriff anfordern
        char* read_addr;
        printf("Auf welche Speicherstelle soll zugegriffen werden? (Beenden mit STRG + C) 0x");
        scanf("%p", &read_addr);

        printf("Greife auf Speicherstelle %p zu\n\n", read_addr);
        char test = *(read_addr);
    }
}