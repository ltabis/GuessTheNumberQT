//
// Created by tabis on 12/10/2019.
//

#include "JSONPacket.hpp"

QJsonObject GuessGame::Data::JSONPacket::createJSONPacket(const QList<QList<std::string>> &object) const
{
    QJsonObject packet;

    for (auto &it : object) {
        QString tmp(it.first().c_str());
        QJsonArray array;
        for (unsigned int i = 1; i < it.size(); ++i)
            array << it.at(i).c_str();
        packet.insert(tmp, array);
    }
    qDebug() << packet;
    return packet;
}
