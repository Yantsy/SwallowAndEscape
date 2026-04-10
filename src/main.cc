#include "quote.hpp"
#include <format>

void addplayer(std::vector<Player>& playerlist, Player player) {
    playerlist.insert(playerlist.end(), player);
};
void updatecposition(Player& player, Food& food) {
    auto& x = player.block.x;
    auto& y = player.block.y;
};
void addblock(Player& player) { player.segments.insert(player.segments.begin(), player.block); }
void removeblock(Player& player) {
    if (!player.segments.empty()) player.segments.pop_back();
};
void updateplayerposition(Player& player) {
    int speed = player.block.w;
    auto& x   = player.block.x;
    auto& y   = player.block.y;
    x += player.xdir * speed;
    y += player.ydir * speed;
    addblock(player);
};

void paintplayer(Player& player, SDL_Renderer* render) {
    auto& features = player.features;
    for (auto& segsheet : player.segments) {
        if (&segsheet == &player.segments[0]) {
            SDL_SetRenderDrawColor(render, features.hr, features.hg, features.hb, features.ha);
            SDL_RenderFillRect(render, &segsheet);
        } else {
            SDL_SetRenderDrawColor(render, features.br, features.bg, features.bb, features.ba);
            SDL_RenderFillRect(render, &segsheet);
        }
    }
}
void paintplayers(std::vector<Player>& playerlist, SDL_Renderer* renderer) {
    for (auto& player : playerlist) {
        if (&player == &playerlist[0]) {
            player.features = { 106, 168, 79, 255, 147, 196, 125, 255 };
            paintplayer(player, renderer);
        } else if (&player == &playerlist[1]) {
            player.features = { 204, 0, 0, 255, 132, 174, 125, 255 };
            paintplayer(player, renderer);
        }
    }
};

