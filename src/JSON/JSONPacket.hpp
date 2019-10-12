//
// Created by tabis on 12/10/2019.
//

#pragma once

#include <QtCore>

#include <iostream>

#define CONNEXION_REQUEST   "Handshake"
#define TURN_MESSAGE        "turn"
#define ANSWER_MESSAGE      "answer"
#define NAME_MESSAGE        "name"
#define CONFIRM_CONNECTION  "Connection"
#define INFO_MESSAGE        "info"
#define END_MESSAGE         "end"

namespace GuessGame {
    namespace Data {
        class JSONPacket {
        public:
            QByteArray createJSONPacket(const QList<QList<std::string>> &object) const;
            QByteArray createJSONIdentificationPacket(const std::string &name) const;
            QJsonObject UnpackToJson(const QByteArray &packet) const;
            QJsonObject createPlayerConfig(const std::string &name,
                    unsigned int triesLeft,
                    int status,
                    std::pair<unsigned int, unsigned int> &bounds,
                    const QDate &startDate) const;

            QJsonArray getJSONFromFile(const std::string &name) const;
            void writeJSONToFile(const std::string &name, const QJsonArray &object) const;
        };
    }
}