#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QObject::connect(ui->but_chif,SIGNAL(clicked()),this,SLOT(crypter()));
    QObject::connect(ui->but_genKey,SIGNAL(clicked()),this,SLOT(genCle()));
    QObject::connect(ui->but_dechiff,SIGNAL(clicked()),this,SLOT(decrypter()));
    ui->lab_avert->hide();
    ui->lab_avert_2->hide();
    //this->setWindowTitle("Chiffre de Vernam (masque jetable)");

    hisDial = new HistoryDialog(this);
    hisDial->setVisible(false);
    connect(ui->but_his,SIGNAL(clicked()),this,SLOT(montrer_historique()));
// boutons clear
    connect(ui->but_trashClear,SIGNAL(clicked()),ui->ptex_clear,SLOT(clear()));
    connect(ui->but_trashKey,SIGNAL(clicked()),ui->ptex_key,SLOT(clear()));
    connect(ui->but_trashChiff,SIGNAL(clicked()),ui->ptex_cryp,SLOT(clear()));

// limiter la saisie
    connect(ui->ptex_clear,SIGNAL(textChanged()),this,SLOT(limit_clear()));
    connect(ui->ptex_key,SIGNAL(textChanged()),this,SLOT(limit_key()));
    connect(ui->ptex_cryp,SIGNAL(textChanged()),this,SLOT(limit_crypt()));

}
void MainWindow::limit_clear() {
    disconnect(ui->ptex_clear,SIGNAL(textChanged()),this,SLOT(limit_clear()));
    limit(ui->ptex_clear);
    connect(ui->ptex_clear,SIGNAL(textChanged()),this,SLOT(limit_clear()));
}
void MainWindow::limit_key() {
    disconnect(ui->ptex_key,SIGNAL(textChanged()),this,SLOT(limit_key()));
    limit(ui->ptex_key);
    connect(ui->ptex_key,SIGNAL(textChanged()),this,SLOT(limit_key()));
}
void MainWindow::limit_crypt() {
    disconnect(ui->ptex_cryp,SIGNAL(textChanged()),this,SLOT(limit_crypt()));
    limit(ui->ptex_cryp);
    connect(ui->ptex_cryp,SIGNAL(textChanged()),this,SLOT(limit_crypt()));
}
void MainWindow::limit(QPlainTextEdit *ptex){
    if(ptex->toPlainText().isEmpty()) return;
    QString text = ptex->toPlainText();
    text = text.simplified();
    for(int i=0;i<text.size();i++){
        if(toupr(text[i].toLatin1()) == '\0') {
            text.remove(text[i]);
        }
    }
    //text.remove(' ');
    text = text.toUpper();
    ptex->clear();
    ptex->appendPlainText(text);

}
void MainWindow::montrer_historique(){
    hisDial->fill();
    hisDial->setVisible(true);
}
void MainWindow::crypter(){
    // verification de la longueur des chaine _ vernam
    ui->lab_avert_2->hide();
    if(ui->ptex_clear->toPlainText().size() != ui->ptex_key->toPlainText().size()) {
        ui->lab_avert->show();
        return;
    }
    else ui->lab_avert->hide();
    ui->ptex_cryp->clear();
    QString aChiff = ui->ptex_clear->toPlainText();
    QString key = ui->ptex_key->toPlainText();
    if(key.isEmpty() || aChiff.isEmpty()) return;

    for(int i=0; i< aChiff.size();i++){
        char* c = vernam_chiffrer(
                    ((QString()+aChiff.at(i))).toUtf8().data(),
                    ((QString()+key.at(i))).toUtf8().data());
        ui->ptex_cryp->setPlainText(ui->ptex_cryp->toPlainText() + QString::fromUtf8(c,sizeof(char)));
        free(c);
    }
    // pour l'historique
    if(!key_exist(key)){
        history_add(hisDial->File_his(),key);
        hisDial->List_key()->prepend(key);
    }
}
bool MainWindow::key_exist(QString &key){
    history_read(hisDial->File_his(),hisDial->List_key());
    for(int i=0;i<hisDial->List_key()->size();i++) {
        if (key == hisDial->List_key()->at(i)){
            ui->lab_avert_2->show();
            return true;
        }
    }
    return false;
}
void MainWindow::decrypter(){
    // verification de la longueur des chaine _ vernam
    if(ui->ptex_cryp->toPlainText().size() != ui->ptex_key->toPlainText().size()) {
        ui->lab_avert->show();
        return;
    }
    else ui->lab_avert->hide();
    ui->lab_avert_2->hide();

    ui->ptex_clear->clear();
    QString aDeChiff = ui->ptex_cryp->toPlainText();
    QString key = ui->ptex_key->toPlainText();
    if(key.isEmpty() || aDeChiff.isEmpty()) return;
    for(int i=0; i< aDeChiff.size();i++){
        char* c = vernam_dechiffrer(
                    ((QString()+aDeChiff.at(i))).toUtf8().data(),
                    ((QString()+key.at(i))).toUtf8().data());
        ui->ptex_clear->setPlainText(ui->ptex_clear->toPlainText() + QString::fromUtf8(c,sizeof(char)));
        free(c);
    }
}
char generateKey(){
    char c = 0;
    c = 'A'+(QRandomGenerator::global()->generate()%26);
    return (c);
}
void MainWindow::genCle(){
    if(ui->ptex_clear->toPlainText().length() == 0) return;
    int len = ui->ptex_clear->toPlainText().size();
    char temp=0;
    QString str;
    for(int i=0;i<len;i++){
         temp = generateKey();
         str = str + QString(temp);
    }

    if(str.isEmpty()) str =" erreur de generation";
    ui->ptex_key->setPlainText(str.normalized(QString::NormalizationForm_KD));
}
MainWindow::~MainWindow()
{
    delete ui;
}

