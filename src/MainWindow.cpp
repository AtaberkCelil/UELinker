#include "MainWindow.h"
#include "AddEditorDialog.h"
#include <QVBoxLayout>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    setWindowTitle("Unreal Launcher");
    setMinimumSize(800, 600); // Larger to accommodate the 5-column grid comfortably

    stackedWidget = new QStackedWidget(this);
    setCentralWidget(stackedWidget);

    // Page 0: Main Page
    mainPage = new QWidget();
    QVBoxLayout *mainLayout = new QVBoxLayout(mainPage);

    addEditorButton = new QPushButton("Add Editor", this);
    connect(addEditorButton, &QPushButton::clicked, this, &MainWindow::openAddEditorDialog);

    viewEditorsButton = new QPushButton("View Editors", this);
    connect(viewEditorsButton, &QPushButton::clicked, this, &MainWindow::showViewEditorsPage);

    mainLayout->addWidget(addEditorButton, 0, Qt::AlignCenter);
    mainLayout->addWidget(viewEditorsButton, 0, Qt::AlignCenter);
    mainLayout->setAlignment(Qt::AlignCenter);

    stackedWidget->addWidget(mainPage);

    // Page 1: View Editors Page
    viewEditorsPage = new ViewEditorsPage(this);
    connect(viewEditorsPage, &ViewEditorsPage::backRequested, this, &MainWindow::showMainPage);
    stackedWidget->addWidget(viewEditorsPage);
}

void MainWindow::openAddEditorDialog() {
    AddEditorDialog dialog(this);
    dialog.exec();
}

void MainWindow::showViewEditorsPage() {
    viewEditorsPage->loadEditors();
    stackedWidget->setCurrentIndex(1);
}

void MainWindow::showMainPage() {
    stackedWidget->setCurrentIndex(0);
}
