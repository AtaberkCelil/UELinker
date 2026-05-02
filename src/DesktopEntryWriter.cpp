#include "DesktopEntryWriter.h"
#include <QDir>
#include <QFile>
#include <QTextStream>
#include <QStandardPaths>

bool DesktopEntryWriter::write(const QString& name, const QString& engineRootPath) {
    // Prefer the standard applications location but fall back to the user's
    // ~/.local/share/applications if that's empty or not writable. Some systems
    // may return a system path (eg. /usr/share/applications) which isn't
    // writable by the user — ensure we use a per-user location.
    QString desktopPath = QStandardPaths::writableLocation(QStandardPaths::ApplicationsLocation);
    if (desktopPath.isEmpty()) {
        desktopPath = QDir::homePath() + "/.local/share/applications";
    }
    QDir dir(desktopPath);
    if (!dir.exists()) {
        // Try to create the directory; if that fails, fall back to ~/.local/share/applications
        if (!dir.mkpath(".")) {
            desktopPath = QDir::homePath() + "/.local/share/applications";
            dir.setPath(desktopPath);
            if (!dir.exists()) {
                if (!dir.mkpath(".")) {
                    return false;
                }
            }
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
    // Only include Icon if the target file exists; some engines may not have
    // the suggested icon and some DEs are picky about invalid icons.
    QString iconPath = engineRootPath + "/Engine/Content/Slate/Testing/PerfCapture.png";
    if (QFile::exists(iconPath)) {
        out << "Icon=" << iconPath << "\n";
    }
    out << "Terminal=false\n";
    out << "Categories=Development;\n";

    file.close();
    // Mark the .desktop file executable so desktop environments treat it as a valid
    // launcher file. Some environments require the executable bit to show the
    // application in menus or treat the file as trusted.
    QFile::Permissions perms = QFile::ReadOwner | QFile::WriteOwner | QFile::ExeOwner |
                               QFile::ReadGroup | QFile::ExeGroup |
                               QFile::ReadOther | QFile::ExeOther; // 0755
    file.setPermissions(perms);

    return true;
}

void DesktopEntryWriter::remove(const QString& name) {
    QString desktopPath = QStandardPaths::writableLocation(QStandardPaths::ApplicationsLocation);
    if (desktopPath.isEmpty()) desktopPath = QDir::homePath() + "/.local/share/applications";
    QDir dir(desktopPath);
    QString fileName = QString("unreal-%1.desktop").arg(name.toLower().replace(" ", "-"));
    QString filePath = dir.absoluteFilePath(fileName);
    
    QFile file(filePath);
    if (file.exists()) {
        file.remove();
    }
}
