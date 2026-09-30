#include <iostream>
#include <print>
#include <QApplication>
#include <QTableWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>

#include "CSVTable.h"
#include "CSVTableModel.h"

int main(int argc, char* argv[]) {
    QApplication a(argc, argv);

    auto root = new QWidget();

    root->setWindowTitle("CSV Viewer");
    root->setGeometry(0, 0, 800, 600);
    root->setStyleSheet("background-color: #1F6B75");

    csv::CSVTable table = csv::from_csv("./customers-100000.csv");

    auto model = new CSVTableModel(table);

    auto view = new QTableView(root);
    view->setModel(model);
    view->setGeometry(0, 0, 800, 600);

    auto layout = new QVBoxLayout(root);
    layout->addWidget(view);

    root->show();

    return QApplication::exec();
}
