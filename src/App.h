#include <X11/Xlib.h>
#include <X11/Xresource.h>
#include <X11/Xutil.h>
#include <vector>
#include <stack>

#include "tree.h"
#include "Structs.h"

class App{

public:
    Display * display;

    unsigned long valuemask;
    int window_height, window_width;
    int screen_number;
    tree<std::string> file_tree;
    std::vector<CollisionBox> collision_boxes;
    std::vector<CollisionBox> folders;
    std::stack<std::string>previous;
    std::string active;
    std::vector<Entry> active_entries;
    bool deleting = false;
    bool getting_input, c_folder, c_file, c_symlink, c_link, cut, pasting;
    std::string input_str;
    KeySym id;
    char data[1];
    std::string to_copy;

    Window window;
    GC graphical_context;
    Font font;
    XGCValues values;
    XKeyEvent event;
    // XFontStruct * fontinfo;

    // App() : active_folder("Documents") { }

    void create_window(int height, int width, const char * name);
    void create_graphical_context();
    void set_font(const char * font);
    void event_loop();
    void draw_side_panel();
    void draw_main_panel();

    void init();
    void get_folders(std::string folder);
    void create_buttons();
    void draw();
    void draw_buttons();
    void draw_folders();
    void get_input();
    void set_highlight_off();
    bool is_highlighted();
    Entry& get_highlighted();

    void check_click(int x, int y);
    void load_directories();

};