#include "janelaprincipal.h"

#include <QApplication>
#include <QtWidgets>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    JanelaPrincipal w;
    w.show();
   // JanelaPrincipal * janela = new JanelaPrincipal();
   // janela->show();
    return QApplication::exec();
}
