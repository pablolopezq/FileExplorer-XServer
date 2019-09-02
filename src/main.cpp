#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <unistd.h>
#include <X11/Xlib.h>
#include <iostream>
#include <cstring>

#include "filesystem_ops.h"

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
int text_y;

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


void draw(){

    Font font = XLoadFont(display, "10x20");

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
    XSetFont(display, gc, font);

    // XDrawPoint(display, win, gc, 5, 5);

    // XDrawLine(display, win, gc, 100, 100, 0, 100);

    // // int x = 30, y = 40;
    // // int h = 15, w = 45;
    // // int angle1 = 0, angle2 = 2.109;
    // // XDrawArc(display, win, gc, x-(w/2), y-(h/2), w, h, angle1, angle2);

    // // XDrawArc(display, win, gc, 50-(15/2), 100-(15/2), 15, 15, 0, 360*64);

    //   {
    //     XPoint points[] = {
    //       {0, 0},
    //       {15, 15},
    //       {0, 15},
    //       {0, 0}
    //     };
    //     int npoints = sizeof(points)/sizeof(XPoint);

    //     XDrawLines(display, win, gc, points, npoints, CoordModeOrigin);
    //   }

    // XDrawRectangle(display, win, gc, 120, 150, 50, 60);
    // XFillRectangle(display, win, gc, 120, 150, 50, 60);

    int x = 30, y = 40;
    int h = 15, w = 45;
    int angle1 = 0, angle2 = 2.109;

    tree<string>::iterator loc;
    string side_path = "Clases";
    loc = find(tr.begin(), tr.end(), side_path);

    while(1)
    {
        XNextEvent(display, (XEvent *)&event);

        switch(event.type) {
        case Expose:
        
            //XDrawRectangle(display, win, gc, 120, 150, 50, 60);
            //XFillRectangle(display, win, gc, 120, 150, 50, 60);

            XDrawLine(display, win, gc, 200, 0, 200, 720);

            // XDrawString(display, win, gc, 10, 50, "test", 4);
            // XDrawString(display, win, gc, 10, 75, "test", 4);
            // XDrawString(display, win, gc, 10, 100, "test", 4);

            text_y = 50;

            if(loc != tr.end()) {
                tree<string>::sibling_iterator sib = tr.begin(loc);
                while(sib != tr.end(loc)) {
                    cout << (*sib) << endl;

                    string dir_name = (*sib);
                    int dir_name_length = strlen(dir_name.c_str());

                    XDrawString(display, win, gc, 10, text_y, dir_name.c_str(), dir_name_length);

                    text_y += 25;

                    ++sib;
                }
            }

            //XDrawArc(display, win, gc, x-(w/2), y-(h/2), w, h, angle1, angle2);
            //XDrawArc(display, win, gc, 50-(15/2), 100-(15/2), 15, 15, 0, 360*64);

            break;

        default:
            break;
        }
    }



    //XSync(display, 0);

}

void init(){
    create_window();
    draw();
}

void close(){
    XCloseDisplay(display);
}

void load_directories(){

    tree<string>::iterator loc, root;

    root = tr.begin();

    string path = "/home/pablo/Documents/Clases";

    tr.insert(root, getFileName(path));

    get_directories(path, tr);

    cout << endl;

    loc=find(tr.begin(), tr.end(), getFileName(path));
    if(loc != tr.end()) {
        tree<string>::sibling_iterator sib = tr.begin(loc);
        while(sib != tr.end(loc)) {
            cout << (*sib) << endl;
            ++sib;
        }
        // cout << endl;
        // tree<string>::iterator sib2=tr.begin(loc);
        // tree<string>::iterator end2=tr.end(loc);
        // while(sib2!=end2) {
        //     for(int i=0; i<tr.depth(sib2) - 1; ++i)
        //         cout << " ";
        
        //     cout << (*sib2) << endl;
        //     ++sib2;
        // }
    }
}

int main (int argc, char *argv[])
{

    load_directories();

    init();
    sleep(5);
    close();

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