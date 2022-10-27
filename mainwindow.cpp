#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent,QApplication *a,QTranslator *tfr,QTranslator *ten)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    App = a;
    ui->setupUi(this);

    //recuperation des translators
    translator_fr = tfr;
    translator_en = ten;
    // avertissement
    ui->lab_avert->hide();
    //construction de l'historique
    hisDial = new HistoryDialog(this);
    hisDial->setVisible(false);
    // signals
        // bouton et action chiffrement-dechiffrement
    connect(ui->rad_chif,SIGNAL(clicked()),this,SLOT(setGraphic_chif()));
    connect(ui->rad_dechif,SIGNAL(clicked()),this,SLOT(setGraphic_dechif()));
    connect(ui->actionChiffrement,SIGNAL(triggered()),this,SLOT(setGraphic_chif()));
    connect(ui->actionDeChiffrement,SIGNAL(triggered()),this,SLOT(setGraphic_dechif()));
        // action quitter
    connect(ui->actionQuitter,SIGNAL(triggered()),this,SLOT(close()));
        //Zoom
    connect(ui->but_zoomIn,SIGNAL(clicked()),this,SLOT(zoom_in()));
    connect(ui->actionZoomer,SIGNAL(triggered()),this,SLOT(zoom_in()));
    connect(ui->but_zoomOut,SIGNAL(clicked()),this,SLOT(zoom_out()));
    connect(ui->actionDezoomer,SIGNAL(triggered()),this,SLOT(zoom_out()));
        //Dark mode
    connect(ui->actionDark_mode,SIGNAL(triggered()),this,SLOT(dark_mode()));
    connect(ui->but_darkMode,SIGNAL(clicked()),this,SLOT(dark_mode_for_but()));
        // langue
    connect(ui->actionFrancais,SIGNAL(triggered()),this,SLOT(lang_fr()));
    connect(ui->actionAnglais,SIGNAL(triggered()),this,SLOT(lang_en()));
        // action et bouton tout effacerdark_mode_for_but()
    connect(ui->actionTout_effacer,SIGNAL(triggered()),this,SLOT(tout_effacer()));
    connect(ui->tbut_tout_effacer,SIGNAL(clicked()),this,SLOT(tout_effacer()));
        // tous les boutons effacer
    connect(ui->but_trashClear,SIGNAL(clicked()),ui->ptex_mes,SLOT(clear()));
    connect(ui->but_trashKey,SIGNAL(clicked()),ui->ptex_key,SLOT(clear()));
    connect(ui->but_trashTraite,SIGNAL(clicked()),ui->ptex_traite,SLOT(clear()));
        // tous les boutons copier
    connect(ui->but_copyClear,SIGNAL(clicked()),ui->ptex_mes,SLOT(selectAll()));
    connect(ui->but_copyKey,SIGNAL(clicked()),ui->ptex_key,SLOT(selectAll()));
    connect(ui->but_copyResult,SIGNAL(clicked()),ui->ptex_traite,SLOT(selectAll()));
    connect(ui->but_copyClear,SIGNAL(clicked()),ui->ptex_mes,SLOT(copy()));
    connect(ui->but_copyKey,SIGNAL(clicked()),ui->ptex_key,SLOT(copy()));
    connect(ui->but_copyResult,SIGNAL(clicked()),ui->ptex_traite,SLOT(copy()));
        //limitation des inputs
    connect(ui->ptex_mes,SIGNAL(textChanged()),this,SLOT(limit_mes()));
    connect(ui->ptex_key,SIGNAL(textChanged()),this,SLOT(limit_key()));
        //traitement
    connect(ui->but_traitement,SIGNAL(clicked()),this,SLOT(traitement()));
    connect(ui->actionChiffrer_deChiffrer,SIGNAL(triggered()),this,SLOT(traitement()));
        // generation de cle
    connect(ui->but_genKey,SIGNAL(clicked()),this,SLOT(genCle()));
        //historique
    connect(ui->tbut_history,SIGNAL(clicked()),this,SLOT(montrer_historique()));
    connect(ui->actionHistorique_des_Cl_s,SIGNAL(triggered()),this,SLOT(montrer_historique()));
        //menu aide
            //contactez nous
    connect(ui->actionContactez_nous,SIGNAL(triggered()),this,SLOT(contacter_nous()));
            //a propos
    connect(ui->actionA_Propos,SIGNAL(triggered()),this,SLOT(a_propos()));


    //init
    update_statusBar();
        //graphique
    initGraphic();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::limit_key() {
    QString text = ui->ptex_key->toPlainText();
    ui->lab_avert->hide();
    if(text.isEmpty()) return;
    text = text.simplified();
    for(int i=0;i<text.size();i++){
        if(!text[i].isLetterOrNumber()) {
            text = text.remove(text[i]);
            text.squeeze();
        }
    }
    text = text.toUpper();

    QString textClear = ui->ptex_mes->toPlainText().simplified().remove(' ');
    if( !text.isEmpty() && !textClear.isEmpty() && textClear.size()>=text.size() && textClear[text.size()-1].isNumber() && !text[text.size()-1].isNumber()){
        text.remove(text.size()-1,1);
        text.squeeze();
        avert_green(text_char_number);
    }
    else if( !text.isEmpty() && !textClear.isEmpty() && textClear.size()>=text.size() && textClear[text.size()-1].isLetter() && !text[text.size()-1].isLetter()){
        text.remove(text.size()-1,1);
        text.squeeze();
        avert_green(text_char_letter);
    }
    else if(chaine_ok()) {
        if(correspond()) {
            avert_green(text_key_correct);
        }
    }
    for(int i=4,len=text.size();i<len;i+=5){
        if(i != len-1) text.insert(++i,' ');
        len=text.size();
    }
    key_exist(text);
    disconnect(ui->ptex_key,SIGNAL(textChanged()),this,SLOT(limit_key()));

    ui->ptex_key->clear();
    ui->ptex_key->appendPlainText(text);

    connect(ui->ptex_key,SIGNAL(textChanged()),this,SLOT(limit_key()));
    update_statusBar();
}

