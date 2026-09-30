//
// Created by kucer on 30.09.2026.
//

#pragma once
#include <qabstractitemmodel.h>

#include "CSVTable.h"


// крч класс через который Qt будет обращаться к CSVTable
class CSVTableModel : public QAbstractTableModel {
    std::unique_ptr<csv::CSVTable> table = nullptr; // пока хз, будет она владеть таблицей или чисто ссылаться на нее
    // мб поменять надо будет на ссылку, и просто правильно удалять обЪект, но пока ptr

    static QVector<QString> from_vec_to_qvec(const std::vector<std::string>& vec) {
        QVector<QString> out{};

        for (const auto& el: vec)
            out.emplace_back(QString::fromStdString(el));

        return out;
    }
public:
    int columnCount(const QModelIndex &parent) const override {
        return static_cast<int>(table->size());
    }

    QVariant data(const QModelIndex &index, int role) const override {
        if (!index.isValid())
            return {};

        if (index.column() >= table->size() || index.row() >= table->size_of_column())
            return {};

        if (role == Qt::DisplayRole)
            return QString::fromStdString(table->get_row(index.row())[index.column()]);

        return {};
    }

    int rowCount(const QModelIndex &parent) const override {
        return static_cast<int>(table->size_of_column());
    }

    QVariant headerData(int section, Qt::Orientation orientation, int role) const override {
        if (role == Qt::DisplayRole && orientation == Qt::Horizontal && section < table->size()) {
            return QString::fromStdString(table->headers()[section]);
        }
        if (role == Qt::DisplayRole && orientation == Qt::Vertical && section < table->size_of_column()) {
            return QString::fromStdString(std::to_string(section + 1));
        }
        return {};
    }

    explicit CSVTableModel(const csv::CSVTable& table) : table(std::move(std::make_unique<csv::CSVTable>(table))) {}

    ~CSVTableModel() override = default;
};
