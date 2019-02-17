//
// Created by Shinan on 2019/1/27.
//

#include "Connection.h"
#include "Stream.h"

Connection::Connection(const SpStream &spStream):spStream_(spStream)
{

}

Connection::~Connection()
{}

void Connection::destroy()
{
  delete this;
}


