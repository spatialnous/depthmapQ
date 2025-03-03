// SPDX-FileCopyrightText: 2020-2024 Petros Koutsolampros
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "vgaparallelmainwindow.hpp"

#include "salalib/vgamodules/vgaangularopenmp.hpp"
#include "salalib/vgamodules/vgametricopenmp.hpp"
#include "salalib/vgamodules/vgavisualglobalopenmp.hpp"
#include "salalib/vgamodules/vgavisuallocaladjmatrix.hpp"
#include "salalib/vgamodules/vgavisuallocalopenmp.hpp"

#include "depthmapX/mainwindowhelpers.hpp"

#include <QInputDialog>
#include <QMenuBar>
#include <QMessageBox>

bool VGAParallelMainWindow::createMenus(MainWindow *mainWindow) {
    QMenu *toolsMenu = MainWindowHelpers::getOrAddRootMenu(mainWindow, tr("&Tools"));
    QMenu *visibilityMenu = MainWindowHelpers::getOrAddMenu(toolsMenu, tr("&Visibility"));
    QMenu *vgaParallelMenu =
        MainWindowHelpers::getOrAddMenu(visibilityMenu, tr("Parallel Visibility Graph Analysis"));

    QAction *visualGlobalAct = new QAction(tr("Global Visibility"), mainWindow);
    connect(visualGlobalAct, &QAction::triggered, this,
            [this, mainWindow] { OnVGAParallel(mainWindow, AnalysisType::VISUAL_GLOBAL_OPENMP); });
    vgaParallelMenu->addAction(visualGlobalAct);
    QAction *visualLocalAct = new QAction(tr("Local Visibility"), mainWindow);
    connect(visualLocalAct, &QAction::triggered, this,
            [this, mainWindow] { OnVGAParallel(mainWindow, AnalysisType::VISUAL_LOCAL_OPENMP); });
    vgaParallelMenu->addAction(visualLocalAct);
    QAction *visualLocalAdjMatrixAct =
        new QAction(tr("Local Visibility (Adjacency Matrix)"), mainWindow);
    connect(visualLocalAdjMatrixAct, &QAction::triggered, this, [this, mainWindow] {
        OnVGAParallel(mainWindow, AnalysisType::VISUAL_LOCAL_ADJMATRIX);
    });
    vgaParallelMenu->addAction(visualLocalAdjMatrixAct);
    QAction *metricAct = new QAction(tr("Global Metric"), mainWindow);
    connect(metricAct, &QAction::triggered, this,
            [this, mainWindow] { OnVGAParallel(mainWindow, AnalysisType::METRIC_OPENMP); });
    vgaParallelMenu->addAction(metricAct);
    QAction *angularAct = new QAction(tr("Global Angular"), mainWindow);
    connect(angularAct, &QAction::triggered, this,
            [this, mainWindow] { OnVGAParallel(mainWindow, AnalysisType::ANGULAR_OPENMP); });
    vgaParallelMenu->addAction(angularAct);

    return true;
}