void MainWindow::limit_mes() {
    QString text = ui->ptex_mes->toPlainText();
    if(text.isEmpty()) return;

    if(ui->rad_chif->isChecked()) {
        for(int i=0;i<text.size();i++){
            if (text[i].isSpace()) continue;
            else if(!text[i].isLetterOrNumber()) {
                text = text.remove(text[i]);
                text.squeeze();
            }
        }
    }else {
        text = text.simplified();
        for(int i=0;i<text.size();i++){
            if(!text[i].isLetterOrNumber()) {
                text = text.remove(text[i]);
                text.squeeze();
            }
        }
        for(int i=4,len=text.size();i<len;i+=5){
            if(i != len-1) text.insert(++i,' ');
            len=text.size();
        }
    }
    text = text.toUpper();

    disconnect(ui->ptex_mes,SIGNAL(textChanged()),this,SLOT(limit_mes()));

    ui->ptex_mes->clear();
    ui->ptex_mes->appendPlainText(text);

    connect(ui->ptex_mes,SIGNAL(textChanged()),this,SLOT(limit_mes()));
    update_statusBar();
}
void MainWindow::initGraphic() {
    ui->rad_chif->setStyleSheet("color:green;border:1px solid green;font-weight:bold");
    initIcon();
}
void MainWindow::initIcon() {
    ui->actionChiffrement->setIcon(QIcon(QPixmap("Close_oojs.png")));
    ui->actionChiffrer_deChiffrer->setIcon(QIcon::fromTheme("system-lock-screen"));
    ui->actionQuitter->setIcon(QIcon::fromTheme("system-log-out"));
    ui->actionTout_effacer->setIcon(QIcon::fromTheme("edit-clear"));
    ui->actionHistorique_des_Cl_s->setIcon(QIcon::fromTheme("user-bookmarks"));
    ui->actionConsulter_notre_aide->setIcon(QIcon::fromTheme("system-help"));
    ui->actionA_Propos->setIcon(QIcon::fromTheme("preferences-desktop-personal"));
    ui->actionContactez_nous->setIcon(QIcon::fromTheme("call-start"));
    ui->actionDezoomer->setIcon(QIcon::fromTheme("zoom-out"));
    ui->actionZoomer->setIcon(QIcon::fromTheme("zoom-in"));
}
void MainWindow::setGraphic_dechif() {
    ui->lab_mes->setText(lab_mes_chif);
    ui->lab_result->setText(lab_result_chif);
    ui->but_traitement->setText(but_trait_chif);
    ui->but_genKey->hide();
    ui->actionDeChiffrement->setIcon(QIcon(QPixmap("Close_oojs.png")));
    ui->actionChiffrement->setIcon(QIcon());
    ui->rad_dechif->toggle();
    ui->rad_chif->setStyleSheet("");ui->rad_dechif->setStyleSheet("color:green;border:1px solid green;font-weight:bold");
    ui->but_traitement->setIcon(QIcon::fromTheme("accessories-calculator"));
    limit_mes();
    ui->retranslateUi(this);
}

