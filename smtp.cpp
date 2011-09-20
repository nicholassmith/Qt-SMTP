#include "smtp.h"

Smtp::Smtp(const QString &from, const QStringList &to, const QString &subject, const QString &body)
{
    server = ""; //your server name
    user = ""; //your SMTP username
    pass = ""; //your SMTP password

    user = user.toLocal8Bit().toBase64();
    pass = pass.toLocal8Bit().toBase64();

    x = 1;
    int recipCount = 0;
    socket = new QTcpSocket(this);

    connect(socket, SIGNAL(readyRead()), this, SLOT(readyRead()));
    connect(socket, SIGNAL(connected()), this, SLOT(connected() ) );
    connect(socket, SIGNAL(error(QAbstractSocket::SocketError)), this,
             SLOT(errorReceived(QAbstractSocket::SocketError)));
    connect(socket, SIGNAL(stateChanged( QAbstractSocket::SocketState)), this,
             SLOT(stateChanged(QAbstractSocket::SocketState)));
    connect(socket, SIGNAL(disconnected()), this,
            SLOT(disconnected()));

    qDebug() << "Signals created";

    for(recipCount = 0; recipCount < to.count(); recipCount++)
    {
        message.append("To: " + to.at(recipCount) + "\n");
    }
    message.append("From: " + from + "\n");
    message.append("Subject: " + subject + "\n");
    message.append("Mime-Version: 1.0;\n");
    message.append("Content-Type: text/html; charset=\"utf8\";\n");
    message.append("Content-Transfer-Encoding: 8bit;\n");
    message.append("\n");
    qDebug() << "body is: " << body;
    message.append(body);
    message.replace(QString::fromLatin1( "\n" ), QString::fromLatin1( "\r\n"));
    message.replace(QString::fromLatin1( "\r\n.\r\n" ), QString::fromLatin1( "\r\n..\r\n" ) );

    this->from = from;
    rcpt = to.at(0);
    recips = to;
    state = Init;
    socket->connectToHost(server, 25);
    if(socket->waitForConnected(30000))
    {
        qDebug("connected");
    }

    t = new QTextStream( socket );
}
Smtp::~Smtp()
{
    qDebug() << "Destroying";
    finished();
    delete t;
    delete socket;
}
void Smtp::stateChanged(QAbstractSocket::SocketState socketState)
{

    qDebug() <<"stateChanged: " << socketState;

}

void Smtp::errorReceived(QAbstractSocket::SocketError socketError)
{
    qDebug() << " error:" <<socketError;

}

void Smtp::disconnected()
{
    qDebug() << "Disconnected: "  << socket->errorString();
}

void Smtp::connected()
{
}

void Smtp::readyRead()
{
    QString rLine;
    do
    {
        rLine = socket->readLine();
        response += rLine;
        qDebug() << "Response is: " << response;
    }
    while (socket->canReadLine() && rLine[3] != ' ');

    rLine.truncate(3);

    if (state == Init && rLine[0] == '2')
    {
        qDebug() << "HELO there";
        *t << "HELO there\r\n";
        t->flush();

        state = Auth;
    }
    else if (state == Auth && rLine[0] == '2')
    {
        // Trying AUTH
        qDebug() << "Auth";
        *t << "AUTH LOGIN" << "\r\n";
        t->flush();
        state = User;
    }
    else if (state == User && rLine[0] == '3')
    {
        //Trying User
        qDebug() << "Username";
        *t << user << "\r\n";
        t->flush();

        state = Pass;
    }
    else if (state == Pass && rLine[0] == '3')
    {
        //Trying pass
        qDebug() << "Pass";
        *t << pass << "\r\n";
        t->flush();

        state = Mail;
    }
    else if (state == Mail && rLine[0] == '2')
    {
        qDebug() << "Mail from";
        *t << "MAIL FROM: " << from << "\r\n";
        t->flush();
        state = Rcpt;
    }
    else if (state == Rcpt && rLine[0] == '2')
    {

        qDebug() << "RCPT TO ";
        *t << "RCPT TO: " << rcpt << "\r\n";
        t->flush();
        if(recips.isEmpty() || x == recips.count() )
        {
            state = Data;
        }
        else
        {
            if(x < recips.count())
            {
                rcpt = recips.at(x);
                x++;
                state = Rcpt;
            }
        }
    }
    else if (state == Data && rLine[0] == '2')
    {
        qDebug() << "Data";
        *t << "DATA\r\n";
        t->flush();
        state = Body;
    }
    else if (state == Body && rLine[0] == '3')
    {
        qDebug() << "Body state";
        *t << message << "\r\n.\r\n";
        t->flush();
        state = Quit;
    }
    else if (state == Quit && rLine[0] == '2')
    {
        qDebug() << "Quit";
        *t << "QUIT\r\n";
        t->flush();
        // here, we just close.
        state = Close;
        emit status( tr( "Message sent" ) );
    }
    else if (state == Close)
    {
        qDebug() << "State == close";
        deleteLater();
        return;
    }
    else
    {
        //something has broken
        state = Close;
    }
    response = "";
}