auto decode(uint32_t code) {
    uint16_t x = code >> 16;
    uint16_t y = code & 0xFFFF;
    auto dx    = (float)x / 10000;
    auto dy    = (float)y / 10000;
    return std::tuple { dx, dy };
};
auto requestfoodposition(int fd, sockaddr* addr, socklen_t len) {
    static int beat = 0;
    //++beat;
    auto packet = EAT;
    sendto(fd, &packet, sizeof(packet), 0, addr, len);
    /*
    uint32_t code { 0 };
    auto* cptr = &code;
    auto cplen = sizeof(code);
    int n      = recvfrom(fd, cptr, cplen, 0, addr, &len);
    std::cout << beat << ".request for foodposition.\n" << std::flush;
    return decode(code);*/
};
auto checkfoodposition(int fd, sockaddr* addr, socklen_t len) {

};
void updatefoodposition(Food& food, Client& client, SDL_Rect& map) {
    auto& x = food.body.x;
    auto& y = food.body.y;
    requestfoodposition(client.fd, client.addr, client.len);
    // x = map.x + map.w * std::get<0>(c);
    // y = map.y + map.h * std::get<1>(c);
};
bool collisionwithfood(Food& food, Player& player) {
    auto dx = (float)std::abs(food.body.x + food.body.w - player.block.x - player.block.w) / 2;
    auto dy = (float)std::abs(food.body.y + food.body.h - player.block.y - player.block.h) / 2;
    if (dx < 5 && dy < 5) return true;
    return false;
};
bool collisionwithboarder(SDL_Rect& boarder, Player& player) {
    int& x = player.block.x;
    int& y = player.block.y;
    int& w = player.block.w;
    int& h = player.block.h;
    if (x < boarder.x) x = boarder.x;
    if (x + w > boarder.x + boarder.w) x = boarder.x + boarder.w - w;
    if (y < boarder.y) y = boarder.y;
    if (y + h > boarder.y + boarder.h) y = boarder.y + boarder.h - h;
    if (x < boarder.x || x + w > boarder.x + boarder.w || y < boarder.y
        || y + h > boarder.y + boarder.h)
        return true;
    return false;
};
void checkcollisionwithfood(
    Food& food, Client& client, SDL_Rect& map, std::vector<Player>& playerlist) {
    for (auto& player : playerlist) {
        if (collisionwithfood(food, player)) {
            updatefoodposition(food, client, map);
        } else {
            removeblock(player);
        }
    }
};
void checkcollisionwithboarder(SDL_Rect& boarder, std::vector<Player>& playerlist) {
    for (auto& player : playerlist) {

        if (collisionwithboarder(boarder, player)) {
            addblock(player);
            removeblock(player);
        } else {
        }
    }
};
void paintfood(Food& food, SDL_Renderer* renderer) {
    SDL_RenderCopy(renderer, food.texture.get(), NULL, &food.body);
};
void keyboard(int& xdir, int& ydir, SDL_Event e) {

    switch (e.key.keysym.scancode) {

    case SDL_SCANCODE_Q: {
        xdir = 0, ydir = 0;
        break;
    }

    case SDL_SCANCODE_W:
    case SDL_SCANCODE_UP:
        ydir = -1, xdir = 0;
        break;

    case SDL_SCANCODE_S:
    case SDL_SCANCODE_DOWN:
        ydir = 1, xdir = 0;
        break;

    case SDL_SCANCODE_A:
    case SDL_SCANCODE_LEFT:
        xdir = -1, ydir = 0;
        break;

    case SDL_SCANCODE_D:
    case SDL_SCANCODE_RIGHT:
        xdir = 1, ydir = 0;
        break;

    default:
        break;
    }
};
void pad(int& xdir, int& ydir, SDL_Event e) {
    switch (e.cbutton.button) {

    case SDL_CONTROLLER_BUTTON_START:
        xdir = 0, ydir = 0;
        break;
    case SDL_CONTROLLER_BUTTON_DPAD_UP:
        ydir = -1, xdir = 0;
        break;
    case SDL_CONTROLLER_BUTTON_DPAD_DOWN:
        ydir = 1, xdir = 0;
        break;
    case SDL_CONTROLLER_BUTTON_DPAD_LEFT:
        xdir = -1, ydir = 0;
        break;
    case SDL_CONTROLLER_BUTTON_DPAD_RIGHT:
        xdir = 1, ydir = 0;
        break;
    default:
        break;
    }
};
void controlleropencheck(SDL_GameController* controller) {
    for (int counter = 0; counter < 1; counter++) {
        if (controller == nullptr) {
            std::cerr << "SDL_GameControllerOpen Error: " << SDL_GetError() << std::endl;
        }
    }
};
void controllerrumbleopencheck(SDL_GameController* controller) {
    for (int counter = 0; counter < 1; counter++) {
        if (controller != nullptr) {

            if (SDL_GameControllerHasRumble(controller) == false) {

                std::cerr << "SDL_GameControllerHasRumble Error: " << SDL_GetError() << std::endl;
            }
        };
    }
};

void boundarycheck(SDL_Rect* object, SDL_Rect map) {
    if (object->x < map.x) object->x = map.x;
    if (object->x > map.x + map.w - object->w) object->x = map.x + map.w - object->w;
    if (object->y < map.y) object->y = map.y;
    if (object->y > map.y + map.h - object->w) object->y = map.y + map.h - object->w;
};

