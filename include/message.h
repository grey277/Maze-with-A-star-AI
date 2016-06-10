//
// Created by grey on 27.05.16.
//

#ifndef QUAKEWITHSOCKETS_MESSAGE_H
#define QUAKEWITHSOCKETS_MESSAGE_H

class message {
public:
    enum { header_length = 5 };
    enum { max_body_length = 2048 };
    enum type { mapm = 1, playerPosition = 2, botPosition = 3 };

    message() : _body_length(0) { }

    const char* data() const { return _data; }

    char* data() { return _data; }

    std::size_t length() const { return header_length + _body_length; }

    const char* body() const { return _data + header_length; }

    char* body() { return _data + header_length; }

    std::size_t body_length() const { return _body_length; }

    type messageType() { return _messageType; }
    const type messageType() const { return _messageType; }

    void messageType(type messageType) { _messageType = messageType; }

    void body_length(std::size_t new_length) {
        _body_length = new_length > max_body_length ? max_body_length : new_length;
    }

    bool decode_header() {
        char header[header_length] = "";
        _messageType = (message::type)(_data[4] - 48);
        std::strncat(header, _data, header_length - 1);
        _body_length = std::atoi(header);
        if (_body_length > max_body_length) {
            _body_length = 0;
            return false;
        }
        return true;
    }

    void encode_header() {
        char header[header_length + 1] = "";
        std::sprintf(header, "%4d%d", static_cast<int>(_body_length), _messageType);
        std::memcpy(_data, header, header_length);
    }

private:
    char _data[header_length + max_body_length];
    std::size_t _body_length;
    type _messageType;
};

#endif //QUAKEWITHSOCKETS_MESSAGE_H
