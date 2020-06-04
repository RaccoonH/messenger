#ifndef LISTOFCOMMANDS_H
#define LISTOFCOMMANDS_H

#include <QtGlobal>

namespace ServerCommand
{
static const quint8 clientRegistration = 11;
static const quint8 clientAuthorization = 12;
static const quint8 clientLoadClientList = 13;

static const quint8 clientLoadDialogMessages = 14;
static const quint8 clientLoadGroupMessages = 15;
static const quint8 clientLoadChannelMessages = 16;

static const quint8 clientSendMessageToUser = 17;
static const quint8 clientSendGroupMessage = 18;
static const quint8 clientSendChannelMessage = 19;

static const quint8 clientLoadUserInfo = 40;
static const quint8 clientLoadChannelInfo = 41;
static const quint8 clientLoadGroupInfo = 42;

static const quint8 clientGetFriendsList = 43;
static const quint8 clientSendNewUserInfo = 44;
static const quint8 clientLoadChannelSubs = 45;
static const quint8 clientLoadGroupUsers = 46;
static const quint8 clientDeleteSubFromChannel = 47;
static const quint8 clientDeleteUserFromGroup = 48;
static const quint8 clientSendNewChannelInfo = 49;
static const quint8 clientSendNewGroupInfo = 60;
static const quint8 clientInviteUserToGroup = 61;
static const quint8 clientSendNewChannel = 62;
static const quint8 clientSendNewGroup = 63;
static const quint8 clientDeleteChannel = 64;
static const quint8 clientDeleteGroup = 65;
static const quint8 clientLeaveChannel = 66;
static const quint8 clientLeaveGroup = 67;
static const quint8 clientSearch = 68;
static const quint8 clientStartNewDialog = 69;
static const quint8 clientJoinToChannel = 80;
static const quint8 clientJoinToGroup = 81;
static const quint8 clientDeleteMessage = 82;
static const quint8 clientEditMessage = 83;
static const quint8 clientAddToFriendsList = 84;
static const quint8 clientDeleteFriend = 85;
static const quint8 clientInviteUserToChannel = 86;


static const quint8 servRegSuccess = 21;
static const quint8 servAuthSuccess = 22;
static const quint8 servLoadedClientList = 23;

static const quint8 servLoadedDialogMessages = 24;
static const quint8 servLoadedGroupMessages = 25;
static const quint8 servLoadedChannelMessages = 26;

static const quint8 servSendDialogMessage = 27;
static const quint8 servSendGroupMessage = 28;
static const quint8 servSendChannelMessage = 29;

static const quint8 servLoadUserInfo = 50;
static const quint8 servLoadChannelInfo = 51;
static const quint8 servLoadGroupInfo = 52;

static const quint8 servGetFriendsList = 53;
static const quint8 servSendNewUserInfo = 54;
static const quint8 servLoadChannelSubs = 55;
static const quint8 servLoadGroupUsers = 56;
static const quint8 servDeleteSubFromChannel = 57;
static const quint8 servDeleteUserFromGroup = 58;
static const quint8 servSendNewChannelInfo = 59;
static const quint8 servSendNewGroupInfo = 70;
static const quint8 servInviteUserToGroup = 71;
static const quint8 servSendNewChannel = 72;
static const quint8 servSendNewGroup = 73;
static const quint8 servDeleteChannel = 74;
static const quint8 servDeleteGroup = 75;
static const quint8 servLeaveChannel = 76;
static const quint8 servLeaveGroup = 77;
static const quint8 servSearch = 78;
static const quint8 servStartNewDialog = 79;
static const quint8 servJoinToChannel = 90;
static const quint8 servJoinToGroup = 91;
static const quint8 servDeleteMessage = 92;
static const quint8 servEditMessage = 93;
static const quint8 servAddToFriendsList = 94;
static const quint8 servDeleteFriend = 95;
static const quint8 servInviteUserToChannel = 96;


static const quint8 errorRegLogin = 31;
static const quint8 errorAuth = 32;
static const quint8 errorUserIsNotExist = 33;
static const quint8 errorLoginIsExistAlready = 34;

}

#endif // LISTOFCOMMANDS_H
