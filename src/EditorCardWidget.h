#pragma once
#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QToolButton>
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
    QLabel* m_nameLabel;
    QToolButton* m_deleteButton;
};
