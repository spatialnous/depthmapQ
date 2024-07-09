// SPDX-FileCopyrightText: 2020-2024 Petros Koutsolampros
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "vgapathsmainwindow.h"

#include "salalib/vgamodules/extractlinkdata.h"
#include "salalib/vgamodules/vgaangularshortestpath.h"
#include "salalib/vgamodules/vgaisovistzone.h"
#include "salalib/vgamodules/vgametricdepthlinkcost.h"
#include "salalib/vgamodules/vgametricshortestpath.h"
#include "salalib/vgamodules/vgametricshortestpathtomany.h"
#include "salalib/vgamodules/vgavisualshortestpath.h"

#include "depthmapX/mainwindowhelpers.h"

#include <QMenuBar>
#include <QMessageBox>

bool VGAPathsMainWindow::createMenus(MainWindow *mainWindow) {
    QMenu *toolsMenu = MainWindowHelpers::getOrAddRootMenu(mainWindow, tr("&Tools"));
    QMenu *visibilitySubMenu = MainWindowHelpers::getOrAddMenu(toolsMenu, tr("&Visibility"));
    QMenu *shortestPathSubMenu =
        MainWindowHelpers::getOrAddMenu(visibilitySubMenu, tr("Shortest Paths"));

    QAction *visibilityShortestPathAct = new QAction(tr("Visibility Shortest Path"), this);
    visibilityShortestPathAct->setStatusTip(tr("Shortest visual path between two selected points"));
    connect(visibilityShortestPathAct, &QAction::triggered, this,
            [this, mainWindow] { OnShortestPath(mainWindow, PathType::VISUAL); });
    shortestPathSubMenu->addAction(visibilityShortestPathAct);

    QAction *metricShortestPathAct = new QAction(tr("&Metric Shortest Path"), this);
    metricShortestPathAct->setStatusTip(tr("Shortest metric path between two selected points"));
    connect(metricShortestPathAct, &QAction::triggered, this,
            [this, mainWindow] { OnShortestPath(mainWindow, PathType::METRIC); });
    shortestPathSubMenu->addAction(metricShortestPathAct);

    QAction *angularShortestPathAct = new QAction(tr("&Angular Shortest Path"), this);
    angularShortestPathAct->setStatusTip(tr("Shortest angular path between two selected points"));
    connect(angularShortestPathAct, &QAction::triggered, this,
            [this, mainWindow] { OnShortestPath(mainWindow, PathType::ANGULAR); });
    shortestPathSubMenu->addAction(angularShortestPathAct);

    QAction *extractLinkDataAct = new QAction(tr("&Extract Link Data"), this);
    extractLinkDataAct->setStatusTip(
        tr("Extracts data from the links and adds them to the attribute table"));
    connect(extractLinkDataAct, &QAction::triggered, this,
            [this, mainWindow] { OnExtractLinkData(mainWindow); });
    visibilitySubMenu->addAction(extractLinkDataAct);

    return true;
}

