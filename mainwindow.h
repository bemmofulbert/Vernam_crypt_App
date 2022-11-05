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

#include <wizardfichier.h>
#include <history.h>

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
private:
    Ui::MainWindow *ui;
    HistoryDialog *hisDial;
    const QString mes_error_length = tr("la clé doivent avoir au moins le meme nombre de lettre que le message");
    const QString mes_error_already = tr("Attention ! cette clé a déjá été utilisé");
    const QString lab_mes_clear = tr("Message Clair :");
    const QString lab_mes_chif = tr("Message Chiffre :");
    const QString lab_result_clear = tr("Resultat du Chiffrement :");
    const QString lab_result_chif = tr("Resultat du Dechiffrement :");
    //QString but_trait_chif = tr("DeChiffrer");
    //QString but_trait_clear = tr("Chiffrer");
    const QString text_char_letter = tr("le prochain caractere doit etre une lettre");
    const QString text_char_number = tr("le prochain caractere doit etre un chiffre");
    const QString text_key_correct = tr("la cle respecte les normes");
    const QString text_uncorrespond = tr("verifier votre chaine\nque les lettres cryptent les lettres\net les chiffres, les chiffres");
    const QString text_vide = tr("Taper du texte a chiffrer puis la cle");
    const QString alert_contact = tr("Contacter nous a l'adresse mail\nfbemmo.megasoft@gmail.com");
    QString green = "green";

    QTranslator *translator_fr,*translator_en;
    QApplication *App ;

    void initIcon();
    void initGraphic();

    WizardFichier *wizs;

protected :
    void closeEvent(QCloseEvent *event);
};
#endif // MAINWINDOW_H
