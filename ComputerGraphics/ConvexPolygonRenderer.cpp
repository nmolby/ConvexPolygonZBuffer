//
//  ConvexPolygonRenderer.cpp
//  ComputerGraphics
//
//  Created by David M. Reed on 1/28/20.
//  Copyright © 2020 David M Reed. All rights reserved.
//

#include <fstream>
#include "ConvexPolygon.hpp"
#include "ConvexPolygonRenderer.hpp"

ConvexPolygonRenderer::ConvexPolygonRenderer(std::string windowTitle, int width, int height, std::string filename) : Renderer(windowTitle, width, height) {
    
    std::ifstream infile(filename.c_str());
    ConvexPolygon polygon;
    _zBuffer.init(height, width);
    for(int y = 0; y < height; y++){
        for(int x = 0; x < width; x++){
            _zBuffer[y][x] = INFINITY;
        }
    }

    // while not end of file read ConvexPolygon objects from file stream
    while (infile >> polygon) {
        // render it
        polygon.render(this);
        // store it in case we want to use it in the future
        _convexPolygons.push_back(polygon);
    }
    infile.close();
}

//overrides Rendered addPoints to utilize zBuffer
size_t ConvexPolygonRenderer::addPoints(const std::vector<Point3D> &pts, const Color &color, float pointSize, const mat4 &objectTransformation) {
    std::vector<Point3D> closePoints;
    
    for(Point3D point: pts){
        //if the new point is closer than the current z value at the (x, y) coordinate,
        //overwrite it and add it to be drawn

        if(_zBuffer[int(point.y + .5)][int(point.x + .5)] > point.z){
            _zBuffer[int(point.y + .5)][int(point.x + .5)] = point.z;
            closePoints.push_back(point);
        }
    }
    
    //call Rendered addPoints with the new points to be drawn
    return Renderer::addPoints(closePoints, color, pointSize, objectTransformation);
}
