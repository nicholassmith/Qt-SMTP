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
