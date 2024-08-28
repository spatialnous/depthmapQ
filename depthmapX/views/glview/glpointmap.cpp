// SPDX-FileCopyrightText: 2017 Petros Koutsolampros
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "glpointmap.h"

#include "salalib/geometrygenerators.h"
#include "salalib/linkutils.h"

void GLPixelMap::loadGLObjects(PointMapDX &pointMap) {
    QtRegion region = pointMap.getRegion();
    m_pointMap.loadRegionData(region.bottomLeft.x, region.bottomLeft.y, region.topRight.x,
                              region.topRight.y);

    if (m_showGrid) {
        std::vector<SimpleLine> gridData;
        double spacing = pointMap.getSpacing();
        double offsetX = region.bottomLeft.x;
        double offsetY = region.bottomLeft.y;
        for (int x = 1; x < pointMap.getCols(); x++) {
            gridData.push_back(SimpleLine(offsetX + x * spacing, region.bottomLeft.y,
                                          offsetX + x * spacing, region.topRight.y));
        }
        for (int y = 1; y < pointMap.getRows(); y++) {
            gridData.push_back(SimpleLine(region.bottomLeft.x, offsetY + y * spacing,
                                          region.topRight.x, offsetY + y * spacing));
        }
        m_grid.loadLineData(gridData, m_gridColour);
    }
    if (m_showLinks) {
        const std::vector<SimpleLine> &mergedPixelLines =
            depthmapX::getMergedPixelsAsLines(pointMap.getInternalMap());
        std::vector<Point2f> mergedPixelLocations;
        for (auto &mergeLine : mergedPixelLines) {
            mergedPixelLocations.push_back(mergeLine.start());
            mergedPixelLocations.push_back(mergeLine.end());
        }

        const std::vector<Point2f> &linkFillTriangles =
            GeometryGenerators::generateMultipleDiskTriangles(32, pointMap.getSpacing() * 0.25,
                                                              mergedPixelLocations);
        m_linkFills.loadTriangleData(linkFillTriangles, qRgb(0, 0, 0));

        std::vector<SimpleLine> linkFillPerimeters =
            GeometryGenerators::generateMultipleCircleLines(32, pointMap.getSpacing() * 0.25,
                                                            mergedPixelLocations);
        linkFillPerimeters.insert(linkFillPerimeters.end(), mergedPixelLines.begin(),
                                  mergedPixelLines.end());
        m_linkLines.loadLineData(linkFillPerimeters, qRgb(0, 255, 0));
    }
}
void GLPixelMap::loadGLObjectsRequiringGLContext(const PointMapDX &currentPointMap) {
    QImage data(currentPointMap.getCols(), currentPointMap.getRows(), QImage::Format_RGBA8888);
    data.fill(Qt::transparent);

    for (int y = 0; y < currentPointMap.getRows(); y++) {
        for (int x = 0; x < currentPointMap.getCols(); x++) {
            PixelRef pix(x, y);
            PafColor colour = currentPointMap.getPointColor(pix);
            if (colour.alphab() != 0) { // alpha == 0 is transparent
                data.setPixelColor(x, y, qRgb(colour.redb(), colour.greenb(), colour.blueb()));
            }
        }
    }
    m_pointMap.loadPixelData(data);
}