void VGAPathsMainWindow::OnShortestPath(MainWindow *mainWindow, PathType pathType) {
    QGraphDoc *graphDoc = mainWindow->activeMapDoc();
    if (graphDoc == nullptr)
        return;

    if (graphDoc->m_communicator) {
        QMessageBox::warning(mainWindow, tr("Warning"),
                             tr("Please wait, another process is running"), QMessageBox::Ok,
                             QMessageBox::Ok);
        return;
    }
    if (graphDoc->m_meta_graph->getDisplayedMapType() != ShapeMap::POINTMAP) {
        QMessageBox::warning(mainWindow, tr("Warning"),
                             tr("Please make sure the displayed map is a VGA map"), QMessageBox::Ok,
                             QMessageBox::Ok);
        return;
    }
    PointMapDX &pointMap = graphDoc->m_meta_graph->getDisplayedPointMap();
    if (pointMap.getSelSet().size() != 2) {
        QMessageBox::warning(mainWindow, tr("Warning"),
                             tr("Please select two cells to create a path between"),
                             QMessageBox::Ok, QMessageBox::Ok);
        return;
    }
    const PixelRef &pixelFrom = *pointMap.getSelSet().begin();
    const PixelRef &pixelTo = *std::next(pointMap.getSelSet().begin());

    graphDoc->m_communicator = new CMSCommunicator();
    switch (pathType) {
    case PathType::VISUAL: {
        graphDoc->m_communicator->setAnalysis(std::unique_ptr<IAnalysis>(
            new VGAVisualShortestPath(pointMap.getInternalMap(), pixelFrom, pixelTo)));
        graphDoc->m_communicator->setPostAnalysisFunc([&pointMap](AnalysisResult &) {
            pointMap.overrideDisplayedAttribute(-2);
            pointMap.setDisplayedAttribute(VGAVisualShortestPath::Column::VISUAL_SHORTEST_PATH);
        });
        break;
    }
    case PathType::METRIC: {
        std::set<PixelRef> pixelsFrom;
        pixelsFrom.insert(pixelFrom);
        graphDoc->m_communicator->setAnalysis(std::unique_ptr<IAnalysis>(
            new VGAMetricShortestPath(pointMap.getInternalMap(), pixelsFrom, pixelTo)));
        graphDoc->m_communicator->setPostAnalysisFunc([&pointMap](AnalysisResult &) {
            pointMap.overrideDisplayedAttribute(-2);
            pointMap.setDisplayedAttribute(VGAMetricShortestPath::Column::METRIC_SHORTEST_PATH);
        });
        break;
    }
    case PathType::ANGULAR: {
        graphDoc->m_communicator->setAnalysis(std::unique_ptr<IAnalysis>(
            new VGAAngularShortestPath(pointMap.getInternalMap(), pixelFrom, pixelTo)));
        graphDoc->m_communicator->setPostAnalysisFunc([&pointMap](AnalysisResult &) {
            pointMap.overrideDisplayedAttribute(-2);
            pointMap.setDisplayedAttribute(VGAAngularShortestPath::Column::ANGULAR_SHORTEST_PATH);
        });
        break;
    }
    }
    graphDoc->m_communicator->SetFunction(CMSCommunicator::FROMCONNECTOR);
    graphDoc->m_communicator->setSuccessUpdateFlags(QGraphDoc::NEW_DATA);
    graphDoc->m_communicator->setSuccessRedrawFlags(QGraphDoc::VIEW_ALL, QGraphDoc::REDRAW_POINTS,
                                                    QGraphDoc::NEW_DATA);

    graphDoc->CreateWaitDialog(tr("Calculating shortest path..."));
    graphDoc->m_thread.render(graphDoc);
}

void VGAPathsMainWindow::OnExtractLinkData(MainWindow *mainWindow) {
    QGraphDoc *graphDoc = mainWindow->activeMapDoc();
    if (graphDoc == nullptr)
        return;

    if (graphDoc->m_communicator) {
        QMessageBox::warning(mainWindow, tr("Warning"),
                             tr("Please wait, another process is running"), QMessageBox::Ok,
                             QMessageBox::Ok);
        return;
    }
    if (graphDoc->m_meta_graph->getDisplayedMapType() != ShapeMap::POINTMAP) {
        QMessageBox::warning(mainWindow, tr("Warning"),
                             tr("Please make sure the displayed map is a VGA map"), QMessageBox::Ok,
                             QMessageBox::Ok);
        return;
    }

    graphDoc->m_communicator->setAnalysis(std::unique_ptr<IAnalysis>(
        new ExtractLinkData(graphDoc->m_meta_graph->getDisplayedPointMap().getInternalMap())));

    graphDoc->m_communicator->SetFunction(CMSCommunicator::FROMCONNECTOR);
    graphDoc->m_communicator->setSuccessUpdateFlags(QGraphDoc::NEW_DATA);
    graphDoc->m_communicator->setSuccessRedrawFlags(QGraphDoc::VIEW_ALL, QGraphDoc::REDRAW_POINTS,
                                                    QGraphDoc::NEW_DATA);

    graphDoc->CreateWaitDialog(tr("Extracting link data.."));
    graphDoc->m_thread.render(graphDoc);
}

