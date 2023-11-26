#include "wizardfichier.h"

unsigned char gen_char_REMAKE();

QString shortFileName_withExt(QString fileName){
    QString result;
    for(int i=fileName.size();i>=0;i--){
        if(fileName[i] == '/') return result;
        result.prepend(fileName[i]);
    }
    return result;
}
QString shortFileName(QString fileName){
    QString result = shortFileName_withExt(fileName);
    if(!result.contains('.')) return fileName;
    for(int i=result.size();i>=0;i--) {
        if(result[i] == '.'){
            result.remove(i,result.size()-1-i);
            return result;
        }
    }
    return "pas dext";
}
QString extFileName(QString fileName) {
    QString result;
    for(int i=fileName.size();i>=0;i--){
        if(fileName[i] == '.') return result;
        result.prepend(fileName[i]);
    }
    return result;
}

QWizardPage* WizardFichier::chiff_createIntro(){
    QWizardPage *intro = new QWizardPage(&wiz_chiff);
    intro->setTitle(tr("Assistant de Chiffrement de fichier"));
    QVBoxLayout *lay = new QVBoxLayout(intro);
    intro->setLayout(lay);
    QLabel *lab_textIntro = new QLabel(chiff_text_intro,intro);
    lay->addWidget(lab_textIntro);

    return intro;
}

QWizardPage* WizardFichier::chiff_createChoiseFile() {
  QWizardPage* chFile = new QWizardPage(&wiz_chiff);
  chFile->setTitle(tr("Choix du fichier"));
  QVBoxLayout *lay = new QVBoxLayout();
  chFile->setLayout(lay);
  QLabel *lab_choiseFile = new QLabel(chiff_choise);
  lay->addWidget(lab_choiseFile);
  QHBoxLayout *lay_path = new QHBoxLayout();
  edit_path = new QLineEdit();

  QPushButton *but_path = new QPushButton(tr("parcourir"));
  connect(but_path,SIGNAL(clicked()),this,SLOT(onChiff_butPath()));

  lay_path->addWidget(edit_path);lay_path->addWidget(but_path);
  lay->addLayout(lay_path);

  return chFile;
}
QWizardPage* WizardFichier::chiff_createDestChoice() {
    QWizardPage* chDir = new QWizardPage(&wiz_chiff);
    chDir->setTitle(tr("Choix de la destination"));
    QVBoxLayout *lay = new QVBoxLayout();
    chDir->setLayout(lay);
    QLabel *lab_choiceDir = new QLabel(chiff_choiceDest);
    lay->addWidget(lab_choiceDir);
    QHBoxLayout *lay_path = new QHBoxLayout();
    edit_pathDest = new QLineEdit();

    QPushButton *but_path = new QPushButton(tr("parcourir"));
    connect(but_path,SIGNAL(clicked()),this,SLOT(onChiff_butPathDest()));

    lay_path->addWidget(edit_pathDest);lay_path->addWidget(but_path);
    lay->addLayout(lay_path);


    return chDir;
}
QWizardPage* WizardFichier::chiff_createFinal() {
    QWizardPage* finPage = new QWizardPage(&wiz_chiff);
    finPage->setTitle(tr("Traitement..."));
    QVBoxLayout *lay = new QVBoxLayout(finPage);
    lay->setSpacing(60);
    finPage->setLayout(lay);
    QLabel *lab_gen = new QLabel(tr("Generation de cle..."));
    lay->addWidget(lab_gen);
    chiff_bar_gen = new QProgressBar();
    chiff_bar_gen->setMinimum(0);
    chiff_bar_gen->setTextVisible(true);
    chiff_bar_gen->setMaximum(100);
    lay->addWidget(chiff_bar_gen);

    QLabel *lab_crypt = new QLabel(tr("Cryptage du fichier..."));
    lay->addWidget(lab_crypt);
    chiff_bar_crypt = new QProgressBar();
    chiff_bar_crypt->setMinimum(0);
    chiff_bar_crypt->setTextVisible(true);
    chiff_bar_crypt->setMaximum(100);
    lay->addWidget(chiff_bar_crypt);

    return finPage;
}

