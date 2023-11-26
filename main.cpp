#include "mainwindow.h"

#include <QApplication>

QTranslator *translator_fr,*translator_en;

void initLanguage(QApplication &a) {
    translator_fr = new QTranslator(&a);
    translator_en = new QTranslator(&a);
    if (translator_fr->load("fr_FR","translations/")) {
        a.installTranslator(translator_fr);
        qDebug("fr_FR : disponible");
    }
    if(translator_en->load("en_EN","translations/")) {
         qDebug("en_EN : disponible");
    }
}


int main(int argc, char *argv[])
{
//    QApplication::setDesktopSettingsAware(false);
//    QApplication::setStyle("gtk");

    QApplication a(argc, argv);

    QCoreApplication::setOrganizationName("BemsProduction");
    QCoreApplication::setOrganizationDomain("bemmoFulbert@gmail.com");
    QCoreApplication::setApplicationName("Vernam");

    QIcon::setFallbackSearchPaths(QStringList("images/iconsTheme"));
    QIcon::setThemeName("Ubuntu");

    QLocale::setDefault(QLocale(QLocale::French,QLocale::France));
    initLanguage(a);
    MainWindow w(nullptr,&a,translator_fr,translator_en);
    w.show();
    QPalette palette;
    palette.setColor(QPalette::Window,QColor(239,239,239));
    w.setPalette(palette);


    return a.exec();

//    char* texte = (char*)malloc(sizeof(char)*5);
//    texte[0]='a';
//    texte[1]='b';
//    texte[2]='z';
//    texte[3]='c';
//    texte[4]='k';
//    char* cle = generer_cle(strlen(texte));
//      printf("%s\n",vernam_chiffrer(texte,cle));
//      printf("---cle--%s---\n",cle);
//      printf("%s\n",vernam_dechiffrer(vernam_chiffrer(texte,cle),cle));
    //printf("%d",toupr('a'));
}
