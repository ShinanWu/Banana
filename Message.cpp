//
// Created by Shinan on 2019/1/19.
//

#include "Message.h"

bool Message::isRetMsg_() const {
    return RetMsg_;
}

void Message::setRetMsg_(bool RetMsg_) {
    Message::RetMsg_ = RetMsg_;
}

const string &Message::getFrom_() const {
    return from_;
}

void Message::setFrom_(const string &from_) {
    Message::from_ = from_;
}

const string &Message::getTo_() const {
    return to_;
}

void Message::setTo_(const string &to_) {
    Message::to_ = to_;
}

int Message::getId_() const {
    return id_;
}

void Message::setId_(int id_) {
    Message::id_ = id_;
}

const string &Message::getData() const {
    return data;
}

void Message::setData(const string &data) {
    Message::data = data;
}
