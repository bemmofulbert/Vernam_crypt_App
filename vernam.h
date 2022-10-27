#ifndef VERNAM_H
#define VERNAM_H

#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define NOMBRE_LETTRE_ALPHABET 26
#define NOMBRE_CHIFFRE 10
#define VALEUR_A_ASCII 'A'
#define VALEUR_0_ASCII '0'
#define FALSE 0
#define TRUE !FALSE

char* vernam_chiffrer(char* texte,char* cle);
char* vernam_dechiffrer(char* texte,char* cle);
char toupr(char c);

#endif // VERNAM_H
