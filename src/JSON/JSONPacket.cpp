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

QJsonArray GuessGame::Data::JSONPacket::getJSONFromFile(const std::string &name) const
{
    QString content;
    QFile file;
    QJsonArray object;

    file.setFileName({ name.c_str() });
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return object;
    content = file.readAll();
    file.close();
    object = QJsonDocument::fromJson(content.toUtf8()).array();
    return object;
}

void GuessGame::Data::JSONPacket::writeJSONToFile(const std::string &name, const QJsonArray &object) const
{
    QJsonDocument document(object);
    QFile file;

    file.setFileName({ name.c_str() });
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        std::cout << "Error" << std::endl;
        return;
    }
    file.write(document.toJson());
}

QJsonObject GuessGame::Data::JSONPacket::createPlayerConfig(
        const std::string &name,
        unsigned int triesLeft,
        int status,
        std::pair<unsigned int, unsigned int> &bounds,
        const QDate &startDate,
        unsigned int unlimited) const
{
    QDate endDate = QDate::currentDate();
    QJsonObject newSave;
    std::vector<std::string> statuses = {"abandonned", "lost", "won"};
    unsigned int interval = bounds.second - bounds.first;

    newSave.insert("name", name.c_str());
    if (unlimited)
        newSave.insert("tries", std::to_string(unlimited - triesLeft).c_str());
    else
        newSave.insert("tries", "unlimited");
    if (status != 1 && unlimited)
        newSave.insert("score", std::to_string(interval * triesLeft).c_str());
    else
        newSave.insert("score", "0");
    newSave.insert("startDate", startDate.toString());
    newSave.insert("endDate", endDate.toString());
    newSave.insert("game status", statuses[status].c_str());
    return newSave;
}
