
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct list_node {
    int val;
    struct list_node *next;
} node_t;

typedef struct list_node node_t;

node_t* add_element_sorted(node_t** head, int wert);


node_t* add_element_sorted(node_t** head, int wert) {
    node_t** aktuell = &head;
    node_t* neu = (node_t*) malloc(sizeof(node_t));
    if (neu == NULL) {
        printf("Allocation error!\n");
        exit(-1);
    }
    neu->val = wert;
    while(*aktuell != NULL && (*aktuell)->val < wert){   //solange der aktuelle Zeigerwert kleiner als der nächste
      aktuell = &(*aktuell)->next; // setze Zeiger auf nächstes Element
    }
    if(aktuell == head) { //wenn beim Head angekommen
        neu->next = head; //setze nächsten auf Head
        return neu;
    } else {
        neu->next = *aktuell;  //Ansonsten setze next auf aktuell und return alten head
        *aktuell = neu;
        return head;
    }
}
node_t* remove_element(node_t* head, int wert) {
    node_t** aktuell = &head;       //Anfang setzen
    node_t* eintrag;

 	while (*aktuell != NULL) {        //Laufe Liste bis ans Ende durch
 		eintrag = *aktuell;             //Setze immer aktuellen Eintrag
    if(eintrag->val == wert){       //Wenn eintrag.val == wert setze aktuellen auf nächsten
      *aktuell = eintrag->next;     //setze aktuell auf nächsten
      free(eintrag);                //lösche allozierten Speicher von eintrag
      if(eintrag == head){          //wenn eintrag == head
        return eintrag->next;       //gib Zeiger auf nächsten zurück
      }else{
        return head;                //Ansonsten gib head zurück
      }
    } else {
      aktuell = &eintrag->next;       //
    }
	}

    printf("%d not in list.\n", wert);

    return head;
}
void print_list(node_t* head) {
	node_t* curr  = head;

    printf("List: ");
 	while (curr != NULL) {
        printf("%d ", curr->val);
	    curr = curr->next;
	}
    printf("\n");
}
int main (int argc, char* argv[]) {
    if (argc != 3) {
        printf("Usage: list <Number of elements to add> <number of elements to remove>\n");
        exit(-1);
    }
    long nadd = strtol(argv[1], NULL, 10);
    long nremove = strtol(argv[2], NULL, 10);

    if (nremove > nadd) {
        printf("Number of elements to remove higher than number of elements to add. Setting the former to %ld.\n", nadd);
        nremove = nadd;
    }

    srand(time(NULL));


    node_t* head = NULL;
    for (int i = 0; i < nadd; i++) {
        head = add_element_sorted(head, rand() % 100);
    }
    print_list(head);
    for (int i = 0; i < nremove; i++) {
        head = remove_element(head, rand() % 100);
    }
    print_list(head);

}
