#pragma once
#include <QWidget>
#include <QGridLayout>
#include <QLayout>

class ViewEditorsPage : public QWidget {
    Q_OBJECT
public:
    explicit ViewEditorsPage(QWidget* parent = nullptr);
    void loadEditors();
    void setColumnCount(int count);

signals:
    void backRequested();

private:
    QGridLayout* m_gridLayout;
    int m_columnCount = 5;
    void clearLayout(QLayout* layout);
};
