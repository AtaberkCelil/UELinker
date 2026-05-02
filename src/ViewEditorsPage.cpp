#include "ViewEditorsPage.h"
#include "EditorCardWidget.h"
#include "ConfigManager.h"
#include <QVBoxLayout>
#include <QPushButton>
#include <QScrollArea>
#include <QCoreApplication>
#include <QPixmap>
#include <QIcon>
#include <algorithm>

ViewEditorsPage::ViewEditorsPage(QWidget* parent) : QWidget(parent) {
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);

    // Back Button
    QPushButton* backButton = new QPushButton(this);
    // Try to load icon relative to application dir first, then fallback to assets/ path
    QString appDir = QCoreApplication::applicationDirPath();
    QPixmap pixmap;
    bool iconLoaded = false;
    if (pixmap.load(appDir + "/assets/AR.png")) {
        backButton->setIcon(QIcon(pixmap));
        iconLoaded = true;
    } else if (pixmap.load("assets/AR.png")) {
        backButton->setIcon(QIcon(pixmap));
        iconLoaded = true;
    }

    backButton->setFixedSize(44, 44);
    // Make icon / text clearly visible
    if (iconLoaded) {
        backButton->setIconSize(QSize(28, 28));
        backButton->setStyleSheet("border: none; background: transparent; color: #ffffff; padding-left: 6px;");
    } else {
        // Fallback to a visible arrow text if icon not found
        backButton->setText("\u2190");
        backButton->setStyleSheet("color: #ffffff; font-size: 22px; font-weight: bold; border: none; background: transparent; padding-left: 6px;");
    }
    connect(backButton, &QPushButton::clicked, this, &ViewEditorsPage::backRequested);

    QHBoxLayout* topLayout = new QHBoxLayout();
    topLayout->addWidget(backButton);
    topLayout->addStretch();
    topLayout->setContentsMargins(8, 8, 8, 8);
    mainLayout->addLayout(topLayout);

    // Scroll Area
    QScrollArea* scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    scrollArea->setFrameShape(QFrame::NoFrame);

    QWidget* scrollWidget = new QWidget();
    m_gridLayout = new QGridLayout(scrollWidget);
    m_gridLayout->setContentsMargins(16, 16, 16, 16);
    m_gridLayout->setSpacing(16);
    
    // Set 5 columns stretch
    for (int i = 0; i < 5; ++i) {
        m_gridLayout->setColumnStretch(i, 1);
    }

    scrollWidget->setLayout(m_gridLayout);
    scrollArea->setWidget(scrollWidget);

    // Keep main background black; scroll area will show cards on dark grey boxes
    mainLayout->addWidget(scrollArea);
}

void ViewEditorsPage::clearLayout(QLayout* layout) {
    QLayoutItem* item;
    while ((item = layout->takeAt(0)) != nullptr) {
        if (item->widget()) {
            item->widget()->deleteLater();
        }
        delete item;
    }
}

void ViewEditorsPage::loadEditors() {
    clearLayout(m_gridLayout);

    QList<EditorEntry> entries = ConfigManager::loadEntries();
    
    // Sort favorites to the top
    std::sort(entries.begin(), entries.end(), [](const EditorEntry& a, const EditorEntry& b) {
        if (a.isFavorite != b.isFavorite) {
            return a.isFavorite > b.isFavorite;
        }
        return a.name < b.name;
    });

    int row = 0;
    int col = 0;

    for (const auto& entry : entries) {
        // Ensure card widgets are parented to the scroll area's widget (the grid's parent)
        QWidget* gridParent = m_gridLayout->parentWidget();
        EditorCardWidget* card = new EditorCardWidget(entry, gridParent);
        connect(card, &EditorCardWidget::deletionRequested, this, &ViewEditorsPage::loadEditors);
        connect(card, &EditorCardWidget::favoriteChanged, this, &ViewEditorsPage::loadEditors);
        
        m_gridLayout->addWidget(card, row, col);
        col++;
        if (col >= 5) {
            col = 0;
            row++;
        }
    }
    
    // We add a stretchable spacer at the end to push cards to the top
    QSpacerItem* spacer = new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding);
    m_gridLayout->addItem(spacer, row + 1, 0, 1, 5);
}