int copy() {
    // initialize SDL video subsystem
    if (SDL_Init(SDL_INIT_VIDEO) != 0 && SDL_Init(SDL_INIT_AUDIO) != 0) {
        std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
        return 1;
    }
    // initialize SDL_mixer audio subsystem
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) != 0) {
        std::cerr << "Mix_OpenAudio Error: " << Mix_GetError() << std::endl;
        return 1;
    }
    // initialize the SDL game controller subsystem
    if (SDL_InitSubSystem(SDL_INIT_GAMECONTROLLER) != 0) {
        std::cerr << "SDL_InitSubSystem Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    // 创建窗口、渲染器（renderer/suface/texture）、事件队列、蛇和食物、地图和控制器等事物

    // Create a window
    SDL_Window* win1 = SDL_CreateWindow("SwallowAndEscape", SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED, 1920, 1080,
        SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_OPENGL);
    if (win1 == nullptr) {
        std::cerr << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
        return 1;
    }
    int ww, wh;
    SDL_GetWindowSize(win1, &ww, &wh);

    // create the renderer for the window

    SDL_Renderer* renderer01 =
        SDL_CreateRenderer(win1, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderer01 == nullptr) {
        std::cerr << "SDL_CreateRenderer Error:" << SDL_GetError() << std::endl;
    }

    // create the map

    int thickness = 15;
    ;
    SDL_Rect map1 = {
        ww / 8,
        wh / 16,
        ww * 3 / 4,
        wh * 3 / 4,
    };

    SDL_Rect map2 = {
        map1.x + thickness,
        map1.y + thickness,
        map1.w - thickness * 2,
        map1.h - thickness * 2,
    };

    // create the snake head and food

    // initialize head direction
    int pdir  = 1;
    int ndir  = 0;
    int cpdir = 1;
    int cndir = 0;

    // food texture

    SDL_Surface* foodsuf = IMG_Load("assets/food.png");

    SDL_Texture* foodtex = SDL_CreateTextureFromSurface(renderer01, foodsuf);

    SDL_FreeSurface(foodsuf);

    // food position

    int fx = (rand() % (map2.w - map2.x)) + map2.x;
    int fy = (rand() % (map2.h - map2.y)) + map2.y;
    // create the rectangles
    std::vector<SDL_Rect> segments;
    std::vector<SDL_Rect> csegments;
    std::vector<SDL_Rect> foods;
    SDL_Rect block = { ww / 2, wh / 2, 15, 15 };
    int speed      = block.w / 3;
    segments.push_back(block);
    SDL_Rect cblock = { map2.x, map2.y, 15, 15 };
    csegments.push_back(cblock);
    SDL_Rect food = { fx, fy, 15, 15 };
    foods.push_back(food);

    // bgm
    Mix_Music* bgm = Mix_LoadMUS("assets/A.mp3");
    if (bgm == nullptr) {
        std::cerr << "Mix_LoadMUS Error: " << Mix_GetError() << std::endl;
    }
    Mix_PlayMusic(bgm, -1);

    // gamecontroler

    SDL_GameController* controller = SDL_GameControllerOpen(0);

    // open the controller

    controlleropencheck(controller);

    // check and start the controller rumble

    controllerrumbleopencheck(controller);

    // map the controller
    SDL_GameControllerAddMappingsFromFile("assets/gamecontrollerdb.txt");

    // 设置游戏主循环以及在玩家操作下，上述事物会如何变化

    // control variable for the game loop
    // event handler
    SDL_Event e;
    bool quit = false;

    int ld, rd, td, bd, md;

    // game loop
    while (!quit) {
        while (SDL_PollEvent(&e)) {

            switch (e.type) {

            case SDL_QUIT: {
                quit = true;
                break;
            }

            case SDL_KEYDOWN: {
                keyboard(pdir, ndir, e);
                break;
            }

            case (SDL_CONTROLLERBUTTONDOWN): {
                pad(pdir, ndir, e);
                break;
            }
            case SDL_CONTROLLERDEVICEADDED: {
                if (controller == nullptr) {
                    controller = SDL_GameControllerOpen(e.cdevice.which);
                    controlleropencheck(controller);
                    controllerrumbleopencheck(controller);
                    std::cout << "controller connected" << std::endl;
                }
                break;
            }
            case SDL_CONTROLLERDEVICEREMOVED: {
                if (controller != nullptr
                    && e.cdevice.which
                        == SDL_JoystickInstanceID(SDL_GameControllerGetJoystick(controller))) {
                    SDL_GameControllerClose(controller);
                    controller = nullptr;
                    std::cout << "controller removed" << std::endl;
                }
                break;
            }
            }
        }
        ld = abs(cblock.x - speed - food.x) + abs(cblock.y - food.y);
        rd = abs(cblock.x + speed - food.x) + abs(cblock.y - food.y);
        td = abs(cblock.x - food.x) + abs(cblock.y - speed - food.y);
        bd = abs(cblock.x - food.x) + abs(cblock.y + speed - food.y);

        std::vector<int> neighbors = { ld, rd, td, bd };

        md = std::min({ neighbors[0], neighbors[1], neighbors[2], neighbors[3] });
        if (md == neighbors[0]) {
            cpdir = -1;
            cndir = 0;
        }
        if (md == neighbors[1]) {
            cpdir = 1;
            cndir = 0;
        }
        if (md == neighbors[2]) {
            cpdir = 0;
            cndir = -1;
        }
        if (md == neighbors[3]) {
            cpdir = 0;
            cndir = 1;
        }
        // update the position of the player's head and body

        block.x += pdir * speed;
        block.y += ndir * speed;

        cblock.x += cpdir * speed;
        cblock.y += cndir * speed;

        segments.insert(segments.begin(), block);

        csegments.insert(csegments.begin(), cblock);

        // check for collision with the boundary
        boundarycheck(&block, map2);
        boundarycheck(&cblock, map2);

        // check for collision with food
        if (std::abs(((block.x + 15) / 2) - ((food.x + 15) / 2)) < 5
            && std::abs(((block.y + 15) / 2) - ((food.y + 15) / 2)) < 5) {
            // check and start the controller rumble
            if (controller != nullptr) {
                SDL_GameControllerRumble(controller, 0x4000, 0x4000, 300);
            } else {
            };
            fx     = floor(((rand() % (map2.w)) + map2.x) / 15) * 15;
            fy     = floor(((rand() % (map2.h)) + map2.y) / 15) * 15;
            food.x = fx;
            food.y = fy;

        } else {
            if (!segments.empty()) {
                segments.pop_back();
            } else {
            }
        }

        if (std::abs(((cblock.x + 15) / 2) - ((food.x + 15) / 2)) < 5
            && std::abs(((cblock.y + 15) / 2) - ((food.y + 15) / 2)) < 5) {
            // check and start the controller rumble
            fx     = floor(((rand() % (map2.w)) + map2.x) / 15) * 15;
            fy     = floor(((rand() % (map2.h)) + map2.y) / 15) * 15;
            food.x = fx;
            food.y = fy;

        } else {
            if (!csegments.empty()) {
                csegments.pop_back();
            } else {
            }
        }

        // check for collision with the boundary of the snake itself
        for (int seg = 0; seg < segments.size(); seg++) {
            if (std::abs(block.x - segments[seg].x) < 15
                && std::abs(block.y - segments[seg].y) < 15) { }
        }
        for (int seg = 0; seg < csegments.size(); seg++) {
            if (std::abs(block.x - csegments[seg].x) < 15
                && std::abs(block.y - csegments[seg].y) < 15) { }
        }

        // 将创建的事物以及玩家操作导致它产生的变化渲染出来
        //  render and present
        SDL_SetRenderDrawColor(renderer01, 165, 222, 229, 255);
        SDL_RenderClear(renderer01);

        SDL_SetRenderDrawColor(renderer01, 254, 253, 202, 200);
        SDL_RenderFillRect(renderer01, &map1);

        SDL_SetRenderDrawColor(renderer01, 224, 249, 181, 255);
        SDL_RenderFillRect(renderer01, &map2);

        for (auto& segsheet : segments) {
            if (&segsheet == &segments[0]) {
                SDL_SetRenderDrawColor(renderer01, 255, 69, 69, 255);
                SDL_RenderFillRect(renderer01, &block);
                continue;
            };
            SDL_SetRenderDrawColor(renderer01, 255, 207, 223, 255);
            SDL_RenderFillRect(renderer01, &segsheet);
        };

        for (auto& csegsheet : csegments) {
            if (&csegsheet == &csegments[0]) {
                SDL_SetRenderDrawColor(renderer01, 47, 79, 79, 255);
                SDL_RenderFillRect(renderer01, &cblock);
                continue;
            };
            SDL_SetRenderDrawColor(renderer01, 192, 192, 192, 255);
            SDL_RenderFillRect(renderer01, &csegsheet);
        };

        SDL_RenderCopy(renderer01, foodtex, NULL, &food);
        SDL_RenderPresent(renderer01);

        SDL_Delay(1000 / 60);
    }

    // 清理程序运行过程中生成的资源，释放内存
    SDL_DestroyTexture(foodtex);
    SDL_GameControllerClose(controller);
    SDL_DestroyRenderer(renderer01);
    SDL_DestroyWindow(win1);

    SDL_Quit();

    return 0;
}

