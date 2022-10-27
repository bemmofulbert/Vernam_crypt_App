#ifndef HISTORYDIALOG_H
#define HISTORYDIALOG_H

#include <history.h>

#include <QDialog>
#include <QFile>

namespace Ui {
class HistoryDialog;
}

class HistoryDialog : public QDialog
{
    Q_OBJECT

public:
    explicit HistoryDialog(QWidget *parent = nullptr);
    ~HistoryDialog();
    inline QList<QString>* List_key(){return list_key;};
    inline QFile* File_his(){ return file_his;};
public slots:
    void quit();
    void clear();
    void fill();

private:
    Ui::HistoryDialog *ui;
    QFile *file_his;
    QWidget *parent;
    QList<QString> *list_key;
};

#endif // HISTORYDIALOG_H
