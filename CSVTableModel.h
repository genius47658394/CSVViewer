//
// Created by kucer on 30.09.2026.
//

#pragma once
#include <qabstractitemmodel.h>

#include "CSVTable.h"


// крч класс через который Qt будет обращаться к CSVTable
class CSVTableModel : public QAbstractTableModel {
    csv::CSVTable& table;

public:
    [[nodiscard]] int columnCount(const QModelIndex &parent) const override {
        return static_cast<int>(table.size());
    }

    [[nodiscard]] QVariant data(const QModelIndex &index, int role) const override {
        if (!index.isValid())
            return {};

        if (index.column() >= table.size() || index.row() >= table.size_of_column())
            return {};

        if (role == Qt::DisplayRole)
            return QString::fromStdString(table[index.column()][index.row()]);

        return {};
    }

    [[nodiscard]] int rowCount(const QModelIndex &parent) const override {
        return static_cast<int>(table.size_of_column());
    }

    [[nodiscard]] QVariant headerData(int section, Qt::Orientation orientation, int role) const override {
        if (role == Qt::DisplayRole && orientation == Qt::Horizontal && section < table.size())
            return QString::fromStdString(table[section].header());

        if (role == Qt::DisplayRole && orientation == Qt::Vertical && section < table.size_of_column())
            return QString::fromStdString(std::to_string(section + 1));

        return {};
    }

    explicit CSVTableModel(csv::CSVTable& table) : table(table) {}

    ~CSVTableModel() override = default;
};
