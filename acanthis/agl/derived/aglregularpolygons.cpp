// SPDX-FileCopyrightText: 2018 Petros Koutsolampros
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "aglregularpolygons.h"

#include <math.h>

void AGLRegularPolygons::loadPolygonData(
    const std::vector<std::pair<Point2f, PafColor>> &colouredPoints, const unsigned int sides,
    const float radius) {
    init(static_cast<int>(colouredPoints.size() * static_cast<size_t>(sides) * 3));
    std::vector<Point2f> points(sides * 3);
    float angle = static_cast<float>(2 * M_PI / sides);
    for (size_t i = 0; i < sides; i++) {
        points[i * 3] = Point2f(cos(static_cast<float>(i) * angle) * radius,
                                sin(static_cast<float>(i) * angle) * radius);
        points[i * 3 + 1] = Point2f(cos(static_cast<float>(i + 1) * angle) * radius,
                                    sin(static_cast<float>(i + 1) * angle) * radius);
        points[i * 3 + 2] = Point2f(0, 0);
    }

    Point2f prevCentre(0, 0);
    for (const auto &colouredPoint : colouredPoints) {
        const Point2f &centre = colouredPoint.first;
        float r = colouredPoint.second.redf();
        float g = colouredPoint.second.greenf();
        float b = colouredPoint.second.bluef();

        for (Point2f &point : points) {
            point.x -= prevCentre.x;
            point.y -= prevCentre.y;
            point.x += centre.x;
            point.y += centre.y;
            add(QVector3D(static_cast<float>(point.x), static_cast<float>(point.y), 0.0f),
                QVector3D(r, g, b));
        }
        prevCentre = centre;
    }
}
