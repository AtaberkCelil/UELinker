#pragma once
#include "ViewEditorsPage.h"
#include <QMainWindow>
#include <QPushButton>
#include <QStackedWidget>


class MainWindow : public QMainWindow {
  Q_OBJECT

public:
  MainWindow(QWidget *parent = nullptr);

private slots:
  void openAddEditorDialog();
  void showViewEditorsPage();
  void showMainPage();
  void toggleLayout();

private:
  QStackedWidget *stackedWidget;
  QWidget *mainPage;
  QPushButton *addEditorButton;
  QPushButton *viewEditorsButton;
  ViewEditorsPage *viewEditorsPage;
  QPushButton *layoutToggleButton;
  bool m_isVertical = true;
};
