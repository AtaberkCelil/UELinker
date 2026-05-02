#include "MainWindow.h"
#include "AddEditorDialog.h"
#include <QVBoxLayout>
#include <QCoreApplication>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
  setWindowTitle("Unreal Launcher");
  setMinimumSize(800, 600);
  resize(800, 600);

  stackedWidget = new QStackedWidget(this);
  setCentralWidget(stackedWidget);

  // Page 0: Main Page
  mainPage = new QWidget();
  QVBoxLayout *mainLayout = new QVBoxLayout(mainPage);
  mainLayout->setContentsMargins(16, 16, 16, 16);
  
  mainLayout->addStretch(); // Spacer above buttons

  viewEditorsButton = new QPushButton("View Editors", this);
  connect(viewEditorsButton, &QPushButton::clicked, this,
          &MainWindow::showViewEditorsPage);

  addEditorButton = new QPushButton("Add Editor", this);
  connect(addEditorButton, &QPushButton::clicked, this,
          &MainWindow::openAddEditorDialog);

  // Place buttons in the layout and make their sizes identical
  QSize btnSize(240, 56);
  viewEditorsButton->setFixedSize(btnSize);
  addEditorButton->setFixedSize(btnSize);

  mainLayout->addWidget(viewEditorsButton, 0, Qt::AlignCenter);
  mainLayout->addSpacing(12);
  mainLayout->addWidget(addEditorButton, 0, Qt::AlignCenter);

  mainLayout->addStretch(); // Spacer below buttons
  mainLayout->setAlignment(Qt::AlignCenter);

  stackedWidget->addWidget(mainPage);

  // Page 1: View Editors Page
  viewEditorsPage = new ViewEditorsPage(this);
  connect(viewEditorsPage, &ViewEditorsPage::backRequested, this,
          &MainWindow::showMainPage);
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

void MainWindow::showMainPage() { stackedWidget->setCurrentIndex(0); }