void MainWindow::setGraphic_chif() {
    ui->lab_mes->setText(lab_mes_clear);
    ui->lab_result->setText(lab_result_clear);
    ui->but_traitement->setText(but_trait_clear);
    ui->but_genKey->show();
    ui->actionDeChiffrement->setIcon(QIcon());
    ui->actionChiffrement->setIcon(QIcon(QPixmap("Close_oojs.png")));
    ui->rad_chif->toggle();
    ui->rad_chif->setStyleSheet("color:green;border:1px solid green;font-weight:bold");ui->rad_dechif->setStyleSheet("");
    ui->but_traitement->setIcon(QIcon::fromTheme("system-lock-screen"));
    ui->retranslateUi(this);
}

void MainWindow::tout_effacer() {
    ui->ptex_mes->clear();
    ui->ptex_key->clear();
    ui->ptex_traite->clear();
}

void MainWindow::avert_red(const QString &text) {
    ui->lab_avert->setStyleSheet("color: red");
    ui->lab_avert->setText(text);
    ui->lab_avert->show();
}
void MainWindow::avert_green(const QString &text) {
    ui->lab_avert->setStyleSheet("color: green");
    ui->lab_avert->setText(text);
    ui->lab_avert->show();
}

bool MainWindow::key_exist(QString &key) {
    history_read(hisDial->File_his(),hisDial->List_key());
    for(int i=0;i<hisDial->List_key()->size();i++) {
        if (key == hisDial->List_key()->at(i)){
            avert_red(mes_error_already);
            return true;
        }
    }
    return false;
}
bool MainWindow::correspond() {
    QString mes=ui->ptex_mes->toPlainText().simplified().remove(' ');
    QString key=ui->ptex_key->toPlainText().simplified().remove(' ');
    for(int i=0;i<mes.size();i++){
        if((mes[i].isLetter() && !key[i].isLetter()) || (!mes[i].isLetter() && key[i].isLetter())) {
            avert_red(text_uncorrespond);
            return false;
        }
        if((mes[i].isNumber() && !key[i].isNumber()) || (!mes[i].isNumber() && key[i].isNumber())) {
            avert_red(text_uncorrespond);
            return false;
        }
    }
    return true;
}
bool MainWindow::chaine_ok(){
    QString mes=ui->ptex_mes->toPlainText().simplified().remove(' ');
    QString key=ui->ptex_key->toPlainText().simplified().remove(' ');
    if(key.isEmpty() || mes.isEmpty()) {
        avert_green(text_vide);
        return false;
    }
    if(mes.size() > key.size()) {
        avert_red(mes_error_length);
        return false;
    }
        return true;
}
bool MainWindow::preTraitement(){
    // verification de la longueur des chaine _ vernam
    ui->lab_avert->hide();
    if(!chaine_ok() || !correspond()) return false;

    ui->ptex_traite->clear();
    return true;
}
void MainWindow::postTraitement() {
    QString key = ui->ptex_key->toPlainText();
    // pour l'historique
    if(!key_exist(key)){
        history_add(hisDial->File_his(),key);
        hisDial->List_key()->prepend(key);
    }
}
void MainWindow::crypter(){
    if(!preTraitement()) return;

    QString aChiff = ui->ptex_mes->toPlainText().simplified().remove(' ');
    QString key = ui->ptex_key->toPlainText().simplified().remove(' ');

    for(int i=0; i< aChiff.size();i++){
        char* c = vernam_chiffrer(
                    ((QString()+aChiff.at(i))).toUtf8().data(),
                    ((QString()+key.at(i))).toUtf8().data());
        ui->ptex_traite->setPlainText(ui->ptex_traite->toPlainText() + QString::fromUtf8(c,sizeof(char)));
        if(( (i+1) %5 == 0)) ui->ptex_traite->setPlainText(ui->ptex_traite->toPlainText() + QString(' '));
        free(c);
    }
    postTraitement();
}

