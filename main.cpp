#include <iostream>
#include <print>
#include <QApplication>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>

#include "CSVTable.h"

int main(int argc, char* argv[]) {
    QApplication a(argc, argv);

    auto widget = std::make_unique<QWidget>();

    widget->setWindowTitle("My First Qt APP");
    widget->setFixedSize(800, 600);
    widget->setStyleSheet("background-color: #708090");

    auto btn1 = std::make_unique<QPushButton>("Button 1");
    auto btn2 = std::make_unique<QPushButton>("Button 2");
    auto btn3 = std::make_unique<QPushButton>("Button 3");
    auto btn4 = std::make_unique<QPushButton>("Button 4");

    auto vbox = std::make_unique<QVBoxLayout>(widget.get());

    vbox->addWidget(btn1.get());
    vbox->addWidget(btn2.get());
    vbox->addWidget(btn3.get());
    vbox->addWidget(btn4.get());

    widget->show();

    csv::CSVTable table = csv::from_csv("./example_columns.csv");

    std::cout << table;

    return QApplication::exec();
}
