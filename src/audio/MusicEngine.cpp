#include "MusicEngine.h"
#include <QDir>

MusicEngine::MusicEngine(QObject* parent) : QObject(parent) {}

void MusicEngine::setMusicFolder(const QString& folder) {
    QDir dir(folder);
    QStringList filters{"*.mp3", "*.wav", "*.ogg", "*.flac"};
    tracks = dir.entryList(filters, QDir::Files);
}

QStringList MusicEngine::playlist() const {
    return tracks;
}

void MusicEngine::playNext() {
    if (!tracks.isEmpty())
        current = tracks.first();
}

void MusicEngine::stop() {
    current.clear();
}

QString MusicEngine::currentTrack() const {
    return current;
}
