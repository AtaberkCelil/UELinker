#pragma once
#include <QList>
#include "EditorEntry.h"

class ConfigManager {
public:
    static QList<EditorEntry> loadEntries();
    static void saveEntry(const EditorEntry& entry);
    static void removeEntry(const QString& name);
};
