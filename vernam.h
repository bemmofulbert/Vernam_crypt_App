#ifndef VERNAM_H
#define VERNAM_H

#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define NOMBRE_LETTRE_ALPHABET 26
#define NOMBRE_CHIFFRE 10
#define VALEUR_A_ASCII 'A'
#define VALEUR_0_ASCII '0'
#define FALSE 0
#define TRUE !FALSE

char* vernam_chiffrer(char* texte,char* cle);
char* vernam_dechiffrer(char* texte,char* cle);
char* vernam_xor(char* texte,char* cle);
int mod10(int number);
int mod26(int number);

int ASCII_to_chiffre(char ascii);
int ASCII_to_numLettre(char ascii);
char chiffre_to_ascii(int chiffre);
char lettre_to_ascii(int lettre);

#endif // VERNAM_H