void VGAParallelMainWindow::OnVGAParallel(MainWindow *mainWindow,
                                          VGAParallelMainWindow::AnalysisType analysisType) {
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
                             tr("Please make sure the displayed map is a vga map"), QMessageBox::Ok,
                             QMessageBox::Ok);
        return;
    }

    graphDoc->m_communicator = new CMSCommunicator();

    switch (analysisType) {
    case AnalysisType::VISUAL_GLOBAL_OPENMP: {

        bool ok;
        QString radiusText = QInputDialog::getText(
            mainWindow, tr("Visibility radius"),
            tr("This is the standard global-visibility analysis, parallelised "
               "with OpenMP.\nRadius can be from 1 to 99 or n"),
            QLineEdit::Normal, "n", &ok);
        if (!ok)
            return;
        auto &map = graphDoc->m_meta_graph->getDisplayedPointMap();
        auto radius = ConvertForVisibility(radiusText.toStdString());
        graphDoc->m_communicator->setAnalysis(std::unique_ptr<IAnalysis>(
            new VGAVisualGlobalOpenMP(map.getInternalMap(), radius, false)));
        graphDoc->m_communicator->setPostAnalysisFunc(
            [&map, radius](std::unique_ptr<IAnalysis> &analysis, AnalysisResult &) {
                map.overrideDisplayedAttribute(-2);
                map.setDisplayedAttribute(VGAVisualGlobalOpenMP::getColumnWithRadius(
                    VGAVisualGlobalOpenMP::Column::VISUAL_INTEGRATION_HH, radius));
            });
        break;
    }
    case AnalysisType::VISUAL_LOCAL_OPENMP: {
        auto &map = graphDoc->m_meta_graph->getDisplayedPointMap();
        graphDoc->m_communicator->setAnalysis(
            std::unique_ptr<IAnalysis>(new VGAVisualLocalOpenMP(map.getInternalMap())));
        graphDoc->m_communicator->setPostAnalysisFunc([&map](std::unique_ptr<IAnalysis> &analysis,
                                                             AnalysisResult &) {
            map.overrideDisplayedAttribute(-2);
            map.setDisplayedAttribute(VGAVisualLocalOpenMP::Column::VISUAL_CLUSTERING_COEFFICIENT);
        });
        break;
    }
    case AnalysisType::VISUAL_LOCAL_ADJMATRIX: {
        auto &map = graphDoc->m_meta_graph->getDisplayedPointMap();
        graphDoc->m_communicator->setAnalysis(
            std::unique_ptr<IAnalysis>(new VGAVisualLocalAdjMatrix(map.getInternalMap(), false)));
        graphDoc->m_communicator->setPostAnalysisFunc(
            [&map](std::unique_ptr<IAnalysis> &analysis, AnalysisResult &) {
                map.overrideDisplayedAttribute(-2);
                map.setDisplayedAttribute(
                    VGAVisualLocalAdjMatrix::Column::VISUAL_CLUSTERING_COEFFICIENT);
            });
        break;
    }
    case AnalysisType::METRIC_OPENMP: {
        bool ok;
        QString radiusText = QInputDialog::getText(
            mainWindow, tr("Metric radius"),
            tr("This is the standard global-metric analysis, parallelised "
               "with OpenMP.\nRadius can be any positive number or n for unlimited radius"),
            QLineEdit::Normal, "n", &ok);
        if (!ok)
            return;
        auto &map = graphDoc->m_meta_graph->getDisplayedPointMap();
        auto radius = ConvertForMetric(radiusText.toStdString());
        graphDoc->m_communicator->setAnalysis(
            std::unique_ptr<IAnalysis>(new VGAMetricOpenMP(map.getInternalMap(), radius, false)));
        graphDoc->m_communicator->setPostAnalysisFunc(
            [&map, radius](std::unique_ptr<IAnalysis> &analysis, AnalysisResult &) {
                map.overrideDisplayedAttribute(-2);
                map.setDisplayedAttribute(VGAMetricOpenMP::getColumnWithRadius(
                    VGAMetricOpenMP::Column::METRIC_MEAN_SHORTEST_PATH_DISTANCE, radius,
                    map.getInternalMap().getRegion()));
            });
        break;
    }
    case AnalysisType::ANGULAR_OPENMP: {

        bool ok;
        QString radiusText = QInputDialog::getText(
            mainWindow, tr("Angular radius"),
            tr("This is the standard global-angular analysis, parallelised "
               "with OpenMP.\nRadius can be any positive number or n for unlimited radius"),
            QLineEdit::Normal, "n", &ok);
        if (!ok)
            return;
        auto &map = graphDoc->m_meta_graph->getDisplayedPointMap();
        auto radius = ConvertForMetric(radiusText.toStdString());
        graphDoc->m_communicator->setAnalysis(
            std::unique_ptr<IAnalysis>(new VGAAngularOpenMP(map.getInternalMap(), radius, false)));
        graphDoc->m_communicator->setPostAnalysisFunc(
            [&map, radius](std::unique_ptr<IAnalysis> &analysis, AnalysisResult &) {
                map.overrideDisplayedAttribute(-2);
                map.setDisplayedAttribute(VGAAngularOpenMP::getColumnWithRadius(
                    VGAAngularOpenMP::Column::ANGULAR_MEAN_DEPTH, radius,
                    map.getInternalMap().getRegion()));
            });
        break;
    }
    case AnalysisType::NONE: {
        QMessageBox::warning(mainWindow, tr("Warning"), tr("Please select an analysis type"),
                             QMessageBox::Ok, QMessageBox::Ok);
        return;
    }
    }

    graphDoc->m_communicator->SetFunction(CMSCommunicator::FROMCONNECTOR);
    graphDoc->m_communicator->setSuccessUpdateFlags(QGraphDoc::NEW_DATA);
    graphDoc->m_communicator->setSuccessRedrawFlags(QGraphDoc::VIEW_ALL, QGraphDoc::REDRAW_GRAPH,
                                                    QGraphDoc::NEW_DATA);

    graphDoc->CreateWaitDialog(tr("Calculating shortest path..."));
    graphDoc->m_thread.render(graphDoc);
}

// Duplicating the radius converters here to keep the module self contained
// TODO: Transfer them to sala

double VGAParallelMainWindow::ConvertForVisibility(const std::string &radius) const {
    if (radius == "n") {
        return -1.0;
    }
    char *end;
    long rad = std::strtol(radius.c_str(), &end, 10);
    if (rad < 1 || rad > 99) {
        throw depthmapX::RuntimeException(
            std::string("Radius for visibility analysis must be n for the whole range or an "
                        "integer between 1 and 99 inclusive. Got ") +
            radius);
    }
    return static_cast<double>(rad);
}

double VGAParallelMainWindow::ConvertForMetric(const std::string &radius) const {
    if (radius == "n") {
        return -1.0;
    }
    char *end;
    double rad = strtod(radius.c_str(), &end);
    if (rad <= 0) {
        throw depthmapX::RuntimeException(
            std::string(
                "Radius for metric vga must be n for the whole range or a positive number. Got ") +
            radius);
    }
    if (std::isnan(rad)) {
        throw depthmapX::RuntimeException("Radius NaN?! Really?");
    }
    if (std::isinf(rad)) {
        throw depthmapX::RuntimeException("Radius inf?! Who are you kidding?");
    }

    return rad;
}
