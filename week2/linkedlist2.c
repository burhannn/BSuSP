#include <stdlib.h>
#include <stdio.h>


typedef struct list_node{
    int value;
    struct list_node* next;
}node_t;

node_t* add_element_sorted(node_t* head, int v){
    node_t* aktuell = head;
    node_t* neu = NULL;
    neu = (node_t*) malloc(sizeof(node_t));
    neu->value = v;
    while(aktuell->next != NULL && aktuell->next->value < v){       // laufe bis richtigen Wert in der Liste
        aktuell = aktuell->next;
    }
    if(aktuell->next == head){      // wenn keine weiteren elemente außer head vorhanden 
        neu->next = head;           // füge neue direkt an den head setze neu = head und gebe zurück
        return neu;
    }
    else
    {
        neu->next = aktuell->next;      // ansonsten lasse neu next auf aktuell next zeigen
        aktuell->next = neu;            // lasse aktuell next auf neu zeigen
    }
    return head;                        //anfangselement der liste zurückgeben

}
node_t* remove_element(node_t* head, int wert) {
    node_t* aktuell = head;       //Anfang setzen
    node_t* eintrag = NULL;
    eintrag = (node_t*) malloc(sizeof(node_t));
    while(aktuell->next != NULL){           // solange nicht am ende der Liste
        eintrag = aktuell->next;            //setze zu löschenden eintrag auf next
        if(eintrag->value == wert){         // wenn zu löschender eintrag gefunden
            aktuell->next= eintrag->next;   // setze aktuellen next gleich dem next des zu löschenden
            free(eintrag);                  // leere den alloziierten Speicher des zu löschenden
            if(eintrag == head){            // falls der zu löschende head ist 
                return eintrag->next;       // gebe dessen next zurück
            }
            return aktuell;                 //ansonsten gebe liste ab aktuellen eintrag zurück
        }
        aktuell = aktuell->next;            // setze bei jedem schleifendurchlauf auf den nächsten eintrag
    }
    free(eintrag);
    free(aktuell);
    return head;                //anfangselement der liste zurückgeben

}

void print_list (node_t* head){
    node_t* aktuell = head;
    while (aktuell != NULL)
    {
        printf("wert: %d\n", aktuell->value);
        aktuell = aktuell->next;
    }
}

int main(){
    node_t* head = (node_t*) malloc(sizeof(node_t));
    head = add_element_sorted(head,1);
    head = add_element_sorted(head,4711);
    head = add_element_sorted(head,17);
    head = add_element_sorted(head,1337);
    print_list(head);
    printf("Löschen von 17......\n");
    head = remove_element(head,17);
    print_list(head);
    return 0;
}
    /*
    //double pointer lösung
node_t* add_element_sorted(node_t* head, int v){
    node_t** curr = &head;

	node_t* new_node = (node_t*) malloc(sizeof(node_t));
	if (new_node == NULL) {
		printf("Allocation error!\n");
		exit(-1);
	}
	new_node->value = v;

	while (*curr != NULL && (*curr)->value < v) {
		curr = &(*curr)->next;
	}

	if (*curr == head) {
		new_node->next = head;
		return new_node;
	} else {
		new_node->next = *curr;
		*curr = new_node;
		return head;
	}
}
*/