#pragma once

#include <QString>

class SpotifyOAuth {
public:
    SpotifyOAuth();

    QString authorizationUrl() const;
    bool exchangeCode(const QString& code);
    bool hasToken() const;

private:
    QString accessToken;
};
