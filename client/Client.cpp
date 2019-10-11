//
// Created by tabis on 11/10/2019.
//

#include "Client.hpp"

#include <QtCore/QDebug>

GuessGame::Client::Client(const QUrl &url, bool debug, QObject *parent) :
QObject(parent),
_url(url),
_debug(debug)
{
    if (_debug)
        qDebug() << "[Client] WebSocket server:" << url;
    connect(&_webSocket, &QWebSocket::connected, this, &Client::onConnected);
    connect(&_webSocket, &QWebSocket::disconnected, this, &Client::closed);
    _webSocket.open(QUrl(url));
}

void GuessGame::Client::onConnected()
{
    if (_debug)
        qDebug() << "[Client] WebSocket connected";
    connect(&_webSocket, &QWebSocket::textMessageReceived,
            this, &Client::onTextMessageReceived);
    _webSocket.sendTextMessage(QStringLiteral("connexion")); // A mettre en JSON
}

void GuessGame::Client::onTextMessageReceived(QString message)
{
    if (_debug)
        qDebug() << "[Client] Message received:" << message;
    _webSocket.close();
}