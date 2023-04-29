#pragma once

#include <algorithm>
#include <vector>
#define M_PI 3.14159265358979323846
#define INF INT_MAX

using namespace std;

struct Point {
    float x;
    float y;
    bool operator==(const Point& p) const {
        return (x == p.x && y == p.y);
    }
};

class triangulate
{
public:
    static Point getCentroidPoint(Point p, Point q, Point r) {
        Point m;
        m.x = p.x / 3. + q.x / 3. + r.x / 3.;
        m.y = p.y / 3. + q.y / 3. + r.y / 3.;
        return m;
    }

    // Given three colinear points p, q, r, the function checks if 
    // point q lies on line segment 'pr' 
    static bool onSegment(Point p, Point q, Point r)
    {
        if (q.x <= max(p.x, r.x) && q.x >= min(p.x, r.x) &&
            q.y <= max(p.y, r.y) && q.y >= min(p.y, r.y))
            return true;
        return false;
    }

    // To find orientation of ordered triplet (p, q, r). 
    // The function returns following values 
    // 0 --> p, q and r are colinear 
    // 1 --> Clockwise 
    // 2 --> Counterclockwise 
    static int orientation(Point p, Point q, Point r)
    {
        Point m = getCentroidPoint(p, q, r);
        if (m == p || m == q || m == r) return 0; // Special case of colinear
        float angle_m_p = atan2(p.y - m.y, p.x - m.x) * 180 / M_PI;
        float angle_m_q = atan2(q.y - m.y, q.x - m.x) * 180 / M_PI;
        float angle_m_r = atan2(r.y - m.y, r.x - m.x) * 180 / M_PI;
        if (angle_m_p == angle_m_q || angle_m_p == angle_m_r || angle_m_q == angle_m_r) return 0; // colinear
        // Let's move angle_m_q and angle_m_r ahead of angle_m_p, if necessary
        if (angle_m_q < angle_m_p) angle_m_q += 360;
        if (angle_m_r < angle_m_p) angle_m_r += 360;
        if (angle_m_q < angle_m_r) return 2; // Counterclockwise
        // This means that angle_m_q > angle_m_r
        return 1; // Clockwise
    }

    // The function that returns true if line segment 'p1q1' 
    // and 'p2q2' intersect. 
    static bool doIntersect(Point p1, Point q1, Point p2, Point q2)
    {
        // Find the four orientations needed for general and 
        // special cases 
        int o1 = orientation(p1, q1, p2);
        int o2 = orientation(p1, q1, q2);
        int o3 = orientation(p2, q2, p1);
        int o4 = orientation(p2, q2, q1);

        // General case 
        if (o1 != o2 && o3 != o4)
            return true;

        // Special Cases 
        // p1, q1 and p2 are colinear and p2 lies on segment p1q1 
        if (o1 == 0 && onSegment(p1, p2, q1)) return true;

        // p1, q1 and p2 are colinear and q2 lies on segment p1q1 
        if (o2 == 0 && onSegment(p1, q2, q1)) return true;

        // p2, q2 and p1 are colinear and p1 lies on segment p2q2 
        if (o3 == 0 && onSegment(p2, p1, q2)) return true;

        // p2, q2 and q1 are colinear and q1 lies on segment p2q2 
        if (o4 == 0 && onSegment(p2, q1, q2)) return true;

        return false; // Doesn't fall in any of the above cases 
    }

    // Returns true if the point p lies inside the polygon
    static bool isInside(vector<Point> &polygon, Point p)
    {
        int n = polygon.size();

        // Special cases for n<3
        if (n == 0) return false;
        if (n == 1) {
            if (polygon[0] == p) return true;
            return false;
        }
        if (n == 2) {
            if (orientation(polygon[0], polygon[1], p) == 0 && onSegment(polygon[0], p, polygon[1])) return true;
            return false;
        }
        // Here we have n>=3

        // Create a point for line segment from p to infinite 
        Point extreme = { INF, p.y };

        // Count intersections of the above line with sides of polygon 
        int count = 0, i = 0;
        do
        {
            int next = (i + 1) % n;

            // Check if the line segment from 'p' to 'extreme' intersects 
            // with the line segment from 'polygon[i]' to 'polygon[next]' 
            if (doIntersect(polygon[i], polygon[next], p, extreme))
            {
                // If the point 'p' is colinear with line segment 'i-next', 
                // then check if it lies on segment. If it lies, return true, 
                // otherwise false 
                if (orientation(polygon[i], p, polygon[next]) == 0)
                    return onSegment(polygon[i], p, polygon[next]);

                count++;
            }
            i = next;
        } while (i != 0);

        // Return true if count is odd, false otherwise 
        return count % 2 == 1;
    }

