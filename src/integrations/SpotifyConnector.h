#pragma once

#include <QString>

class SpotifyConnector {
public:
    SpotifyConnector();

    bool isConnected() const;
    QString currentTrack() const;
    void connectAccount();

private:
    bool connected;
};
