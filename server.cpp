#pragma once
#define SPIP { "8.136.30.21" }
#include <arpa/inet.h>
#include <cstring>
#include <format>
#include <iostream>
#include <map>
#include <memory>
#include <netinet/in.h>
#include <queue>
#include <sys/socket.h>
#include <unistd.h>
struct PlayerState { };

struct Features {
    int hr, hg, hb;
    int br, bg, bb;
};

struct Player {
    int id;
    sockaddr_in from { }, to { };
    PlayerState state;
    Features features;
};

enum PacketType : int { JOIN = 1, SEND = 2, RETURN = 3, PAUSE = 0, QUIT = -1 };

class NetObject {

private:
    std::queue<int> players;
    std::map<int, PlayerState> packet;
    int fd;
    auto join(int id) { };
    auto send() { };
    auto ret() { };
    auto pause() { };
    auto quit() { };

public:
    auto react(PacketType type, Player player) {
        switch (type) {
        case JOIN: {
            join(player.id);
            break;
        }
        case SEND: {
            send();
        }
        case RETURN: {
            ret();
        }
        case PAUSE: {
            pause();
        }
        case QUIT: {
            quit();
        }
        }
    };
};
int main() {
    int fd = socket(AF_INET, SOCK_DGRAM, 0);
    sockaddr_in addr { }; // 创建专用地址结构体
    addr.sin_family      = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port        = 9000;
    if (bind(fd, (sockaddr*)&addr, sizeof(addr)) == -1) {
        close(fd);
        std::cerr << "Binding to" << addr.sin_addr.s_addr << ":" << addr.sin_port << " Failed\n";
    }

    NetObject server;
    while (true) {
        auto buf = std::make_unique<int>;
        PacketType type;
        Player player;
        server.react(type, player);
    };
}