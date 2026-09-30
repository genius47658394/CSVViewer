#include <iostream>
#include <print>
#include <QApplication>
#include <QTableWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>

#include "CSVTable.h"

static QVector<std::string> Qvec_from_std_vec(const std::vector<std::string>& vec) {
    QVector<std::string> line{};

    for (const auto& el: vec)
        line.emplace_back(el);

    return line;
}

static QVector<QVector<std::string>> create_2d_from_table(const csv::CSVTable & table) {
    QVector<QVector<std::string>> out{};

    out.emplace_back(Qvec_from_std_vec(table.headers()));

    for (std::size_t i = 0; i < table.size_of_column(); ++i)
        out.emplace_back(Qvec_from_std_vec(table.get_row(i)));

    return out;
}

static QTableWidget* create_table_widget(const QVector<QVector<std::string>>& vec, QWidget* root) {
    auto table_widget = new QTableWidget(root);

    int rows = vec.size();
    int cols = vec[0].size();

    table_widget->setRowCount(rows);
    table_widget->setColumnCount(cols);

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            auto item = new QTableWidgetItem(QString::fromStdString(vec[i][j]));
            table_widget->setItem(i, j, item);
        }
    }

    return table_widget;
}

int main(int argc, char* argv[]) {
    QApplication a(argc, argv);

    auto root = new QWidget();

    root->setWindowTitle("CSV Viewer");
    root->setGeometry(0, 0, 800, 600);

    csv::CSVTable table = csv::from_csv("./customers-100000.csv");

    QVector<QVector<std::string>> vec = create_2d_from_table(table);

    auto table_widget = create_table_widget(vec, root);

    auto* layout = new QVBoxLayout(root);
    layout->addWidget(table_widget);

    root->show();



    return QApplication::exec();
}
