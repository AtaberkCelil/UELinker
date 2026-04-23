#pragma once
#include <QString>

class DesktopEntryWriter {
public:
    static bool write(const QString& name, const QString& engineRootPath);
};
