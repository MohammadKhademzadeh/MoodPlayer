#include "SpotifyConnector.h"

SpotifyConnector::SpotifyConnector() : connected(false) {}

bool SpotifyConnector::isConnected() const {
    return connected;
}

QString SpotifyConnector::currentTrack() const {
    if (!connected)
        return "No Spotify connection";

    return "Unknown track";
}

void SpotifyConnector::connectAccount() {
    // OAuth integration will be implemented here.
    // User authentication and Spotify Web API access will be added later.
    connected = true;
}
