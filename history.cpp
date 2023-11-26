#include <history.h>

QFile* history_open(QWidget *parent,QFile *file_his){

    if(!file_his->open(QIODevice::ReadWrite | QIODevice::Text)){
        QMessageBox::critical(parent,"erreur historique","le systeme de fichier\nest inaccesible",QMessageBox::Ok);
        return nullptr;
    }
    return file_his;
}


bool history_add(QFile *file_his,QString &key){
    if(file_his != nullptr){
        QTextStream in(file_his);
        while(!in.atEnd()) {
              in.readLine(0);
        }
        in << key << "\n";
        in.flush();
        return true;
    }
    else return false;
}

bool history_clear(QWidget *parent,QFile *file_his) {
    if (file_his->remove()){
        file_his->close();
        history_open(parent,file_his);
        return true;
    }
    return false;
}

void history_read(QFile* file_his, QList<QString> *list_key) {
    QTextStream in(file_his);
    while (!in.atEnd()) {
        list_key->prepend(in.readLine());
    }
}

