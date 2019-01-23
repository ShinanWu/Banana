//
// Created by Shinan on 2019/1/23.
//
//每一个连接的客户端生成一个client对象

#include <unistd.h>
#include "Client.h"
#include "Message.h"
#include <errno.h>
#include <logging.h>
Client::Client(int clientFd)
        : fd_(clientFd)
        , recvLen_(0)
        , needRecvLen_(HEADER_LEN)
        , sendLen_(0)
        , needSendLen_(0)
        , stat_(RECVHEAD)
{

}

bool Client::recvOnePck() {
    char* curPos = &recvBuf_[recvLen_];
    int recvLen = read(fd_, curPos, needRecvLen_);
    if(recvLen == 0){setStat_(TOCLOSE); return false;}//对端关闭连接
    if(recvLen == -1){
        if(errno == EINTR || errno == EWOULDBLOCK || errno == EAGAIN){
            return false; //需要继续接收数据
        } else{
            LOG(ERROR) << "unexpected error! need close this client, errno:" << errno;
            setStat_(TOCLOSE);
        }
    } else{
        recvLen_ += recvLen;
        if(recvLen_ < needRecvLen_){    //还没收全一帧数据
            return false;
        } else{                        //收全头或者完整帧
            if(getStat_() == RECVHEAD){
                needRecvLen_ = int((int*)&recvBuf_[0]); //收完头继续收payload
                return false;
            } else{                         //收完完整一帧
                return true;
            }
        }
    }
}

bool Client::getPck(vector<char>& recvPck) {
    recvPck = recvBuf_;
    return true;
}

bool Client::sendPck() {
    return false;
}

bool Client::setPck(const string &sendPck) {
    return false;
}

int Client::getFd_() const {
    return fd_;
}

void Client::setFd_(int fd_) {
    Client::fd_ = fd_;
}

int Client::getStat_() const {
    return stat_;
}

void Client::setStat_(int stat_) {
    Client::stat_ = stat_;
}


