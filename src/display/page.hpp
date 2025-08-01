#ifndef DISPLAY_PAGE_HPP
#define DISPLAY_PAGE_HPP

#include <raylib.h>

class Page {
  public:
    Page();
    ~Page();

    void draw();

  private:
    Texture framebuffer;
};

#endif // DISPLAY_PAGE_HPP
