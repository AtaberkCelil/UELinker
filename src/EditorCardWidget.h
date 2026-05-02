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
    void favoriteChanged();

private slots:
    void showDeleteMenu();
    void toggleFavorite();

private:
    EditorEntry m_entry;
    QLabel* m_nameLabel;
    QToolButton* m_deleteButton;
    QToolButton* m_favoriteButton;
    QPixmap m_emptyPix;
    QPixmap m_fullPix;
    void updateFavoriteIcon(bool hovered = false);
protected:
    bool eventFilter(QObject* watched, QEvent* event) override;
};
