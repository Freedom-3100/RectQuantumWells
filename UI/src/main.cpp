//
// Created by Mikhail on 06.11.2026.
//

#include <QApplication>

#include "MainWindow.hpp"
#include "SystemRegistry.hpp"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    MainWindow window(availableSystems());
    window.show();

    return QApplication::exec();
}
