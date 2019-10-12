//
// Created by tabis on 12/10/2019.
//

#pragma once

#include <QtCore>

namespace GuessGame {
    namespace Data {
        class JSONPacket {
        public:
            QJsonObject createJSONPacket(const QList<QList<std::string>> &object) const;
        private:
            QJsonObject _json;
        };
    }
}