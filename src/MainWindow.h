#pragma once
#include <QMainWindow>
#include <QPushButton>

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

private slots:
    void openAddEditorDialog();

private:
    QPushButton *addEditorButton;
};
