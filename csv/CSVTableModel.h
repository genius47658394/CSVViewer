//
// Created by kucer on 30.09.2026.
//

#pragma once

#include <qabstractitemmodel.h>

#include "CSVTable.h"

// крч класс через который Qt будет обращаться к CSVTable
class CSVTableModel : public QAbstractTableModel {
    Q_OBJECT

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

    bool setData(const QModelIndex &index, const QVariant &value, int role) override {
        if (Qt::EditRole == role) {
            if (!checkIndex(index)) return false;

            table[index.column()][index.row()] = std::move(value.toString().toStdString());
            return true;
            // QString result;
            //
            // for (int col = 0; col < table.size(); ++col) {
            //     for (int row = 0; row < table.size_of_column(); ++row) {
            //         result += table[col][row] + ' ';
            //     }
            // }
            //
            // emit editCompleted(result);
            // return true;
        }

        return false;
    }

    [[nodiscard]] Qt::ItemFlags flags(const QModelIndex &index) const override {
        return Qt::ItemIsEditable | QAbstractTableModel::flags(index);
    }

    explicit CSVTableModel(std::reference_wrapper<csv::CSVTable> table) : table(table) {}

    ~CSVTableModel() override = default;

    signals:
        void editCompleted(const QString &);
};
