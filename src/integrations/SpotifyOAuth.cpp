#include "SpotifyOAuth.h"

SpotifyOAuth::SpotifyOAuth() {}

QString SpotifyOAuth::authorizationUrl() const {
    return "https://accounts.spotify.com/authorize";
}

bool SpotifyOAuth::exchangeCode(const QString& code) {
    if (code.isEmpty())
        return false;

    // Token exchange with Spotify API will be implemented here.
    accessToken = "pending-token";
    return true;
}

bool SpotifyOAuth::hasToken() const {
    return !accessToken.isEmpty();
}
