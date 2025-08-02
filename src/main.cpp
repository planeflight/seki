#include <raylib.h>

#include <iostream>

#include "layout.hpp"
#include "parser/parse_html.hpp"
#include "util/file.hpp"
#include "util/types.hpp"

#ifdef PLATFORM_WEB
#include <emscripten/emscripten.h>
#endif

static constexpr i32 window_width = 1600, window_height = 900;

struct Global {
    Font font;
    LayoutBox root;
} g;

void init() {
    std::string source = load_file("./tests/index.html");
    std::cout << source << std::endl;
    HTMLParser parser(source);
    Node *r = parser.parse();
    std::cout << r->s << std::endl;

    // load font
    // load big for better aliasing
    g.font = LoadFontEx(
        "./res/Open_Sans/static/OpenSans-Regular.ttf", 100, nullptr, 0);

    // TEST LayoutBox
    g.root.dimensions = {.rect = {0.0f, 0.0f, window_width, window_height}};
    g.root.children.push_back(heading<1>("Hikes", g.font));
    g.root.children.back()->dimensions.border.set_uniform(10.0f);
    g.root.children.back()->dimensions.border.color = RED;
    g.root.children.back()->dimensions.margin.set_uniform(10.0f);
    g.root.children.back()->dimensions.margin.color = YELLOW;
    g.root.children.back()->dimensions.padding.set_uniform(10.0f);
    g.root.children.back()->dimensions.padding.color = GREEN;
    g.root.children.back()->text_align = TextAlign::RIGHT;

    g.root.children.push_back(heading<3>("Lakes Trail", g.font));
    g.root.children.push_back(heading<3>("General Sherman Loop", g.font));
    g.root.children.push_back(heading<1>("Backcountry", g.font));
    g.root.construct_dimensions();
}

void input(f32 dt) {}

void update(f32 dt) {}

void render(f32 dt) {
    BeginDrawing();
    ClearBackground(WHITE);

    g.root.render();

    EndDrawing();
}

void quit() {
    UnloadFont(g.font);
    CloseWindow();
}

void frame();

int main() {
    InitWindow(window_width, window_height, "Seki");
    init();

#ifdef PLATFORM_WEB
    emscripten_set_main_loop(frame, 0, 1);
#else
    SetTargetFPS(60);
    while (!WindowShouldClose()) {
        frame();
    }
#endif

    quit();

    return 0;
}

void frame() {
    f32 dt = GetFrameTime();
    input(dt);
    update(dt);
    render(dt);
}