void VGAPathsMainWindow::OnMakeIsovistZones(MainWindow *mainWindow) {
    QGraphDoc *graphDoc = mainWindow->activeMapDoc();
    if (graphDoc == nullptr)
        return;

    if (graphDoc->m_communicator) {
        QMessageBox::warning(mainWindow, tr("Warning"),
                             tr("Please wait, another process is running"), QMessageBox::Ok,
                             QMessageBox::Ok);
        return;
    }
    if (graphDoc->m_meta_graph->getDisplayedMapType() != ShapeMap::POINTMAP) {
        QMessageBox::warning(mainWindow, tr("Warning"),
                             tr("Please make sure the displayed map is a VGA map"), QMessageBox::Ok,
                             QMessageBox::Ok);
        return;
    }

    std::map<std::string, std::set<PixelRef>> originPointSets;
    float restrictDistance = -1;

    auto &map = graphDoc->m_meta_graph->getDisplayedPointMap();
    graphDoc->m_communicator->setAnalysis(std::unique_ptr<IAnalysis>(
        new VGAIsovistZone(map.getInternalMap(), originPointSets, restrictDistance)));

    graphDoc->m_communicator->setPostAnalysisFunc([&map](AnalysisResult &result) {
        map.overrideDisplayedAttribute(-2);
        map.setDisplayedAttribute(result.getAttributes()[0]);
    });

    graphDoc->m_communicator->SetFunction(CMSCommunicator::FROMCONNECTOR);
    graphDoc->m_communicator->setSuccessUpdateFlags(QGraphDoc::NEW_DATA);
    graphDoc->m_communicator->setSuccessRedrawFlags(QGraphDoc::VIEW_ALL, QGraphDoc::REDRAW_POINTS,
                                                    QGraphDoc::NEW_DATA);

    graphDoc->CreateWaitDialog(tr("Making isovist zones.."));
    graphDoc->m_thread.render(graphDoc);
}

void VGAPathsMainWindow::OnMetricShortestPathsToMany(MainWindow *mainWindow) {
    QGraphDoc *graphDoc = mainWindow->activeMapDoc();
    if (graphDoc == nullptr)
        return;

    if (graphDoc->m_communicator) {
        QMessageBox::warning(mainWindow, tr("Warning"),
                             tr("Please wait, another process is running"), QMessageBox::Ok,
                             QMessageBox::Ok);
        return;
    }
    if (graphDoc->m_meta_graph->getDisplayedMapType() != ShapeMap::POINTMAP) {
        QMessageBox::warning(mainWindow, tr("Warning"),
                             tr("Please make sure the displayed map is a VGA map"), QMessageBox::Ok,
                             QMessageBox::Ok);
        return;
    }

    std::set<PixelRef> pixelsFrom;
    std::set<PixelRef> pixelsTo;
    auto &map = graphDoc->m_meta_graph->getDisplayedPointMap();
    graphDoc->m_communicator->setAnalysis(std::unique_ptr<IAnalysis>(
        new VGAMetricShortestPathToMany(map.getInternalMap(), pixelsFrom, pixelsTo)));

    graphDoc->m_communicator->setPostAnalysisFunc([&map](AnalysisResult &result) {
        map.overrideDisplayedAttribute(-2);
        map.setDisplayedAttribute(result.getAttributes()[0]);
    });

    graphDoc->m_communicator->SetFunction(CMSCommunicator::FROMCONNECTOR);
    graphDoc->m_communicator->setSuccessUpdateFlags(QGraphDoc::NEW_DATA);
    graphDoc->m_communicator->setSuccessRedrawFlags(QGraphDoc::VIEW_ALL, QGraphDoc::REDRAW_POINTS,
                                                    QGraphDoc::NEW_DATA);

    graphDoc->CreateWaitDialog(tr("Making isovist zones.."));
    graphDoc->m_thread.render(graphDoc);
}
