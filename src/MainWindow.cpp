#include "MainWindow.h"
#include "AddEditorDialog.h"
#include <QVBoxLayout>
#include <QWidget>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    setWindowTitle("Unreal Launcher");
    setMinimumSize(400, 300);

    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(centralWidget);

    addEditorButton = new QPushButton("Add Editor", this);
    connect(addEditorButton, &QPushButton::clicked, this, &MainWindow::openAddEditorDialog);

    layout->addWidget(addEditorButton, 0, Qt::AlignCenter);
    
    setCentralWidget(centralWidget);
}

void MainWindow::openAddEditorDialog() {
    AddEditorDialog dialog(this);
    dialog.exec();
}
