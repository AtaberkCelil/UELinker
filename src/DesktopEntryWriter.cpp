#include "DesktopEntryWriter.h"
#include <QDir>
#include <QFile>
#include <QTextStream>
#include <QStandardPaths>

bool DesktopEntryWriter::write(const QString& name, const QString& engineRootPath) {
    QString desktopPath = QStandardPaths::writableLocation(QStandardPaths::ApplicationsLocation);
    QDir dir(desktopPath);
    if (!dir.exists()) {
        if (!dir.mkpath(".")) {
            return false;
        }
    }

    QString fileName = QString("unreal-%1.desktop").arg(name.toLower().replace(" ", "-"));
    QString filePath = dir.absoluteFilePath(fileName);

    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        return false;
    }

    QTextStream out(&file);
    out << "[Desktop Entry]\n";
    out << "Type=Application\n";
    out << "Name=" << name << "\n";
    out << "Exec=" << engineRootPath << "/Engine/Binaries/Linux/UnrealEditor\n";
    out << "Icon=" << QDir::homePath() << "/.local/share/icons/UE.png\n";
    out << "Terminal=false\n";
    out << "Categories=Development;\n";

    file.close();
    file.setPermissions(QFile::ReadOwner | QFile::WriteOwner | QFile::ReadGroup | QFile::ReadOther); // 0644

    return true;
}
