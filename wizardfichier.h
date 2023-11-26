#ifndef WIZARDFICHIER_H
#define WIZARDFICHIER_H

#include <QWizard>
#include <QWizardPage>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QObject>
#include <QLineEdit>
#include <QPushButton>
#include <QFileDialog>
#include <QMessageBox>
#include <QProgressBar>
#include <QRandomGenerator>
#include <QDir>
#include <QWidget>
#include <QCryptographicHash>
#include <QTextStream>

class WizardFichier: public QObject
{
    Q_OBJECT

public:
    WizardFichier():QObject(){
        initWizard_chif();
        initWizard_deChif();
    }

    static QString calcul_shasum(const QString filename,char separateur=' ');

    QWizardPage* chiff_createIntro();
    QWizardPage* chiff_createChoiseFile();
    QWizardPage* chiff_createFinal();
    QWizardPage* chiff_createDestChoice();
    void initWizard_chif();
    int genCle_bin_REMAKE(const char *nomfichier, const char* nomDestination);
    int encrypt_bin_REMAKE(const char* nomfichier, const char* nomCle, const char* nomDestination);

    int decrypt_bin_REMAKE(const char* nomfichier, const char* nomCle, const char* nomDestination);
    QWizardPage* dechiff_createIntro();
    QWizardPage* dechiff_createChoiseKey();
    QWizardPage* dechiff_createChoiseFile();
    QWizardPage* dechiff_createDestChoice();
    QWizardPage* dechiff_createFinal();
    void initWizard_deChif();

    inline QWizard * getWiz_Chif() { return &wiz_chiff; };
    inline QWizard * getWiz_DeChif() { return &wiz_dechiff; };

public slots:
    void onChiff_currentIdChange(int i);
    void onChiff_butPath();
    void onChiff_butPathDest();

    void onDeChiff_currentIdChange(int i);
    void onDechiff_butPathKey();
    void onDechiff_butPathFile();
    void onDechiff_butPathDest();

private:
    //assistant text
    const QString chiff_text_intro = tr("Bienvenue dans l'assistance de chiffrement de fichier<br><br> Cliquer sur \"suivant\" pour continuer");
    const QString chiff_choise = tr("Veuillez choisir le fichier a chiffrer :<br><br>");
    const QString chiff_choiceDest = tr("Veuillez choisir la destination du fichier crypter<br> et de la cle :<br><br>");

    const QString dechiff_text_intro = tr("Bienvenue dans l'assistance de dechiffrement de fichier<br><br> Cliquer sur \"suivant\" pour continuer");
    const QString dechiff_choiseKey = tr("Veuillez choisir le fichier servant de cle :<br><br>");
    const QString dechiff_choiseFile = tr("Veuillez choisir le fichier crypte :<br><br>");
    const QString dechiff_choiceDest = tr("Veuillez choisir la destination du fichier clair :<br><br>");
    //

    QWizard wiz_chiff;
        QLineEdit* edit_path;
        QLineEdit* edit_pathDest;
        QString path;
        QString pathDest;
        QProgressBar * chiff_bar_gen;
        QProgressBar * chiff_bar_crypt;
    QWizard wiz_dechiff;
        QLineEdit* dechif_edit_pathKey;
        QLineEdit* dechif_edit_pathFile;
        QLineEdit* dechif_edit_pathDest;
        QString dechif_pathKey;
        QString dechif_pathFile;
        QString dechif_pathDest;
        QProgressBar * dechiff_bar;



};

#endif // WIZARDFICHIER_H
