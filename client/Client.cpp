//
// Created by tabis on 11/10/2019.
//

#include "Client.hpp"

#include <QtCore/QDebug>
#include <server/server.hpp>

GuessGame::Client::Client(const QUrl &url, bool debug, QObject *parent) :
QObject(parent),
_url(url),
_debug(debug)
{
    if (_debug)
        qDebug() << "WebSocket server:" << url;
    connect(&_webSocket, &QWebSocket::connected, this, &Client::onConnected);
    connect(&_webSocket, &QWebSocket::disconnected, this, &Client::closed);
    _webSocket.open(QUrl(url));
}

void GuessGame::Client::onConnected()
{
    if (_debug)
        qDebug() << "WebSocket connected";
    connect(&_webSocket, &QWebSocket::textMessageReceived,
            this, &Client::onTextMessageReceived);
    _webSocket.sendTextMessage(QStringLiteral("Hello, world!"));
}

void GuessGame::Client::onTextMessageReceived(QString message)
{
    if (_debug)
        qDebug() << "Message received:" << message;
    _webSocket.close();
}