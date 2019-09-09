#include "App.h"
#include "filesystem_ops.h"

#include <cstring>

void App::create_window(int win_height, int win_width, const char * name){

    active_folder = "Compiladores1";

    window_height = win_height;
    window_width = win_width;

    display = XOpenDisplay(NULL);
    if (display == NULL) {
        fprintf(stderr, "Cannot connect to X server\n");
        exit (-1);
    }

    screen_number = DefaultScreen(display);
    root_window = RootWindow(display, screen_number);
    white_pixel = WhitePixel(display, screen_number);
    black_pixel = BlackPixel(display, screen_number);

    window = XCreateSimpleWindow(display,
                          root_window,
                          0, 0,
                          window_width, window_height,
                          2, black_pixel,
                          white_pixel);

    XStoreName(display, window, name);
    XSelectInput(display, window, ExposureMask | ButtonPressMask);


    XMapWindow(display, window);
    XSync(display, 0);
}

void App::close_window(){
    XCloseDisplay(display);
}

void App::set_gc(const char * font){

    active_font = font;

    values.cap_style = CapButt;
    values.join_style = JoinBevel;
    valuemask = GCCapStyle | GCJoinStyle;

    graphical_context = XCreateGC(display, window, valuemask, &values);
    if (graphical_context < 0) {
        fprintf(stderr, "XCreateGC: \n");
    }

    XSetBackground(display, graphical_context, WhitePixel(display, screen_number));
    XSetForeground(display, graphical_context, BlackPixel(display, screen_number));
    XSetFillStyle(display, graphical_context, FillSolid);
    XSetLineAttributes(display, graphical_context, 2, LineSolid, CapRound, JoinRound);

}

void App::draw_loop(){

    tree<string>::iterator loc;
    std::vector<side_text> collision_boxes;
    string side_path = "Clases";
    loc = find(tr.begin(), tr.end(), side_path);

    while(1)
    {
        XNextEvent(display, (XEvent *)&event);

        fontinfo = XLoadQueryFont(display, active_font.c_str());
        XSetFont(display, graphical_context, fontinfo->fid);

        switch(event.type) {
        case Expose:
            // XDrawRectangle(display, window, graphical_context, 120, 150, 50, 60);
            // XFillRectangle(display, window, graphical_context, 120, 150, 50, 60);

            XDrawLine(display, window, graphical_context, 250, 0, 250, 720);

            // XDrawString(display, win, gc, 10, 50, "test", 4);
            // XDrawString(display, win, gc, 10, 75, "test", 4);
            // XDrawString(display, win, gc, 10, 100, "test", 4);

            text_y = 50;
            text_x = 30;

            if(loc != tr.end()) {

                std::cout << "Loading side panel paths...\n";

                tree<string>::sibling_iterator sib = tr.begin(loc);

                std::cout << "Aca\n";

                while(sib != tr.end(loc)) {
                    cout << (*sib) << endl;

                    string dir_name = (*sib);
                    int dir_name_length = strlen(dir_name.c_str());

                    std::cout << "Loaded: " << dir_name << std::endl;

                    XDrawString(display, window, graphical_context, text_x, text_y, dir_name.c_str(), dir_name_length);

                    int font_direction, font_ascent, font_descent;
                    XCharStruct text_structure;
                    XTextExtents(fontinfo, dir_name.c_str(), dir_name_length,
                                 &font_direction, &font_ascent, &font_descent,
                                 &text_structure);

                    side_text st;
                    st.text = dir_name;
                    st.top = text_y - fontinfo->max_bounds.ascent;
                    st.left = text_x + fontinfo->min_bounds.lbearing;
                    st.right = (fontinfo->max_bounds.rbearing - fontinfo->min_bounds.lbearing) * dir_name_length;
                    st.bottom = text_y + fontinfo->max_bounds.ascent + fontinfo->max_bounds.descent;

                    cout << "Made struct\n";

                    collision_boxes.push_back(st);

                    cout << "Added collision box on \n" << st.top << " -- Top\n" 
                                                        << st.bottom << " -- Bottom\n" 
                                                        << st.left << " -- Left\n" 
                                                        << st.right << " -- Right\n" 
                                                        << "For text: " << st.text << endl;

                    text_y += 25;

                    ++sib;
                }

                cout << "Vector size " << collision_boxes.size() << endl;
            }
            
            draw_buttons(collision_boxes);
            //draw_main_folders(collision_boxes);

            break;

        case ButtonPress:

            cout << collision_boxes.size() << endl;

            mouse_x = event.x;
            mouse_y = event.y;

            std::cout << "Click on [" << mouse_x << "," << mouse_y << "]\n";

            check_click(event.x, event.y, collision_boxes);

            break;

        default:
            break;
        }

        //XSync(display, 0);
    }
}

