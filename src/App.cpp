#include "App.h"
#include "FilesystemOps.h"

#include <iostream>
#include <cstring>

using std::cout;
using std::endl;

void App::create_window(int height, int width, const char * name){

    // active_folder = "Documents";

    window_height = height;
    window_width = width;

    display = XOpenDisplay(NULL);
    if (display == NULL) {
        fprintf(stderr, "Cannot connect to X server\n");
        exit (-1);
    }

    screen_number = DefaultScreen(display);
    Window root_window = RootWindow(display, screen_number);
    unsigned long white_pixel = WhitePixel(display, screen_number);
    unsigned long black_pixel = BlackPixel(display, screen_number);

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

void App::init(){
    load_directories();
    create_buttons();
    get_folders("Clases");
    active = "Clases";
}

void App::create_graphical_context(){
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
    font = XLoadFont(display, "10x20");
    XSetFont(display, graphical_context, font);
    XDrawRectangle(display, window, graphical_context, 20, 20, 30, 30);
}

void App::set_font(const char * font_name){
    std::cout << "Loading font: " << font_name << std::endl;
    font = XLoadFont(display, "10x20");
    std::cout << "Loaded font: " << font_name << std::endl;
    std::cout << "Setting font: " << font_name << std::endl;
    XSetFont(display, graphical_context, font);
    std::cout << "Set font: " << font_name << std::endl;
}

void App::event_loop(){

    // cout << "Entered event loop\n";

    while(1){

        // cout << "Waiting for event...\n";

        XNextEvent(display, (XEvent *)&event);

        // cout << "Event caught\n";

        switch(event.type){
            case Expose:
                XClearWindow(display, window);
                // draw_buttons();
                // draw_side_panel();
                //draw_main_panel();
                draw();
                break;
            
            case ButtonPress:
                std::cout << "Click on [" << event.x << "," << event.y << "]\n";
                check_click(event.x, event.y);
                break;
        }

    }

}

void App::check_click(int x, int y){
    
    cout << collision_boxes.size() << endl;

    for(int i = 0; i < collision_boxes.size(); i++){

        CollisionBox box = collision_boxes.at(i);
        
        if(x >= box.left && x <= box.right && y >= box.top && y <= box.bottom){
            
            cout << "Click on " << box.text << " button\n";

            if(box.type == ButtonType::FOLDER_NAV){
                previous.push(active);
                active = box.text;
                get_folders(box.text);
            }

            if(box.type == ButtonType::BACK){
                if(!previous.empty()){
                    std::string go_to = previous.top();
                    previous.pop();
                    active = go_to;
                    get_folders(go_to);
                }
            }

            XEvent event;
            event.type = Expose;
            XSendEvent(display, window, false, ExposureMask, &event);

            break;
        }
    }

    for (int i = 0; i < folders.size(); i++){
        
        CollisionBox box = folders.at(i);
        
        if(x >= box.left && x <= box.right && y >= box.top && y <= box.bottom){
            
            cout << "Click on " << box.text << " button\n";

            if(box.type == ButtonType::FOLDER_NAV){
                previous.push(active);
                active = box.text;
                get_folders(box.text);
            }

            XEvent event;
            event.type = Expose;
            XSendEvent(display, window, false, ExposureMask, &event);

            break;
        }
    }
    
}

void App::create_buttons(){

    // Create - Delete buttons
    std::vector<string> buttons{"Create Folder", "Delete Folder", 
                           "Create File", "Delete File"};

    int button_x = 270;
    int button_y = 10;
    int button_width = 150;
    int button_height = 30;

    for(int i = 0; i < 4; i++){

        string name = buttons.at(i);

        CollisionBox box;
        box.text = name;
        box.top = button_y;
        box.left = button_x;
        box.right = button_x + button_width;
        box.bottom = button_y + button_height;

        switch(i){ 
            case 0:
                box.type = ButtonType::CREATE_FOLDER;
                break;
            case 1:
                box.type = ButtonType::DELETE_FOLDER;
                break;
            case 2:
                box.type = ButtonType::CREATE_FILE;
                break;
            case 3:
                box.type = ButtonType::DELETE_FILE;
                break;
        }

        // cout << "Created col box for " << box.text << endl;

        collision_boxes.push_back(box);

        button_x += 200;
    }

    std::vector<string> buttons1{"home", "Desktop", "Documents", "Downloads"};

    button_x = 30;
    button_y = 80;

    for(int i = 0; i < buttons1.size(); i++){

        //cout << "Entered loop\n";

        string name = buttons1.at(i);
        int name_len = strlen(name.c_str());

        // cout << "Drawing: " << name << " with length " << name_len << endl;

        // XDrawString(display, window, graphical_context, button_x, button_y, name.c_str(), name_len);

        CollisionBox box;
        box.text = name;
        box.top = button_y;
        box.left = button_x;
        box.right = button_x + 150;
        box.bottom = button_y + 30;
        box.type = ButtonType::FOLDER_NAV;

        cout << "Added collision box on \n" << box.top << " -- Top\n" 
                                            << box.bottom << " -- Bottom\n" 
                                            << box.left << " -- Left\n" 
                                            << box.right << " -- Right\n" 
                                            << "For text: " << box.text << endl;

        collision_boxes.push_back(box);

        button_y += 30;
    }

    CollisionBox box;
    box.text = "Back";
    box.top = 20;
    box.left = 30;
    box.right = 90;
    box.bottom = 50;
    box.type = ButtonType::BACK;

    collision_boxes.push_back(box);
}

void App::get_folders(std::string folder){

    folders.clear();

    int folder_x = 275;
    int folder_y = 80;
    int text_x = 275;
    int text_y = 150;

    // std::cout << "Getting folders on " << folder << std::endl;

    tree<std::string>::iterator main_panel_loc;

    // set_font("7x14");
    // cout << "Set font\n";

    //active_folder = "Compiladores1";

    cout << "Trying to fetch: " << folder << "...\n";
    main_panel_loc = std::find(file_tree.begin(), file_tree.end(), folder);    
    cout << "Fetched active folder " << folder << endl;;

    if(main_panel_loc != file_tree.end()) {

        // std::cout << "Loading side panel paths...\n";

        tree<string>::sibling_iterator sib = file_tree.begin(main_panel_loc);

        // std::cout << "Aca\n";

        while(sib != file_tree.end(main_panel_loc)) {

            // cout << "Entering loop\n";

            cout << (*sib) << endl;

            string dir_name = (*sib);
            int dir_name_length = strlen(dir_name.c_str());

            CollisionBox box;
            box.text = dir_name;
            box.top = folder_y;
            box.left = folder_x;
            box.right = folder_x + 100;
            box.bottom = folder_y + 50;
            box.type = ButtonType::FOLDER_NAV;

            // cout << "Made struct\n";

            folders.push_back(box);

            // cout << "Added collision box on \n" << box.top << " -- Top\n" 
            //                                     << box.bottom << " -- Bottom\n" 
            //                                     << box.left << " -- Left\n" 
            //                                     << box.right << " -- Right\n" 
            //                                     << "For text: " << box.text << endl;

            text_y += 100;
            folder_y += 100;
            
            // cout << "Increasing iterator...\n";
            ++sib;
            // cout << "Increased iterator\n";
        }
    }
}

void App::draw(){

    draw_buttons();
    draw_folders();    
}

void App::load_directories(){

    tree<std::string>::iterator loc, root;

    root = file_tree.begin();

    std::string path = "/home";

    file_tree.insert(root, get_file_name(path));

    get_directories(path, file_tree);
}

void App::draw_buttons(){

    XDrawLine(display, window, graphical_context, 250, 50, 1280, 50);
    XDrawLine(display, window, graphical_context, 250, 0, 250, 720);

    for (int i = 0; i < collision_boxes.size(); i++){
        
        CollisionBox box = collision_boxes.at(i);

        // cout << "Fetched " << box.text << endl;

        XDrawRectangle(display, window, graphical_context, box.left, box.top, box.right - box.left, box.bottom - box.top);
        XDrawString(display, window, graphical_context, box.left + 10, box.top + 20, box.text.c_str(), strlen(box.text.c_str()));

        // cout << "Drew button " << box.text << endl;
    }

    XDrawLine(display, window, graphical_context, 250, 0, 250, 720);
}

void App::draw_folders(){

    int folder_x = 275;
    int folder_y = 80;
    int text_x = 275;
    int text_y = 150;

    for (int i = 0; i < folders.size(); i++){
        
        CollisionBox box = folders.at(i);

        string dir_name = box.text;
        int dir_name_length = strlen(dir_name.c_str());

        XDrawRectangle(display, window, graphical_context, folder_x, folder_y, 100, 50);

        XDrawString(display, window, graphical_context, text_x, text_y, dir_name.c_str(), dir_name_length);

        text_y += 100;
        folder_y += 100;
    }
    

}

// void App::draw_side_panel(){

//     // cout << "Drawing side panel\n";

//     set_font("10x20");

//     XDrawLine(display, window, graphical_context, 250, 0, 250, 720);

//     vector<string> buttons{"home", "Desktop", "Documents"};

//     int button_x = 30;
//     int button_y = 50;

//     for(int i = 0; i < buttons.size(); i++){

//         //cout << "Entered loop\n";

//         string name = buttons.at(i);
//         int name_len = strlen(name.c_str());

//         // cout << "Drawing: " << name << " with length " << name_len << endl;

//         XDrawString(display, window, graphical_context, button_x, button_y, name.c_str(), name_len);

//         int font_direction, font_ascent, font_descent;
//         XCharStruct text_structure;
//         XTextExtents(fontinfo, name.c_str(), name_len,
//                         &font_direction, &font_ascent, &font_descent,
//                         &text_structure);

//         CollisionBox box;
//         box.text = name;
//         box.top = button_y - fontinfo->max_bounds.ascent;
//         box.left = button_x + fontinfo->min_bounds.lbearing;
//         box.right = (fontinfo->max_bounds.rbearing - fontinfo->min_bounds.lbearing) * name_len;
//         box.bottom = button_y + fontinfo->max_bounds.ascent + fontinfo->max_bounds.descent;

//         // cout << "Created col box for " << box.text << endl;

//         collision_boxes.push_back(box);

//         button_y += 30;
//     }
// }

// void App::draw_main_panel(){

//     //cout << "Entered draw main panel\n";

//     int folder_x = 275;
//     int folder_y = 80;
//     int text_x = 275;
//     int text_y = 150;

//     set_font("7x14");
//     // cout << "Set font\n";

//     //active_folder = "Compiladores1";

//     cout << "Trying to fetch: " << active_folder << "...\n";
//     main_panel_loc = find(file_tree.begin(), file_tree.end(), active_folder);    
//     cout << "Fetched active folder " << active_folder << endl;;

//     if(main_panel_loc != file_tree.end()) {

//         // std::cout << "Loading side panel paths...\n";

//         tree<string>::sibling_iterator sib = file_tree.begin(main_panel_loc);

//         // std::cout << "Aca\n";

//         while(sib != file_tree.end(main_panel_loc)) {
//             cout << (*sib) << endl;

//             string dir_name = (*sib);
//             int dir_name_length = strlen(dir_name.c_str());

//             // std::cout << "Loaded: " << dir_name << std::endl;

//             XDrawRectangle(display, window, graphical_context, folder_x, folder_y, 100, 50);

//             XDrawString(display, window, graphical_context, text_x, text_y, dir_name.c_str(), dir_name_length);

//             int font_direction, font_ascent, font_descent;
//             XCharStruct text_structure;
//             XTextExtents(fontinfo, dir_name.c_str(), dir_name_length,
//                             &font_direction, &font_ascent, &font_descent,
//                             &text_structure);

//             CollisionBox box;
//             box.text = dir_name;
//             box.top = folder_y;
//             box.left = folder_x;
//             box.right = folder_x + 100;
//             box.bottom = folder_y + 50;
//             box.is_folder = true;

//             // cout << "Made struct\n";

//             collision_boxes.push_back(box);

//             // cout << "Added collision box on \n" << st.top << " -- Top\n" 
//             //                                     << st.bottom << " -- Bottom\n" 
//             //                                     << st.left << " -- Left\n" 
//             //                                     << st.right << " -- Right\n" 
//             //                                     << "For text: " << st.text << endl;

//             text_y += 100;
//             folder_y += 100;

//             ++sib;
//         }
//     }
// }
