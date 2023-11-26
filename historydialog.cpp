#include "historydialog.h"
#include "ui_historydialog.h"

HistoryDialog::HistoryDialog(QWidget *parent, QPlainTextEdit* parent_ptex_Key) :
    QDialog(parent),
    ui(new Ui::HistoryDialog)
{
    ui->setupUi(this);
    setWindowTitle(tr("Historique des Clés"));
    file_his = new QFile(HISTORY_NAME,this);    
    this->parent = parent;
    list_key = new QList<QString>();
    if(!history_open(this,file_his)) {
        QMessageBox::critical(parent,tr("erreur historique"),tr("impossible d'acceder\na l'historique"));
        setVisible(false);
        return;
    }

    this->parent_ptex_Key = parent_ptex_Key;

    history_read(file_his,list_key);
    connect(ui->his_but_quit,SIGNAL(clicked()),this,SLOT(quit()));
    connect(ui->his_but_clear,SIGNAL(clicked()),this,SLOT(clear()));
    connect(this,SIGNAL(finished(int)),this,SLOT(quit()));
    connect(ui->his_listWid,SIGNAL(itemDoubleClicked(QListWidgetItem*)),this,SLOT(onDoubleclick_Key(QListWidgetItem*)));
}

void HistoryDialog::quit(){
    setVisible(false);
}
void HistoryDialog::fill(){
    ui->his_listWid->clear();
    history_read(file_his,list_key);
    for(int i=0;i<list_key->size();i++) {
        ui->his_listWid->addItem(list_key->at(i));
    }
}
void HistoryDialog::clear() {
    history_clear(parent,file_his);
    ui->his_listWid->clear();
    list_key->clear();
}

void HistoryDialog::onDoubleclick_Key(QListWidgetItem* item) {
    parent_ptex_Key->setPlainText(item->text());
    quit();
    ((QMainWindow*)parentWidget())->statusBar()->showMessage(tr("Cle copie"));
}

void HistoryDialog::init() {
    setWindowTitle(tr("Historique des Clés"));
    ui->his_but_quit->setText(tr("Quitter"));
    ui->his_but_clear->setText(tr("clear"));
}

void HistoryDialog::showNow(){
    init();
    setVisible(true);
}

HistoryDialog::~HistoryDialog()
{
    delete ui;
}
