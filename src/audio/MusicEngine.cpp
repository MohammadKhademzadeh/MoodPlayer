#include "MusicEngine.h"
#include <QDir>
#include <QUrl>

MusicEngine::MusicEngine(QObject* parent) : QObject(parent)
{
    player = new QMediaPlayer(this);
    audioOutput = new QAudioOutput(this);
    player->setAudioOutput(audioOutput);
}

void MusicEngine::setMusicFolder(const QString& folder) {
    QDir dir(folder);
    QStringList filters{"*.mp3", "*.wav", "*.ogg", "*.flac"};
    tracks = dir.entryList(filters, QDir::Files);
}

QStringList MusicEngine::playlist() const {
    return tracks;
}

void MusicEngine::playNext() {
    if (!tracks.isEmpty()) {
        current = tracks.first();
        player->setSource(QUrl::fromLocalFile(current));
        player->play();
    }
}

void MusicEngine::play() {
    player->play();
}

void MusicEngine::stop() {
    player->stop();
    current.clear();
}

QString MusicEngine::currentTrack() const {
    return current;
}
