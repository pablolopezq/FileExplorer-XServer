#include <X11/Xlib.h>
#include <vector>

#include "tree.h"
#include "Structs.h"

using std::vector;

class App{

private:

    Window window;
    Display * display;
    GC graphical_context;
    XGCValues values;
    XFontStruct * fontinfo;
    XKeyEvent event;

public:

    unsigned long valuemask;
    int window_height, window_width;
    int screen_number;
    tree<std::string>::iterator main_panel_loc;
    tree<std::string> file_tree;
    string active_folder;
    vector<CollisionBox*> collision_boxes;

    App() : active_folder("Documents") { }

    void create_window(int height, int width, const char * name);
    void create_graphical_context();
    void set_font(const char * font);
    void draw_loop();
    void draw_buttons();
    void draw_side_panel();
    void draw_main_panel();

    void check_click(int x, int y);
    void load_directories();

};