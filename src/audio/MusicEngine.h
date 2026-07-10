#pragma once

#include <QObject>
#include <QStringList>
#include <QMediaPlayer>
#include <QAudioOutput>

class MusicEngine : public QObject {
    Q_OBJECT

public:
    explicit MusicEngine(QObject* parent = nullptr);

    void setMusicFolder(const QString& folder);
    QStringList playlist() const;
    void playNext();
    void stop();
    void play();
    QString currentTrack() const;

private:
    QStringList tracks;
    QString current;
    QMediaPlayer* player;
    QAudioOutput* audioOutput;
};
