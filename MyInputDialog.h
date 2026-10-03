//
// Created by kucer on 03.10.2026.
//

#pragma once
#include <QInputDialog>

class MyInputDialog : public QInputDialog {
    Q_OBJECT

public:
    MyInputDialog(QWidget* parent = nullptr) : QInputDialog(parent) {
        setLabelText("Enter path to the file: ");
        setTextEchoMode(QLineEdit::Normal);
        setWindowTitle("Path");
        resize(200, 200);
    }
};
