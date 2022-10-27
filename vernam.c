#include "vernam.h"



//void attend(int milliseconds){
//    clock_t debut = clock();
//    clock_t fin = clock();
//    while(fin < (debut + 1000*milliseconds)) fin = clock();
//}
//int nombre_aleatoire(int max){ // genere un nombre aleatoire dans l'intervalle 0-max
//    srand(time(NULL));
//    return (rand() % max);

//}
//char lettre_aleatoire(int seed){
//    return (VALEUR_A_ASCII +((nombre_aleatoire(NOMBRE_LETTRE_ALPHABET-seed)+seed)%26));
//}

//char* generer_cle(int nombre_lettre){
//    if (nombre_lettre <=0) return NULL;
//    char* result = (char*)malloc(sizeof(char)*nombre_lettre+1);
//    char temp = 0;
//    for(int i=0;i<nombre_lettre;i++) {
//        temp = lettre_aleatoire(i);
//        result[i] = temp;
//    }result[nombre_lettre] = '\0';
//    return result;
//}
char toupr(char c){
    char space = ' ';
    if (c >= 'A' && c <= 'Z') return c;
    else if (c >= 'a' && c <= 'z') return (c-space);
    else return '\0';
}
char isNumber(char c) {
    if(c >= '0' && c<= '9') return TRUE;
    return FALSE;
}
void strupr(char* texte){
    size_t len = strlen(texte);

    for(size_t i=0;i<len;i++){
        texte[i]=toupr(texte[i]);
    }
}
char vernam_apte(char* texte,char* cle){// verifie que les le message clair et la cle sont correcte pour le chiffrement
    if(strlen(texte) != strlen(cle)) return FALSE;
    return TRUE;
}

char lettre_numero(char lettreAscii) { return lettreAscii-VALEUR_A_ASCII; }

char lettre_ascii(char lettre){ return lettre+VALEUR_A_ASCII; }

char chiffre_numero(char chiffreAscii) { return chiffreAscii-VALEUR_0_ASCII; }

char chiffre_ascii(char chiffre){ return chiffre+VALEUR_0_ASCII; }

char vernam_chiffrer_chiffre(char chiffre,char cle){// chriffre le message grace a la cle
    char num_l=0;
    char result;
    if(!isNumber(chiffre) || !isNumber(cle)) return -1;
    num_l = chiffre_numero(chiffre) + chiffre_numero(cle);
    result = chiffre_ascii((num_l % NOMBRE_CHIFFRE));
    return result;
}

char vernam_dechiffrer_chiffre(char chiffre,char cle){// chriffre le message grace a la cle
    char num_l=0;
    char result;
    if(!isNumber(chiffre) || !isNumber(cle)) return -1;
    num_l = chiffre_numero(chiffre) - chiffre_numero(cle);
    while(num_l < 0){ num_l +=NOMBRE_CHIFFRE; }
    result = chiffre_ascii((num_l % NOMBRE_CHIFFRE));
    return result;
}

char* vernam_chiffrer(char* texte,char* cle) {// chriffre le message grace a la cle
    if(!vernam_apte(texte,cle)) return NULL;;
    size_t len = strlen(cle);
    char num_l=0;
    char* result = (char*)malloc(sizeof(char)*len+1);
    for(size_t i=0;i< len;i++){
        if(texte[i]==' ' || cle[i]==' ') continue;
        if(isNumber(texte[i]) && isNumber(cle[i])) result[i] = vernam_chiffrer_chiffre(texte[i],cle[i]);
        else {
            texte[i] = toupr(texte[i]); cle[i] = toupr(cle[i]);
            num_l = lettre_numero(texte[i]) + lettre_numero(cle[i]);
            result[i] = lettre_ascii((num_l % NOMBRE_LETTRE_ALPHABET));
        }
    }
    result[len] = '\0';
    return result;
}
char* vernam_dechiffrer(char* texte,char* cle) {// dechriffre le message grace a la cle
    if(!vernam_apte(texte,cle)) return NULL;

    size_t len = strlen(cle);
    char num_l=0;
    char* result = (char*)malloc(sizeof(char)*len+1);
    for(size_t i=0;i< len;i++){
        if(texte[i]==' ' || cle[i]==' ') continue;
        if(isNumber(texte[i]) && isNumber(cle[i])) result[i] = vernam_dechiffrer_chiffre(texte[i],cle[i]);
        else{
            texte[i] = toupr(texte[i]); cle[i] = toupr(cle[i]);
            num_l = lettre_numero(texte[i]) - lettre_numero(cle[i]);
                while(num_l <0){
                    num_l +=NOMBRE_LETTRE_ALPHABET;
                }
            result[i] = lettre_ascii((num_l % NOMBRE_LETTRE_ALPHABET));
        }
    }
    result[len] = '\0';
    return result;
}


