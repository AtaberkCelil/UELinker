#include "ConfigManager.h"
#include <QDir>
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QStandardPaths>
#include <QTextStream>

namespace {
    QString getConfigPath() {
        QString configDir = QDir::homePath() + "/.config/UnrealLauncher";
        QDir dir(configDir);
        if (!dir.exists()) {
            dir.mkpath(".");
        }
        return configDir + "/editors.json";
    }

    QList<EditorEntry> parseDesktopFiles() {
        QList<EditorEntry> entries;
        QString desktopPath = QStandardPaths::writableLocation(QStandardPaths::ApplicationsLocation);
        QDir dir(desktopPath);
        if (!dir.exists()) return entries;

        QStringList filters;
        filters << "unreal-*.desktop";
        QFileInfoList files = dir.entryInfoList(filters, QDir::Files);

        for (const QFileInfo& fileInfo : files) {
            QFile file(fileInfo.absoluteFilePath());
            if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
                QTextStream in(&file);
                QString name;
                QString path;
                while (!in.atEnd()) {
                    QString line = in.readLine();
                    if (line.startsWith("Name=")) {
                        name = line.mid(5).trimmed();
                    } else if (line.startsWith("Exec=")) {
                        QString exec = line.mid(5).trimmed();
                        if (exec.endsWith("/Engine/Binaries/Linux/UnrealEditor")) {
                            path = exec.left(exec.length() - QString("/Engine/Binaries/Linux/UnrealEditor").length());
                        } else {
                            path = exec;
                        }
                    }
                }
                if (!name.isEmpty() && !path.isEmpty()) {
                    entries.append({name, path});
                }
            }
        }
        return entries;
    }

    void writeJson(const QList<EditorEntry>& entries) {
        QJsonArray array;
        for (const auto& entry : entries) {
            QJsonObject obj;
            obj["name"] = entry.name;
            obj["path"] = entry.path;
            obj["isFavorite"] = entry.isFavorite;
            array.append(obj);
        }
        QJsonDocument doc(array);
        QFile file(getConfigPath());
        if (file.open(QIODevice::WriteOnly)) {
            file.write(doc.toJson());
        }
    }
}

QList<EditorEntry> ConfigManager::loadEntries() {
    QFile file(getConfigPath());
    if (!file.exists()) {
        QList<EditorEntry> entries = parseDesktopFiles();
        writeJson(entries);
        return entries;
    }

    if (!file.open(QIODevice::ReadOnly)) {
        return QList<EditorEntry>();
    }

    QByteArray data = file.readAll();
    QJsonDocument doc = QJsonDocument::fromJson(data);
    QList<EditorEntry> entries;
    if (doc.isArray()) {
        QJsonArray array = doc.array();
        for (int i = 0; i < array.size(); ++i) {
            QJsonObject obj = array[i].toObject();
            EditorEntry entry;
            entry.name = obj["name"].toString();
            entry.path = obj["path"].toString();
            entry.isFavorite = obj["isFavorite"].toBool(false);
            entries.append(entry);
        }
    }
    return entries;
}

void ConfigManager::saveEntry(const EditorEntry& entry) {
    QList<EditorEntry> entries = loadEntries();
    bool found = false;
    for (int i = 0; i < entries.size(); ++i) {
        if (entries[i].name == entry.name) {
            entries[i].path = entry.path;
            found = true;
            break;
        }
    }
    if (!found) {
        entries.append(entry);
    }
    writeJson(entries);
}

void ConfigManager::removeEntry(const QString& name) {
    QList<EditorEntry> entries = loadEntries();
    for (int i = 0; i < entries.size(); ++i) {
        if (entries[i].name == name) {
            entries.removeAt(i);
            break;
        }
    }
    writeJson(entries);
}
