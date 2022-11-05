#include "vernam.h"


// genere un nombre aleatoire dans l'intervalle 0-max
int nombre_aleatoire(int max){
    srand(time(NULL));
    return (rand() % max);
}

// retourne le chiffre correspondant du caractere ascii en parametre
int ASCII_to_chiffre(char ascii) { return (ascii-VALEUR_0_ASCII); }

//A correspond a 0, retourne le numero de la lettre dans l'aphabet du caractere en parametre
int ASCII_to_numLettre(char ascii) { return (ascii-VALEUR_A_ASCII); }

// converti en caractere ascii le chiffre en parametre
char chiffre_to_ascii(int chiffre) { return (chiffre + VALEUR_0_ASCII); }

// converti en caractere ascii la lettre en parametre
char lettre_to_ascii(int lettre) { return (lettre + VALEUR_A_ASCII); }

//genere une lettre aleatoire
char lettre_aleatoire(){
    return lettre_to_ascii(nombre_aleatoire(NOMBRE_LETTRE_ALPHABET));
}

//genere un chiffre aleatoire
char chiffre_aleatoire(){
    return chiffre_to_ascii(nombre_aleatoire(NOMBRE_CHIFFRE));
}

//dit si le caractere en parametre est un chiffre ou pas
char isNumber(char c) {
    if(c >= '0' && c<= '9') return TRUE;
    return FALSE;
}

//dit si le caractere en parametre est une lettre ou pas
char isLetter(char c) {
    if(c >= 'A' && c<= 'Z') return TRUE;
    return FALSE;
}

//genere une cle au meme nombre de caractere qu'un message
char* generer_cle(char* toProcess){
    if( toProcess == NULL) return NULL;
    int nombre_lettre = strlen(toProcess);
    if (nombre_lettre <=0) return NULL;
    char* result = (char*)malloc(sizeof(char)*nombre_lettre+1);
    char temp = 0;
    for(int i=0;i<nombre_lettre;i++) {
        if (isLetter(toProcess[i])) temp = lettre_aleatoire();
        else if ((isNumber(toProcess[i]))) temp = chiffre_aleatoire();
        else continue;
        result[i] = temp;
    }
    result[nombre_lettre] = '\0';
    return result;
}

// verifie que les le message clair et la cle sont correcte pour le chiffrement
char vernam_apte(char* texte,char* cle){
    if(texte == NULL || cle == NULL) return FALSE;
    if(strlen(texte) > strlen(cle)) return FALSE;
    return TRUE;
}


char vernam_chiffrer_chiffre(char chiffre,char cle){// chriffre le message grace a la cle
    char num_l=0;
    char result;
    if(!isNumber(chiffre) || !isNumber(cle)) return -1;
    num_l = ASCII_to_chiffre(chiffre) + ASCII_to_chiffre(cle);
    result = chiffre_to_ascii(mod10(num_l));
    return result;
}

char vernam_dechiffrer_chiffre(char chiffre,char cle){// chriffre le message grace a la cle
    char num_l=0;
    char result;
    if(!isNumber(chiffre) || !isNumber(cle)) return -1;
    num_l = ASCII_to_chiffre(chiffre) - ASCII_to_chiffre(cle);
    result = chiffre_to_ascii(mod10(num_l));
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
            texte[i] = toupper(texte[i]); cle[i] = toupper(cle[i]);
            num_l = ASCII_to_numLettre(texte[i]) + ASCII_to_numLettre(cle[i]);
            result[i] = lettre_to_ascii(mod26(num_l));
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
            texte[i] = toupper(texte[i]); cle[i] = toupper(cle[i]);
            num_l = ASCII_to_numLettre(texte[i]) - ASCII_to_numLettre(cle[i]);
            result[i] = lettre_to_ascii(mod26(num_l));
        }
    }
    result[len] = '\0';
    return result;
}
unsigned char gen_char(){
    srand(time(NULL));
    unsigned char c = rand()%256;
    return c;
}

int genCle_bin(char* nomfichier,char* nomDestination) {
    FILE *file=NULL,*cle=NULL;
    file = fopen(nomfichier,"rb");
    cle = fopen(nomDestination,"wb");
    if(file == NULL || cle == NULL) return FALSE;

    unsigned char temp,tempKey;
    int vide = fread(&temp,sizeof(unsigned char),1,file);
    if (vide == 0) return FALSE;
    tempKey = gen_char();
    do{
        tempKey = gen_char();
        fwrite(&tempKey,sizeof(unsigned char),1,cle);
    }while(fread(&temp,sizeof(unsigned char),1,file) != 0);

    fclose(file);fclose(cle);
    return TRUE;
}

int encrypt_decrypt_bin(char* nomfichier,char* nomCle,char* nomDestination) {
    FILE *file=NULL,*cle=NULL,*crypt=NULL;
    file= fopen(nomfichier,"rb");
    cle = fopen(nomCle,"rb");
    crypt = fopen(nomDestination,"wb");
    if(file==NULL || cle==NULL || crypt==NULL) return FALSE;
    char temp,tempKey,tempRes;

    int vide = fread(&temp,sizeof(char),1,file);
    int videKey = fread(&tempKey,sizeof(char),1,cle);
    if(vide == 0 || videKey == 0) return FALSE;
    do{
        tempRes= temp ^ tempKey;
        fwrite(&tempRes,sizeof(char),1,crypt);
    }while(fread(&temp,sizeof(char),1,file) && fread(&tempKey,sizeof(char),1,cle)!=0);

    fclose(file);fclose(cle);fclose(crypt);
    return TRUE;
}
int mod(int number,int modAt){
    while(number < 0){ number +=modAt; }
    return (number % modAt);

}

int mod10(int number){
    return mod(number,10);
}
int mod26(int number){
    return mod(number,26);

}


char* vernam_xor(char* texte,char* cle) {// dechriffre le message grace a la cle
    if(!vernam_apte(texte,cle)) return NULL;

    size_t len = strlen(cle);
    char* result = (char*)malloc(sizeof(char)*len+1);
    for(size_t i=0;i< len;i++){
            result[i] = texte[i] ^ cle[i];
    }
    result[len] = '\0';
    return result;
}
//char toupr(char c){
//    char space = ' ';
//    if (c >= 'A' && c <= 'Z') return c;
//    else if (c >= 'a' && c <= 'z') return (c-space);
//    else return '\0';
//}

//void strupr(char* texte){
//    size_t len = strlen(texte);

//    for(size_t i=0;i<len;i++){
//        texte[i]=toupr(texte[i]);
//    }
//}
