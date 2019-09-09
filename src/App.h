#include <unistd.h>
#include <X11/Xlib.h>
#include <iostream>
#include <string>
#include <vector>

#include "tree.h"
#include "structs.h"

class App{

    Window window, root_window;
    Display * display;
    GC graphical_context;
    XGCValues values;
    XFontStruct * fontinfo;
    XKeyEvent event;

    tree<std::string> tr;
    tree<string>::iterator loc;

    std::string side_path = "Clases";
    std::string active_folder;
    std::string active_font;

    int window_height,
        window_width,
        screen_number,
        text_x, text_y,
        mouse_x, mouse_y;

    unsigned long white_pixel, black_pixel, valuemask;

public:
    void create_window(int win_height, int win_width, const char * name);
    void close_window();
    void set_gc(const char * font);
    void draw_loop();

    void draw_buttons(std::vector<side_text> & collision_boxes);
    void draw_main_folders(std::vector<side_text> & collision_boxes);

    void load_directories();
    bool check_click(int x, int y, std::vector<side_text> &side_texts);

};