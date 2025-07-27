// SPDX-FileCopyrightText: 2017 Petros Koutsolampros
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "gllatticemap.hpp"

#include "salalib/geometrygenerators.hpp"
#include "salalib/linkutils.hpp"

void GLPixelMap::loadGLObjects(LatticeMapDM &latticeMap) {
    Region4f region = latticeMap.getRegion();
    m_latticeMap.loadRegionData(region.bottomLeft.x, region.bottomLeft.y, region.topRight.x,
                                region.topRight.y);

    if (m_showGrid) {
        std::vector<SimpleLine> gridData;
        double spacing = latticeMap.getSpacing();
        double offsetX = region.bottomLeft.x;
        double offsetY = region.bottomLeft.y;
        for (int x = 1; x < latticeMap.getCols(); x++) {
            gridData.push_back(SimpleLine(offsetX + x * spacing, region.bottomLeft.y,
                                          offsetX + x * spacing, region.topRight.y));
        }
        for (int y = 1; y < latticeMap.getRows(); y++) {
            gridData.push_back(SimpleLine(region.bottomLeft.x, offsetY + y * spacing,
                                          region.topRight.x, offsetY + y * spacing));
        }
        m_grid.loadLineData(gridData, m_gridColour);
    }
    if (m_showLinks) {
        const std::vector<SimpleLine> &mergedPixelLines =
            sala::getMergedPixelsAsLines(latticeMap.getInternalMap());
        std::vector<Point2f> mergedPixelLocations;
        for (auto &mergeLine : mergedPixelLines) {
            mergedPixelLocations.push_back(mergeLine.start());
            mergedPixelLocations.push_back(mergeLine.end());
        }

        const std::vector<Point2f> &linkFillTriangles =
            GeometryGenerators::generateMultipleDiskTriangles(32, latticeMap.getSpacing() * 0.25,
                                                              mergedPixelLocations);
        m_linkFills.loadTriangleData(linkFillTriangles, qRgb(0, 0, 0));

        std::vector<SimpleLine> linkFillPerimeters =
            GeometryGenerators::generateMultipleCircleLines(32, latticeMap.getSpacing() * 0.25,
                                                            mergedPixelLocations);
        linkFillPerimeters.insert(linkFillPerimeters.end(), mergedPixelLines.begin(),
                                  mergedPixelLines.end());
        m_linkLines.loadLineData(linkFillPerimeters, qRgb(0, 255, 0));
    }
}
void GLPixelMap::loadGLObjectsRequiringGLContext(const LatticeMapDM &currentLatticeMap) {
    QImage data(currentLatticeMap.getCols(), currentLatticeMap.getRows(), QImage::Format_RGBA8888);
    data.fill(Qt::transparent);

    for (int y = 0; y < currentLatticeMap.getRows(); y++) {
        for (int x = 0; x < currentLatticeMap.getCols(); x++) {
            PixelRef pix(x, y);
            PafColor colour = currentLatticeMap.getPointColor(pix);
            if (colour.alphab() != 0) { // alpha == 0 is transparent
                data.setPixelColor(x, y, qRgb(colour.redb(), colour.greenb(), colour.blueb()));
            }
        }
    }
    m_latticeMap.loadPixelData(data);
}
