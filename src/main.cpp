#include <raylib.h>

#include <iostream>

#include "layout.hpp"
#include "parser/parse_css.hpp"
#include "parser/parse_html.hpp"
#include "style.hpp"
#include "util/file.hpp"
#include "util/image_download.hpp"
#include "util/types.hpp"

#ifdef PLATFORM_WEB
#include <emscripten/emscripten.h>
#endif

static constexpr i32 window_width = 1600, window_height = 900;

struct Global {
    Font font;
    LayoutBox *root = nullptr;
    Image icon;
} g;

int s = 1;
void print_nodes(const Node *r) {
    std::cout << r->s << ": " << (int)r->type << std::endl;
    std::cout << "children: " << r->children.size() << std::endl;
    for (const Node *c : r->children) {
        s++;
        std::cout << "  ";
        print_nodes(c);
    }
}

void demo() {
    g.root = new LayoutBox;
    // TEST LayoutBox
    g.root->dimensions = {.rect = {0.0f, 0.0f, window_width, 0.0f}};
    g.root->text_align = TextAlign::CENTER;
    TextBox *t = g.root->push_child<TextBox>(heading(1, "Hikes", g.font));
    t->dimensions.border.set_uniform(10.0f);
    t->dimensions.border.color = RED;
    t->dimensions.margin.set_uniform(10.0f);
    t->dimensions.margin.color = YELLOW;
    t->dimensions.padding.set_uniform(10.0f);
    t->dimensions.padding.color = GREEN;
    // g.root->children.back()->text_align = TextAlign::RIGHT;

    g.root->push_child<TextBox>(heading(3, "Lakes Trail", g.font));
    g.root->children.back()->text_align = TextAlign::LEFT;
    g.root->children.back()->text_color = RED;
    g.root->push_child<TextBox>(heading(3, "General Sherman Loop", g.font));
    g.root->push_child<TextBox>(heading(1, "Backcountry", g.font));
    // paragraph
    ParagraphBox *p = g.root->create_child<ParagraphBox>(
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
    p->push_child<TextBox>(heading(2, "Text", g.font));
    p->children[0]->dimensions.padding.set_uniform(10.0f);
    p->children[0]->dimensions.padding.color = BLUE;

    p = g.root->create_child<ParagraphBox>(
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
    p->text_align = TextAlign::RIGHT;
    g.root->dimensions.padding.set_uniform(50.0f);
    g.root->dimensions.padding.color = GREEN;

    auto *img = g.root->create_child<ImageBox>(
        "https://cdn.mos.cms.futurecdn.net/Nx5uwKj9tzBi43ZCB7TakX.jpg");
    img->texture = load_texture(
        "https://cdn.mos.cms.futurecdn.net/Nx5uwKj9tzBi43ZCB7TakX.jpg");

    g.root->construct_dimensions();
}

void init() {
    std::string source = load_file("./tests/index.html");
    std::cout << source << std::endl;
    HTMLParser parser(source);
    Node *r = parser.parse();
    print_nodes(r);
    std::cout << s << std::endl;

    // CSS
    CSSParser css_parser(load_file("./tests/index.css"));
    StyleSheet *style_sheet = css_parser.parse();
    for (Rule &r : style_sheet->rules) {
        std::cout << "Rule:------\n" << r << "\n";
    }

    // load icon
    g.icon = LoadImage("./res/icon.png");
    SetWindowIcon(g.icon);

    // load font
    // load big for better aliasing
    g.font = LoadFontEx(
        "./res/Open_Sans/static/OpenSans-Regular.ttf", 100, nullptr, 0);

    g.root = generate_layout(r, g.font);
    apply_style(g.root, *style_sheet);
    g.root->dimensions.rect.width = window_width;
    g.root->construct_dimensions();

    // TEST: download_image
    init_downloader();
    // download_image(
    //     "https://cdn.mos.cms.futurecdn.net/Nx5uwKj9tzBi43ZCB7TakX.jpg",
    //     "./res/server/save.png");

    // demo();

    delete style_sheet;
}

void input(f32 dt) {}

void update(f32 dt) {}

void render(f32 dt) {
    BeginDrawing();
    ClearBackground(WHITE);

    g.root->render();

    EndDrawing();
}

void quit() {
    if (g.root != nullptr) delete g.root;
    UnloadFont(g.font);
    UnloadImage(g.icon);
    CloseWindow();
    quit_downloader();
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
