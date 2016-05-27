//
// Created by grey on 27.05.16.
//

#ifndef QUAKEWITHSOCKETS_ROOM_H
#define QUAKEWITHSOCKETS_ROOM_H

#include "session.h"

class room {
public:
    room() { }
    void join(participant_ptr session) {
        sessions_.insert(session);
    }

    void leave(participant_ptr session) {
        sessions_.erase(session);
    }

    void deliver(const message& msg ) {
        for(auto session : sessions_) {
            session->deliver(msg);
        }
    }

private:
    std::set<participant_ptr> sessions_;
};

#endif //QUAKEWITHSOCKETS_ROOM_H