void MainWindow::decrypter(){
   if(!preTraitement()) return;

    QString aDeChiff = ui->ptex_mes->toPlainText().simplified().remove(' ');
    QString key = ui->ptex_key->toPlainText().simplified().remove(' ');
    if(key.isEmpty() || aDeChiff.isEmpty()) return;
    for(int i=0; i< aDeChiff.size();i++){
        char* c = vernam_dechiffrer(
                    ((QString()+aDeChiff.at(i))).toUtf8().data(),
                    ((QString()+key.at(i))).toUtf8().data());
        ui->ptex_traite->setPlainText(ui->ptex_traite->toPlainText() + QString::fromUtf8(c,sizeof(char)));
        if(( (i+1) %5 == 0)) ui->ptex_traite->setPlainText(ui->ptex_traite->toPlainText() + QString(' '));
        free(c);
    }
    postTraitement();
}

void MainWindow::traitement() {
    if(ui->rad_chif->isChecked()) crypter();
    else decrypter();
}

char generateLetter(){
    char c = 0;
    c = 'A'+(QRandomGenerator::global()->generate()%26);
    return (c);
}
char generateNumber(){
    char c = 0;
    c = '0'+(QRandomGenerator::global()->generate()%10);
    return (c);
}
void MainWindow::genCle(){
    if(ui->ptex_mes->toPlainText().isEmpty()) return;
    QString text = ui->ptex_mes->toPlainText().simplified().remove(' ');
    char temp=0;
    QString str;
    //do{
        //str.clear();
        for(int i=0;i<text.size();i++){
             if(text[i].isLetter()) temp = generateLetter();
             else if (text[i].isNumber()) temp=generateNumber();
             str = str + QString(temp);
        }
    // }while (key_exist(str));
    //ui->lab_avert->hide();
    if(str.isEmpty()) str =tr("erreur de generation");
    ui->ptex_key->setPlainText(str);
}

void MainWindow::montrer_historique(){
    hisDial->fill();
    hisDial->setMaximumWidth(this->width());
    hisDial->setVisible(true);
}

void MainWindow::lang_fr() {
    ui->actionFrancais->setChecked(true);
    ui->actionAnglais->setChecked(false);
    if(!translator_fr->isEmpty()) {
        App->installTranslator(translator_fr);
        ui->retranslateUi(this);
        qDebug("langue actuelle :fr_FR");
    }
}

void MainWindow::lang_en() {
    ui->actionAnglais->setChecked(true);
    ui->actionFrancais->setChecked(false);
    if(!translator_en->isEmpty()) {
        App->installTranslator(translator_en);
        ui->retranslateUi(this);
        qDebug("langue actuelle :en_EN");
    }
}



void MainWindow::zoom_in() {
        ui->ptex_mes->zoomIn(2);
        ui->ptex_key->zoomIn(2);
        ui->ptex_traite->zoomIn(2);
}
void MainWindow::zoom_out() {
        ui->ptex_mes->zoomOut(2);
        ui->ptex_key->zoomOut(2);
        ui->ptex_traite->zoomOut(2);
}
void MainWindow::update_statusBar(){
    this->statusBar()->showMessage(tr("message: ")
                                   + QString::number(ui->ptex_mes->toPlainText().simplified().remove(' ').size())
                                   +tr(" caracteres | Cle: ")
                                   + QString::number(ui->ptex_key->toPlainText().simplified().remove(' ').size())
                                   +tr(" caracteres"));
}
void MainWindow::dark_mode() {
    if (ui->actionDark_mode->isChecked()){
        this->setStyleSheet("background-color:#404142;color:white;");
    }else this->setStyleSheet("");
}
void MainWindow::dark_mode_for_but() {
    ui->actionDark_mode->toggle();
    dark_mode();
}

void MainWindow::closeEvent(QCloseEvent *event){
    QMessageBox mesBox(QMessageBox::Information,tr("quitter ?"),tr("<center>Etes vous sur de vouloir quitter l'application</center>"),
                                                                   QMessageBox::Yes|QMessageBox::No,this);
    if(mesBox.exec() == QMessageBox::Yes){
        event->accept();
    }else event->ignore();
}
void MainWindow::contacter_nous(){
    QMessageBox::information(this,tr("contact"),alert_contact);
}
void MainWindow::a_propos() {
    QMessageBox::information(this,tr("A propos"),tr("logiciel de chiffrement utilisant <br>le chiffre de Vernam sous licence libre<br><br>")
                             +tr("Fait par :")+ " <br><B>BEMMO MBOBDA FULBERT ALEXANDRE<br>"
                             +"DNJOMOU YOMBA WILFRIED LOIC<br>"
                             +"TONBA DJIMGOU BRAILAIN LOIC<br>"
                             +"</B>");
}
