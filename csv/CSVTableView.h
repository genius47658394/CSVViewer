//
// Created by kucer on 03.10.2026.
//

#pragma once

#include <QHeaderView>
#include <qtableview.h>

class CSVTableView : public QTableView {
    Q_OBJECT
public:
    CSVTableView(QWidget* parent = nullptr) : QTableView(parent) {
        setGeometry(0, 0, 800, 600);
        setShowGrid(false);
        setAlternatingRowColors(true);
        verticalHeader()->setDefaultSectionSize(28);
    }
};