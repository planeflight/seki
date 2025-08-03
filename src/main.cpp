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
    Image icon;
} g;

void init() {
    std::string source = load_file("./tests/index.html");
    std::cout << source << std::endl;
    HTMLParser parser(source);
    Node *r = parser.parse();
    std::cout << r->s << std::endl;

    // load icon
    g.icon = LoadImage("./res/icon.png");
    SetWindowIcon(g.icon);

    // load font
    // load big for better aliasing
    g.font = LoadFontEx(
        "./res/Open_Sans/static/OpenSans-Regular.ttf", 100, nullptr, 0);

    // TEST LayoutBox
    g.root.dimensions = {.rect = {0.0f, 0.0f, window_width, window_height}};
    g.root.text_align = TextAlign::CENTER;
    TextBox *t = g.root.push_child<TextBox>(heading<1>("Hikes", g.font));
    t->dimensions.border.set_uniform(10.0f);
    t->dimensions.border.color = RED;
    t->dimensions.margin.set_uniform(10.0f);
    t->dimensions.margin.color = YELLOW;
    t->dimensions.padding.set_uniform(10.0f);
    t->dimensions.padding.color = GREEN;
    // g.root.children.back()->text_align = TextAlign::RIGHT;

    g.root.push_child<TextBox>(heading<3>("Lakes Trail", g.font));
    g.root.children.back()->text_align = TextAlign::LEFT;
    g.root.children.back()->text_color = RED;
    g.root.push_child<TextBox>(heading<3>("General Sherman Loop", g.font));
    g.root.push_child<TextBox>(heading<1>("Backcountry", g.font));
    // paragraph
    ParagraphBox *p = g.root.create_child<ParagraphBox>(
        "Placeholder text is temporary text used to indicate where actual "
        "content should go, often seen in website forms or design mockups. It "
        "helps designers and developers focus on layout and visual elements "
        "without being distracted by the content itself. A common example "
        "is \" Lorem ipsum, \" a pseudo-Latin text used to simulate the "
        "appearance of text. ",
        g.font,
        23.0f);
    p->dimensions.margin.set_uniform(25.0f);
    p->dimensions.margin.color = YELLOW;

    p = g.root.create_child<ParagraphBox>(
        "Placeholder text is temporary text used to indicate where actual "
        "content should go, often seen in website forms or design mockups. It "
        "helps designers and developers focus on layout and visual elements "
        "without being distracted by the content itself. A common example "
        "is \" Lorem ipsum, \" a pseudo-Latin text used to simulate the "
        "appearance of text. ",
        g.font,
        23.0f);
    p->dimensions.margin.set_uniform(45.0f);
    p->dimensions.margin.color = RED;
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
    UnloadImage(g.icon);
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
