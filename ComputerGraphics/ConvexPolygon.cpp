//
//  ConvexPolygon.cpp
//  ConvexPolygon
//
//  Created by David M. Reed on 11/24/19.
//  Copyright © 2019 David M Reed. All rights reserved.
//

#include "ConvexPolygonRenderer.hpp"
#include "ConvexPolygon.hpp"

ConvexPolygon::ConvexPolygon(const std::vector<Point3D> &pts, const Color &color, const float scaleX, const float scaleY, const float translateX, const float translateY, const float theta) {
    set(pts, color, scaleX, scaleY, translateX, translateY, theta);
}

void ConvexPolygon::set(const std::vector<Point3D> &pts, const Color &color, const float scaleX, const float scaleY, const float translateX, const float translateY, const float theta) {
    _pts = pts;
    _color = color;
    _scaleX = scaleX;
    _scaleY = scaleY;
    _translateX = translateX;
    _translateY = translateY;
    _theta = theta;
}

Point3D ConvexPolygon::centerPoint() const {
    Point3D center;
    // sum coordinates of points and divide by number of points to find center
    auto numPoints = _pts.size();
    for (auto i=0; i<numPoints; ++i) {
        center.x += _pts[i].x;
        center.y += _pts[i].y;
        center.z += _pts[i].z;
    }
    center.x /= numPoints;
    center.y /= numPoints;
    center.z /= numPoints;
    return center;
}

void ConvexPolygon::render(ConvexPolygonRenderer *renderer) {

    Point3D center = centerPoint();
    // note last matrix is applied to point first
    // so we first translate by -center so it is centered at (0, 0)
    // next apply scale and rotate
    // translate back to original point
    // apply translation
    mat4 transform = Translate(_translateX, _translateY, 0) * Translate(center.x, center.y, 0) * RotateZ(_theta) * Scale(_scaleX, _scaleY, 1.0) * Translate(-center.x, -center.y, 0.0);

    // transform each point and put in vector of <vec4>
    Point3D p;
    std::vector<vec4> transformedPts;
    auto numPoints = _pts.size();
    for (auto i=0; i<numPoints; ++i) {
        vec4 v = vec4(_pts[i].x, _pts[i].y, _pts[i].z, 1.0);
        v = transform * v;
        transformedPts.push_back(v);
    }
    // scanline fill the polygon
    _polygonFill(renderer, transformedPts);
}

void ConvexPolygon::_polygonFill(ConvexPolygonRenderer *renderer, std::vector<vec4> transformedPts) {

    // find min and max y vertices of polygon
    float floatMinY, floatMaxY;
    floatMinY = floatMaxY = transformedPts[0].y;
    auto numPoints = transformedPts.size();
    for (auto i=1; i<numPoints; ++i) {
        auto y = transformedPts[i].y;
        if (y < floatMinY) {
            floatMinY = y;
        }
        else if (y > floatMaxY) {
            floatMaxY = y;
        }
    }
    int minY = int(floatMinY + 0.5);
    int maxY = int(floatMaxY + 0.5);

    // list of points to draw
    std::vector<Point3D> fillPts;

    // vector for the intersection points for a given y value
    std::vector<int> intersections;
    for (auto y=minY; y<=maxY; ++y) {
        // clear out the intersections from previous y value
        intersections.clear();

        // for each edge
        auto p0 = transformedPts[numPoints - 1];
        for (auto i=0; i<numPoints; ++i) {
            auto p1 = transformedPts[i];
            int x;

            // else if one point above y and one point below y
            if ((p0.y <= y and y <= p1.y) or (p0.y >= y and y >= p1.y)) {
                // if horizontal line since we need to divide by y differences for parametric equation
                if (fabs(p0.y - p1.y) < 0.001) {
                    x = int(p0.x + 0.5);
                    intersections.push_back(x);
                    x = int(p1.x + 0.5);
                    intersections.push_back(x);
                }
                else {
                    //  use parametric equation to find intersection point
                    auto t = (y - p0.y) / (p1.y - p0.y);
                    x = int(p0.x + t * (p1.x - p0.x) + 0.5);
                    intersections.push_back(x);
                }
            }
            // move to next pair of points
            p0 = p1;
        }

        // fill points between the intersection points
        if (intersections.size() >= 1) {
            // may be more than two intersection points if hits vertices
            // so find min and max x for the y
            int minX = intersections[0];
            int maxX = intersections[0];
            for (auto j=1; j<intersections.size(); ++j) {
                if (intersections[j] < minX) {
                    minX = intersections[j];
                }
                else if (intersections[j] > maxX) {
                    maxX = intersections[j];
                }
            }
            // add points to fill between them
            for (int x=minX; x<=maxX; ++x) {
                fillPts.push_back(Point3D(x, y, 0.0));
            }
        }
    }
    _renderID = renderer->addPoints(fillPts, _color, 1.0, mat4());
}

std::istream& operator>>(std::istream &is, ConvexPolygon &polygon) {
    std::vector<Point3D> pts;
    Point3D p;
    float x, y, r, g, b;
    float sx, sy, tx, ty, theta;
    int numPts;

    // read number of points
    is >> numPts;
    // read each point
    for (int i=0; i<numPts; ++i) {
        is >> x >> y;
        p.x = x;
        p.y = y;
        pts.push_back(p);
    }

    // read color
    is >> r >> g >> b;
    // read scale, translate, and rotate
    is >> sx >> sy >> tx >> ty >> theta;

    Color color(r, g, b);
    polygon.set(pts, color, sx, sy, tx, ty, theta);
    return is;
}
