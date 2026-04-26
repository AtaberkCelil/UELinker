#pragma once
#include <QMainWindow>
#include <QPushButton>
#include <QStackedWidget>
#include "ViewEditorsPage.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

private slots:
    void openAddEditorDialog();
    void showViewEditorsPage();
    void showMainPage();

private:
    QStackedWidget *stackedWidget;
    QWidget *mainPage;
    QPushButton *addEditorButton;
    QPushButton *viewEditorsButton;
    ViewEditorsPage *viewEditorsPage;
};
