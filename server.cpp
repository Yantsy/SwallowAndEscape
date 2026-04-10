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

enum PacketType : uint8_t { JOIN = 1, EAT = 2, PAUSE = 3, QUIT = 4 };

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
        case EAT: {
            send();
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
    addr.sin_port        = htons(9000);
    socklen_t addrlen    = sizeof(addr);

    std::vector<sockaddr_in> clients { };
    sockaddr_in from { };
    socklen_t fromlen = sizeof(from);
    if (bind(fd, (sockaddr*)&addr, sizeof(addr)) == -1) {
        close(fd);
        std::cerr << "Binding to" << addr.sin_addr.s_addr << ":" << addr.sin_port << " Failed\n";
    } else {
        std::cout << "server listening on :9000\n";
    };
    while (true) {
        ++beat;
        sockaddr_in from { };
        socklen_t fromlen = sizeof(from);
        uint8_t packet    = 0;
        auto foodposition = randnum();
        auto* fp          = &foodposition;
        auto fplen        = sizeof(foodposition);
        int n             = recvfrom(fd, &packet, sizeof(packet), 0, (sockaddr*)&from, &fromlen);
        if (n < 0) continue;
        if (clients.empty()) {
            clients.push_back(from);
        };
        for (auto& c : clients) {
            if (c.sin_addr.s_addr == from.sin_addr.s_addr && c.sin_port == from.sin_port) {
            } else {
                clients.push_back(from);
            };
        }
        if (packet == EAT) {
            for (auto c : clients) {
                auto addr    = &c;
                auto addrlen = sizeof(c);
                int m        = sendto(fd, fp, fplen, 0, (sockaddr*)addr, addrlen);
                std::cout << beat << ".send food position to" << c.sin_addr.s_addr << ":"
                          << c.sin_port << "sucessfully.\n"
                          << std::flush;
            }
        }
        // int m = sendto(fd, fp, fplen, 0, (sockaddr*)&from, fromlen);
        sleep(1);
    };
}