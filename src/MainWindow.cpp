#include "MainWindow.h"
#include "AddEditorDialog.h"
#include <QVBoxLayout>
#include <QCoreApplication>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
  setWindowTitle("Unreal Launcher");
  
  // Start in 9:16 mode by default
  setMinimumSize(360, 640);
  resize(450, 800);

  stackedWidget = new QStackedWidget(this);
  setCentralWidget(stackedWidget);

  // Page 0: Main Page
  mainPage = new QWidget();
  QVBoxLayout *mainLayout = new QVBoxLayout(mainPage);
  mainLayout->setContentsMargins(16, 16, 16, 16);
  
  // Top-right layout for the toggle button
  QHBoxLayout *topLayout = new QHBoxLayout();
  topLayout->addStretch();
  layoutToggleButton = new QPushButton(mainPage);
  layoutToggleButton->setFixedSize(40, 40);
  
  // Load arrow icon
  QString appDir = QCoreApplication::applicationDirPath();
  QPixmap arPix;
  const QStringList arCandidates = {
      appDir + "/assets/AR.png",
      appDir + "/../assets/AR.png",
      QString("./assets/AR.png"),
      QString("assets/AR.png")
  };
  for (const QString &p : arCandidates) {
      if (arPix.load(p)) break;
  }
  
  if (!arPix.isNull()) {
      layoutToggleButton->setIcon(QIcon(arPix));
      layoutToggleButton->setIconSize(QSize(24, 24));
  } else {
      layoutToggleButton->setText("AR"); // Fallback
  }

  layoutToggleButton->setToolTip("Toggle Layout (9:16 / 16:9)");
  layoutToggleButton->setStyleSheet("background: #3a3a3a; color: white; border-radius: 4px; border: none;");
  connect(layoutToggleButton, &QPushButton::clicked, this, &MainWindow::toggleLayout);
  topLayout->addWidget(layoutToggleButton);
  mainLayout->addLayout(topLayout);

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
  viewEditorsPage->setColumnCount(2); // Match 9:16 initial state
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

void MainWindow::toggleLayout() {
    if (m_isVertical) {
        // Switch to 16:9
        setMinimumSize(800, 450);
        resize(1280, 720);
        viewEditorsPage->setColumnCount(5);
        m_isVertical = false;
    } else {
        // Switch to 9:16
        setMinimumSize(360, 640);
        resize(450, 800);
        viewEditorsPage->setColumnCount(2);
        m_isVertical = true;
    }
}
