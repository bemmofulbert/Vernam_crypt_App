#ifndef HISTORY_H
#define HISTORY_H
#include <QFile>
#include <QMessageBox>
#include <QTextStream>
#include <QStack>

#define HISTORY_NAME "key_history.dat"

bool history_clear(QWidget *parent,QFile *file_his);
bool history_add(QFile *file_his,QString &key);
QFile* history_open(QWidget *parent,QFile *file_his);
void history_read(QFile* file_his, QList<QString> *list_key);

#endif // HISTORY_H
