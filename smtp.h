/*
Copyright (c) 2012 Nicholas Smith

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.*/

#ifndef SMTP_H
#define SMTP_H

#include <QTcpSocket>
#include <QString>
#include <QTextStream>
#include <QDebug>
#include <QMessageBox>

class Smtp : public QObject
{
    Q_OBJECT


public:
    //Smtp( const QString &from, const QStringList &to,
      //  const QString &subject, const QString &body );
    Smtp(const QString &serverName, const QString &username, const QString &password, const QString &from, const QStringList &to, const QString &subject, const QString &body);
    ~Smtp();
    void initalise();

signals:
        void status( const QString &);
        void finished();

private slots:
        void stateChanged(QAbstractSocket::SocketState );
        void errorReceived(QAbstractSocket::SocketError );
        void disconnected();
        void connected();
        void readyRead();

private:
        QString message;
        QTextStream *t;
        QTcpSocket *socket;
        QString from;
        QString rcpt;
        QString response;
        QStringList recips;
        QString server;
        QString user;
        QString pass;
        int x;
        enum states{Auth,User,Pass,Rcpt,Mail,Data,Init,Body,Quit,Close};
        int state;
};

#endif
