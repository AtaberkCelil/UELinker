#include "ViewEditorsPage.h"
#include "EditorCardWidget.h"
#include "ConfigManager.h"
#include <QVBoxLayout>
#include <QPushButton>
#include <QScrollArea>

ViewEditorsPage::ViewEditorsPage(QWidget* parent) : QWidget(parent) {
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);

    // Back Button
    QPushButton* backButton = new QPushButton(this);
    backButton->setIcon(QIcon(QPixmap("assets/AR.png")));
    backButton->setFixedSize(40, 40);
    backButton->setStyleSheet("border: none; background: transparent;");
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
    int row = 0;
    int col = 0;

    for (const auto& entry : entries) {
        EditorCardWidget* card = new EditorCardWidget(entry, this);
        connect(card, &EditorCardWidget::deletionRequested, this, &ViewEditorsPage::loadEditors);
        
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