void WizardFichier::initWizard_chif() {

    wiz_chiff.addPage(chiff_createIntro());
    wiz_chiff.addPage(chiff_createChoiseFile());
    wiz_chiff.addPage(chiff_createDestChoice());
    wiz_chiff.addPage(chiff_createFinal());

    wiz_chiff.setWindowTitle(tr("Assistant de Chiffrement de Fichier"));
    QPixmap watermark = QPixmap("images/logo_long_vert.jpg");
    wiz_chiff.setPixmap(QWizard::WatermarkPixmap, watermark);
    wiz_chiff.setWizardStyle(QWizard::ClassicStyle);
    connect(&wiz_chiff,SIGNAL(currentIdChanged(int)),this,SLOT(onChiff_currentIdChange(int)));
}
void WizardFichier::onChiff_butPath(){
    edit_path->setText(QFileDialog::getOpenFileName(&wiz_chiff,tr("selectionner le fichier a crypter")));
}
void WizardFichier::onChiff_butPathDest() {
    edit_pathDest->setText(QFileDialog::getExistingDirectory(&wiz_chiff,tr("selectionner le dossier de destination du fichier crypte")));
}
void WizardFichier::onChiff_currentIdChange(int i) {
    if(i == 3){
        pathDest = edit_pathDest->text();
        if(pathDest.isEmpty()) {
            QMessageBox::information(&wiz_chiff,tr("champ vide"),tr("Veullez remplir le champ pour continuer"));
            wiz_chiff.back();
            return;
        }
        QDir dir(pathDest);
        if(!dir.isReadable() || !dir.exists()){
            QMessageBox::warning(&wiz_chiff,tr("erreur"),tr("Le repertoire indique n'est pas lisible"));
            wiz_chiff.back();
            return;
        }
        wiz_chiff.button(QWizard::FinishButton)->setEnabled(false);

        QString shortName(shortFileName_withExt(path));
        char* nomfichier = (char*)malloc(sizeof(char)*1024);
                strcpy(nomfichier,path.toUtf8().data());
        char* nomCle = (char*)malloc(sizeof(char)*1024);
                strcpy(nomCle,(pathDest+"/Cle_"+shortName).toUtf8().data());
        char* nomDestination = (char*)malloc(sizeof(char)*1024);
                strcpy(nomDestination,(pathDest+QString("/Crypt_")+ shortName).toUtf8().data());

        genCle_bin_REMAKE(nomfichier,nomCle);
        //Hash
        QFile fileHash(pathDest+"/Hash_"+shortName+".txt");
        if (fileHash.open(QIODevice::WriteOnly | QIODevice::Text)){
            QTextStream in(&fileHash);

            QString hashCle = calcul_shasum((pathDest+"/Cle_"+shortName));
            QString hashCrypt = calcul_shasum(pathDest+"/Crypt_"+ shortName);

            in << "--------------fichier contenant des Hash SHA-1-------------" << '\n' << '\n'
               << pathDest << "/Crypt_" << shortName.toUtf8().data() << " : " << '\n' << '\t' << hashCrypt << '\n' << '\n'
               << pathDest << "/Cle_" << shortName.toUtf8().data() << " : " << '\n' << '\t' << hashCle;
            in.flush();
            fileHash.close();
        }

        if(encrypt_bin_REMAKE(nomfichier,nomCle,nomDestination)){
            QMessageBox::information(&wiz_chiff,tr("cryptage termine"),tr("chiffrement effectue avec succes<br>")
                                                                          +tr("Le fichier chiffre et la cle de chiffrement sont enregistres aux chemins suivants :<br><br>")
                                                                          +"<b>"+nomDestination+"<br>"+nomCle+"<br>"+fileHash.fileName()+"</b>");
        }else QMessageBox::critical(&wiz_chiff,tr("erreur"),"un probleme est survenu");

        free(nomfichier);
        free(nomCle);
        free(nomDestination);

        wiz_chiff.button(QWizard::FinishButton)->setEnabled(true);
    }
    else if(i == 2) {
        path = edit_path->text();
        if(path.isEmpty()) {
            QMessageBox::information(&wiz_chiff,tr("champ vide"),tr("Veullez remplir le champ pour continuer"));
            wiz_chiff.back();
            return;
        }
        QFile fileTest(path);
        if (!fileTest.open(QIODevice::ReadOnly)) {
            wiz_chiff.back();
            QMessageBox::warning(&wiz_chiff,tr("erreur"),tr("le fichier ne peut pas etre ouvert"));
            return;
        }else fileTest.close();
    }
}