void App::load_directories(){

    std::cout << "Loading directories...\n";

    tree<std::string>::iterator loc, root;

    root = tr.begin();

    std::string path = "/home/pablo/Documents/Clases";

    tr.insert(root, get_file_name(path));

    get_directories(path, tr);

    std::cout << "Loaded directories\n";
}

bool App::check_click(int x, int y, vector<side_text> & side_texts){
    //cout << side_texts.size() << endl;
    for(int i = 0; i < side_texts.size(); i++){
        side_text st = side_texts.at(i);
        //cout << "Checking for collision on " << st.text << " button\n";
        if(x >= st.left && x <= st.right && y >= st.top && y <= st.bottom){
            cout << "Click on " << st.text << " button\n";
        }
    }
}

void App::draw_buttons(std::vector<side_text> &collision_boxes){
    XDrawLine(display, window, graphical_context, 250, 50, 1280, 50);

    XDrawRectangle(display, window, graphical_context, 270, 10, 150, 30);
    XDrawString(display, window, graphical_context, 280, 30, "Create Folder", 13);
    side_text side;
    side.text = "Create Folder";
    side.top = 10;
    side.bottom = 40;
    side.left = 270;
    side.right = 420;

    collision_boxes.push_back(side);
}

void App::draw_main_folders(std::vector<side_text> &collision_boxes){

    active_folder = "Clases";
    active_font = "5x7";

    cout << "Loading font: " << active_font << "...\n";

    fontinfo = XLoadQueryFont(display, active_font.c_str());
    XSetFont(display, graphical_context, fontinfo->fid);

    cout << "Loaded font\n";
    cout << "Trying to fetch: " << active_folder << "...\n";

    tree<string>::iterator loc;
    loc = find(tr.begin(), tr.end(), active_folder);
    
    cout << "Fetched active folder\n";

    if(loc != tr.end()) {

        std::cout << "Loading side panel paths...\n";

        tree<string>::sibling_iterator sib = tr.begin(loc);

        std::cout << "Aca\n";

        while(sib != tr.end(loc)) {
            cout << (*sib) << endl;

            string dir_name = (*sib);
            int dir_name_length = strlen(dir_name.c_str());

            std::cout << "Loaded: " << dir_name << std::endl;

            XDrawString(display, window, graphical_context, text_x, text_y, dir_name.c_str(), dir_name_length);

            int font_direction, font_ascent, font_descent;
            XCharStruct text_structure;
            XTextExtents(fontinfo, dir_name.c_str(), dir_name_length,
                            &font_direction, &font_ascent, &font_descent,
                            &text_structure);

            side_text st;
            st.text = dir_name;
            st.top = text_y - fontinfo->max_bounds.ascent;
            st.left = text_x + fontinfo->min_bounds.lbearing;
            st.right = (fontinfo->max_bounds.rbearing - fontinfo->min_bounds.lbearing) * dir_name_length;
            st.bottom = text_y + fontinfo->max_bounds.ascent + fontinfo->max_bounds.descent;

            cout << "Made struct\n";

            collision_boxes.push_back(st);

            cout << "Added collision box on \n" << st.top << " -- Top\n" 
                                                << st.bottom << " -- Bottom\n" 
                                                << st.left << " -- Left\n" 
                                                << st.right << " -- Right\n" 
                                                << "For text: " << st.text << endl;

            text_y += 25;

            ++sib;
        }
    }
}