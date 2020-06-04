#include "client.h"
#include "QDebug"

Client::Client()
{

}

Client::~Client()
{
    delete _info;
    delete _chat;
}
