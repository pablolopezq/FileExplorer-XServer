#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <unistd.h>
#include <X11/Xlib.h>
#include <iostream>
#include <cstring>
#include <utility>

#include "filesystem_ops.h"
#include "structs.h"

#include "App.h"

using namespace std;
 
Display *display;
Window root_window;
int screen_num;
int screen_width;
int screen_height;
unsigned long white_pixel;
unsigned long black_pixel;

Window win;
int win_width;
int win_height;
int win_x;
int win_y;
int win_border_width;

GC gc;
XGCValues values;
unsigned long valuemask;

XKeyEvent event;

tree<string> tr;
int text_y, text_x;

void create_window(){

    display = XOpenDisplay(NULL);
    if (display == NULL) {
        fprintf(stderr, "Cannot connect to X server\n");
        exit (-1);
    }

    screen_num = DefaultScreen(display);
    root_window = RootWindow(display, screen_num);
    white_pixel = WhitePixel(display, screen_num);
    black_pixel = BlackPixel(display, screen_num);
    win_width = 1280;
    win_height = 720;

    win_x = win_y = 0;
    win_border_width = 2;

    win = XCreateSimpleWindow(display,
                          root_window,
                          win_x, win_y,
                          win_width, win_height,
                          win_border_width, white_pixel,
                          white_pixel);

    XStoreName(display, win, "File Explorer | Pablo");
    XSelectInput(display, win, ExposureMask | ButtonPressMask);


    XMapWindow(display, win);
    XSync(display, 0);

}

bool check_click(int x, int y, vector<side_text> & side_texts){
    cout << side_texts.size() << endl;
    for(int i = 0; i < side_texts.size(); i++){
        side_text st = side_texts.at(i);
        cout << "Checking for collision on " << st.text << " button\n";
        if(x >= st.left && x <= st.right && y >= st.top && y <= st.bottom){
            cout << "Click on " << st.text << " button\n";
        }
    }
}

void draw(vector<side_text> & side_texts){

    Font font = XLoadFont(display, "10x20");
    XFontStruct * fontinfo = XLoadQueryFont(display, "10x20");

    values.cap_style = CapButt;
    values.join_style = JoinBevel;
    valuemask = GCCapStyle | GCJoinStyle;

    gc = XCreateGC(display, win, valuemask, &values);
    if (gc < 0) {
        fprintf(stderr, "XCreateGC: \n");
    }

    XSetForeground(display, gc, BlackPixel(display, screen_num));
    XSetBackground(display, gc, WhitePixel(display, screen_num));
    XSetFillStyle(display, gc, FillSolid);
    XSetLineAttributes(display, gc, 2, LineSolid, CapRound, JoinRound);
    XSetFont(display, gc, fontinfo->fid);

    tree<string>::iterator loc;
    string side_path = "Clases";
    loc = find(tr.begin(), tr.end(), side_path);

    int mouse_x, mouse_y;
    int cont;

    while(1)
    {
        XNextEvent(display, (XEvent *)&event);

        switch(event.type) {
        case Expose:
            //XDrawRectangle(display, win, gc, 120, 150, 50, 60);
            //XFillRectangle(display, win, gc, 120, 150, 50, 60);

            XDrawLine(display, win, gc, 250, 0, 250, 720);

            // XDrawString(display, win, gc, 10, 50, "test", 4);
            // XDrawString(display, win, gc, 10, 75, "test", 4);
            // XDrawString(display, win, gc, 10, 100, "test", 4);

            text_y = 50;
            text_x = 30;

            if(loc != tr.end()) {

                tree<string>::sibling_iterator sib = tr.begin(loc);

                while(sib != tr.end(loc)) {
                    //cout << (*sib) << endl;

                    string dir_name = (*sib);
                    int dir_name_length = strlen(dir_name.c_str());

                    XDrawString(display, win, gc, text_x, text_y, dir_name.c_str(), dir_name_length);

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

                    side_texts.push_back(st);

                    // cout << "Added collision box on \n" << st.top << " -- Top\n" 
                    //                                     << st.bottom << " -- Bottom\n" 
                    //                                     << st.left << " -- Left\n" 
                    //                                     << st.right << " -- Right\n" 
                    //                                     << "For text: " << st.text << endl;

                    text_y += 25;

                    ++sib;
                }

                cout << "Vector size " << side_texts.size() << endl;                

            }

            break;

        case ButtonPress:

            cout << side_texts.size() << endl;

            mouse_x = event.x;
            mouse_y = event.y;

            cout << "Click on [" << mouse_x << "," << mouse_y << "]\n";

            check_click(mouse_x, mouse_y, side_texts);

            break;

        default:
            break;
        }
    }



    //XSync(display, 0);

}