int main() {
    // initialize SDL video subsystem
    if (SDL_Init(SDL_INIT_VIDEO) != 0 && SDL_Init(SDL_INIT_AUDIO) != 0) {
        std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
        return 1;
    }
    // initialize SDL_mixer audio subsystem
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) != 0) {
        std::cerr << "Mix_OpenAudio Error: " << Mix_GetError() << std::endl;
        return 1;
    }
    // initialize the SDL game controller subsystem
    if (SDL_InitSubSystem(SDL_INIT_GAMECONTROLLER) != 0) {
        std::cerr << "SDL_InitSubSystem Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    // 创建窗口、渲染器（renderer/suface/texture）、事件队列、蛇和食物、地图和控制器等事物

    // Create a window

    auto win1 = std::unique_ptr<SDL_Window, SDLDeleter<SDL_Window>>(SDL_CreateWindow("SwallowAndEsc"
                                                                                     "ape",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1920, 1080,
        SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_OPENGL));

    if (win1 == nullptr) {
        std::cerr << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
        return 1;
    }
    int ww, wh;
    SDL_GetWindowSize(win1.get(), &ww, &wh);

    // create the renderer for the window

    auto renderer01 = std::unique_ptr<SDL_Renderer, SDLDeleter<SDL_Renderer>>(
        SDL_CreateRenderer(win1.get(), -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC));
    if (renderer01 == nullptr) {
        std::cerr << "SDL_CreateRenderer Error:" << SDL_GetError() << std::endl;
    }

    // create the map

    int thickness = 15;
    ;
    SDL_Rect map1 = {
        ww / 8,
        wh / 16,
        ww * 3 / 4,
        wh * 3 / 4,
    };

    SDL_Rect map2 = {
        map1.x + thickness,
        map1.y + thickness,
        map1.w - thickness * 2,
        map1.h - thickness * 2,
    };
    // create bgm
    Mix_Music* bgm = Mix_LoadMUS("assets/A.mp3");
    if (bgm == nullptr) {
        std::cerr << "Mix_LoadMUS Error: " << Mix_GetError() << std::endl;
    }
    // Mix_PlayMusic(bgm, -1);

    // create controler
    //  gamecontroler
    SDL_GameController* controller = SDL_GameControllerOpen(0);
    // open the controller
    controlleropencheck(controller);
    // check and start the controller rumble
    controllerrumbleopencheck(controller);
    // map the controller
    SDL_GameControllerAddMappingsFromFile("assets/gamecontrollerdb.txt");
    //  create players

    std::vector<Player> playerlist { };
    Player lp(350, 400, 15, 15), ap(ww / 2, wh / 2, 15, 15), rp(ww / 2, wh / 2, 15, 15);
    playerlist.insert(playerlist.end(), std::move(ap));
    playerlist.insert(playerlist.end(), std::move(lp));
    auto& cp = playerlist[0];
    auto& p0 = playerlist[1];

    // create food
    Food food(ww / 2, wh / 2, 15, 15);
    auto foodsuf = std::unique_ptr<SDL_Surface>(IMG_Load("assets/food.png"));

    auto foodtex = std::unique_ptr<SDL_Texture, SDLDeleter<SDL_Texture>>(
        SDL_CreateTextureFromSurface(renderer01.get(), foodsuf.get()));
    food.texture.reset(foodtex.get());

    // options
    bool quit      = false;
    bool newplayer = false;
    SDL_Event e;
    int fd    = socket(AF_INET, SOCK_DGRAM, 0);
    int flags = fcntl(fd, F_GETFL, 0);
    fcntl(fd, F_SETFL, flags | O_NONBLOCK);
    sockaddr_in addr { };
    socklen_t len        = sizeof(addr);
    addr.sin_family      = AF_INET;
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    addr.sin_port        = htons(9000);
    Client client { fd, (sockaddr*)&addr, len };
    auto packet = JOIN;
    int n       = sendto(fd, &packet, sizeof(packet), 0, client.addr, client.len);
    uint32_t code { 0 };
    auto* cptr = &code;
    auto cplen = sizeof(code);
    // gameloop
    while (!quit) {
        while (SDL_PollEvent(&e)) {
            switch (e.type) {
            case SDL_QUIT: {
                quit = true;
                break;
            }

            case SDL_KEYDOWN: {
                keyboard(p0.xdir, p0.ydir, e);
                break;
            }

            case (SDL_CONTROLLERBUTTONDOWN): {
                pad(p0.xdir, p0.ydir, e);
                break;
            }
            case SDL_CONTROLLERDEVICEADDED: {
                if (controller == nullptr) {
                    controller = SDL_GameControllerOpen(e.cdevice.which);
                    controlleropencheck(controller);
                    controllerrumbleopencheck(controller);
                    std::cout << "controller connected" << std::endl;
                }
                break;
            }
            case SDL_CONTROLLERDEVICEREMOVED: {
                if (controller != nullptr
                    && e.cdevice.which
                        == SDL_JoystickInstanceID(SDL_GameControllerGetJoystick(controller))) {
                    SDL_GameControllerClose(controller);
                    controller = nullptr;
                    std::cout << "controller removed" << std::endl;
                }
                break;
            }
            }
        }

        // paint the map
        SDL_SetRenderDrawColor(renderer01.get(), 165, 222, 229, 255);
        SDL_RenderClear(renderer01.get());

        SDL_SetRenderDrawColor(renderer01.get(), 254, 253, 202, 200);
        SDL_RenderFillRect(renderer01.get(), &map1);

        SDL_SetRenderDrawColor(renderer01.get(), 224, 249, 181, 255);
        SDL_RenderFillRect(renderer01.get(), &map2);
        // update players'positions

        int n = recvfrom(fd, cptr, cplen, 0, (sockaddr*)&addr, &len);
        if (n > 0) {
            auto c  = decode(code);
            auto& x = food.body.x;
            auto& y = food.body.y;
            x       = map2.x + map2.w * std::get<0>(c);
            y       = map2.y + map2.h * std::get<1>(c);
        } else if (n == -1) {
            if (errno == EAGAIN || errno == EWOULDBLOCK) {

            } else {
                // 真正的错误
                perror("recvfrom");
                break;
            }
        };
        updateplayerposition(p0);
        updateplayerposition(rp);
        updatecposition(cp, food);
        checkcollisionwithfood(food, client, map2, playerlist);
        checkcollisionwithboarder(map2, playerlist);
        // update players'outlooks
        paintplayers(playerlist, renderer01.get());
        paintfood(food, renderer01.get());
        SDL_RenderPresent(renderer01.get());
        SDL_Delay(1000 / 60);
    };
}