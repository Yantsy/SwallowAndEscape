#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_gamecontroller.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_video.h>
#include <algorithm>
#include <arpa/inet.h>
#include <array>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <map>
#include <memory>
#include <netinet/in.h>
#include <queue>
#include <sys/socket.h>
#include <unistd.h>
#include <vector>

struct PlayerState { };

struct Features {
    int hr, hg, hb, ha;
    int br, bg, bb, ba;
};

struct Player {
    int id;
    int orig_x, orig_y, w, h;
    SDL_Rect block;
    std::vector<SDL_Rect> segments;
    sockaddr_in from { }, to { };
    PlayerState state;
    Features features;
    Player(int x, int y, int w, int h) {
        block.x = x;
        block.y = y;
        block.w = w;
        block.h = h;
        segments.insert(segments.end(), block);
    }
};

struct Food {
    std::unique_ptr<SDL_Texture> texture { nullptr };
    SDL_Rect body;
    int x, y;
};

enum PacketType : int { JOIN = 1, SEND = 2, RETURN = 3, PAUSE = 0, QUIT = -1 };

class NetObject {

private:
    std::queue<int> players;
    std::map<int, PlayerState> packet;
    int fd;
    auto join() { };
    auto send() { };
    auto ret() { };
    auto pause() { };
    auto quit() { };

public:
    auto react(PacketType type, Player player) {
        switch (type) {
        case JOIN: {
            join();
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
