#include "student.h"

int main(){
    student_t* studenten = (student_t*) malloc(10* sizeof(student_t));
    char* name = "Student";
    /* Lösung 1
    for (int i = 0; i < 10; i++)
    {
        studenten[i].id = i;
        strcpy(studenten[i].name,name);
        studenten[i].name[7] = i +48; // starte bei ascii + '0'
        studenten[i].name[8] = 32; //'\0'; Nullbyte
        studenten[i].note = 1.0+ (i%10);
        printf(" %s, %f\n", studenten[i].name, studenten[i].note);
    }
    */
   // lösung mit bytegenauer addresierung der pointer mit derefenzierung
    for (int i = 0; i < 10; i++)
    {
        (*(studenten +i)).id = i;
        strcpy((*(studenten +i)).name, name);
        *((char*)(studenten + i) + sizeof(int)+7) = i +48; // starte bei ascii + '0'
        (*(studenten +i)).name[8] = 32; //'\0'; Nullbyte
        (*(studenten +i)).note = 1.0+ (i%3);
        printf(" %s, %f\n", (*(studenten +i)).name, (*(studenten +i)).note);
    }
    return 0;
}