unsigned char gen_char_REMAKE(){
    unsigned char c = QRandomGenerator::global()->generate()%256;
    return c;
}

int WizardFichier::genCle_bin_REMAKE(const char *nomfichier, const char* nomDestination) {


    QFile fTemp(nomfichier);
    qint64 taille = fTemp.size() , tailleCurrent =0;
    fTemp.close();

    FILE *file=NULL,*cle=NULL;
    file = fopen(nomfichier,"rb");
    cle = fopen(nomDestination,"wb");

    if(file == NULL || cle == NULL) return false;

    unsigned char temp,tempKey;
    int vide = fread(&temp,sizeof(unsigned char),1,file);
    if (vide == 0) return -1;
    tempKey = gen_char_REMAKE();
    tailleCurrent +=1;
    do{
        fwrite(&tempKey,sizeof(unsigned char),1,cle);
        tempKey = gen_char_REMAKE();
        tailleCurrent +=1;
        chiff_bar_gen->setValue(tailleCurrent*100/taille);
    }while(fread(&temp,sizeof(unsigned char),1,file) != 0);

    fclose(file);fclose(cle);
    return true;
}

int WizardFichier::encrypt_bin_REMAKE(const char* nomfichier, const char* nomCle, const char* nomDestination) {

    QFile fTemp(nomfichier);
    qint64 taille = fTemp.size() , tailleCurrent =0;
    fTemp.close();

    FILE *file=NULL,*cle=NULL,*crypt=NULL;
    file= fopen(nomfichier,"rb");
    cle = fopen(nomCle,"rb");
    crypt = fopen(nomDestination,"wb");
    if(file==NULL || cle==NULL || crypt==NULL) return true;
    char temp,tempKey,tempRes;

    int vide = fread(&temp,sizeof(char),1,file);
    int videKey = fread(&tempKey,sizeof(char),1,cle);
    if(vide == 0 || videKey == 0) {
        qDebug("here");
        return -1;
    }
    do{
        tailleCurrent+=1;
        chiff_bar_crypt->setValue(tailleCurrent*100/taille);
        tempRes= temp ^ tempKey;
        fwrite(&tempRes,sizeof(char),1,crypt);

    }while(fread(&temp,sizeof(char),1,file)!=0 && fread(&tempKey,sizeof(char),1,cle)!=0);

    fclose(file);fclose(cle);fclose(crypt);
    return true;
}
// DEChiff--------------------------------------------------------

void WizardFichier::initWizard_deChif() {
    wiz_dechiff.addPage(dechiff_createIntro());
    wiz_dechiff.addPage(dechiff_createChoiseKey());
    wiz_dechiff.addPage(dechiff_createChoiseFile());
    wiz_dechiff.addPage(dechiff_createDestChoice());
    wiz_dechiff.addPage(dechiff_createFinal());

    wiz_dechiff.setWindowTitle(tr("Assistant de DeChiffrement de Fichier"));
    QPixmap watermark = QPixmap("images/logo_long_vert.jpg");
    wiz_dechiff.setPixmap(QWizard::WatermarkPixmap, watermark);
    wiz_dechiff.setWizardStyle(QWizard::ClassicStyle);
    connect(&wiz_dechiff,SIGNAL(currentIdChanged(int)),this,SLOT(onDeChiff_currentIdChange(int)));
}

QWizardPage* WizardFichier::dechiff_createIntro(){
    QWizardPage *intro = new QWizardPage();
    intro->setTitle(tr("Assistant de DeChiffrement de fichier"));
    QVBoxLayout *lay = new QVBoxLayout();
    intro->setLayout(lay);
    QLabel *lab_textIntro = new QLabel(dechiff_text_intro);
    lay->addWidget(lab_textIntro);

    return intro;
}

