#pragma once
#include <QWidget>
#include <QGridLayout>
#include <QLayout>

class ViewEditorsPage : public QWidget {
    Q_OBJECT
public:
    explicit ViewEditorsPage(QWidget* parent = nullptr);
    void loadEditors();

signals:
    void backRequested();

private:
    QGridLayout* m_gridLayout;
    void clearLayout(QLayout* layout);
};
