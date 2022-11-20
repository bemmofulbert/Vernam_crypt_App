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
    connect(ui->but_lang,SIGNAL(clicked()),this,SLOT(toggle_lang()));
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
    connect(ui->but_chiff,SIGNAL(clicked()),this,SLOT(traitement()));
    connect(ui->but_deChiff,SIGNAL(clicked()),this,SLOT(traitement()));
    connect(ui->actionChiffrer_deChiffrer,SIGNAL(triggered()),this,SLOT(traitement()));
        // generation de cle
    connect(ui->but_genKey,SIGNAL(clicked()),this,SLOT(genCle()));
        //historique
    connect(ui->tbut_history,SIGNAL(clicked()),this,SLOT(montrer_historique()));
    connect(ui->actionHistorique_des_Cl_s,SIGNAL(triggered()),this,SLOT(montrer_historique()));
        // activer/desactiver l'historique des cles
    connect(ui->actionActiver_la_sauvegarde_des_Cl_s,SIGNAL(triggered()),this,SLOT(montrer_avert_noSave()));
        //import export
            //open
    connect(ui->but_openMes,SIGNAL(clicked()),this,SLOT(openMes_fromFile()));
    connect(ui->but_openKey,SIGNAL(clicked()),this,SLOT(openKey_fromFile()));
            //save
    connect(ui->but_saveMes,SIGNAL(clicked()),this,SLOT(saveMes_toFile()));
    connect(ui->but_saveKey,SIGNAL(clicked()),this,SLOT(saveKey_toFile()));
    connect(ui->but_saveTraite,SIGNAL(clicked()),this,SLOT(saveTraite_toFile()));
        //menu aide
            //contactez nous
    connect(ui->actionContactez_nous,SIGNAL(triggered()),this,SLOT(contacter_nous()));
            //a propos
    connect(ui->actionA_Propos,SIGNAL(triggered()),this,SLOT(a_propos()));

    // init Wizard
    wizs = new WizardFichier();

    connect(ui->actionChiffrer_un_fichier,SIGNAL(triggered()),wizs->getWiz_Chif(),SLOT(exec()));
    connect(ui->actionDechiffrer_un_fichier,SIGNAL(triggered()),wizs->getWiz_DeChif(),SLOT(exec()));

    //Brouiller
    connect(ui->but_Brouil,SIGNAL(clicked()),this,SLOT(toggleBrouil()));
    //Hash
    connect(ui->actionCalculer_Hash,SIGNAL(triggered()),this,SLOT(actCalculer_Hash()));
    //init
    update_statusBar();
        //graphique
    initGraphic();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::limit_key() { // mettre en forme le champ de saisie de la cle

    QString text = ui->ptex_key->toPlainText();
    QTextCursor cursor = ui->ptex_key->textCursor();
    int curPos = cursor.position();
    bool badCharacter = false;
    int oldSize = text.size();

    if(!BrouilleurActive) ui->lab_avert->hide();
    if(text.isEmpty()) return;
    text = text.simplified().remove(' ');
    text.squeeze();
    for(int i=0;i<text.size();i++){
        if(!text[i].isLetterOrNumber()) {
            text = text.remove(text[i]);
            text.squeeze();
            badCharacter = true;
        }
    }
    text = text.toUpper();

    QString textClear = ui->ptex_mes->toPlainText().simplified().remove(' ');
    textClear.squeeze();
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
    if(text.size()>=oldSize){
        if(!badCharacter){
            if(((curPos-1)>=0 && (curPos-1)<text.size()) && text[curPos-1].isSpace()) cursor.setPosition(curPos+1);
            else cursor.setPosition(curPos);
        }else {
            cursor.setPosition(curPos-1);
        }
    }else cursor.setPosition(curPos-1);
    ui->ptex_key->setTextCursor(cursor);

    update_statusBar();
}