QWizardPage* WizardFichier::dechiff_createChoiseKey() {
  QWizardPage* chFile = new QWizardPage();
  chFile->setTitle(tr("Choix du fichier Cle"));
  QVBoxLayout *lay = new QVBoxLayout();
  chFile->setLayout(lay);
  QLabel *lab_choiseFile = new QLabel(dechiff_choiseKey);
  lay->addWidget(lab_choiseFile);
  QHBoxLayout *lay_path = new QHBoxLayout();
  dechif_edit_pathKey = new QLineEdit();

  QPushButton *but_path = new QPushButton(tr("parcourir"));
  connect(but_path,SIGNAL(clicked()),this,SLOT(onDechiff_butPathKey()));

  lay_path->addWidget(dechif_edit_pathKey);lay_path->addWidget(but_path);
  lay->addLayout(lay_path);

  return chFile;
}
void WizardFichier::onDechiff_butPathKey(){
    dechif_edit_pathKey->setText(QFileDialog::getOpenFileName(&wiz_dechiff,tr("selectionner le fichier Cle")));
}

QWizardPage* WizardFichier::dechiff_createChoiseFile() {
  QWizardPage* chFile = new QWizardPage();
  chFile->setTitle(tr("Choix du fichier Crypte"));
  QVBoxLayout *lay = new QVBoxLayout();
  chFile->setLayout(lay);
  QLabel *lab_choiseFile = new QLabel(dechiff_choiseFile);
  lay->addWidget(lab_choiseFile);
  QHBoxLayout *lay_path = new QHBoxLayout();
  dechif_edit_pathFile = new QLineEdit();

  QPushButton *but_path = new QPushButton(tr("parcourir"));
  connect(but_path,SIGNAL(clicked()),this,SLOT(onDechiff_butPathFile()));

  lay_path->addWidget(dechif_edit_pathFile);lay_path->addWidget(but_path);
  lay->addLayout(lay_path);

  return chFile;
}
void WizardFichier::onDechiff_butPathFile(){
    dechif_edit_pathFile->setText(QFileDialog::getOpenFileName(&wiz_dechiff,tr("selectionner le fichier Crypte")));
}

QWizardPage* WizardFichier::dechiff_createDestChoice() {
    QWizardPage* chDir = new QWizardPage();
    chDir->setTitle(tr("Choix de la destination"));
    QVBoxLayout *lay = new QVBoxLayout();
    chDir->setLayout(lay);
    QLabel *lab_choiceDir = new QLabel(dechiff_choiceDest);
    lay->addWidget(lab_choiceDir);
    QHBoxLayout *lay_path = new QHBoxLayout();
    dechif_edit_pathDest = new QLineEdit();

    QPushButton *but_path = new QPushButton(tr("parcourir"));
    connect(but_path,SIGNAL(clicked()),this,SLOT(onDechiff_butPathDest()));

    lay_path->addWidget(dechif_edit_pathDest);lay_path->addWidget(but_path);
    lay->addLayout(lay_path);


    return chDir;
}
void WizardFichier::onDechiff_butPathDest(){
    dechif_edit_pathDest->setText(QFileDialog::getExistingDirectory(&wiz_dechiff,tr("selectionner le dossier de destination du fichier Clair")));
}

QWizardPage* WizardFichier::dechiff_createFinal() {
    QWizardPage* finPage = new QWizardPage();
    finPage->setTitle(tr("Dechiffrement..."));
    QVBoxLayout *lay = new QVBoxLayout();
    lay->setSpacing(60);
    finPage->setLayout(lay);
    dechiff_bar = new QProgressBar();
    dechiff_bar->setMinimum(0);
    dechiff_bar->setTextVisible(true);
    dechiff_bar->setMaximum(100);
    lay->addWidget(dechiff_bar);

    return finPage;
}