void init(){

    vector<side_text> side_texts;

    create_window();
    draw(side_texts);
}

void close(){
    XCloseDisplay(display);
}

void load_directories(){

    tree<string>::iterator loc, root;

    root = tr.begin();

    string path = "/home/pablo/Documents/Clases";

    tr.insert(root, get_file_name(path));

    get_directories(path, tr);
}

int main (int argc, char *argv[])
{

    App app;

    app.create_window(720, 1280, "File Explorer | Pablo");
    app.set_gc("10x20");
    app.load_directories();
    app.draw_loop();

    sleep(10);

    app.close_window();

    // load_directories();

    // init();
    // sleep(5);
    // close();

    // Display                 *display;
    // Visual                  *visual;
    // int                     depth;
    // int                     text_x;
    // int                     text_y;
    // XSetWindowAttributes    frame_attributes;
    // Window                  frame_window;
    // XFontStruct             *fontinfo;
    // XGCValues               gr_values;
    // GC                      graphical_context;
    // XKeyEvent               event;
    // char                    hello_string[] = "Sistemas Operativos 2";
    // int                     hello_string_length = strlen(hello_string);
 
    // display = XOpenDisplay(NULL);
    // visual = DefaultVisual(display, 0);
    // depth  = DefaultDepth(display, 0);
     
    // frame_attributes.background_pixel = XWhitePixel(display, 0);
    // /* create the application window */
    // frame_window = XCreateWindow(display, XRootWindow(display, 0),
    //                              0, 0, 800, 600, 5, depth,
    //                              InputOutput, visual, CWBackPixel,
    //                              &frame_attributes);
    // XStoreName(display, frame_window, "File Explorer - Pablo");
    // XSelectInput(display, frame_window, ExposureMask | StructureNotifyMask);
 
    // fontinfo = XLoadQueryFont(display, "10x20");
    // gr_values.font = fontinfo->fid;
    // gr_values.foreground = XBlackPixel(display, 0);
    // graphical_context = XCreateGC(display, frame_window,
    //                               GCFont+GCForeground, &gr_values);
    // XMapWindow(display, frame_window);
 
    // while ( 1 ) {
    //     XNextEvent(display, (XEvent *)&event);
    //     switch ( event.type ) {
    //         case Expose:
    //         {
    //             XWindowAttributes window_attributes;
    //             int font_direction, font_ascent, font_descent;
    //             XCharStruct text_structure;
    //             XTextExtents(fontinfo, hello_string, hello_string_length,
    //                          &font_direction, &font_ascent, &font_descent,
    //                          &text_structure);
    //             XGetWindowAttributes(display, frame_window, &window_attributes);
    //             text_x = (window_attributes.width - text_structure.width)/2;
    //             text_y = (window_attributes.height -
    //                       (text_structure.ascent+text_structure.descent))/2;
    //             XDrawString(display, frame_window, graphical_context,
    //                         text_x, text_y, hello_string, hello_string_length);
    //             break;
    //         }
    //         default:
    //             break;
    //     }
    // }
    return(0);
}