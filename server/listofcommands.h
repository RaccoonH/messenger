#ifndef LISTOFCOMMANDS_H
#define LISTOFCOMMANDS_H

#include <QtGlobal>

namespace ServerCommand
{
    static const quint8 clientRegistration = 11;
    static const quint8 clientAuthorization = 12;
    static const quint8 clientSendMessageToUser = 13;

    static const quint8 servRegSuccess = 21;
    static const quint8 servAuthSuccess = 22;

    static const quint8 errorRegLogin = 31;
    static const quint8 errorAuth = 32;
}

#endif // LISTOFCOMMANDS_H