void WizardFichier::onDeChiff_currentIdChange(int i) {
    QFile fileTest;
    QDir dir;
    switch (i) {
    case 2:
        dechif_pathKey = dechif_edit_pathKey->text();
        if(dechif_pathKey.isEmpty()) {
            QMessageBox::information(&wiz_dechiff,tr("champ vide"),tr("Veullez remplir le champ pour continuer"));
            wiz_dechiff.back();
            return;
        }
        fileTest.setFileName(dechif_pathKey);
        if (!fileTest.open(QIODevice::ReadOnly)) {
            wiz_dechiff.back();
            QMessageBox::warning(&wiz_dechiff,tr("erreur"),tr("le fichier ne peut pas etre ouvert"));
            return;
        }else fileTest.close();
        break;
    case 3:
        dechif_pathFile = dechif_edit_pathFile->text();
        if(dechif_pathFile.isEmpty()) {
            QMessageBox::information(&wiz_dechiff,tr("champ vide"),tr("Veullez remplir le champ pour continuer"));
            wiz_dechiff.back();
            return;
        }
        fileTest.setFileName(dechif_pathFile);
        if (!fileTest.open(QIODevice::ReadOnly)) {
            wiz_dechiff.back();
            QMessageBox::warning(&wiz_dechiff,tr("erreur"),tr("le fichier ne peut pas etre ouvert"));
            return;
        }else fileTest.close();
        break;
    case 4:
        dechif_pathDest = dechif_edit_pathDest->text();
        if(dechif_pathDest.isEmpty()) {
            QMessageBox::information(&wiz_dechiff,tr("champ vide"),tr("Veullez remplir le champ pour continuer"));
            wiz_dechiff.back();
            return;
        }
        dir.setPath(dechif_pathDest);
        if(!dir.isReadable() || !dir.exists()){
            QMessageBox::warning(&wiz_dechiff,tr("erreur"),tr("Le repertoire indique n'est pas lisible"));
            wiz_dechiff.back();
            return;
        }
        wiz_dechiff.button(QWizard::FinishButton)->setEnabled(false);
        if(true){
            QString shortName(shortFileName_withExt(dechif_pathFile));
            char* nomfichier = (char*)malloc(sizeof(char)*1024);
                    strcpy(nomfichier,dechif_pathFile.toUtf8().data());
            char* nomCle = (char*)malloc(sizeof(char)*1024);
                    strcpy(nomCle,(dechif_pathKey).toUtf8().data());
            char* nomDestination = (char*)malloc(sizeof(char)*1024);
                    nomDestination =strcpy(nomDestination,(dechif_pathDest+QString("/Clair_")+ shortName).toUtf8().data());

            if(decrypt_bin_REMAKE(nomfichier,nomCle,nomDestination)){
                QMessageBox::information(&wiz_dechiff,tr("Dechiffrement termine"),tr("Dechiffrement effectue avec succes<br>")
                                                                              +tr("Le fichier en clair est enregistre au chemin suivant :<br><br>")
                                                                              +"<b>"+nomDestination+"</b>");
            }else QMessageBox::critical(&wiz_dechiff,tr("erreur"),"un probleme est survenu");

            free(nomfichier);
            free(nomCle);
            free(nomDestination);

            wiz_dechiff.button(QWizard::FinishButton)->setEnabled(true);
        }
        break;
    default:
        break;
    }
}

int WizardFichier::decrypt_bin_REMAKE(const char* nomfichier, const char* nomCle, const char* nomDestination) {

    QFile fTemp(nomfichier);
    qint64 taille = fTemp.size() , tailleCurrent =0;
    fTemp.close();

    FILE *file=NULL,*cle=NULL,*crypt=NULL;
    file= fopen(nomfichier,"rb");
    cle = fopen(nomCle,"rb");
    crypt = fopen(nomDestination,"wb");
    if(file==NULL || cle==NULL || crypt==NULL) return true;
    char temp,tempKey,tempRes;

    int vide = fread(&temp,sizeof(char),1,file);
    int videKey = fread(&tempKey,sizeof(char),1,cle);
    if(vide == 0 || videKey == 0) {
        qDebug("here");
        return -1;
    }
    do{
        tailleCurrent+=1;
        dechiff_bar->setValue(tailleCurrent*100/taille);
        tempRes= temp ^ tempKey;
        fwrite(&tempRes,sizeof(char),1,crypt);

    }while(fread(&temp,sizeof(char),1,file)!=0 && fread(&tempKey,sizeof(char),1,cle)!=0);

    fclose(file);fclose(cle);fclose(crypt);
    return true;
}

QString WizardFichier::calcul_shasum(const QString filename, char separateur){
    QFile file(filename);
    if(file.open(QIODevice::ReadOnly)){
        QByteArray hash = QCryptographicHash::hash(file.readAll(),QCryptographicHash::Sha1);
        file.close();
        return hash.toHex(separateur);
    }else return "";
}

