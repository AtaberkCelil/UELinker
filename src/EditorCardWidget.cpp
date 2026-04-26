#include "EditorCardWidget.h"
#include "ConfigManager.h"
#include "DesktopEntryWriter.h"
#include <QHBoxLayout>
#include <QMenu>
#include <QMessageBox>
#include <QDir>
#include <QPixmap>

EditorCardWidget::EditorCardWidget(const EditorEntry& entry, QWidget* parent)
    : QWidget(parent), m_entry(entry) {
    
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    QHBoxLayout* layout = new QHBoxLayout(this);
    layout->setContentsMargins(8, 8, 8, 8);

    m_iconLabel = new QLabel(this);
    m_iconLabel->setPixmap(QPixmap("assets/UE.png").scaled(32, 32, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    m_iconLabel->setFixedSize(32, 32);

    m_nameLabel = new QLabel(entry.name, this);
    m_nameLabel->setStyleSheet("color: #ffffff;");

    m_deleteButton = new QPushButton(this);
    m_deleteButton->setIcon(QIcon(QPixmap("assets/TRA.png")));
    m_deleteButton->setFixedSize(32, 32);
    m_deleteButton->setCursor(Qt::PointingHandCursor);
    m_deleteButton->setStyleSheet("border: none; background: transparent;");

    layout->addWidget(m_iconLabel);
    layout->addWidget(m_nameLabel, 1); // Expand to fill available space
    layout->addWidget(m_deleteButton);

    connect(m_deleteButton, &QPushButton::clicked, this, &EditorCardWidget::showDeleteMenu);
}

void EditorCardWidget::showDeleteMenu() {
    QMenu menu(this);
    
    QAction* deleteAppAction = menu.addAction("Delete App");
    QAction* deleteFilesAction = menu.addAction("Delete Files");

    QAction* selectedAction = menu.exec(m_deleteButton->mapToGlobal(QPoint(0, m_deleteButton->height())));

    if (selectedAction == deleteAppAction) {
        DesktopEntryWriter::remove(m_entry.name);
        ConfigManager::removeEntry(m_entry.name);
        emit deletionRequested();
    } else if (selectedAction == deleteFilesAction) {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::warning(this, "Warning",
            QString("This will permanently delete all engine source files at:\n%1\nThis action cannot be undone.").arg(m_entry.path),
            QMessageBox::Ok | QMessageBox::Cancel);
        
        if (reply == QMessageBox::Ok) {
            DesktopEntryWriter::remove(m_entry.name);
            QDir dir(m_entry.path);
            if (dir.exists()) {
                dir.removeRecursively();
            }
            ConfigManager::removeEntry(m_entry.name);
            emit deletionRequested();
        }
    }
}