    // We assume that points of this triangle are not colinear
    // Let's order them in CCW direction
    static vector<Point> makeTriangleCCW(vector<Point> triangle) {
        Point p = triangle[0];
        Point q = triangle[1];
        Point r = triangle[2];
        Point m = getCentroidPoint(p, q, r);
        float angle_m_p = atan2(p.y - m.y, p.x - m.x) * 180 / M_PI;
        float angle_m_q = atan2(q.y - m.y, q.x - m.x) * 180 / M_PI;
        float angle_m_r = atan2(r.y - m.y, r.x - m.x) * 180 / M_PI;
        // Let's sort these angles from the smallest to the biggest, along with their points
        vector<float> angles{ angle_m_p, angle_m_q, angle_m_r };
        int n = 3;
        for (int i = 0; i < n-1; i++) {
            for (int k = i + 1; k < n; k++) {
                if (angles[i] > angles[k]) {
                    swap(angles[i], angles[k]);
                    swap(triangle[i], triangle[k]);
                }
            }
        }
        return triangle;
    }

    // Triangulates the polygon using the Ear Clipping algorithm.
    // Polygon is given as a vector of consecutive boundary points (CW or CCW).
    // Returns vector of triangles, where each triangle is in the form of vector<Point> with 3 Points.
    static vector<vector<Point>> getTriangles(vector<Point> polygon) {
        vector<vector<Point>> triangles;
        int count = -1; // Count for how many points we moved along polygon without finding any ear
        int f_c_pos = -1; // Position of first of consecutive points
        int s_c_pos = -1; // Position of second of consecutive points
        int t_c_pos = -1; // Position of third of consecutive points
        while (polygon.size() >= 4) {
            // Get 3 consecutive points from the polygon
            f_c_pos = (f_c_pos + 1) % polygon.size(); count++;
            if (count >= polygon.size()) break; // We circled the polygon without finding any ears, we don't want infinite loop
            Point p = polygon[f_c_pos];
            s_c_pos = (f_c_pos + 1) % polygon.size();
            Point q = polygon[s_c_pos];
            t_c_pos = (s_c_pos + 1) % polygon.size();
            Point r = polygon[t_c_pos];
            if (orientation(p, q, r) == 0) { // Consecutive points are colinear
                polygon.erase(polygon.begin()+s_c_pos); // Remove the middle point
                f_c_pos--; // In the next iteration take the same p point, and next 2 points
                count = -1; // Reset counter
                continue;
            }
            // Now we know that p, q and r are not colinear
            Point m = getCentroidPoint(p, q, r);            
            if (isInside(polygon, m)) {
                bool other_polygon_point_inside = false;
                int pos = (t_c_pos + 1) % polygon.size();
                vector<Point> triangle{p, q, r};
                while (pos != f_c_pos) {
                    Point h = polygon[pos];
                    if (isInside(triangle, h)) {
                        other_polygon_point_inside = true;
                        break;
                    }
                    pos = (pos + 1) % polygon.size();
                }
                if (other_polygon_point_inside) continue; // p, q, r do not form an ear
                // other_polygon_point_inside is false here, so we found an ear
                polygon.erase(polygon.begin() + s_c_pos); // Remove the middle point
                triangles.push_back(makeTriangleCCW(triangle));
                f_c_pos--; // In the next iteration take the same p point, and next 2 points
                count = -1; // Reset counter
                continue;
            }
        }
        if (polygon.size() == 3) {
            if (orientation(polygon[0], polygon[1], polygon[2])!=0)
                triangles.push_back(makeTriangleCCW(polygon));
        }
        return triangles;
    }
};

