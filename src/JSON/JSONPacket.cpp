//
// Created by tabis on 12/10/2019.
//

#include "JSONPacket.hpp"

QByteArray GuessGame::Data::JSONPacket::createJSONPacket(const QList<QList<std::string>> &object) const
{
    QJsonObject packet;
    QJsonDocument document;

    for (auto &it : object) {
        QString tmp(it.first().c_str());
        QJsonArray array;
        for (int i = 1; i < it.size(); ++i)
            array << it.at(i).c_str();
        packet.insert(tmp, array);
    }
    document.setObject(packet);
    return document.toJson();
}

QByteArray GuessGame::Data::JSONPacket::createJSONIdentificationPacket(const std::string &name) const
{
    QJsonObject packet;
    QJsonDocument document;

    packet.insert(CONNEXION_REQUEST, name.c_str());
    document.setObject(packet);
    return document.toJson();
}

QJsonObject GuessGame::Data::JSONPacket::UnpackToJson(const QByteArray &packet) const
{
    QJsonDocument document;

    return document.fromJson(packet).object();
}

QJsonObject GuessGame::Data::JSONPacket::getJSONFromFile(const std::string &name) const
{
    QString content;
    QFile file;
    QJsonObject object;

    file.setFileName({ name.c_str() });
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QJsonObject tmp;
        tmp.insert("error", "yes");
        return tmp;
    }
    content = file.readAll();
    file.close();
    object = QJsonDocument::fromJson(content.toUtf8()).object();
    object.insert("error", "none");
    return object;
}

void GuessGame::Data::JSONPacket::writeJSONToFile(const std::string &name, const QJsonObject &object) const
{
    QJsonDocument document(object);
    QFile file;

    file.setFileName({ name.c_str() });
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return;
    file.write(document.toJson());
}
