#define SPIP { "8.136.30.21" }
#include <arpa/inet.h>
#include <cstring>
#include <format>
#include <iostream>
#include <map>
#include <memory>
#include <netinet/in.h>
#include <queue>
#include <random>
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

auto encode(float x, float y) {
    auto a     = static_cast<uint16_t>(x * 10000);
    auto b     = static_cast<uint16_t>(y * 10000);
    uint32_t c = a << 16;
    c |= b;
    return c;
};
auto randnum() {
    std::mt19937 mt(std::random_device { }());
    auto x = std::uniform_real_distribution<float>(0.0f, 1.0f)(mt);
    auto y = std::uniform_real_distribution<float>(0.0f, 1.0f)(mt);
    return encode(x, y);
};
int main() {
    int fd = socket(AF_INET, SOCK_DGRAM, 0);
    int beat { 0 };
    sockaddr_in addr { }; // 创建专用地址结构体
    addr.sin_family      = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port        = 9000;
    auto addrlen         = sizeof(addr);
    if (bind(fd, (sockaddr*)&addr, sizeof(addr)) == -1) {
        close(fd);
        std::cerr << "Binding to" << addr.sin_addr.s_addr << ":" << addr.sin_port << " Failed\n";
    };
    while (true) {
        ++beat;
        auto foodposition = randnum();
        auto* fp          = &foodposition;
        auto fplen        = sizeof(foodposition);
        int n             = sendto(fd, fp, fplen, 0, (sockaddr*)&addr, addrlen);
        std::cout << beat << ".send food position sucessfully.\n" << std::flush;
    };
}