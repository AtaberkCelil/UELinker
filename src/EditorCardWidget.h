#pragma once
#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include "EditorEntry.h"

class EditorCardWidget : public QWidget {
    Q_OBJECT
public:
    explicit EditorCardWidget(const EditorEntry& entry, QWidget* parent = nullptr);

signals:
    void deletionRequested();

private slots:
    void showDeleteMenu();

private:
    EditorEntry m_entry;
    QLabel* m_iconLabel;
    QLabel* m_nameLabel;
    QPushButton* m_deleteButton;
};
