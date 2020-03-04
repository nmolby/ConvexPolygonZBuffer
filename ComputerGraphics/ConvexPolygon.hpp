//
//  ConvexPolygon.hpp
//  ConvexPolygon
//
//  Created by David M. Reed on 11/24/19.
//  Copyright © 2019 David M Reed. All rights reserved.
//

#ifndef ConvexPolygon_hpp
#define ConvexPolygon_hpp

#include <iostream>

#include "graphics.hpp"

class ConvexPolygonRenderer;

class ConvexPolygon {

public:
    /// set ConvexPolygon to specified value
    /// @param pts vector of coordinate points (must form a convex polygon)
    /// @param color color for polygon
    /// @param scaleX x-scale factor
    /// @param scaleY y-scale factor
    /// @param translateX x-translate amount
    /// @param translateY y-translate amount
    /// @param theta - rotate Z amount in degrees
    ConvexPolygon(const std::vector<Point3D> &pts = std::vector<Point3D>(), const Color &color = Color(0, 0, 0), const float scaleX = 1.0, const float scaleY = 1.0, const float translateX = 0.0, const float translateY = 0.0, const float theta = 0.0);

    /// set ConvexPolygon to specified value
    /// @param pts vector of coordinate points (must form a convex polygon)
    /// @param color color for polygon
    /// @param scaleX x-scale factor
    /// @param scaleY y-scale factor
    /// @param translateX x-translate amount
    /// @param translateY y-translate amount
    /// @param theta - rotate Z amount in degrees
    void set(const std::vector<Point3D> &pts, const Color &color, const float scaleX = 1.0, const float scaleY = 1.0, const float translateX = 0.0, const float translateY = 0.0, const float theta = 0.0);

    /// returns center point of ConvexPolygon
    Point3D centerPoint() const;

    /// sets ConvexPolygon up to be drawn as a filled polygon for its coordinate and color
    /// @param renderer the Renderer to use to draw the polygon
    void render(ConvexPolygonRenderer *renderer);

private:
    void _polygonFill(ConvexPolygonRenderer *renderer, std::vector<vec4> transformedPts);

    std::vector<Point3D> _pts;
    Color _color;
    float _scaleX, _scaleY;
    float _translateX, _translateY;
    float _theta;
    size_t _renderID;
};

/// input operator for ConvexPolygon
/// @param is input stream
/// @param polygon poylgon to store data from stream in
std::istream& operator>>(std::istream &is, ConvexPolygon &polygon);

#endif /* ConvexPolygon_hpp */
