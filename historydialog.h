#ifndef HISTORYDIALOG_H
#define HISTORYDIALOG_H

#include "qlistwidget.h"
#include <history.h>

#include <QDialog>
#include <QFile>
#include <QPlainTextEdit>
#include <QListWidgetItem>
#include <QMainWindow>
#include <QStatusBar>

namespace Ui {
class HistoryDialog;
}

class HistoryDialog : public QDialog
{
    Q_OBJECT

public:
    explicit HistoryDialog(QWidget *parent = nullptr,QPlainTextEdit* parent_ptex_Key=nullptr);
    ~HistoryDialog();
    inline QList<QString>* List_key(){return list_key;};
    inline QFile* File_his(){ return file_his;};
public slots:
    void quit();
    void clear();
    void fill();
    void onDoubleclick_Key(QListWidgetItem* item);
    void init();
    void showNow();
private:
    Ui::HistoryDialog *ui;
    QFile *file_his;
    QWidget *parent;
    QList<QString> *list_key;
    QPlainTextEdit *parent_ptex_Key;
};

#endif // HISTORYDIALOG_H
