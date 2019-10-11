#pragma once

#include <QMainWindow>
#include <QtWebSockets/QWebSocket>

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow {
Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

    ~MainWindow();

private slots:
    void on_sendButton_clicked();

private:
    Ui::MainWindow *_ui;
    QWebSocket _webSocket;
};