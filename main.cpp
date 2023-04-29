#include <iostream>
#include <vector>
#include "triangulate.h"

using namespace std;

int main()
{
    /*
    // Demonstration to check if a point is inside of the given polygon
    vector<Point> polygon{ {0, 0}, {10, 0}, {10, 10}, {0, 10} };
    Point p = {20, 20};
    triangulate::isInside(polygon, p) ? cout << "Yes \n" : cout << "No \n";
    */

    /*
    // Demonstration how to reorder points of the triangle to be in CCW direction
    vector<Point> triangle{ {2.5, 2.5}, {5, 0}, {0, 0}};
    triangle = triangulate::makeTriangleCCW(triangle);
    cout << "Points of the triangle in CCW direction:" << endl;
    for (int i = 0; i < triangle.size(); i++) {        
        printf("{%.2f, %.2f} ", triangle[i].x, triangle[i].y);
    }
    */

    /*
    // Demonstration to triangulate a polygon, a simple example
    vector<Point> polygon{ {0, 0}, {10, 0}, {10, 10}, {0, 10} };
    vector<vector<Point>> triangles = triangulate::getTriangles(polygon);
    for (int i = 0; i < triangles.size(); i++) {
        vector<Point> tr = triangles[i];
        cout << "Triangle " << i + 1 << ": ";
        for (int k = 0; k < tr.size(); k++) {
            printf("{%.2f, %.2f} ", tr[k].x, tr[k].y);
        }
        cout << endl;
    }
    */

    // Demonstration to triangulate a polygon, a more complex example
    // You can visualize the polygon with https://www.math10.com/en/geometry/geogebra/fullscreen.html
    vector<Point> polygon{ 
        {-10, -10}, {-5, -5}, {-8.05086, -2.46784}, {-12.9521, -1.3146}, {-7.35892, 2.89469},
        {-1.76575, 2.89469}, {-1.93873, -0.62267}, {1.52096, -2.75614}, {-1.93873, -4.7743}, {-2.22704, -7.7727}, {2.03991, -10.0215}
    };
    vector<vector<Point>> triangles = triangulate::getTriangles(polygon);
    for (int i = 0; i < triangles.size(); i++) {
        vector<Point> tr = triangles[i];
        cout << "Triangle " << i + 1 << ": ";
        for (int k = 0; k < tr.size(); k++) {
            printf("{%.2f, %.2f} ", tr[k].x, tr[k].y);
        }
        cout << endl;
    }
    /*
    Triangle 1: {-12.95, -1.31} {-8.05, -2.47} {-7.36, 2.89}
    Triangle 2: {-8.05, -2.47} {-1.77, 2.89} {-7.36, 2.89}
    Triangle 3: {-8.05, -2.47} {-1.94, -0.62} {-1.77, 2.89}
    Triangle 4: {-8.05, -2.47} {1.52, -2.76} {-1.94, -0.62}
    Triangle 5: {-1.94, -4.77} {1.52, -2.76} {-8.05, -2.47}
    Triangle 6: {-10.00, -10.00} {2.04, -10.02} {-2.23, -7.77}
    Triangle 7: {-10.00, -10.00} {-2.23, -7.77} {-5.00, -5.00}
    Triangle 8: {-5.00, -5.00} {-1.94, -4.77} {-8.05, -2.47}
    Triangle 9: {-2.23, -7.77} {-1.94, -4.77} {-5.00, -5.00}

    Triangle 1: D, C, E
    Triangle 2: C, F, E
    Triangle 3: C, G, F
    Triangle 4: C, H, G
    Triangle 5: I, H, C
    Triangle 6: A, K, J
    Triangle 7: A, J, B
    Triangle 8: B, I, C
    Triangle 9: J, I, B
    */
}
