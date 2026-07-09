#include "SettingsManager.h"
#include <QSettings>

QString SettingsManager::musicDirectory() {
    QSettings settings("MoodPlayer", "MoodPlayer");
    return settings.value("music/directory", "").toString();
}

void SettingsManager::setMusicDirectory(const QString& path) {
    QSettings settings("MoodPlayer", "MoodPlayer");
    settings.setValue("music/directory", path);
}
