#include <QApplication>
#include <QVBoxLayout>
#include <QLabel>

#include "CSVTable.h"
#include "CSVTableModel.h"
#include "CSVTableView.h"
#include "MyInputDialog.h"
#include "Window.h"


int main(int argc, char* argv[]) {
    QApplication a(argc, argv);

    auto root = new Window();

    auto dialog = new MyInputDialog(root);

    QString path{};

    if (dialog->exec() == QDialog::Accepted)
        path = dialog->textValue();

    csv::CSVTable table{path.toStdString()};

    // table должен жить больше чем model, чтобы не было dangling reference(висячая ссылка)
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
