#include <X11/Xlib.h>
#include <vector>
#include <array>

#include "tree.h"
#include "Structs.h"

using std::vector;

class App{

public:

    Window window;
    Display * display;
    GC graphical_context;
    XGCValues values;
    XFontStruct * fontinfo;
    XKeyEvent event;

    unsigned long valuemask;
    int window_height, window_width;
    int screen_number;
    tree<std::string>::iterator main_panel_loc;
    tree<std::string> file_tree;
    std::vector<CollisionBox> collision_boxes;
    string active_folder;

    // App() : active_folder("Documents") { }

    void create_window(int height, int width, const char * name);
    void create_graphical_context();
    void set_font(const char * font);
    void event_loop();
    void draw_buttons();
    void draw_side_panel();
    void draw_main_panel();

    void init();
    void create_buttons();
    void draw();

    void check_click(int x, int y);
    void load_directories();

};