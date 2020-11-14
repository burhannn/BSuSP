#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define ANZ 10

struct student
{
    int id;
    char name [9];
    float note;
};

typedef struct student student_t;

int main(){
    char* name = "Student";
    //student_t studenten[ANZ]; //ohne malloc
    student_t* studenten = (student_t*) malloc(ANZ * sizeof(student_t));    //Speicher reservieren
    for (int i = 0; i < ANZ; i++)
    {
        studenten[i].id = i;
        strcpy(studenten[i].name,name);
        studenten[i].note = 4.0 - i %4;
        //studenten[i].name[7] = (char) i + 48; //ohne Pointer
        //studenten[i].name[8] = '\0'; 
        *((char*)studenten + i * sizeof(student_t) + sizeof(int) + 7) = (char) i + 48;
        *((char*)studenten + i * sizeof(student_t) + sizeof(int) + 8) = 32;
    }
    free(studenten);        //Speicher freigeben
    printf("Ihre %d Studenten \n\n", ANZ);
    for (int i = 0; i < ANZ; i++)
    {
        printf("ID: %d, ", studenten[i].id);
        printf("Name: %s, ", studenten[i].name);
        printf("Note: %0.2f \n", studenten[i].note);
    }
    return 0;
}
