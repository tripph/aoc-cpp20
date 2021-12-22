#include <iostream>
#include <vector>
#include <cassert>
#include <map>
#include <algorithm>

struct Point {
    int x, y;

    auto operator<=>(const Point &) const = default;

    friend std::ostream &operator<<(std::ostream &os, const Point &point) {
        os << "{x: " << point.x << ", y: " << point.y << "}";
        return os;
    }
};


class LineSegment {
public:
    LineSegment(Point start, Point end) : start(start), end(end) {}

    [[nodiscard]] bool isValid() const {
        return start.x == end.x || start.y == end.y;
    }

    [[nodiscard]] std::vector<Point> getCoveredPoints() const {
        std::vector<Point> v = {};
        //horizontal
        if (start.x != end.x && start.y == end.y) {
            std::cout << "getCoveredPoints horizontal: " << start <<  end << std::endl;
            for (int i = start.x; i <= end.x; i++) {
                v.push_back(Point{i, start.y});
            }
        }
            //vertical
        if (start.y != end.y && start.x == end.x) {
            std::cout << "getCoveredPoints vertical: " << start << end << std::endl;
            for (int i = start.y; i <= end.y; i++) {
                v.push_back(Point{start.x, i});
            }
        }
        std::cout << "Points: ";
        std::for_each(v.begin(), v.end(), [](Point p) { std::cout << p << ", ";});
        std::cout << std::endl;
        return v;
    }

private:
    Point start;
    Point end;

};

std::vector<Point> flattenLines(const std::vector<LineSegment> &lines) {
    auto points = std::vector<Point>{};

    for (LineSegment l: lines) {
        auto coveredPoints = l.getCoveredPoints();
        for (Point p: coveredPoints) {
            points.push_back(p);
        }
    }
    return points;
}

std::map<Point, int> countPointOverlaps(const std::vector<Point> &points) {
    auto m = std::map<Point, int>{};
    for (Point p: points) {
        if (m.contains(p)) {
            m[p]++;
        } else {
            m[p] = 1;
        }
    }
    return m;
}


int findMaxOverlapCount(const std::map<Point, int> &map) {
    int max = 0;
    for (auto &kv: map) {
        auto v = kv.second;
        if (v > max) {
            max = v;
        }
    }
    return max;
}

std::vector<LineSegment> genMockLines() {
    return {
            LineSegment(Point{0, 0}, Point{4, 0}),
            LineSegment(Point{0, 0}, Point{0, 4}),
            LineSegment(Point{1, 1}, Point{3, 1}),
            LineSegment(Point{1, 1}, Point{1, 3}),
            LineSegment(Point{2, 0}, Point{2, 0}),
    };
}


Point createFarthestPoint(const std::vector<Point>&  points) {
    //combines highest X and Y coord into a (new? please make sure it doesn't already exist before creating or else we will have a false overlap count) point
    int max_y = std::max_element(points.begin(), points.end(), [](Point const &a, Point const &b) {
        std::cout << "finding max_y :" << a << b<< std::endl;
        return a.y < b.y;
    }).base()->y;
    int max_x = std::max_element(points.begin(), points.end(), [](Point const &a, Point const &b) {
        std::cout << "finding max_x :" << a << b<< std::endl;
       return a.x < b.x;
    }).base()->x;

    return Point{max_x, max_y};
}

void renderLines(const std::vector<LineSegment>& lines, const std::map<Point, int>& overlapCounts) {
    //find bounds of grid for given points
    //add filler points to overlapCounts with a value of 0
    //actually output points

}

int main() {
    std::cout << "Hello, World!" << std::endl;
    auto line_1 = LineSegment(Point{1, 1}, Point{3, 1});
    auto line_2 = LineSegment(Point{1, 1}, Point{1, 3});
    auto point1Count = line_1.getCoveredPoints().size();
    auto point2Count = line_2.getCoveredPoints().size();
    assert(point1Count == 3);
    assert(point2Count == 3);
    auto farthest = createFarthestPoint(flattenLines({line_1, line_1}));
    std::cout << "Max point for line1+2: " << farthest  << std::endl;
    assert(farthest.x == 3 && farthest.y == 3);

    auto flattened = flattenLines(std::vector{line_1, line_2});
    assert(flattened.size() == 6);
    auto mapped = countPointOverlaps(flattened);
    for (const auto&[key, value]: mapped) {
        std::cout << "Map value " << key << ": " << value << std::endl;
    }
    int overlaps = findMaxOverlapCount(mapped);
    assert(overlaps == 2);

//    auto flat_mock = flattenLines(genMockLines());
//    auto map_mock = countPointOverlaps(flat_mock);
//    int mock_overlaps = findMaxOverlapCount(map_mock);
//    std::cout << "Mock Overlaps: " << mock_overlaps << std::endl;

    return 0;
}
