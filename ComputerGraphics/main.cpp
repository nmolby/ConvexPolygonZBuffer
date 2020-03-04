//
//  main.cpp
//  
//
//  Created by David M. Reed on 2/18/15.
//  Copyright (c) 2015 David Reed. All rights reserved.
//

#include <iostream>

using std::cout;
using std::cin;
using std::cerr;
using std::endl;
using std::string;

#include "graphics.hpp"
#include "ConvexPolygonRenderer.hpp"

//----------------------------------------------------------------------

int main(int argc, const char * argv[]) {

    string filename;
    if (argc > 1) {
        filename = argv[1];
    }
    else {
        cout << "enter filename: ";
        cin >> filename;
    }

    // create the class
    Renderer *renderer = new ConvexPolygonRenderer("ConvexPolygon", 960, 540, filename);

    // and enter the run loop
    renderer->runLoop();
}
