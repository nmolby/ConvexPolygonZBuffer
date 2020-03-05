#ifndef Array2D_hpp
#define Array2D_hpp

#include <memory>

// MARK: version 3 smart pointers as one dimensional array

// class for representing a 2D array of data
class Array2D {
public:
    /// initialize two-dimensional array with specified size
    /// @param numRows number of rows in 2D array
    /// @param numColumns number of columns in 2D array
    Array2D(int numRows = 0, int numColumns = 0);

    /// helper method for constructor or allows you to reconstruct with different size
    /// @param numRows number of rows in 2D array
    /// @param numColumns number of columns in 2D array
    void init(int numRows, int numColumns);

    /// returns the address of the first element in the row
    /// thus we could write:
    /// float *row = array2D[rowNumber];
    /// and since pointers can be treated as arrays we can now write
    /// float x = row[columnNumber];
    /// or in one step array2D[rowNumber][columnNumber]
    /// note if columnNumber is >= _numColumns you are accessing elements in later rows
    float* operator[](const int row) const;

private:
    Array2D(const Array2D &);
    std::unique_ptr<float[]> _data;
    int _numRows;
    int _numColumns;
};

inline Array2D::Array2D(int numRows, int numColumns) {
    init(numRows, numColumns);
}

inline void Array2D::init(int numRows, int numColumns) {
    _numRows = numRows;
    _numColumns = numColumns;
    _data = nullptr;

    if (_numRows > 0 && _numColumns > 0) {
        _data = std::make_unique<float[]>(numRows * numColumns);
    }
}

inline float* Array2D::operator[](const int row) const {
    return &_data[row * _numColumns];
}

#endif /* Array2D_hpp */


//
//  ConvexPolygonRenderer.hpp
//  ComputerGraphics
//
//  Created by David M. Reed on 1/28/20.
//  Copyright © 2020 David M Reed. All rights reserved.
//

#ifndef ConvexPolygonRenderer_hpp
#define ConvexPolygonRenderer_hpp

#include "graphics.hpp"
#include "ConvexPolygon.hpp"

class ConvexPolygonRenderer: public Renderer {
public:

    /// constructor
    /// @param windowTitle title for window
    /// @param width window width
    /// @param height window height
    ConvexPolygonRenderer(std::string windowTitle, int width, int height, std::string filename);

    virtual ~ConvexPolygonRenderer() noexcept {}
    
    /// add points to be rendered
    /// overrides parent addPoints to utilize z Buffer
    /// @param pts vector of Point3D to render
    /// @param color color to render each point in
    /// @param pointSize size of each point (defaults to 1.0)
    /// @param objectTransformation transformation to apply to each point (defaults to identity matrix)
    /// @return position identifier for the drawable added (can be sent to removeDrawable method)
    size_t addPoints(const std::vector<Point3D> &pts, const Color &color, float pointSize, const mat4 &objectTransformation);
    
private:
    std::vector<ConvexPolygon> _convexPolygons;
    Array2D _zBuffer;
    
};

#endif /* ConvexPolygonRenderer_hpp */

//
//  Array2D.hpp
//  Array2D
//
//  Created by David M. Reed on 2/19/20.
//  Copyright © 2020 David Reed. All rights reserved.
//

