#include <QApplication>
#include <QTableWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>
#include <QHeaderView>

#include "CSVTable.h"
#include "CSVTableModel.h"

class Window : public QWidget {
    // Q_OBJECT

public:
    Window(QWidget* parent = nullptr) {
        setWindowTitle("CSV Viewer");
        resize(800, 600);
        setStyleSheet(R"(
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
    }
};

class CSVTableView : public QTableView {
public:
    CSVTableView(QWidget* parent = nullptr) {
        setGeometry(0, 0, 800, 600);
        setShowGrid(false);
        setAlternatingRowColors(true);
        verticalHeader()->setDefaultSectionSize(28);
    }
};

int main(int argc, char* argv[]) {
    QApplication a(argc, argv);

    auto root = new Window();

    csv::CSVTable table{"./sample-30mb.csv"};

    // table доолжен жить больше чем model, чтобы не было dangling reference(висячая ссылка)
    auto model = new CSVTableModel(std::ref(table));

    auto view = new CSVTableView(root);
    view->setModel(model);

    auto table_name_label = new QLabel(root);
    table_name_label->setText(QString::fromStdString(table.name()));

    auto layout = new QVBoxLayout(root);

    layout->addWidget(table_name_label);
    layout->addWidget(view);

    root->show();

    return QApplication::exec();
}
