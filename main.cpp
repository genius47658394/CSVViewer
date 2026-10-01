#include <QApplication>
#include <QTableWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>
#include <QHeaderView>

#include "CSVTable.h"
#include "CSVTableModel.h"

int main(int argc, char* argv[]) {
    QApplication a(argc, argv);

    auto root = new QWidget();

    root->setWindowTitle("CSV Viewer");
    root->setGeometry(0, 0, 800, 600);
    root->setStyleSheet(R"(
        QWidget {
            background-color: #0F1720;
            color: #E6EDF3;
        }

        QTableView {
            background-color: #111C27;
            alternate-background-color: #162331;
            color: #E6EDF3;
            border: none;

            selection-background-color: #167D8D;
            selection-color: #FFFFFF;

            gridline-color: #263746;
        }

        QTableView::item {
            padding: 5px;
        }

        QHeaderView::section {
            background-color: #1B3340;
            color: #E6EDF3;
            padding: 7px;

            border: none;
            border-right: 1px solid #263746;
            border-bottom: 1px solid #35C7B5;
        }
    )");

    csv::CSVTable table = csv::from_csv("./customers-100000.csv");

    // table доолжен жить больше чем model, чтобы не было dangling reference(висячая ссылка)
    auto model = new CSVTableModel(table);

    auto view = new QTableView(root);

    view->setModel(model);
    view->setGeometry(0, 0, 800, 600);
    view->setShowGrid(false);
    view->setAlternatingRowColors(true);
    view->verticalHeader()->setDefaultSectionSize(28);

    auto table_name_label = new QLabel(root);
    table_name_label->setText(QString::fromStdString(table.name().data()));

    auto layout = new QVBoxLayout(root);

    layout->addWidget(table_name_label);
    layout->addWidget(view);

    root->show();

    return QApplication::exec();
}
