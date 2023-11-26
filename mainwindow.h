#ifndef MAINWINDOW_H
#define MAINWINDOW_H

extern "C" {
    #include "vernam.h"
}

#include <QtGlobal>
#include <QMainWindow>
#include <QRandomGenerator>
#include <historydialog.h>
#include <QPlainTextEdit>
#include <QChar>
#include <QString>
#include <QMessageBox>
#include <QCloseEvent>
#include <QLocale>
#include <QTranslator>
#include <QCoreApplication>
#include <QFileDialog>
#include <QMovie>
#include <QKeyEvent>
#include <QSettings>


#include <wizardfichier.h>
#include <history.h>
#include <helpdialog.h>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr,QApplication *a=nullptr,QTranslator *tfr=nullptr,QTranslator *ten=nullptr);
    ~MainWindow();
    bool key_exist(QString &key);
    void avert_red(const QString &text);
    void avert_green(const QString &text);
    void decrypter();
    void crypter();
    bool preTraitement();
    bool chaine_ok();
    void postTraitement();
    void update_statusBar();
    bool correspond();
    void initConfiguration();


public slots:
    void genCle();
    void setGraphic_chif();
    void setGraphic_dechif();
    void tout_effacer();
    void limit_mes();
    void limit_key();
    void traitement();
    void montrer_historique();
    void contacter_nous();
    void a_propos();
    void zoom_in();
    void zoom_out();
    void dark_mode();
    void dark_mode_for_but();
    void lang_fr();
    void lang_en();
    void openMes_fromFile();
    void openKey_fromFile();
    void saveMes_toFile();
    void saveKey_toFile();
    void saveTraite_toFile();
    void toggle_lang();
    void montrer_avert_noSave();

    void toggleBrouil();
    void actCalculer_Hash();
    void launch_wiz_chiff();
    void launch_wiz_deChiff();
    void montrer_avert_A1(bool active);
    void launch_help();

private:
    Ui::MainWindow *ui;
    HistoryDialog *hisDial;

    const QString alert_contact = tr("Contacter nous a l'adresse mail\nfbemmo.megasoft@gmail.com");
    QString green = "green";

    QTranslator *translator_fr,*translator_en;
    QApplication *App ;

    void initIcon();
    void initGraphic();

    bool BrouilleurActive = false;
    QString BrouilleurKey;
    int oldX=(QRandomGenerator::global()->generate()%320);
    int oldY=(QRandomGenerator::global()->generate()%240);
    int BrouilleurPos=0;
    QString alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

    QSettings *settings;

protected :
    void closeEvent(QCloseEvent *event);

    void keyPressEvent(QKeyEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
};
#endif // MAINWINDOW_H