void MainWindow::limit_mes() { // mettre en forme le champ de saisie du message

    QString text = ui->ptex_mes->toPlainText();
    QTextCursor cursor = ui->ptex_mes->textCursor();
    int curPos = cursor.position();
    bool badCharacter = false;
    int oldSize = text.size();

    if(text.isEmpty()) return;

    if(ui->rad_chif->isChecked()) {
        for(int i=0;i<text.size();i++){
            if (text[i].isSpace()) continue;
            else if(!text[i].isLetterOrNumber()) {
                text = text.remove(text[i]);
                text.squeeze();
                badCharacter = true;
            }
        }
    }else {
        text = text.simplified().remove(' ');
        for(int i=0;i<text.size();i++){
            if(!text[i].isLetterOrNumber()) {
                text = text.remove(text[i]);
                text.squeeze();
                badCharacter = true;
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

    if(ui->rad_chif->isChecked()) {
        if(text.size()>=oldSize){
            if(!badCharacter){
                cursor.setPosition(curPos);
            }else {
                cursor.setPosition(curPos-1);
            }
        }else cursor.setPosition(curPos-1);
    }
    else {
        if(text.size()>=oldSize){
            if(!badCharacter){
                if(((curPos-1)>=0 && (curPos-1)<text.size()) && text[curPos-1].isSpace()) cursor.setPosition(curPos+1);
                else cursor.setPosition(curPos);
            }else {
                cursor.setPosition(curPos-1);
            }
        }else cursor.setPosition(curPos-1);
    }
    ui->ptex_mes->setTextCursor(cursor);

    update_statusBar();
}
void MainWindow::initGraphic() { // initialiser les elements graphique de la fenetre
    ui->but_deChiff->hide();
    ui->rad_chif->setStyleSheet("color:green;border:1px solid green;font-weight:bold");
    initIcon();
    ui->progBar_Brouilleur->hide();
}
void MainWindow::initIcon() { // initialiser les icones
    ui->actionChiffrement->setIcon(QIcon(QPixmap("images/Close_oojs.png")));
    ui->actionChiffrer_deChiffrer->setIcon(QIcon::fromTheme("system-lock-screen"));
    ui->actionQuitter->setIcon(QIcon::fromTheme("system-log-out"));
    ui->actionTout_effacer->setIcon(QIcon::fromTheme("user-trash-full"));
    ui->actionHistorique_des_Cl_s->setIcon(QIcon::fromTheme("user-bookmarks"));
    ui->actionConsulter_notre_aide->setIcon(QIcon::fromTheme("system-help"));
    ui->actionA_Propos->setIcon(QIcon::fromTheme("preferences-desktop-personal"));
    ui->actionContactez_nous->setIcon(QIcon::fromTheme("call-start"));
    ui->actionDezoomer->setIcon(QIcon::fromTheme("zoom-out"));
    ui->actionZoomer->setIcon(QIcon::fromTheme("zoom-in"));
    ui->but_lang->setIcon(QIcon(QPixmap("images/french.png")));
    QMovie *deco = new QMovie("images/cube11.gif",QByteArray(),this);
    deco->setScaledSize(QSize(25,25));
    ui->lab_deco->setMovie(deco);deco->start();
}

void MainWindow::setGraphic_dechif() {
    ui->lab_mes->setText(lab_mes_chif);
    ui->lab_result->setText(lab_result_chif);
     ui->but_genKey->hide();
     ui->but_chiff->hide();
     ui->but_deChiff->show();
    ui->actionDeChiffrement->setIcon(QIcon(QPixmap("images/Close_oojs.png")));
    ui->actionChiffrement->setIcon(QIcon());
    ui->rad_dechif->toggle();
    ui->rad_chif->setStyleSheet("");ui->rad_dechif->setStyleSheet("color:"+green+";border:1px solid "+green+";font-weight:bold");
    limit_mes();
    ui->retranslateUi(this);
}

void MainWindow::setGraphic_chif() {
    ui->lab_mes->setText(lab_mes_clear);
    ui->lab_result->setText(lab_result_clear);
    ui->but_genKey->show();
    ui->but_chiff->show();
    ui->but_deChiff->hide();
    ui->actionDeChiffrement->setIcon(QIcon());
    ui->actionChiffrement->setIcon(QIcon(QPixmap("images/Close_oojs.png")));
    ui->rad_chif->toggle();
    ui->rad_chif->setStyleSheet("color:"+green+";border:1px solid "+green+";font-weight:bold");ui->rad_dechif->setStyleSheet("");
    ui->retranslateUi(this);
}

void MainWindow::tout_effacer() { // TOUT EFFACER
    ui->ptex_mes->clear();
    ui->ptex_key->clear();
    ui->ptex_traite->clear();
}

void MainWindow::avert_red(const QString &text) {
    if (!BrouilleurActive) {
        ui->lab_avert->setStyleSheet("color: red");
        ui->lab_avert->setText(text);
        ui->lab_avert->show();
    }
}
void MainWindow::avert_green(const QString &text) {
    if (!BrouilleurActive) {
        ui->lab_avert->setStyleSheet("color: green");
        ui->lab_avert->setText(text);
        ui->lab_avert->show();
    }
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
bool MainWindow::correspond() { // verifie que les champs respecte les contraintes
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
bool MainWindow::chaine_ok(){ // une autre couche pour vernam_apte()
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
void MainWindow::crypter(){ // Crypter le message et afficher le resultat
    if(!preTraitement()) return;

    QString aChiff = ui->ptex_mes->toPlainText();
    QString key = ui->ptex_key->toPlainText();
        aChiff = aChiff.simplified().remove(' ');
        key = key.simplified().remove(' ');

    char* c;

    for(int i=0; i< aChiff.size();i++){
        if (ui->actionA_1->isChecked())        c = vernam_chiffrer_1((QString() + aChiff.at(i)).toUtf8().data(),
                                                                   (QString()+key.at(i)).toUtf8().data());
        else c = vernam_chiffrer((QString() + aChiff.at(i)).toUtf8().data(),
                                 (QString()+key.at(i)).toUtf8().data());

        ui->ptex_traite->setPlainText(ui->ptex_traite->toPlainText() + QString::fromUtf8(c,sizeof(char)));

        if(( (i+1) %5 == 0)) ui->ptex_traite->setPlainText(ui->ptex_traite->toPlainText() + QString(' '));

        free(c);
    }
    if (ui->actionActiver_la_sauvegarde_des_Cl_s->isChecked()) postTraitement();

}

void MainWindow::decrypter(){ // DeCrypter le message et afficher le resultat
   if(!preTraitement()) return;

    QString aDeChiff = ui->ptex_mes->toPlainText();
    QString key = ui->ptex_key->toPlainText();
        aDeChiff = aDeChiff.simplified().remove(' ');
        key = key.simplified().remove(' ');

    if(key.isEmpty() || aDeChiff.isEmpty()) return;

    char* c;

    for(int i=0; i< aDeChiff.size();i++){
        if (ui->actionA_1->isChecked())         c = vernam_dechiffrer_1((QString() + aDeChiff.at(i)).toUtf8().data(),
                                                                      (QString()+key.at(i)).toUtf8().data());
        else c = vernam_dechiffrer((QString() + aDeChiff.at(i)).toUtf8().data(),
                              (QString()+key.at(i)).toUtf8().data());

        ui->ptex_traite->setPlainText(ui->ptex_traite->toPlainText() + QString::fromUtf8(c,sizeof(char)));

        if(( (i+1) %5 == 0)) ui->ptex_traite->setPlainText(ui->ptex_traite->toPlainText() + QString(' '));

        free(c);
    }
    if (ui->actionActiver_la_sauvegarde_des_Cl_s->isChecked()) postTraitement();

}

void MainWindow::traitement() {
    App->setOverrideCursor(QCursor(Qt::WaitCursor));

    if(ui->rad_chif->isChecked()) crypter();
    else decrypter();

    App->setOverrideCursor(QCursor(Qt::ArrowCursor));
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
    if(str.isEmpty()) str =tr("erreur de generation");
    ui->ptex_key->setPlainText(str);
}

void MainWindow::montrer_avert_noSave() {
    if (!ui->actionActiver_la_sauvegarde_des_Cl_s->isChecked())
        QMessageBox::information(this,"sauvegarde des cles desactive",
                                 "Vous ne pourrez plus savoir si vous avez deja utilise une cle !");
}

void MainWindow::montrer_historique(){
    hisDial->fill();
    hisDial->setMaximumWidth(this->width());
    hisDial->setVisible(true);
}
void MainWindow::toggle_lang(){
    if(ui->actionAnglais->isChecked()) {
        lang_fr();
    }else lang_en();
    wizs->initWizard_chif();
    wizs->initWizard_deChif();
}
void MainWindow::lang_fr() {
    ui->actionFrancais->setChecked(true);
    ui->actionAnglais->setChecked(false);
    if(!translator_fr->isEmpty()) {
        App->installTranslator(translator_fr);
        ui->retranslateUi(this);
        qDebug("langue actuelle :fr_FR");
        ui->but_lang->setIcon(QIcon(QPixmap("images/french.png")));
    }
}

void MainWindow::lang_en() {
    ui->actionAnglais->setChecked(true);
    ui->actionFrancais->setChecked(false);
    if(!translator_en->isEmpty()) {
        App->installTranslator(translator_en);
        ui->retranslateUi(this);
        qDebug("langue actuelle :en_EN");
        ui->but_lang->setIcon(QIcon(QPixmap("images/english.png")));
    }
}

void MainWindow::openMes_fromFile() {
    QString fileName = QFileDialog::getOpenFileName(this,tr("Ouvrir le fichier contenant le message"),QString(),
                                 tr("Fichiers Textes(*.txt *.dat)"));
    if(fileName.isEmpty()) return;
    QFile file(fileName);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::critical(this,tr("erreur"),"Un probleme s'est produit\nlors de l'ouverture du fichier");
        return;
    }
    ui->ptex_mes->clear();
    QTextStream out(&file);
    while (!out.atEnd()) {
        ui->ptex_mes->setPlainText(ui->ptex_mes->toPlainText()+out.readLine());
    }
    file.close();
}

void MainWindow::openKey_fromFile() {
    QString fileName = QFileDialog::getOpenFileName(this,tr("Ouvrir le fichier contenant la Cle"),QString(),
                                 tr("Fichiers Textes(*.txt *.dat)"));
    if(fileName.isEmpty()) return;
    QFile file(fileName);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::critical(this,tr("erreur"),"Un probleme s'est produit\nlors de l'ouverture du fichier");
        return;
    }
    ui->ptex_key->clear();
    QTextStream out(&file);
    while (!out.atEnd()) {
        ui->ptex_key->setPlainText(ui->ptex_key->toPlainText()+out.readLine());
    }
    file.close();
}

void MainWindow::saveMes_toFile() {
    QString fileName = QFileDialog::getSaveFileName(this,tr("Entrez le nom du fichier de sauvegarde du message"),QString(),
                                 tr("Fichiers Textes(*.txt *.dat)"));
    if(fileName.isEmpty()) return;
    QFile file(fileName);
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::critical(this,tr("erreur"),"Un probleme s'est produit\nlors de lors de l'enregistrement");
        return;
    }
    QTextStream in(&file);
    in << ui->ptex_mes->toPlainText();
    in.flush();
    file.close();
}

void MainWindow::saveKey_toFile() {
    QString fileName = QFileDialog::getSaveFileName(this,tr("Entrez le nom du fichier de sauvegarde de la cle"),QString(),
                                 tr("Fichiers Textes(*.txt *.dat)"));
    if(fileName.isEmpty()) return;
    QFile file(fileName);
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::critical(this,tr("erreur"),"Un probleme s'est produit\nlors de lors de l'enregistrement");
        return;
    }
    QTextStream in(&file);
    in << ui->ptex_key->toPlainText();
    in.flush();
    file.close();
}

void MainWindow::saveTraite_toFile() {
    QString fileName = QFileDialog::getSaveFileName(this,tr("Entrez le nom du fichier de sauvegarde de la cle"),QString(),
                                 tr("Fichiers Textes(*.txt *.dat)"));
    if(fileName.isEmpty()) return;
    QFile file(fileName);
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::critical(this,tr("erreur"),"Un probleme s'est produit\nlors de lors de l'enregistrement");
        return;
    }
    QTextStream in(&file);
    in << ui->ptex_traite->toPlainText();
    in.flush();
    file.close();
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
    QString key = ui->ptex_key->toPlainText().simplified().remove(' ');
    QString mes = ui->ptex_mes->toPlainText().simplified().remove(' ');
    int keyLen = key.size(), mesLen = mes.size();
    QString resultMes;
    if(mesLen > 1){
        resultMes += tr("message: ") + QString::number(mesLen)+ tr(" caracteres");
    }
    if(mesLen > 1 && keyLen > 1) {
        resultMes += " | ";
    }
    if(keyLen > 1){
        resultMes += tr("Cle: ") + QString::number(keyLen) +tr(" caracteres");
    }
    this->statusBar()->showMessage(resultMes);
}
void MainWindow::dark_mode() {
    if (ui->actionDark_mode->isChecked()){
        this->setStyleSheet("background-color:#404142;color:white;");
        green = "lightgreen";
    }else {
        this->setStyleSheet("");
        green = "green";
    }

    ui->ptex_traite->setStyleSheet("border:2px dashed "+green);
    if(ui->rad_chif->isChecked()) {
        ui->rad_chif->click();
    }else {
        ui->rad_dechif->click();
    }
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
    QMessageBox about(QMessageBox::Information,tr("A propos"),tr("logiciel de chiffrement utilisant <br>le chiffre de Vernam sous licence libre<br><br>")
                             +tr("Fait par :")+ " <br><B>BEMMO MBOBDA FULBERT ALEXANDRE<br>"
                             +"DNJOMOU YOMBA WILFRIED LOIC<br>"
                             +"TONBA DJIMGOU BRAILAIN LOIC<br>"
                             +"</B>",QMessageBox::Ok,this);
    about.setIconPixmap(QPixmap("images/logo_long.jpg").scaled(150,84));
    about.exec();
}
void MainWindow::keyPressEvent(QKeyEvent *event){
    const int keyEsc = 16777216;
    if (BrouilleurActive){
        if(event->key() == keyEsc){
            BrouilleurActive = false;
            releaseMouse();
            setMouseTracking(false);
            ui->but_Brouil->setStyleSheet("");
            menuBar()->setEnabled(true);
            ui->ptex_mes->setEnabled(true);
            ui->ptex_traite->setEnabled(true);
            ui->lab_deco->movie()->start();
            ui->lab_avert->hide();
            ui->progBar_Brouilleur->hide();
        }
    }
}
void MainWindow::mouseMoveEvent(QMouseEvent *event) {
    const int x=event->pos().x(),y=event->pos().y();
    int k=0;
    QString cleActuel = ui->ptex_key->toPlainText().simplified().remove(" ");

    if(cleActuel.isEmpty()) {
        ui->progBar_Brouilleur->setValue(100);
        return;
    }
    if (BrouilleurActive) {
        if (x>=oldX && y>=oldY) k=0;
        else if (x>=oldX && y<oldY) k=1;
        else if (x<oldX && y>=oldY) k=2;
        else if (x<oldX && y<oldY) k=3;
    }
    char oldC = cleActuel[BrouilleurPos].toLatin1();
    char newC = oldC;
    if(QChar(oldC).isLetter())
         newC = lettre_to_ascii(mod26(ASCII_to_numLettre(oldC)+ASCII_to_numLettre(BrouilleurKey[k].toLatin1())));
    else if(QChar(oldC).isNumber())
         newC = chiffre_to_ascii(mod10(ASCII_to_chiffre(oldC)+ASCII_to_chiffre(BrouilleurKey[k].toLatin1())));

    cleActuel = cleActuel.replace(BrouilleurPos,1,QChar(newC));
    ui->ptex_key->setPlainText(cleActuel);
    if(++BrouilleurPos >= cleActuel.size()) BrouilleurPos=0;

    ui->progBar_Brouilleur->setValue((BrouilleurPos+1)*100/cleActuel.size());
}
void MainWindow::toggleBrouil() {
    if(!BrouilleurActive) {
        BrouilleurActive = true;
        grabMouse();
        setMouseTracking(true);
        ui->but_Brouil->setStyleSheet("background-color:green");

        genCle();
        BrouilleurKey.clear();
        char c;
        for(int i=0;i<4;i++) {
            c='A'+(char)(QRandomGenerator::global()->generate()%26);
            BrouilleurKey.append(c);
        }
        menuBar()->setEnabled(false);
        ui->ptex_mes->setEnabled(false);
        ui->ptex_traite->setEnabled(false);
        ui->lab_deco->movie()->stop();
        ui->progBar_Brouilleur->show();
        if(ui->actionDark_mode->isChecked()) ui->lab_avert->setStyleSheet("color: lightblue");
        else ui->lab_avert->setStyleSheet("color: blue");
        ui->lab_avert->setText(brouilleur_indication);
        ui->lab_avert->show();
    }
}

void MainWindow::actCalculer_Hash() {
    QString filename = QFileDialog::getOpenFileName(this,tr("Selectionner le fichier dont vous voulrz le hash Sha1"));
    if(filename.isEmpty()) return;
    QMessageBox::information(this,"hash Sha1",tr("fichier : ")+filename+"<br><br>Hash Sha-1 : <b>"+
                             WizardFichier::calcul_shasum(filename) +"</b>");
}
