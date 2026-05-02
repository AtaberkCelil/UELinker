#pragma once
#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QToolButton>
#include "EditorEntry.h"
class QLineEdit;

class EditorCardWidget : public QWidget {
    Q_OBJECT
public:
    explicit EditorCardWidget(const EditorEntry& entry, QWidget* parent = nullptr);

signals:
    void deletionRequested();
    void favoriteChanged();
    void nameChanged(const QString& oldName, const EditorEntry& newEntry);

private slots:
    void showDeleteMenu();
    void toggleFavorite();
    void startEditingName();
    void commitNameEdit();

private:
    EditorEntry m_entry;
    QLabel* m_nameLabel;
    QLineEdit* m_nameEdit = nullptr;
    QToolButton* m_deleteButton;
    QToolButton* m_favoriteButton;
    QPixmap m_emptyPix;
    QPixmap m_fullPix;
    void updateFavoriteIcon(bool hovered = false);
protected:
    bool eventFilter(QObject* watched, QEvent* event) override;
};
