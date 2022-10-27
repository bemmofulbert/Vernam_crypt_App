#include "historydialog.h"
#include "ui_historydialog.h"

HistoryDialog::HistoryDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::HistoryDialog)
{
    ui->setupUi(this);
    setWindowTitle("Historique des Clés");
    file_his = new QFile(HISTORY_NAME,this);    
    this->parent = parent;
    list_key = new QList<QString>();
    if(!history_open(this,file_his)) {
        QMessageBox::critical(parent,"erreur historique","impossible d'acceder\na l'historique");
        setVisible(false);
        return;
    }
    history_read(file_his,list_key);
    connect(ui->his_but_quit,SIGNAL(clicked()),this,SLOT(quit()));
    connect(ui->his_but_clear,SIGNAL(clicked()),this,SLOT(clear()));
    connect(this,SIGNAL(finished(int)),this,SLOT(quit()));
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

HistoryDialog::~HistoryDialog()
{
    delete ui;
}
