//
// Created by kucer on 03.10.2026.
//

#pragma once

#include <qwidget.h>
#include <QInputDialog>


class Window : public QWidget {
    Q_OBJECT

public:
    Window(QWidget* parent = nullptr) : QWidget(parent) {
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
