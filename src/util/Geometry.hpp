#pragma once

#include <array>
#include <cmath>

namespace util {

    struct Point2D {
        float x{0.0f};
        float y{0.0f};

        constexpr Point2D(float x, float y) 
            : x(x), y(y) {}

        float getMagnitude() const {
            return std::sqrt(x * x + y * y);
        }
    };

    struct Point3D {
        float x{0.0f};
        float y{0.0f};
        float z{0.0f};

        constexpr Point3D(float x, float y, float z)
            : x(x), y(y), z(z) {}

    /**
        OpenCV uses the axes coordinate system of:
        x positive: right
        y positive: down
        z positive: forwards away from camera

        Converts the axes to:
        x positive: forwards away from camera
        y positive: left
        z positive: up
    **/
        static constexpr Point3D convertFromOpenCVToNormalAxes(const Point3D& point) {
            return Point3D(point.z, -point.x, -point.y);
        }

        float getMagnitude() const {
            return std::sqrt(x * x + y * y + z * z);
        }
    };

}  // namespace util