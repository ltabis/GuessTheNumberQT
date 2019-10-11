//
// Created by tabis on 11/10/2019.
//

#pragma once

#include <QtCore>
#include <QtWebSockets/QWebSocket>

namespace GuessGame {
    class Client : public QObject {
    Q_OBJECT
    public:
            explicit Client(const QUrl &url, bool debug = false, QObject *parent = nullptr);

    Q_SIGNALS:
            void closed();

    private Q_SLOTS:
            void onConnected();
            void onTextMessageReceived(QString message);

    private:
            QWebSocket _webSocket;
            QUrl _url;
            bool _debug;
        };
}