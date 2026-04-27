#include "EditorCardWidget.h"
#include "ConfigManager.h"
#include "DesktopEntryWriter.h"
#include <QHBoxLayout>
#include <QMenu>
#include <QMessageBox>
#include <QDir>
#include <QPixmap>
#include <QCoreApplication>
#include <QIcon>
#include <QPainter>

EditorCardWidget::EditorCardWidget(const EditorEntry& entry, QWidget* parent)
    : QWidget(parent), m_entry(entry) {
    
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    QHBoxLayout* layout = new QHBoxLayout(this);
    layout->setContentsMargins(8, 8, 8, 8);

    QString appDir = QCoreApplication::applicationDirPath();

    // Create an inner content frame so the card visuals and contents are together
    QWidget* content = new QWidget(this);
    content->setStyleSheet("background: #2b2b2b; border: 1px solid #3a3a3a; border-radius: 8px;");
    QHBoxLayout* contentLayout = new QHBoxLayout(content);
    contentLayout->setContentsMargins(12, 8, 12, 8);

    m_nameLabel = new QLabel(entry.name, content);
    m_nameLabel->setStyleSheet("color: #ffffff; padding-left: 8px;");

    // Delete button inside the card (on the right)
    // Use QToolButton which is more suitable for icon-only buttons inside compact widgets
    m_deleteButton = new QToolButton(content);
    QPixmap delPix;
    // Try multiple candidate paths so icon loads regardless of working dir or binary location
    const QStringList candidates = {
        appDir + "/assets/TRA.png",
        appDir + "/../assets/TRA.png",
        QString("./assets/TRA.png"),
        QString("assets/TRA.png"),
        QString("build/assets/TRA.png")
    };
    for (const QString &p : candidates) {
        if (delPix.load(p)) break;
    }
    if (!delPix.isNull()) {
        m_deleteButton->setIcon(QIcon(delPix));
        m_deleteButton->setIconSize(QSize(20, 20));
    } else {
        // Create a simple fallback trash icon (white) so the button is visibly an icon
        QPixmap fallback(20,20);
        fallback.fill(Qt::transparent);
        QPainter p(&fallback);
        p.setRenderHint(QPainter::Antialiasing);
        p.setPen(Qt::NoPen);
        p.setBrush(QBrush(Qt::white));
        // Draw a simple bin: rectangle body and small lid
        p.drawRect(5,7,10,10);
        p.drawRect(4,5,12,2);
        p.end();
        m_deleteButton->setIcon(QIcon(fallback));
        m_deleteButton->setIconSize(QSize(20,20));
    }
    m_deleteButton->setFixedSize(40, 40);
    m_deleteButton->setCursor(Qt::PointingHandCursor);
    m_deleteButton->setStyleSheet("border: none; background: transparent; color: #ffffff;");

    contentLayout->addWidget(m_nameLabel, 1);
    contentLayout->addWidget(m_deleteButton, 0, Qt::AlignRight | Qt::AlignVCenter);
    layout->addWidget(content);

    // Make the card a bit wider and taller so delete button is clearly inside the card
    setMinimumWidth(260);
    setMinimumHeight(56);
    this->setContentsMargins(0,0,0,0);

    // Debug: log whether TRA.png loaded
#ifdef QT_DEBUG
    if (!delPix.isNull()) {
        qDebug("EditorCardWidget: loaded TRA.png icon");
    } else {
        qDebug("EditorCardWidget: TRA.png not loaded, using text fallback");
    }
#endif

    connect(m_deleteButton, &QToolButton::clicked, this, &EditorCardWidget::showDeleteMenu);
}

void EditorCardWidget::showDeleteMenu() {
    QMenu menu(this);
    
    QAction* deleteAppAction = menu.addAction("Delete App");
    QAction* deleteFilesAction = menu.addAction("Delete Files");

    // Use cursor position for menu placement to ensure it appears where the user clicked
    QAction* selectedAction = menu.exec(QCursor::pos());

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
