#pragma once

#include <QString>

class SettingsManager {
public:
    static QString musicDirectory();
    static void setMusicDirectory(const QString& path);
};
