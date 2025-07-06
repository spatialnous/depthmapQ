// SPDX-FileCopyrightText: 2011-2012 Tasos Varoudis
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "compatibilitydefines.hpp"
#include "consts.hpp"
#include "interfaceversion.hpp"
#include "mainwindow.hpp"

#include "dialogs/AgentAnalysisDlg.hpp"
#include "dialogs/AttributeChooserDlg.hpp"
#include "dialogs/AttributeSummary.hpp"
#include "dialogs/AxialAnalysisOptionsDlg.hpp"
#include "dialogs/ColumnPropertiesDlg.hpp"
#include "dialogs/ConvertShapesDlg.hpp"
#include "dialogs/FilePropertiesDlg.hpp"
#include "dialogs/GridDialog.hpp"
#include "dialogs/InsertColumnDlg.hpp"
#include "dialogs/IsovistPathDlg.hpp"
#include "dialogs/LayerChooserDlg.hpp"
#include "dialogs/MakeLayerDlg.hpp"
#include "dialogs/MakeOptionsDlg.hpp"
#include "dialogs/NewLayerDlg.hpp"
#include "dialogs/OptionsDlg.hpp"
#include "dialogs/PromptReplace.hpp"
#include "dialogs/PushDialog.hpp"
#include "dialogs/RenameObjectDlg.hpp"
#include "dialogs/SegmentAnalysisDlg.hpp"
#include "dialogs/TopoMetDlg.hpp"
#include "salalib/agents/agentanalysis.hpp"
#include "views/depthmapview/depthmapview.hpp"
#include "views/viewhelpers.hpp"

#include "salalib/agents/agentprogram.hpp"
#include "salalib/entityparsing.hpp"
#include "salalib/exportutils.hpp"
#include "salalib/importutils.hpp"
#include "salalib/linkutils.hpp"
#include "salalib/salaprogram.hpp"

#include <QFile>
#include <QMessageBox>
#include <QMetaType>
#include <QString>
#include <QtCore/QFile>
#include <QtWidgets/QFileDialog>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QPushButton>
#include <stdio.h>

QT_BEGIN_NAMESPACE
Q_DECLARE_METATYPE(std::string)

QGraphDoc::QGraphDoc(const QString &author, const QString &organisation) {
    m_opened_name = "";
    m_flag_lock = false;
    for (int i = 0; i < VIEW_TYPES; i++) {
        m_redraw_flag[i] = REDRAW_DONE;
        m_remenu_flag[i] = false;
        m_view[i] = NULL;
    }
    m_communicator = NULL;

    m_make_algorithm = 0;  // algorithm to make graph
    m_make_maxdist = -1.0; // maximum distance you can see (set to -1.0 for infinite)

    m_meta_graph = new MetaGraphDM;

    modifiedFlag = false;

    modify_prog = false;
    m_num_steps = 0;
    m_record = 0;
    m_step = 0;
    m_num_records = 0;

    std::string date = ViewHelpers::getCurrentDate();
    QString version = QString(TITLE_BASE);

    m_meta_graph->getInternalData().fileProperties.setProperties(
        author.toStdString(), organisation.toStdString(), date, version.toStdString());

    qRegisterMetaType<std::string>();
    connect(&m_thread, &RenderThread::runtimeExceptionThrown, this,
            &QGraphDoc::exceptionThrownInRenderThread);
    connect(&m_thread, &RenderThread::showWarningMessage, this,
            &QGraphDoc::messageFromRenderThread);
    connect(&m_thread, &RenderThread::closeWaitDialog, this, &QGraphDoc::DestroyWaitDialog);
}
void QGraphDoc::exceptionThrownInRenderThread(int type, std::string message) {
    if (type == sala::PointMapExceptionType::NO_ISOVIST_ANALYSIS) {
        std::stringstream message;
        message << "This operation requires isovist analysis. To run it go to: ";
        message << "Tools -> Visibility -> Run Visibility Graph Analysis... ";
        message << "and select \"Calculate isovist properties\"";
        QMessageBox::warning(this, tr("Warning"), tr(message.str().c_str()), QMessageBox::Ok,
                             QMessageBox::Ok);
    }
}

void QGraphDoc::messageFromRenderThread(QString title, QString message) {
    QMessageBox::warning(this, title, message, QMessageBox::Ok, QMessageBox::Ok);
}

bool QGraphDoc::SetRedrawFlag(int viewtype, int flag, int reason,
                              QWidget *originator) // (almost) thread safe
{

    if (viewtype == VIEW_ALL && flag != REDRAW_DONE) {
        ((MainWindow *)m_mainFrame)->updateGLWindows(true, flag == REDRAW_TOTAL);
    }
    if (viewtype == VIEW_MAP && flag == REDRAW_TOTAL) {
        ((MainWindow *)m_mainFrame)->updateGLWindows(false, true);
    }

    if (!m_flag_lock) {
        m_flag_lock = true;
        if (viewtype) {
            // it's the view calling itself
            if (m_redraw_flag[viewtype] < flag) {
                m_redraw_flag[viewtype] = flag;
                QApplication::postEvent(m_view[viewtype], new QEvent(QEvent::FocusIn));
            }
            if (flag == REDRAW_DONE) {
                m_redraw_flag[viewtype] = flag;
            }
        } else {
            for (int i = 1; i < VIEW_TYPES; i++) {
                if (m_view[i] && m_redraw_flag[i] < flag) {
                    m_redraw_flag[i] = flag;
                    QApplication::postEvent(m_view[i], new QEvent(QEvent::FocusIn));
                }
            }
        }
        m_flag_lock = false;
        return true;
    }
    return false;
}

void QGraphDoc::UpdateMainframestatus() {
    QString s1, s2, s3;
    if (!m_meta_graph->viewingNone()) {
        int n = 0;
        int state = m_meta_graph->getState();
        // showing the axial graph
        if ((state & MetaGraphDM::DX_SHAPEGRAPHS) &&
            m_meta_graph->getViewClass() & MetaGraphDM::DX_VIEWAXIAL) {
            n = (int)m_meta_graph->getDisplayedShapeGraph().getShapeCount();
        } else if ((state & MetaGraphDM::DX_DATAMAPS) &&
                   m_meta_graph->getViewClass() & MetaGraphDM::DX_VIEWDATA) {
            n = (int)m_meta_graph->getDisplayedDataMap().getShapeCount();
        }
        // either showing or constructing the VGA graph
        else if ((state & MetaGraphDM::DX_POINTMAPS) &&
                 m_meta_graph->getViewClass() & MetaGraphDM::DX_VIEWVGA) {
            n = (int)m_meta_graph->getDisplayedPointMap().getInternalMap().getFilledPointCount();
        }
        if (n > 0) {
            s1 = QString("%1   ").arg(n);
        }
        Region4f r = m_meta_graph->getBoundingBox();
        s2 = QString("%1,  %2   ").arg(r.width()).arg(r.height());
        s3 = QString("%1,  %2   ").arg(m_position.x).arg(m_position.y);
        ((MainWindow *)m_mainFrame)->UpdateStatus(s1, s2, s3);
    }
}

void QGraphDoc::SetUpdateFlag(int type, bool modified) {
    switch (type) {
    case NEW_FILE:
        QApplication::postEvent(
            (QObject *)m_mainFrame,
            new QmyEvent((enum QEvent::Type)FOCUSGRAPH, (void *)this, CONTROLS_LOADALL));
        break;
    case NEW_DATA:
        QApplication::postEvent(
            (QObject *)m_mainFrame,
            new QmyEvent((enum QEvent::Type)FOCUSGRAPH, (void *)this, CONTROLS_LOADATTRIBUTES));
        break;
    case NEW_TABLE:
        QApplication::postEvent(
            (QObject *)m_mainFrame,
            new QmyEvent((enum QEvent::Type)FOCUSGRAPH, (void *)this, CONTROLS_LOADGRAPH));
        break;
    case DELETED_TABLE:
        QApplication::postEvent(
            (QObject *)m_mainFrame,
            new QmyEvent((enum QEvent::Type)FOCUSGRAPH, (void *)this, CONTROLS_RELOADGRAPH));
        break;
    }
    SetRemenuFlag(VIEW_ALL, true); // Tell the views to update their menus
    if (modified) {
        modifiedFlag = true;
    }
}

LayerManagerImpl &QGraphDoc::getLayers(int type, std::optional<size_t> layer) {
    LayerManagerImpl *tab = NULL;
    if (type == -1) {
        type = m_meta_graph->getViewClass();
    }
    switch (type & MetaGraphDM::DX_VIEWFRONT) {
    case MetaGraphDM::DX_VIEWVGA:
        tab = (!layer.has_value()) ? &(m_meta_graph->getDisplayedPointMap().getLayers()) //
                                   : &(m_meta_graph->getPointMaps()[layer.value()].getLayers());
        break;
    case MetaGraphDM::DX_VIEWAXIAL:
        tab = (!layer.has_value()) ? &(m_meta_graph->getDisplayedShapeGraph().getLayers())
                                   : &(m_meta_graph->getShapeGraphs()[layer.value()].getLayers());
        break;
    case MetaGraphDM::DX_VIEWDATA:
        tab = (!layer.has_value()) ? &(m_meta_graph->getDisplayedDataMap().getLayers()) //
                                   : &(m_meta_graph->getDataMaps()[layer.value()].getLayers());
        break;
    }
    return *tab;
}

const LayerManagerImpl &QGraphDoc::getLayers(int type, std::optional<size_t> layer) const {
    const LayerManagerImpl *tab = NULL;
    if (type == -1) {
        type = m_meta_graph->getViewClass() & MetaGraphDM::DX_VIEWFRONT;
    }
    switch (type) {
    case MetaGraphDM::DX_VIEWVGA:
        tab = (!layer.has_value()) ? &(m_meta_graph->getDisplayedPointMap().getLayers()) //
                                   : &(m_meta_graph->getPointMaps()[layer.value()].getLayers());
        break;
    case MetaGraphDM::DX_VIEWAXIAL:
        tab = (!layer.has_value()) ? &(m_meta_graph->getDisplayedShapeGraph().getLayers())
                                   : &(m_meta_graph->getShapeGraphs()[layer.value()].getLayers());
        break;
    case MetaGraphDM::DX_VIEWDATA:
        tab = (!layer.has_value()) ? &(m_meta_graph->getDisplayedDataMap().getLayers()) //
                                   : &(m_meta_graph->getDataMaps()[layer.value()].getLayers());
        break;
    }
    return *tab;
}

AttributeTableHandle &QGraphDoc::getAttributeTableHandle(int type, std::optional<size_t> layer) {
    AttributeTableHandle *tab = NULL;
    if (type == -1) {
        type = m_meta_graph->getViewClass();
    }
    switch (type & MetaGraphDM::DX_VIEWFRONT) {
    case MetaGraphDM::DX_VIEWVGA:
        tab = (!layer.has_value())
                  ? &(m_meta_graph->getDisplayedPointMap().getAttributeTableHandle())
                  : &(m_meta_graph->getPointMaps()[layer.value()].getAttributeTableHandle());
        break;
    case MetaGraphDM::DX_VIEWAXIAL:
        tab = (!layer.has_value())
                  ? &(m_meta_graph->getDisplayedShapeGraph().getAttributeTableHandle())
                  : &(m_meta_graph->getShapeGraphs()[layer.value()].getAttributeTableHandle());
        break;
    case MetaGraphDM::DX_VIEWDATA:
        tab = (!layer.has_value())
                  ? &(m_meta_graph->getDisplayedDataMap().getAttributeTableHandle())
                  : &(m_meta_graph->getDataMaps()[layer.value()].getAttributeTableHandle());
        break;
    }
    return *tab;
}

const AttributeTableHandle &QGraphDoc::getAttributeTableHandle(int type,
                                                               std::optional<size_t> layer) const {
    const AttributeTableHandle *tab = NULL;
    if (type == -1) {
        type = m_meta_graph->getViewClass() & MetaGraphDM::DX_VIEWFRONT;
    }
    switch (type) {
    case MetaGraphDM::DX_VIEWVGA:
        tab = (!layer.has_value())
                  ? &(m_meta_graph->getDisplayedPointMap().getAttributeTableHandle())
                  : &(m_meta_graph->getPointMaps()[layer.value()].getAttributeTableHandle());
        break;
    case MetaGraphDM::DX_VIEWAXIAL:
        tab = (!layer.has_value())
                  ? &(m_meta_graph->getDisplayedShapeGraph().getAttributeTableHandle())
                  : &(m_meta_graph->getShapeGraphs()[layer.value()].getAttributeTableHandle());
        break;
    case MetaGraphDM::DX_VIEWDATA:
        tab = (!layer.has_value())
                  ? &(m_meta_graph->getDisplayedDataMap().getAttributeTableHandle())
                  : &(m_meta_graph->getDataMaps()[layer.value()].getAttributeTableHandle());
        break;
    }
    return *tab;
}

/////////////////////////////////////////////////////////////////////////////
// QGraphDoc commands

// New layer and delete layer

void QGraphDoc::OnLayerNew() {
    CNewLayerDlg dlg;
    if (QDialog::Accepted == dlg.exec()) {
        // insert a new layer of the correct type
        // for now, 0 = axial map, and 1 = data map
        ShapeMapDM *map;
        if (dlg.m_layer_type == 0) {
            int ref = m_meta_graph->addShapeMap(dlg.m_name.toStdString());
            m_meta_graph->setDisplayedDataMapRef(ref);
            map = &(m_meta_graph->getDataMaps()[ref]);
        } else if (dlg.m_layer_type == 1) {
            int ref = m_meta_graph->addShapeGraph(dlg.m_name.toStdString(), ShapeMap::CONVEXMAP);
            m_meta_graph->setDisplayedShapeGraphRef(ref);
            map = &m_meta_graph->getShapeGraphs()[size_t(ref)];
        } else if (dlg.m_layer_type == 2) {
            int ref = m_meta_graph->addShapeGraph(dlg.m_name.toStdString(), ShapeMap::AXIALMAP);
            m_meta_graph->setDisplayedShapeGraphRef(ref);
            map = &m_meta_graph->getShapeGraphs()[size_t(ref)];
        } else if (dlg.m_layer_type == 3) {
            int ref = m_meta_graph->addShapeGraph(dlg.m_name.toStdString(), ShapeMap::PESHMAP);
            m_meta_graph->setDisplayedShapeGraphRef(ref);
            map = &m_meta_graph->getShapeGraphs()[size_t(ref)];
        }

        Region4f r = m_meta_graph->getBoundingBox();
        if (r.atZero()) {
            r = Region4f(Point2f(-50.0, -50.0), Point2f(50.0, 50.0));
        }
        map->init(0, r);
        map->setEditable(true);

        SetUpdateFlag(NEW_TABLE);
        SetRedrawFlag(VIEW_ALL, REDRAW_GRAPH, NEW_DATA);
    }
}

void QGraphDoc::OnLayerDelete() {
    // Delete the currently displayed map
    if (QMessageBox::Yes == QMessageBox::question(this, tr("Notice"),
                                                  tr("Are you sure you want to delete this "
                                                     "map?\nThis action cannot be undone"),
                                                  QMessageBox::Yes | QMessageBox::No,
                                                  QMessageBox::No)) {
        m_meta_graph->removeDisplayedMap();
        SetUpdateFlag(DELETED_TABLE);
        SetRedrawFlag(VIEW_ALL, REDRAW_GRAPH, NEW_DATA);
    }
}

// Note: import and export are now on the Layer menu, not the file menu

// Import file types: .cat, .dxf, .ntf
// Txt files for points and lines (shapes)
void QGraphDoc::CreateWaitDialog(const QString &description, QWidget *pr) {
    modify_prog = false;
    m_waitdlg = new QProgressDialog();
    connect(m_waitdlg, SIGNAL(canceled()), this, SLOT(cancel_wait()));
    m_waitdlg->setCancelButtonText(tr("&Cancel"));
    m_waitdlg->setWindowTitle(description);
    m_waitdlg->setWindowFlags(Qt::WindowStaysOnTopHint | Qt::Dialog);
    m_waitdlg->show();
    Tid_progress = startTimer(50);
    m_timer.start();
}

void QGraphDoc::DestroyWaitDialog() {
    modify_prog = true;
    killTimer(Tid_progress);
    QApplication::postEvent((QObject *)m_waitdlg, new QEvent(QEvent::Close));
}

void QGraphDoc::cancel_wait() {
    if (m_communicator) {
        m_waitdlg->setLabelText(QString(" attempting to cancel"));
        if (m_communicator->IsCancelled()) {
        } else {
            m_communicator->Cancel();
        }
        // Don't cancel --- cancel should be handled by the thread!
    }
}

void QGraphDoc::timerEvent(QTimerEvent *event) {
    if (event->timerId() == Tid_progress) {
        if (modify_prog) {
            QApplication::postEvent((QObject *)m_waitdlg, new QEvent(QEvent::Close));
            killTimer(Tid_progress);
            return;
        }

        QString lstr = QString("Step %1 of %2").arg(m_step).arg(m_num_steps);
        m_waitdlg->setValue(m_record);

        double percent = (100.0 * double(m_record)) / double(m_num_records);
        QString str;
        int timeleft = 1 + int((100.0 / percent - 1.0) * double(m_timer.elapsed() / 1000));
        if (percent > 0.5) {
            if (timeleft >= 3600) {
                str = QString(" : Estimated %1 hours %2 minutes remaining")
                          .arg(timeleft / 3600)
                          .arg((timeleft / 60) % 60);
            } else if (timeleft >= 60) {
                str = QString(" : Estimated %1 minutes %2 seconds remaining")
                          .arg((timeleft / 60) % 60)
                          .arg(timeleft % 60);
            } else {
                str = QString(" : Estimated %1 seconds remaining").arg(timeleft % 60);
            }
        }
        lstr += str;
        m_waitdlg->setLabelText(lstr);
    }
}

void QGraphDoc::ProcPostMessage(int m, int x) {
    switch (m) {
    case Communicator::NUM_STEPS:
        m_num_steps = x;
        break;
    case Communicator::NUM_RECORDS:
        if (!modify_prog)
            m_waitdlg->setRange(0, x);
        m_record = 0;
        m_num_records = x;
        break;
    case Communicator::CURRENT_STEP:
        m_step = x;
        break;
    case Communicator::CURRENT_RECORD:
        m_record = x;
        if (m_record > m_num_records)
            m_record = m_num_records;
        break;
    }
}

void QGraphDoc::OnVGALinksFileImport() {
    if (m_communicator) {
        return; // Locked
    }

    // change the view before loading the file to make the changes apparent
    if (m_view[VIEW_MAP])
        ((QDepthmapView *)m_view[VIEW_MAP])->m_showlinks = true;
    SetRedrawFlag(VIEW_MAP, REDRAW_POINTS, NEW_DEPTHMAPVIEW_SETUP);

    QString template_string;
    template_string += "Text files (*.txt *.csv)\n";
    template_string += "All files (*.*)";

    QFileDialog::Options options;
    QString selectedFilter;
    QString infile = QFileDialog::getOpenFileName(0, tr("Import Links File"), "", template_string,
                                                  &selectedFilter, options);

    if (!infile.size()) {
        // no file selected
        return;
    }

    std::string fileName = infile.toStdString();

    std::ifstream fileStream(fileName);
    if (fileStream.fail()) {
        QMessageBox::warning(this, tr("Warning"),
                             tr("Unable to read text file.\nPlease check that "
                                "another program is not using it."),
                             QMessageBox::Ok, QMessageBox::Ok);
        return;
    } else {
        try {
            PointMapDM &currentMap = m_meta_graph->getDisplayedPointMap();
            std::vector<PixelRefPair> newLinks = sala::pixelateMergeLines(
                EntityParsing::parseLines(fileStream, '\t'), currentMap.getInternalMap());
            sala::mergePixelPairs(newLinks, currentMap.getInternalMap());
            SetRedrawFlag(VIEW_MAP, REDRAW_POINTS, NEW_DEPTHMAPVIEW_SETUP);
        } catch (EntityParsing::EntityParseException &e) {
            std::stringstream message;
            message << "Unable to parse text file\n\n";
            message << fileName;
            message << "\n\n Error: ";
            message << e.what();
            QMessageBox::warning(this, tr("Warning"), tr(message.str().c_str()), QMessageBox::Ok,
                                 QMessageBox::Ok);
        } catch (sala::InvalidLinkException &e) {
            std::stringstream message;
            message << "Unable to import links\n\n";
            message << fileName;
            message << "\n\n Error: ";
            message << e.what();
            QMessageBox::warning(this, tr("Warning"), tr(message.str().c_str()), QMessageBox::Ok,
                                 QMessageBox::Ok);
        }
    }
}

void QGraphDoc::OnGenerateIsovistsFromFile() {
    if (m_communicator) {
        return; // Locked
    }

    QString template_string;
    template_string += "Text files (*.txt *.csv)\n";
    template_string += "All files (*.*)";

    QFileDialog::Options options;
    QString selectedFilter;
    QString infile = QFileDialog::getOpenFileName(0, tr("Import Isovists File"), "",
                                                  template_string, &selectedFilter, options);

    if (!infile.size()) {
        // no file selected
        return;
    }

    std::string fileName = infile.toStdString();

    std::ifstream fileStream(fileName);
    if (fileStream.fail()) {
        QMessageBox::warning(this, tr("Warning"),
                             tr("Unable to read text file.\nPlease check that "
                                "another program is not using it."),
                             QMessageBox::Ok, QMessageBox::Ok);
        return;
    } else {
        // communicator and wait dialog to prevent draw while this operation is in
        // progress and also because the isovist generation requires both
        m_communicator = new CMSCommunicator;
        m_communicator->SetInfile2(qPrintable(infile));
        CreateWaitDialog(tr("Generating isovists..."));
        m_communicator->SetFunction(CMSCommunicator::MAKEISOVISTSFROMFILE);
        m_thread.render(this);
    }
}

void QGraphDoc::OnFileImport() {
    if (m_communicator) {
        return; // Locked
    }

    QString template_string;
    template_string += "All formats(*.dxf *.ntf *.gml *.cat *.rt1 *.mif *.txt *.csv)\n";
    template_string += "Drawing files (*.dxf *.ntf *.gml *.cat *.rt1)\nMapInfo "
                       "map (*.mif)\nText files (*.txt *.csv)\n";

    template_string += "All files (*.*)";

    QFileDialog::Options options;
    QString selectedFilter;
    QStringList infiles = QFileDialog::getOpenFileNames(0, tr("Import Files"), "", template_string,
                                                        &selectedFilter, options);

    if (!infiles.size()) {
        return;
    }

    // this is placed here as a proxy to be queried later so that we can find
    // if there was something else in the graph after importing. If there was
    // then the view should not be reset, if there wasn't then the view can
    // be reset to point to the newly imported objects
    bool graphHadNullBoundsBeforeImport = m_meta_graph->getBoundingBox().atZero();

    QFilePath filepath(infiles[0]);
    QString ext = filepath.m_ext;
    if (ext == tr("CAT") || ext == tr("DXF") || ext == tr("NTF") || ext == tr("RT1") ||
        ext == tr("MIF") || ext == tr("GML") || ext == tr("")) {
        for (int i = 1; i < infiles.size(); i++) {
            QFilePath filepath(infiles[i]);
            if (filepath.m_ext != ext ||
                !(filepath.m_ext == tr("RT1") || filepath.m_ext == tr("NTF") ||
                  filepath.m_ext == tr("GML") || filepath.m_ext == tr(""))) {
                QMessageBox::warning(
                    this, tr("Warning"),
                    tr("You have selected more than one file.  Unfortunately, this "
                       "feature is only currently available with NTF, GML and Tiger "
                       "line files.\nPlease select a single file to import only."),
                    QMessageBox::Ok, QMessageBox::Ok);
                return;
            }
        }
        // OS files don't have GML suffix for some bizarre reason
        if (ext == tr("")) {
            ext = tr("GML");
        }
        int graph_option = MetaGraphDM::DX_ADD;
        bool ok = true;

        if (ok) {
            m_communicator = new CMSCommunicator;
            if (ext != tr("RT1") && ext != tr("NTF") &&
                ext != tr("GML")) { // ntf, gml & rt1 use filesets (all others use
                                    // standard file at a time)
                m_communicator->SetInfile(qPrintable(infiles[0]));
            }
            if (ext != tr("MIF")) {
                CreateWaitDialog(tr("Importing file..."));
                m_communicator->SetFunction(CMSCommunicator::IMPORT);
                if (ext == tr("CAT")) {
                    m_communicator->SetOption(MetaGraphDM::DX_CAT | graph_option);
                } else if (ext == tr("DXF")) {
                    m_communicator->SetOption(MetaGraphDM::DX_DXF | graph_option);
                } else if (ext == tr("NTF")) {
                    m_communicator->SetOption(MetaGraphDM::DX_NTF | graph_option);
                    m_communicator->SetFileSet(infiles);
                } else if (ext == tr("GML")) {
                    m_communicator->SetOption(MetaGraphDM::DX_GML | graph_option);
                    m_communicator->SetFileSet(infiles);
                } else if (ext == tr("RT1")) {
                    m_communicator->SetOption(MetaGraphDM::DX_RT1 | graph_option);
                    m_communicator->SetFileSet(infiles);
                }
            } else {
                int thedot = infiles[0].lastIndexOf('.');
                QString infile2 = infiles[0].left(thedot + 1) + tr("mid");
                m_communicator->SetInfile2(qPrintable(infile2));
                CreateWaitDialog(tr("Importing file..."));
                m_communicator->SetFunction(CMSCommunicator::IMPORTMIF);
            }
            m_thread.render(this);
        }
    } else if (ext == tr("TXT") || ext == tr("CSV")) {
        std::ifstream file(infiles[0].toLatin1());
        if (file.fail()) {
            QMessageBox::warning(this, tr("Warning"),
                                 tr("Unable to read text file.\nPlease check that "
                                    "another program is not using it."),
                                 QMessageBox::Ok, QMessageBox::Ok);
        } else {
            std::unique_ptr<Communicator> comm(new CMSCommunicator());
            std::vector<ShapeMap> newShapeMaps;
            try {
                newShapeMaps = sala::importFile(
                    file, comm.get(), filepath.m_name.toStdString(), sala::ImportType::DATAMAP,
                    (ext == tr("CSV")) ? sala::ImportFileType::CSV : sala::ImportFileType::TSV);
            } catch (Communicator::CancelledException &e) {
                QMessageBox::warning(this, tr("Info"), tr("User cancelled import"), QMessageBox::Ok,
                                     QMessageBox::Ok);
            }
            m_meta_graph->setViewClass(MetaGraphDM::DX_SHOWSHAPETOP);

            if (newShapeMaps.size() > 0) {
                for (auto &shapeMap : newShapeMaps) {
                    m_meta_graph->getDataMaps().emplace_back(
                        std::make_unique<ShapeMap>(std::move(shapeMap)));
                    m_meta_graph->getDataMaps().back().invalidateDisplayedAttribute();
                    m_meta_graph->getDataMaps().back().setDisplayedAttribute(-1);
                }
                // This should have added a new data map:
                SetUpdateFlag(NEW_TABLE);

                if (graphHadNullBoundsBeforeImport) {
                    SetRedrawFlag(VIEW_ALL, REDRAW_TOTAL, NEW_TABLE);
                } else {
                    SetRedrawFlag(VIEW_ALL, REDRAW_GRAPH, NEW_TABLE);
                }
            } else {
                QMessageBox::warning(this, tr("Warning"), tr("Unable to import text file.\n \
													Unable to import text file.\n\
													Depthmap can import tab-delimited or comma separated files.\n\
													There must be some spatial data.\n\
													The spatial data can either be:\n\
													Points with X and Y values, or\n\
													points with Easting and Northing values, or\n\
													lines with X1,Y1 and X2,Y2 values"),
                                     QMessageBox::Ok, QMessageBox::Ok);
            }
        }
    } else {
        QMessageBox::warning(this, tr("Warning"),
                             tr("Unrecognised file format.  Sorry, unable to import this file."),
                             QMessageBox::Ok, QMessageBox::Ok);
    }
}

// Export file types: .txt (point files)

void QGraphDoc::OnFileExport() {
    if (m_communicator) {
        QMessageBox::warning(this, tr("Notice"),
                             tr("Sorry, cannot export as another process is running"),
                             QMessageBox::Ok, QMessageBox::Ok);
        return; // Locked
    }
    if (m_meta_graph->viewingNone()) {
        QMessageBox::warning(this, tr("Notice"),
                             tr("Sorry, cannot export as there is no data to export"),
                             QMessageBox::Ok, QMessageBox::Ok);
        return; // No graph to export
    }

    QString suffix;
    int mode = -1;

    int view_class = m_meta_graph->getViewClass();
    if (view_class & MetaGraphDM::DX_VIEWAXIAL) {
        mode = 0;
        suffix = m_meta_graph->getDisplayedShapeGraph().getName().c_str();
    } else if (view_class & MetaGraphDM::DX_VIEWDATA) {
        mode = 1;
        suffix = m_meta_graph->getDisplayedDataMap().getName().c_str();
    } else if (view_class & MetaGraphDM::DX_VIEWVGA) {
        if (m_meta_graph->getDisplayedPointMap().getInternalMap().isProcessed()) {
            mode = 2;
            suffix = tr("vga");
        } else {
            mode = 3;
            suffix = tr("points");
        }
    }

    if (mode == -1) {
        QMessageBox::warning(this, tr("Notice"),
                             tr("Sorry, saving the currently displayed "
                                "layer is not supported"),
                             QMessageBox::Ok, QMessageBox::Ok);
        return;
    }
    suffix.replace(' ', '_');

    QFilePath path(m_opened_name);
    QString defaultname =
        path.m_path + (path.m_name.isEmpty() ? windowTitle() : path.m_name) + tr("_") + suffix;

    QString template_string = tr("Tab-delimited text file (*.txt)\n");
    if (mode < 3) {
        template_string += tr("Comma separated values file (*.csv)\n");
        template_string += tr("Graph file (*.graph)\n");
        template_string += tr("MapInfo file (*.mif)\n");
        template_string += tr("Pajek (*.net)\n");
    }
    template_string += tr("All files (*.*)");

    QFileDialog::Options options;
    QString selectedFilter;
    QString outfile = QFileDialog::getSaveFileName(0, tr("Save Output As"), defaultname,
                                                   template_string, &selectedFilter, options);
    if (outfile.isEmpty()) {
        return;
    }

    FILE *fp = fopen(outfile.toLatin1(), "wb");
    fclose(fp);

    QFilePath filepath(outfile);
    QString ext = filepath.m_ext;

    if (ext != tr("MIF") && ext != tr("GRAPH") && ext != tr("NET")) {
        std::ofstream stream(outfile.toLatin1());
        char delimiter = '\t';
        if (ext == tr("CSV")) {
            delimiter = ',';
        }
        if (stream.fail() || stream.bad()) {
            QMessageBox::warning(this, tr("Notice"), tr("Sorry, unable to open file for export"),
                                 QMessageBox::Ok, QMessageBox::Ok);
            mode = -1;
        }

        switch (mode) {
        case 0:
            m_meta_graph->getDisplayedShapeGraph().getInternalMap().output(stream, delimiter);
            break;
        case 1:
            m_meta_graph->getDisplayedDataMap().getInternalMap().output(stream, delimiter);
            break;
        case 2:
            m_meta_graph->getDisplayedPointMap().getInternalMap().outputSummary(stream, delimiter);
            break;
        case 3:
            m_meta_graph->getDisplayedPointMap().getInternalMap().outputPoints(stream, delimiter);
            break;
        default:
            break;
        }
        stream.close();
    } else if (ext == tr("GRAPH")) {
        if (mode >= 3) {
            QMessageBox::warning(this, tr("Notice"),
                                 tr("Sorry, only VGA, axial and shape data "
                                    "can be exported to graph files"),
                                 QMessageBox::Ok, QMessageBox::Ok);
            return;
        }

        if (m_meta_graph->write(outfile.toStdString(), METAGRAPH_VERSION, true) !=
            MetaGraphReadWrite::ReadWriteStatus::OK) { // <- true writes current layer only
            QMessageBox::warning(this, tr("Notice"), tr("Sorry, unable to open file for export"),
                                 QMessageBox::Ok, QMessageBox::Ok);
        }
    } else if (ext == tr("NET")) {
        if (mode != 0 && mode != 2) {
            QMessageBox::warning(this, tr("Notice"),
                                 tr("Sorry, only VGA graphs or shape graphs "
                                    "can be exported to Pajek .net files"),
                                 QMessageBox::Ok, QMessageBox::Ok);
            return;
        }
        std::ofstream stream(outfile.toLatin1());
        if (!stream) {
            QMessageBox::warning(this, tr("Notice"), tr("Sorry, unable to open file for export"),
                                 QMessageBox::Ok, QMessageBox::Ok);
        } else {
            if (mode == 0) {
                m_meta_graph->getDisplayedShapeGraph().getInternalMap().outputNet(stream);
            } else if (mode == 2) {
                m_meta_graph->getDisplayedPointMap().getInternalMap().outputNet(stream);
            }
        }
    } else {
        if (mode >= 3) {
            QMessageBox::warning(this, tr("Notice"),
                                 tr("Sorry, only VGA, axial and shape data "
                                    "can be exported to MapInfo tables"),
                                 QMessageBox::Ok, QMessageBox::Ok);
            return;
        }

        int thedot = outfile.indexOf('.');
        QString outfile2 = outfile.left(thedot + 1) + tr("mid");

        std::ofstream miffile(outfile.toLatin1());
        if (miffile.fail() || miffile.bad()) {
            QMessageBox::warning(this, tr("Notice"), tr("Sorry, unable to open file for export"),
                                 QMessageBox::Ok, QMessageBox::Ok);
            mode = -1;
        }

        std::ofstream midfile(outfile2.toLatin1());
        if (midfile.fail() || midfile.bad()) {
            QMessageBox::warning(
                this, tr("Notice"),
                tr("Sorry, unable to open associated .mid file (%1) for export (%2)")
                    .arg(outfile2.toLatin1())
                    .arg(midfile.fail() ? "failed reading" : "bad file"),
                QMessageBox::Ok, QMessageBox::Ok);
            mode = -1;
        }

        if (mode == 0) {
            m_meta_graph->getDisplayedShapeGraph().getInternalMap().outputMifMap(miffile, midfile);
        } else if (mode == 1) {
            m_meta_graph->getDisplayedDataMap().getInternalMap().outputMifMap(miffile, midfile);
        } else if (mode == 2) {
            m_meta_graph->getDisplayedPointMap().getInternalMap().outputMif(miffile, midfile);
        }
    }
}

void QGraphDoc::OnFileExportMapGeometry() {
    if (m_communicator) {
        QMessageBox::warning(this, tr("Notice"),
                             tr("Sorry, cannot export as another process is running"),
                             QMessageBox::Ok, QMessageBox::Ok);
        return; // Locked
    }
    if (m_meta_graph->viewingNone()) {
        QMessageBox::warning(this, tr("Notice"),
                             tr("Sorry, cannot export as there is no data to export"),
                             QMessageBox::Ok, QMessageBox::Ok);
        return; // No graph to export
    }

    QString suffix;
    int mode = -1;

    int view_class = m_meta_graph->getViewClass();
    if (view_class & MetaGraphDM::DX_VIEWAXIAL) {
        mode = 0;
        suffix = m_meta_graph->getDisplayedShapeGraph().getName().c_str();
    } else if (view_class & MetaGraphDM::DX_VIEWDATA) {
        mode = 1;
        suffix = m_meta_graph->getDisplayedDataMap().getName().c_str();
    }

    if (mode == -1) {
        QMessageBox::warning(this, tr("Notice"),
                             tr("Sorry, saving the currently displayed layer "
                                "is not supported"),
                             QMessageBox::Ok, QMessageBox::Ok);
        return;
    }
    suffix.replace(' ', '_');

    QFilePath path(m_opened_name);
    QString defaultname =
        path.m_path + (path.m_name.isEmpty() ? windowTitle() : path.m_name) + tr("_") + suffix;

    QString template_string = tr("Chiron and Alasdair Transfer Format file (*.cat)\n");

    QFileDialog::Options options;
    QString selectedFilter;
    QString outfile = QFileDialog::getSaveFileName(0, tr("Save Output As"), defaultname,
                                                   template_string, &selectedFilter, options);
    if (outfile.isEmpty()) {
        return;
    }

    FILE *fp = fopen(outfile.toLatin1(), "wb");
    fclose(fp);

    QFilePath filepath(outfile);
    QString ext = filepath.m_ext;

    if (ext == "CAT") {
        std::ofstream stream(outfile.toLatin1());
        if (stream.fail() || stream.bad()) {
            QMessageBox::warning(this, tr("Notice"), tr("Sorry, unable to open file for export"),
                                 QMessageBox::Ok, QMessageBox::Ok);
            mode = -1;
        }

        switch (mode) {
        case 0:
            exportUtils::writeMapShapesAsCat(
                m_meta_graph->getDisplayedShapeGraph().getInternalMap(), stream);
            break;
        case 1:
            exportUtils::writeMapShapesAsCat(m_meta_graph->getDisplayedDataMap().getInternalMap(),
                                             stream);
            break;
        default:
            break;
        }
        stream.close();
    }
}

void QGraphDoc::OnFileExportLinks() {
    if (m_communicator) {
        QMessageBox::warning(this, tr("Notice"),
                             tr("Sorry, cannot export as another process is running"),
                             QMessageBox::Ok, QMessageBox::Ok);
        return; // Locked
    }
    if (m_meta_graph->viewingNone()) {
        QMessageBox::warning(this, tr("Notice"),
                             tr("Sorry, cannot export as there is no data to export"),
                             QMessageBox::Ok, QMessageBox::Ok);
        return; // No graph to export
    }

    QString suffix;
    int mode = -1;

    int view_class = m_meta_graph->getViewClass();
    if (view_class & MetaGraphDM::DX_VIEWAXIAL) {
        mode = 5;
        suffix = tr("unlinks");
    } else if (view_class & MetaGraphDM::DX_VIEWDATA) {
        mode = 6;
        suffix = tr("links");
    } else if (view_class & MetaGraphDM::DX_VIEWVGA) {
        if (m_meta_graph->getDisplayedPointMap().getInternalMap().isProcessed()) {
            mode = 4;
            suffix = tr("merge_lines");
        }
    }

    if (mode == -1) {
        QMessageBox::warning(this, tr("Notice"),
                             tr("Sorry, saving the currently displayed "
                                "layer is not supported"),
                             QMessageBox::Ok, QMessageBox::Ok);
        return;
    }
    suffix.replace(' ', '_');

    QFilePath path(m_opened_name);
    QString defaultname =
        path.m_path + (path.m_name.isEmpty() ? windowTitle() : path.m_name) + tr("_") + suffix;

    QString template_string = tr("Tab-delimited text file (*.txt)\n");
    template_string += tr("Comma separated values file (*.csv)\n");
    template_string += tr("All files (*.*)");

    QFileDialog::Options options;
    QString selectedFilter;
    QString outfile = QFileDialog::getSaveFileName(0, tr("Save Output As"), defaultname,
                                                   template_string, &selectedFilter, options);
    if (outfile.isEmpty()) {
        return;
    }

    FILE *fp = fopen(outfile.toLatin1(), "wb");
    fclose(fp);

    QFilePath filepath(outfile);
    QString ext = filepath.m_ext;

    std::ofstream stream(outfile.toLatin1());
    char delimiter = '\t';
    if (ext == "CSV") {
        delimiter = ',';
    }
    if (stream.fail() || stream.bad()) {
        QMessageBox::warning(this, tr("Notice"), tr("Sorry, unable to open file for export"),
                             QMessageBox::Ok, QMessageBox::Ok);
        mode = -1;
    }

    switch (mode) {
    case 0:
        m_meta_graph->getDisplayedShapeGraph().getInternalMap().output(stream, delimiter);
        break;
    case 1:
        m_meta_graph->getDisplayedDataMap().getInternalMap().output(stream, delimiter);
        break;
    case 2:
        m_meta_graph->getDisplayedPointMap().getInternalMap().outputSummary(stream, delimiter);
        break;
    case 3:
        m_meta_graph->getDisplayedPointMap().getInternalMap().outputPoints(stream, delimiter);
        break;
    case 4:
        m_meta_graph->getDisplayedPointMap().getInternalMap().outputMergeLines(stream, delimiter);
        break;
    case 5:
        // note: specific to line graphs
        m_meta_graph->getDisplayedShapeGraph().getInternalMap().outputUnlinkPoints(stream,
                                                                                   delimiter);
        break;
    default:
        break;
    }
    stream.close();
}

void QGraphDoc::OnAxialConnectionsExportAsDot() {
    if (m_communicator) {
        QMessageBox::warning(this, tr("Notice"),
                             tr("Sorry, cannot export as another process is running"),
                             QMessageBox::Ok, QMessageBox::Ok);
        return; // Locked
    }
    if (m_meta_graph->viewingNone()) {
        QMessageBox::warning(this, tr("Notice"),
                             tr("Sorry, cannot export as there is no data to export"),
                             QMessageBox::Ok, QMessageBox::Ok);
        return; // No graph to export
    }

    auto &shapeGraph = m_meta_graph->getDisplayedShapeGraph();

    QString suffix = tr("axial_connections");

    QFilePath path(m_opened_name);
    QString defaultname =
        path.m_path + (path.m_name.isEmpty() ? windowTitle() : path.m_name) + tr("_") + suffix;

    QString template_string = tr("Dot graph file (*.dot)");

    QFileDialog::Options options;
    QString selectedFilter;
    QString outfile = QFileDialog::getSaveFileName(0, tr("Save Output As"), defaultname,
                                                   template_string, &selectedFilter, options);
    if (outfile.isEmpty()) {
        return;
    }

    FILE *fp = fopen(outfile.toLatin1(), "wb");
    fclose(fp);

    std::ofstream stream(outfile.toLatin1());

    if (stream.fail() || stream.bad()) {
        QMessageBox::warning(this, tr("Notice"), tr("Sorry, unable to open file for export"),
                             QMessageBox::Ok, QMessageBox::Ok);
        return;
    }
    shapeGraph.getInternalMap().writeAxialConnectionsAsDotGraph(stream);

    stream.close();
}

void QGraphDoc::OnAxialConnectionsExportAsPairCSV() {
    if (m_communicator) {
        QMessageBox::warning(this, tr("Notice"),
                             tr("Sorry, cannot export as another process is running"),
                             QMessageBox::Ok, QMessageBox::Ok);
        return; // Locked
    }
    if (m_meta_graph->viewingNone()) {
        QMessageBox::warning(this, tr("Notice"),
                             tr("Sorry, cannot export as there is no data to export"),
                             QMessageBox::Ok, QMessageBox::Ok);
        return; // No graph to export
    }

    auto &shapeGraph = m_meta_graph->getDisplayedShapeGraph();

    QString suffix = tr("axial_connections");

    QFilePath path(m_opened_name);
    QString defaultname =
        path.m_path + (path.m_name.isEmpty() ? windowTitle() : path.m_name) + tr("_") + suffix;

    QString template_string = tr("CSV graph file (*.csv)");

    QFileDialog::Options options;
    QString selectedFilter;
    QString outfile = QFileDialog::getSaveFileName(0, tr("Save Output As"), defaultname,
                                                   template_string, &selectedFilter, options);
    if (outfile.isEmpty()) {
        return;
    }

    FILE *fp = fopen(outfile.toLatin1(), "wb");
    fclose(fp);

    std::ofstream stream(outfile.toLatin1());

    if (stream.fail() || stream.bad()) {
        QMessageBox::warning(this, tr("Notice"), tr("Sorry, unable to open file for export"),
                             QMessageBox::Ok, QMessageBox::Ok);
        return;
    }
    shapeGraph.getInternalMap().writeAxialConnectionsAsPairsCSV(stream);

    stream.close();
}

void QGraphDoc::OnSegmentConnectionsExportAsPairCSV() {
    if (m_communicator) {
        QMessageBox::warning(this, tr("Notice"),
                             tr("Sorry, cannot export as another process is running"),
                             QMessageBox::Ok, QMessageBox::Ok);
        return; // Locked
    }
    if (m_meta_graph->viewingNone()) {
        QMessageBox::warning(this, tr("Notice"),
                             tr("Sorry, cannot export as there is no data to export"),
                             QMessageBox::Ok, QMessageBox::Ok);
        return; // No graph to export
    }

    auto &shapeGraph = m_meta_graph->getDisplayedShapeGraph();

    QString suffix = tr("segment_connections");

    QFilePath path(m_opened_name);
    QString defaultname =
        path.m_path + (path.m_name.isEmpty() ? windowTitle() : path.m_name) + tr("_") + suffix;

    QString template_string = tr("CSV graph file (*.csv)");

    QFileDialog::Options options;
    QString selectedFilter;
    QString outfile = QFileDialog::getSaveFileName(0, tr("Save Output As"), defaultname,
                                                   template_string, &selectedFilter, options);
    if (outfile.isEmpty()) {
        return;
    }

    FILE *fp = fopen(outfile.toLatin1(), "wb");
    fclose(fp);

    std::ofstream stream(outfile.toLatin1());

    if (stream.fail() || stream.bad()) {
        QMessageBox::warning(this, tr("Notice"), tr("Sorry, unable to open file for export"),
                             QMessageBox::Ok, QMessageBox::Ok);
        return;
    }
    shapeGraph.getInternalMap().writeSegmentConnectionsAsPairsCSV(stream);

    stream.close();
}

void QGraphDoc::OnPointmapExportConnectionsAsCSV() {
    if (m_communicator) {
        QMessageBox::warning(this, tr("Notice"),
                             tr("Sorry, cannot export as another process is running"),
                             QMessageBox::Ok, QMessageBox::Ok);
        return; // Locked
    }
    if (m_meta_graph->viewingNone()) {
        QMessageBox::warning(this, tr("Notice"),
                             tr("Sorry, cannot export as there is no data to export"),
                             QMessageBox::Ok, QMessageBox::Ok);
        return; // No graph to export
    }
    if (!(m_meta_graph->getViewClass() & MetaGraphDM::DX_VIEWVGA)) {
        QMessageBox::warning(this, tr("Error"), tr("Make sure a Visibility Graph is visible"),
                             QMessageBox::Ok, QMessageBox::Ok);
        return; // No graph to export
    }
    if (!m_meta_graph->viewingProcessedPoints()) {
        QMessageBox::warning(this, tr("Error"),
                             tr("Make sure the visibility graph was created (Tools "
                                "-> Visibility -> Make Visibility Graph)"),
                             QMessageBox::Ok, QMessageBox::Ok);
        return; // No graph to export
    }

    auto &pointMap = m_meta_graph->getDisplayedPointMap();

    QString suffix = tr("connectivity");

    QFilePath path(m_opened_name);
    QString defaultname =
        path.m_path + (path.m_name.isEmpty() ? windowTitle() : path.m_name) + tr("_") + suffix;

    QString template_string = tr("CSV graph file (*.csv)");

    QFileDialog::Options options;
    QString selectedFilter;
    QString outfile = QFileDialog::getSaveFileName(0, tr("Save Output As"), defaultname,
                                                   template_string, &selectedFilter, options);
    if (outfile.isEmpty()) {
        return;
    }

    FILE *fp = fopen(outfile.toLatin1(), "wb");
    fclose(fp);

    std::ofstream stream(outfile.toLatin1());

    if (stream.fail() || stream.bad()) {
        QMessageBox::warning(this, tr("Notice"), tr("Sorry, unable to open file for export"),
                             QMessageBox::Ok, QMessageBox::Ok);
        return;
    }
    pointMap.getInternalMap().outputConnectionsAsCSV(stream, ",");

    stream.close();
}

void QGraphDoc::OnSwapColours() {
    DisplayParams displayparams;
    if (m_meta_graph->getViewClass() & MetaGraphDM::DX_VIEWVGA) {
        displayparams = m_meta_graph->getDisplayedPointMap().getDisplayParams();
        float blue = displayparams.blue;
        displayparams.blue = displayparams.red;
        displayparams.red = blue;
        m_meta_graph->getDisplayedPointMap().setDisplayParams(displayparams);
    } else if (m_meta_graph->getViewClass() & MetaGraphDM::DX_VIEWAXIAL) {
        displayparams = m_meta_graph->getDisplayedShapeGraph().getDisplayParams();
        float blue = displayparams.blue;
        displayparams.blue = displayparams.red;
        displayparams.red = blue;
        m_meta_graph->getDisplayedShapeGraph().setDisplayParams(displayparams);
    } else if (m_meta_graph->getViewClass() & MetaGraphDM::DX_VIEWDATA) {
        displayparams = m_meta_graph->getDisplayedDataMap().getDisplayParams();
        float blue = displayparams.blue;
        displayparams.blue = displayparams.red;
        displayparams.red = blue;
        m_meta_graph->getDisplayedDataMap().setDisplayParams(displayparams);
    }

    SetRedrawFlag(VIEW_ALL, QGraphDoc::REDRAW_GRAPH, NEW_DEPTHMAPVIEW_SETUP);
}

/////////////////////////////////////////////////////////////////////////////

void QGraphDoc::OnEditGrid() {
    if (m_communicator) {
        QMessageBox::warning(this, tr("Warning"), tr("Please wait, another task is running"),
                             QMessageBox::Ok, QMessageBox::Ok);
        return;
    }
    bool newmap = false;

    if (m_meta_graph->getPointMaps().empty() ||
        m_meta_graph->getDisplayedPointMap().getInternalMap().isProcessed()) {
        // this can happen if there are no displayed maps -- so flag new map
        // required:
        newmap = true;
    } else if (m_meta_graph->getDisplayedPointMap().getInternalMap().getFilledPointCount() != 0) {
        if (QMessageBox::Yes !=
            QMessageBox::question(this, tr("Notice"),
                                  tr("This will clear existing points.  Do you want to continue?"),
                                  QMessageBox::Yes | QMessageBox::No, QMessageBox::No))
            return;
    }
    Region4f r = m_meta_graph->getRegion();
    CGridDialog dlg(std::max(r.width(), r.height()));
    if (QDialog::Accepted == dlg.exec()) {
        if (newmap) {
            m_meta_graph->addNewPointMap();
        }
        m_meta_graph->setGrid(dlg.getSpacing(), Point2f(0.0f, 0.0f));
        m_meta_graph->setShowGrid(true);
        SetUpdateFlag(NEW_TABLE);
        SetRedrawFlag(VIEW_ALL, REDRAW_GRAPH, NEW_DATA);
    }
}

// AV TV // semifilled
void QGraphDoc::OnFillPoints(const Point2f &p,
                             int fill_type) // semifilled = 0 (intention to use
                                            // semifilled steps for part filled)
{
    int state = m_meta_graph->getState();
    if (m_communicator) {
        QMessageBox::warning(this, tr("Notice"), tr("Please wait, another task is running"),
                             QMessageBox::Ok, QMessageBox::Ok);
        return;
    }
    if (~state & MetaGraphDM::DX_LINEDATA) {
        QMessageBox::warning(this, tr("Notice"),
                             tr("Sorry, line drawing data must be loaded before "
                                "points may be filled"),
                             QMessageBox::Ok, QMessageBox::Ok);
        return;
    }
    if (~state & MetaGraphDM::DX_POINTMAPS) {
        QMessageBox::warning(this, tr("Notice"), tr("Please make grid before filling"),
                             QMessageBox::Ok, QMessageBox::Ok);
        return;
    }
    if (m_meta_graph->viewingProcessed()) {
        QMessageBox::warning(this, tr("Notice"),
                             tr("Sorry, once the graph has been constructed, more "
                                "points may not be added"),
                             QMessageBox::Ok, QMessageBox::Ok);
        return;
    }

    m_communicator = new CMSCommunicator();
    m_communicator->SetSeedPoint(p);
    m_communicator->SetOption(fill_type); // AV TV
    CreateWaitDialog(tr("Generating grid points..."));
    m_communicator->SetFunction(CMSCommunicator::MAKEPOINTS);

    m_thread.render(this);
}

// convert any shape layer to any other (certain rules apply)

void QGraphDoc::OnLayerConvert() {
    if (m_communicator) {
        QMessageBox::warning(this, tr("Notice"), tr("Please wait, another task is running"),
                             QMessageBox::Ok, QMessageBox::Ok);
        return;
    }

    CMakeLayerDlg dlg;

    dlg.m_mapin = MAKELAYER_DATA; // mapin is a data map (assume)
    dlg.m_mapout = 0;             // no possible map out

    if (m_meta_graph->viewingProcessedLines()) {
        int type = m_meta_graph->getDisplayedShapeGraph().getInternalMap().getMapType();
        // map in is an axial map (of sorts, something with a graph...)
        dlg.m_mapin = MAKELAYER_AXIAL;
        dlg.m_origin = QString("Shape Graphs: ") +
                       QString(m_meta_graph->getDisplayedShapeGraph().getName().c_str());
        if (type == ShapeMap::AXIALMAP) {
            // note, ShapeMap::ALLLINEMAP is deliberately excluded, as making a
            // segment map from it would overproduce segments:
            dlg.m_mapout = MAKELAYER_DRAWING | MAKELAYER_DATA | MAKELAYER_SEGMENT;
        } else {
            // convex, segment, pesh and all line maps cannot be converted to segment
            // maps:
            dlg.m_mapout = MAKELAYER_DRAWING | MAKELAYER_DATA;
        }
    } else if (m_meta_graph->viewingProcessedShapes()) {
        //
        dlg.m_origin =
            QString("Data Maps: ") + QString(m_meta_graph->getDisplayedDataMap().getName().c_str());
        // possible to go to anything apart from a data map
        dlg.m_mapout = MAKELAYER_DRAWING | MAKELAYER_AXIAL | MAKELAYER_CONVEX | MAKELAYER_SEGMENT;
    }

    if (dlg.m_mapout != 0 && QDialog::Accepted == dlg.exec()) {
        m_communicator = new CMSCommunicator();
        m_communicator->SetString(dlg.m_layer_name);
        m_communicator->SetOption(dlg.m_keeporiginal ? 1 : 0,
                                  0); // <- option 0 used for retain original flag
        m_communicator->SetOption(dlg.m_push_values ? 1 : 0,
                                  1); // <- option 1 used for push values flag
        //
        if (dlg.m_mapout == MAKELAYER_DRAWING) {
            // this one new: data or graph (any sort) -> drawing
            m_communicator->SetOption(
                dlg.m_mapin == MAKELAYER_DATA ? 0 : 1,
                1); // <- option 1 in this case signifies base as graph or data
            // (option 1 overidden, as data cannot be pushed to a drawing layer)
            CreateWaitDialog(tr("Constructing segment map..."));
            m_communicator->SetFunction(CMSCommunicator::MAKEDRAWING);
        }
        if (dlg.m_mapout == MAKELAYER_DATA) {
            // this one new: graph (any sort) -> data
            CreateWaitDialog(tr("Constructing segment map..."));
            m_communicator->SetFunction(CMSCommunicator::MAKEGATESMAP);
        } else if (dlg.m_mapout == MAKELAYER_CONVEX) {
            // this one new: data -> convex map
            CreateWaitDialog(tr("Constructing convex map..."));
            m_communicator->SetFunction(CMSCommunicator::MAKECONVEXMAP);
        } else if (dlg.m_mapout == MAKELAYER_AXIAL) {
            // this one originally data -> axial map
            CreateWaitDialog(tr("Constructing axial map..."));
            m_communicator->SetFunction(CMSCommunicator::MAKEUSERMAPSHAPE);
        } else if (dlg.m_mapout == MAKELAYER_SEGMENT) {
            if (dlg.m_mapin == MAKELAYER_AXIAL) {
                // this one originally axial -> segment map
                // use option 2 to specify percentage removal
                m_communicator->SetOption(dlg.m_remove_stubs ? dlg.m_percentage : 0, 2);
                CreateWaitDialog(tr("Constructing segment map..."));
                m_communicator->SetFunction(CMSCommunicator::MAKESEGMENTMAP);
            } else {
                // this one originally data -> segment map
                CreateWaitDialog(tr("Constructing segment map..."));
                m_communicator->SetFunction(CMSCommunicator::MAKEUSERSEGMAPSHAPE);
            }
        }
        m_thread.render(this);
    }
}

void QGraphDoc::OnLayerConvertDrawing() {
    if (m_communicator) {
        QMessageBox::warning(this, tr("Notice"), tr("Please wait, another task is running"),
                             QMessageBox::Ok, QMessageBox::Ok);
        return;
    }

    CMakeLayerDlg dlg;

    dlg.m_mapin = MAKELAYER_DRAWING; // mapin is a drawing map
    dlg.m_mapout = MAKELAYER_DATA | MAKELAYER_AXIAL | MAKELAYER_CONVEX | MAKELAYER_SEGMENT;
    dlg.m_origin = QString(tr("Drawing Layers: All Displayed"));

    if (QDialog::Accepted == dlg.exec()) {
        m_communicator = new CMSCommunicator();
        m_communicator->SetString(dlg.m_layer_name);
        m_communicator->SetOption(dlg.m_keeporiginal ? 1 : 0,
                                  0);     // <- option 0 used for retain original flag
        m_communicator->SetOption(-1, 1); // this is used to distinguish between 0
                                          // or 1 used when converting data layer
        //
        if (dlg.m_mapout == MAKELAYER_DATA) {
            // this one originally drawing -> data map
            CreateWaitDialog(tr("Constructing data map..."));
            m_communicator->SetFunction(CMSCommunicator::MAKEGATESMAP);
        } else if (dlg.m_mapout == MAKELAYER_AXIAL) {
            // this one originally drawing -> axial map
            CreateWaitDialog(tr("Constructing axial map..."));
            m_communicator->SetFunction(CMSCommunicator::MAKEUSERMAP);
        } else if (dlg.m_mapout == MAKELAYER_CONVEX) {
            // this one new: drawing -> convex map
            CreateWaitDialog(tr("Constructing convex map..."));
            m_communicator->SetFunction(CMSCommunicator::MAKECONVEXMAP);
        } else if (dlg.m_mapout == MAKELAYER_SEGMENT) {
            // this one originally drawing -> seg map
            CreateWaitDialog(tr("Constructing segment map..."));
            m_communicator->SetFunction(CMSCommunicator::MAKEUSERSEGMAP);
        }
        m_thread.render(this);
    }
}

// arbitrary isovist
void QGraphDoc::OnMakeIsovist(const Point2f &seed, double angle) {
    int state = m_meta_graph->getState();
    if (m_communicator) {
        QMessageBox::warning(this, tr("Notice"), tr("Please wait, another task is running"),
                             QMessageBox::Ok, QMessageBox::Ok);
        return;
    }
    if (~state & MetaGraphDM::DX_LINEDATA) {
        QMessageBox::warning(this, tr("Notice"),
                             tr("Sorry, line drawing data must be loaded before an "
                                "isovist can be constructed"),
                             QMessageBox::Ok, QMessageBox::Ok);
        return;
    }

    m_communicator = new CMSCommunicator();
    m_communicator->SetSeedPoint(seed);
    m_communicator->SetSeedAngle(angle);

    if (angle >= 0) {
        CIsovistPathDlg dlg;
        if (dlg.exec() == QDialog::Accepted) {
            m_communicator->SetSeedFoV(dlg.fov_angle);
        }
    }

    CreateWaitDialog(tr("Constructing BSP tree to calculate isovist..."));
    m_communicator->SetFunction(CMSCommunicator::MAKEISOVIST);

    m_thread.render(this);
}

void QGraphDoc::OnToolsIsovistpath() {
    int state = m_meta_graph->getState();
    if (state & MetaGraphDM::DX_LINEDATA) {
        int view = m_meta_graph->getViewClass();
        if ((view & (MetaGraphDM::DX_VIEWDATA | MetaGraphDM::DX_VIEWAXIAL)) != 0 &&
            m_meta_graph->isSelected()) {
            CIsovistPathDlg dlg;
            if (dlg.exec() == QDialog::Accepted) {
                double angle = dlg.fov_angle;
                m_communicator = new CMSCommunicator();
                m_communicator->SetSeedAngle(angle);
                CreateWaitDialog(tr("Constructing BSP tree to calculate isovists..."));
                m_communicator->SetFunction(CMSCommunicator::MAKEISOVISTPATH);
                m_thread.render(this);
            }
        } else {
            // an explanation of what you need to do as it isn't obvious!
            QMessageBox::warning(this, tr("Notice"),
                                 tr("To get a path you will need some lines for a path.\nTo do so, "
                                    "have an active map which is either a data map or line map of "
                                    "some sort.\nNext, select lines or polylines that you wish to "
                                    "turn into isovist paths.\nThere is no need for you to have a "
                                    "grid set.\n"),
                                 QMessageBox::Ok, QMessageBox::Ok);
        }
    }
}

// all line map
void QGraphDoc::OnToolsAxialMap(const Point2f &seed) {
    int state = m_meta_graph->getState();
    if (m_communicator) {
        QMessageBox::warning(this, tr("Notice"), tr("Please wait, another task is running"),
                             QMessageBox::Ok, QMessageBox::Ok);
        return;
    }
    if (~state & MetaGraphDM::DX_LINEDATA) {
        QMessageBox::warning(this, tr("Notice"),
                             tr("Sorry, line drawing data must be loaded before an "
                                "axial map can be constructed"),
                             QMessageBox::Ok, QMessageBox::Ok);
        return;
    }

    if (m_meta_graph->hasAllLineMap()) {
        int ret = QMessageBox::warning(
            this, tr("My Application"),
            tr("This will overwrite your existing all-line map, do you want to "
               "continue?"),
            QMessageBox::Yes | QMessageBox::No | QMessageBox::Yes, QMessageBox::No);
        if (ret != QMessageBox::Yes)
            return;
    }

    // This is easy too... too easy... hmm... crossed-fingers, here goes:
    m_communicator = new CMSCommunicator();
    m_communicator->SetSeedPoint(seed);
    CreateWaitDialog(tr("Constructing all line axial map..."));
    m_communicator->SetFunction(CMSCommunicator::MAKEALLLINEMAP);
    m_thread.render(this);
}

// fewest line map
void QGraphDoc::OnToolsMakeFewestLineMap() {
    int state = m_meta_graph->getState();
    if (m_communicator) {
        QMessageBox::warning(this, tr("Warning"), tr("Please wait, another task is running"),
                             QMessageBox::Ok, QMessageBox::Ok);
        return;
    }
    if (~state & MetaGraphDM::DX_SHAPEGRAPHS) {
        QMessageBox::warning(this, tr("Warning"),
                             tr("Sorry, all line map must exist in order to "
                                "construct fewest line map"),
                             QMessageBox::Ok, QMessageBox::Ok);
        return;
    } else if (!m_meta_graph->hasAllLineMap()) {
        QMessageBox::warning(this, tr("Warning"),
                             tr("Sorry, all line map must exist in order to "
                                "construct fewest line map"),
                             QMessageBox::Ok, QMessageBox::Ok);
        return;
    }

    int replace = 0;

    // check for existing axial maps and warn user if necessary:
    if (m_meta_graph->hasFewestLineMaps()) {
        CPromptReplace dlg;
        dlg.m_message = tr("There is already a fewest line axial map, would you "
                           "like to add to it or replace it?");
        int result = dlg.exec();
        if (result == QDialog::Rejected) {
            return;
        } else if (result == 2) {
            replace = 1;
        }
    }

    // This is easy too... too easy... hmm... crossed-fingers, here goes:
    m_communicator = new CMSCommunicator();
    CreateWaitDialog(tr("Constructing fewest line axial map..."));
    m_communicator->SetFunction(CMSCommunicator::MAKEFEWESTLINEMAP);
    m_communicator->SetOption(replace);
    m_thread.render(this);
}

void QGraphDoc::OnToolsRunAxa() {
    int state = m_meta_graph->getState();
    if (m_communicator) {
        QMessageBox::warning(this, tr("Warning"), tr("Please wait, another task is running"),
                             QMessageBox::Ok, QMessageBox::Ok);
        return;
    }

    CAxialAnalysisOptionsDlg dlg(m_meta_graph);

    if (QDialog::Accepted == dlg.exec()) {
        m_communicator = new CMSCommunicator();
        CreateWaitDialog(tr("Performing axial line analysis..."));
        m_communicator->SetFunction(CMSCommunicator::AXIALANALYSIS);
        m_thread.render(this);
    }
}

void QGraphDoc::OnToolsRunSeg() {
    int state = m_meta_graph->getState();
    if (m_communicator) {
        QMessageBox::warning(this, tr("Warning"), tr("Please wait, another task is running"),
                             QMessageBox::Ok, QMessageBox::Ok);
        return;
    }

    CSegmentAnalysisDlg dlg(m_meta_graph);

    if (QDialog::Accepted == dlg.exec()) {
        m_communicator = new CMSCommunicator();
        CreateWaitDialog(tr("Performing segment line analysis..."));
        m_communicator->SetFunction(dlg.m_analysis_type == 1
                                        ? CMSCommunicator::SEGMENTANALYSISANGULAR
                                        : CMSCommunicator::SEGMENTANALYSISTULIP);
        m_thread.render(this);
    }
}

void QGraphDoc::OnToolsTopomet() {
    int state = m_meta_graph->getState();
    if (m_communicator) {
        QMessageBox::warning(this, tr("Warning"), tr("Please wait, another task is running"),
                             QMessageBox::Ok, QMessageBox::Ok);
        return;
    }

    CTopoMetDlg dlg;

    if (QDialog::Accepted == dlg.exec()) {
        m_communicator = new CMSCommunicator();
        ((MainWindow *)m_mainFrame)->m_options.outputType = dlg.m_analysisType;
        ((MainWindow *)m_mainFrame)->m_options.radius = dlg.m_dradius;
        ((MainWindow *)m_mainFrame)->m_options.selOnly = dlg.m_selected_only;
        if (dlg.isAnalysisTopological()) {
            CreateWaitDialog(tr("Performing topological analysis..."));
        } else {
            CreateWaitDialog(tr("Performing metric analysis..."));
        }
        m_communicator->SetFunction(CMSCommunicator::TOPOMETANALYSIS);
        m_thread.render(this);
    }
}

///////////////////////////////////////////////////////////////////////////////////////////

// New agent functionality:

void QGraphDoc::OnToolsAgentRun() {
    if (m_communicator) {
        QMessageBox::warning(this, tr("Warning"), tr("Please wait, another task is running"),
                             QMessageBox::Ok, QMessageBox::Ok);
        return;
    }

    CAgentAnalysisDlg dlg;
    dlg.m_timesteps = 5000;
    dlg.m_release_rate = 0.1;
    dlg.m_release_location = 0;
    dlg.m_frames = 1000;
    dlg.m_fov = 15;
    dlg.m_steps = 3;
    dlg.m_record_trails = false;
    dlg.m_trail_count = 50;
    dlg.m_names.push_back("<None>");
    for (size_t i = 0; i < m_meta_graph->getDataMaps().size(); i++) {
        dlg.m_names.push_back(m_meta_graph->getDataMaps()[i].getName());
    }
    dlg.m_gatelayer = -1;

    if (QDialog::Accepted != dlg.exec()) {
        return;
    }

    int agentAlgorithm = AgentProgram::SEL_STANDARD;
    if (dlg.m_occlusion == 1) {
        agentAlgorithm = AgentProgram::SEL_LOS;
    } else if (dlg.m_occlusion == 2) {
        agentAlgorithm = AgentProgram::SEL_LOS_OCC;
    } else {
        // (dlg.m_occlusion - 2) should be from 1...8
        agentAlgorithm = AgentProgram::SEL_OCCLUSION + (dlg.m_occlusion - 2);
    }

    std::vector<Point2f> releasePoints;
    std::optional<size_t> randomReleaseLocationSeed = 0;

    std::optional<AgentAnalysis::TrailRecordOptions> recordTrails =
        dlg.m_record_trails
            ? std::make_optional(AgentAnalysis::TrailRecordOptions{
                  dlg.m_trail_count == 0
                      ? std::nullopt
                      : std::make_optional(static_cast<size_t>(dlg.m_trail_count)),
                  std::ref(
                      m_meta_graph->createNewShapeMap(sala::ImportType::DATAMAP, "Agent trails")
                          .getInternalMap())})
            : std::nullopt;

    if (dlg.m_release_location == 1) {
        randomReleaseLocationSeed = std::nullopt;
        std::set<int> selected = m_meta_graph->getDisplayedPointMap().getSelSet();
        for (auto sel : selected) {
            releasePoints.push_back(
                m_meta_graph->getDisplayedPointMap().getPoint(sel).getLocation());
        }
    }

    // then go:
    std::unique_ptr<IAnalysis> analysis(new AgentAnalysis(
        m_meta_graph->getDisplayedPointMap().getInternalMap(), dlg.m_timesteps, dlg.m_release_rate,
        dlg.m_frames, dlg.m_fov, dlg.m_steps, agentAlgorithm, randomReleaseLocationSeed,
        releasePoints,
        dlg.m_gatelayer == -1 ? std::nullopt
                              : std::make_optional(std::ref(
                                    m_meta_graph->getDataMaps()[dlg.m_gatelayer].getInternalMap())),
        recordTrails));

    m_communicator = new CMSCommunicator();

    m_communicator->setAnalysis(std::move(analysis));
    CreateWaitDialog(tr("Performing agent analysis..."));
    m_communicator->SetFunction(CMSCommunicator::AGENTANALYSIS);

    m_thread.render(this);
}

/////////////////////////////////////////////////////////////////////////////

void QGraphDoc::OnEditUndo() {
    if (!m_meta_graph->canUndo()) {
        QMessageBox::warning(this, tr("Warning"), tr("Sorry, no undo available for this map"),
                             QMessageBox::Ok, QMessageBox::Ok);
        return;
    }

    m_meta_graph->undo();
    modifiedFlag = true;
    SetRedrawFlag(VIEW_ALL, REDRAW_GRAPH, NEW_DATA);
}

void QGraphDoc::OnEditClear() {
    int state = m_meta_graph->getState();

    int editable = m_meta_graph->isEditable();
    if (editable == MetaGraphDM::DX_NOT_EDITABLE) {
        QMessageBox::warning(this, tr("Warning"),
                             tr("Cannot delete: the geometry forming this graph cannot be edited."),
                             QMessageBox::Ok, QMessageBox::Ok);
        return;
    } else if (editable == MetaGraphDM::DX_EDITABLE_OFF) {
        QMessageBox::warning(this, tr("Warning"),
                             tr("Cannot delete: this graph is currently uneditable."),
                             QMessageBox::Ok, QMessageBox::Ok);
        return;
    }

    bool modified = false;
    if (m_meta_graph->viewingUnprocessedPoints()) {
        modified = m_meta_graph->clearPoints();
    } else if (m_meta_graph->viewingProcessedLines()) {
        modified = m_meta_graph->getDisplayedShapeGraph().removeSelected();
    } else if (m_meta_graph->viewingProcessedShapes()) {
        modified = m_meta_graph->getDisplayedDataMap().removeSelected();
    }

    if (modified) {
        modifiedFlag = true;
    }

    SetRedrawFlag(VIEW_ALL, REDRAW_GRAPH, NEW_DATA);
}

/////////////////////////////////////////////////////////////////////////////

void QGraphDoc::OnToolsMakeGraph() {
    int state = m_meta_graph->getState();
    if (m_communicator) {
        QMessageBox::warning(this, tr("Warning"), tr("Please wait, another task is running"),
                             QMessageBox::Ok, QMessageBox::Ok);
        return;
    }
    if (!m_meta_graph->viewingUnprocessedPoints()) {
        QMessageBox::warning(this, tr("Warning"),
                             tr("Sorry, you need an unprocessed set of points to "
                                "make the visibility graph"),
                             QMessageBox::Ok, QMessageBox::Ok);
        return;
    }

    CMakeOptionsDlg dlg;
    dlg.m_boundarygraph = false;
    if (dlg.exec() != QDialog::Accepted) {
        return;
    }
    if (dlg.m_boundarygraph) {
        m_make_algorithm = 1;
    } else {
        m_make_algorithm = 0;
    }
    if (dlg.m_restrict_visibility) {
        m_make_maxdist = dlg.m_maxdist;
    } else {
        m_make_maxdist = -1.0;
    }

    m_communicator = new CMSCommunicator();
    CreateWaitDialog(tr("Constructing graph..."));
    m_communicator->SetFunction(CMSCommunicator::MAKEGRAPH);

    m_thread.render(this);
}

void QGraphDoc::OnToolsUnmakeGraph() {
    int state = m_meta_graph->getState();
    if (m_communicator) {
        QMessageBox::warning(this, tr("Notice"), tr("Please wait, another task is running"),
                             QMessageBox::Ok, QMessageBox::Ok);
        return;
    }
    if (~state & MetaGraphDM::DX_POINTMAPS) {
        QMessageBox::warning(this, tr("Notice"), tr("Please make grid before filling"),
                             QMessageBox::Ok, QMessageBox::Ok);
        return;
    }
    if (m_meta_graph->viewingProcessed()) {
        if (QMessageBox::Yes != QMessageBox::question(this, tr("Notice"),
                                                      tr("This will clear existing data and "
                                                         "attributes. Do you want to continue?"),
                                                      QMessageBox::Yes | QMessageBox::No,
                                                      QMessageBox::No))
            return;
    }
    bool removeLinks = false;
    if (m_meta_graph->getDisplayedPointMap().getInternalMap().getMergedPixelPairs().size() > 0) {
        removeLinks =
            QMessageBox::Yes ==
            QMessageBox::question(this, tr("Notice"), tr("Would you also like to clear the links?"),
                                  QMessageBox::Yes | QMessageBox::No, QMessageBox::No);
    }
    bool ok = m_meta_graph->unmakeGraph(removeLinks);
    if (ok) {
        SetUpdateFlag(QGraphDoc::NEW_DATA);
    }
    SetRedrawFlag(QGraphDoc::VIEW_ALL, QGraphDoc::REDRAW_GRAPH, QGraphDoc::NEW_DATA);
}

/////////////////////////////////////////////////////////////////////////////

void QGraphDoc::OnToolsRun() {
    if (m_communicator) {
        QMessageBox::warning(this, tr("Warning"), tr("Please wait, another task is running"),
                             QMessageBox::Ok, QMessageBox::Ok);
        return;
    }

    // This is easy!
    COptionsDlg dlg;

    dlg.m_layer_names.push_back("<None>");
    for (auto &dataMap : m_meta_graph->getDataMaps()) {
        dlg.m_layer_names.push_back(dataMap.getName());
    }

    if (QDialog::Accepted != dlg.exec()) {
        return;
    }

    if (dlg.m_output_type == AnalysisType::ISOVIST && !m_meta_graph->hasVisibleDrawingShapes()) {
        // isovists won't work without some lines visible
        QMessageBox::warning(this, tr("Notice"),
                             tr("No visible drawing shapes to create isovists on"), QMessageBox::Ok,
                             QMessageBox::Ok);
        return;
    }

    // This is easy too... too easy... hmm... crossed-fingers, here goes:
    m_communicator = new CMSCommunicator();
    CreateWaitDialog(tr("Analysing graph..."));
    m_communicator->SetFunction(CMSCommunicator::ANALYSEGRAPH);

    m_thread.render(this);
}

void QGraphDoc::OnToolsPD() {
    if (m_communicator) {
        QMessageBox::warning(this, tr("Warning"), tr("Please wait, another process is running"),
                             QMessageBox::Ok, QMessageBox::Ok);
        return;
    }

    if (m_meta_graph->viewingProcessed()) {
        if (m_meta_graph->isSelected()) {

            // This is easy too... too easy... hmm... crossed-fingers, here goes:
            m_communicator = new CMSCommunicator();
            CreateWaitDialog(tr("Calculating step depth..."));
            m_communicator->SetFunction(CMSCommunicator::POINTDEPTH);

            m_thread.render(this);
        }
    }
}

void QGraphDoc::OnToolsMPD() {
    if (m_communicator) {
        QMessageBox::warning(this, tr("Warning"), tr("Please wait, another process is running"),
                             QMessageBox::Ok, QMessageBox::Ok);
        return;
    }

    if (m_meta_graph->viewingProcessedPoints() ||
        (m_meta_graph->viewingProcessedLines() &&
         m_meta_graph->getDisplayedShapeGraph().getInternalMap().isSegmentMap())) {
        if (m_meta_graph->isSelected()) {

            // This is easy too... too easy... hmm... crossed-fingers, here goes:
            m_communicator = new CMSCommunicator();
            CreateWaitDialog(tr("Calculating metric depth..."));
            m_communicator->SetFunction(CMSCommunicator::METRICPOINTDEPTH);

            m_thread.render(this);
        }
    }
}

void QGraphDoc::OnToolsAPD() {
    if (m_communicator) {
        QMessageBox::warning(this, tr("Warning"), tr("Please wait, another process is running"),
                             QMessageBox::Ok, QMessageBox::Ok);
        return;
    }

    if (m_meta_graph->viewingProcessedPoints()) {
        if (m_meta_graph->isSelected()) {

            // This is easy too... too easy... hmm... crossed-fingers, here goes:
            m_communicator = new CMSCommunicator();
            CreateWaitDialog(tr("Calculating angular depth..."));
            m_communicator->SetFunction(CMSCommunicator::ANGULARPOINTDEPTH);

            m_thread.render(this);
        }
    }
}

void QGraphDoc::OnToolsTPD() {
    if (m_communicator) {
        QMessageBox::warning(this, tr("Warning"), tr("Please wait, another process is running"),
                             QMessageBox::Ok, QMessageBox::Ok);
        return;
    }

    if (m_meta_graph->viewingProcessedLines() &&
        m_meta_graph->getDisplayedShapeGraph().getInternalMap().isSegmentMap()) {
        if (m_meta_graph->isSelected()) {

            // This is easy too... too easy... hmm... crossed-fingers, here goes:
            m_communicator = new CMSCommunicator();
            CreateWaitDialog(tr("Calculating topological depth..."));
            m_communicator->SetFunction(CMSCommunicator::TOPOLOGICALPOINTDEPTH);

            m_thread.render(this);
        }
    }
}

/////////////////////////////////////////////////////////////////////////////
static int sequenceNumber = 1;
bool QGraphDoc::OnNewDocument() {
    m_base_title = tr("Untitled%1").arg(sequenceNumber++);

    SetRemenuFlag(QGraphDoc::VIEW_ALL, true);
    return TRUE;
}

int QGraphDoc::OnOpenDocument(char *lpszPathName) {
    if (m_communicator) {
        QMessageBox::warning(this, tr("Warning"), tr("Please wait, another process is running"),
                             QMessageBox::Ok, QMessageBox::Ok);
        return FALSE;
    }

    m_opened_name = QString(lpszPathName);

    auto ok = m_meta_graph->readFromFile(lpszPathName);
    QFilePath path(m_opened_name);

    SetUpdateFlag(QGraphDoc::NEW_FILE, false);
    int ret = FALSE;

    switch (ok) {
    case MetaGraphReadWrite::ReadWriteStatus::OK:
        m_base_title = path.m_name;
        ret = TRUE;
        break;
    case MetaGraphReadWrite::ReadWriteStatus::WARN_BUGGY_VERSION:
        QMessageBox::warning(this, tr("Warning"),
                             tr("this graph was made with a version of the sala "
                                "library that contained slight errors"),
                             QMessageBox::Ok, QMessageBox::Ok);
        ret = TRUE;
        break;
    case MetaGraphReadWrite::ReadWriteStatus::WARN_CONVERTED:
        QMessageBox::warning(
            this, tr("Warning"),
            tr("Warning: this graph was made with an older version of the sala library.\n"
               "Some aspects of the graph may not have been translated to the new "
               "version."),
            QMessageBox::Ok, QMessageBox::Ok);
        ret = TRUE;
        break;
    case MetaGraphReadWrite::ReadWriteStatus::NOT_A_GRAPH:
        QMessageBox::warning(this, tr("Warning"),
                             tr("Unable to open graph: not recognised as a graph file."),
                             QMessageBox::Ok, QMessageBox::Ok);
        break;
    case MetaGraphReadWrite::ReadWriteStatus::DAMAGED_FILE:
        QMessageBox::warning(this, tr("Warning"),
                             tr("Unable to open graph: the graph file is damaged."),
                             QMessageBox::Ok, QMessageBox::Ok);
        break;
    case MetaGraphReadWrite::ReadWriteStatus::DISK_ERROR:
        QMessageBox::warning(this, tr("Warning"),
                             tr("Unable to open graph: an error occurred while "
                                "trying to read from the disk."),
                             QMessageBox::Ok, QMessageBox::Ok);
        break;
    case MetaGraphReadWrite::ReadWriteStatus::NEWER_VERSION:
        QMessageBox::warning(this, tr("Warning"),
                             tr("Unable to open graph: this graph has been written "
                                "by a newer version of %1.")
                                 .arg(APP_NAME.c_str()),
                             QMessageBox::Ok, QMessageBox::Ok);
        break;
    case MetaGraphReadWrite::ReadWriteStatus::DEPRECATED_VERSION:
        QMessageBox::warning(this, tr("Warning"),
                             tr("Unable to open graph: this is a graph file format "
                                "not supported by this version of %1.")
                                 .arg(APP_NAME.c_str()),
                             QMessageBox::Ok, QMessageBox::Ok);
        break;
    default: {
        std::string err = MetaGraphReadWrite::getReadMessage(ok);
        QMessageBox::warning(this, tr("Warning"), tr("Unable to open graph: error number "),
                             QMessageBox::Ok, QMessageBox::Ok);
    } break;
    }

    return ret;
}

bool QGraphDoc::OnFileSave() {
    QString newName = m_opened_name;
    if (newName.isEmpty()) {
        newName = m_base_title + tr(".graph");
        QFileDialog::Options options;
        QString outfile = QFileDialog::getSaveFileName(
            0, tr("Save As"), newName, tr("Graph file (*.graph)\nAll files (*.*)"), 0, options);
        if (outfile.isEmpty())
            return false;

        m_opened_name = outfile;

        FILE *fp = fopen(m_opened_name.toLatin1(), "wb");
        fclose(fp);

        OnSaveDocument(outfile);

        QFilePath path(m_opened_name);
        m_base_title = path.m_name;
        return true;
    }

    OnSaveDocument(newName);
    return true;
}

bool QGraphDoc::OnFileSaveAs() {
    // This is based on Microsoft's "DoSave" function, but
    // it allows two options for saving: one as the current
    // graph format, and one as the original

    QString newName = m_opened_name;
    if (newName.isEmpty()) {
        newName = m_base_title + tr(".graph");
    }

    QFileDialog::Options options;
    QString outfile = QFileDialog::getSaveFileName(
        0, tr("Save As"), newName,
        tr("Graph file (*.graph)\nsala %1 graph (*.graph)\nAll files (*.*)").arg(METAGRAPH_VERSION),
        0, options);

    if (outfile.isEmpty())
        return false;

    FILE *fp = fopen(outfile.toLatin1(), "wb");
    fclose(fp);

    OnSaveDocument(outfile.toLatin1());

    // reset the title and change the document name
    m_opened_name = outfile;
    QFilePath path(m_opened_name);
    m_base_title = path.m_name;
    return true;
}

int QGraphDoc::OnSaveDocument(QString lpszPathName) {
    // default: save in current version format
    int version = m_meta_graph->getVersion();
    // version == -1 is unsaved, which is fine to save in current version
    if (version != -1 && version != METAGRAPH_VERSION) {
        if (QMessageBox::Yes ==
            QMessageBox::question(this, tr("Notice"),
                                  tr("This will overwrite the file with the latest graph format, "
                                     "which may not be readable by previous versions of "
                                     "%1.\nDo you want to overwrite it?")
                                      .arg(APP_NAME.c_str()),
                                  QMessageBox::Yes | QMessageBox::No, QMessageBox::No))
            return OnSaveDocument(lpszPathName, METAGRAPH_VERSION);
        else
            return TRUE;
    }
    return OnSaveDocument(lpszPathName, METAGRAPH_VERSION);
}

int QGraphDoc::OnSaveDocument(QString lpszPathName, int version) {
    if (m_communicator) {
        QMessageBox::warning(this, tr("Warning"), tr("Please wait, another process is running"),
                             QMessageBox::Ok, QMessageBox::Ok);
        return FALSE;
    }

    if (m_meta_graph->getState() & MetaGraphDM::DX_BUGGY) {
        if (QMessageBox::No ==
            QMessageBox::question(
                this, tr("Notice"),
                tr("This graph file was created by a version of the sala library with "
                   "slight errors.  Are you sure you want to save it?"),
                QMessageBox::Yes | QMessageBox::No, QMessageBox::No))
            return FALSE;
    }

    modifiedFlag = true;

    auto ok = m_meta_graph->write(lpszPathName.toStdString(), version);
    if (ok == MetaGraphReadWrite::ReadWriteStatus::OK) {
        modifiedFlag = false;
        return TRUE;
    } else if (ok == MetaGraphReadWrite::ReadWriteStatus::DISK_ERROR) {
        QMessageBox::warning(this, tr("Warning"),
                             tr("Unable to save graph: is there enough disk space?"),
                             QMessageBox::Ok, QMessageBox::Ok);
    }

    return FALSE;
}

bool QGraphDoc::OnCloseDocument(int index) {
    if (m_communicator) {
        QMessageBox::warning(
            this, tr("Warning"),
            tr("%1 is processing, please kill the process to continue").arg(APP_NAME.c_str()),
            QMessageBox::Ok, QMessageBox::Ok);
        return false;
    }
    int i;
    for (i = 1; i < VIEW_TYPES; i++)
        if (m_view[i])
            break;

    if (modifiedFlag) {
        int result = QMessageBox::question(
            this, tr("Notice"), tr("Do you want to save the changes?"),
            QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel, QMessageBox::Cancel);
        if (QMessageBox::Yes == result) {
            OnFileSave();
            if (i == VIEW_TYPES) {
                modifiedFlag = false;
                QApplication::postEvent(
                    (QObject *)m_mainFrame,
                    new QmyEvent((enum QEvent::Type)FOCUSGRAPH, (void *)this, CONTROLS_DESTROYALL));
            }
            return true;
        } else if (QMessageBox::No == result) {
            if (i == VIEW_TYPES) {
                modifiedFlag = false;
                QApplication::postEvent(
                    (QObject *)m_mainFrame,
                    new QmyEvent((enum QEvent::Type)FOCUSGRAPH, (void *)this, CONTROLS_DESTROYALL));
            }
            return true;
        } else
            return false;
    }
    if (i == VIEW_TYPES) {
        QApplication::postEvent(
            (QObject *)m_mainFrame,
            new QmyEvent((enum QEvent::Type)FOCUSGRAPH, (void *)this, CONTROLS_DESTROYALL));
    }
    return true;
}

void QGraphDoc::OnPushToLayer() {
    if (m_meta_graph->viewingProcessed()) {
        int toplayerclass = (m_meta_graph->getViewClass() & MetaGraphDM::DX_VIEWFRONT);
        std::string origin_layer;
        std::string origin_attribute;
        std::map<std::pair<int, int>, std::string> names;
        // I'm just going to allow push from any layer to any other layer
        // (apart from VGA graphs, which cannot map onto themselves
        if (toplayerclass == MetaGraphDM::DX_VIEWVGA) {
            // bit clunky just to get two names out...
            auto &map = m_meta_graph->getDisplayedPointMap();
            origin_layer = std::string("Visibility Graphs: ") + map.getName();
            origin_attribute = map.getAttributeTable().getColumnName(map.getDisplayedAttribute());
        } else if (toplayerclass == MetaGraphDM::DX_VIEWAXIAL) {
            // bit clunky just to get two names out...
            auto &map = m_meta_graph->getDisplayedShapeGraph();
            origin_layer = std::string("Shape Graphs: ") + map.getName();
            origin_attribute = map.getAttributeTable().getColumnName(map.getDisplayedAttribute());
        } else if (toplayerclass == MetaGraphDM::DX_VIEWDATA) {
            // bit clunky just to get two names out...
            auto &map = m_meta_graph->getDisplayedDataMap();
            origin_layer = std::string("Data Maps: ") + map.getName();
            origin_attribute = map.getAttributeTable().getColumnName(map.getDisplayedAttribute());
        } else {
            // eek!
            QMessageBox::warning(this, tr("Warning"),
                                 tr("No valid map displayed from which to push data"),
                                 QMessageBox::Ok, QMessageBox::Ok);
            return;
        }
        // layers to push to:
        size_t i;
        std::vector<ShapeMapDM> &datamaps = m_meta_graph->getDataMaps();
        for (i = 0; i < datamaps.size(); i++) {
            if (toplayerclass != MetaGraphDM::DX_VIEWDATA ||
                i != m_meta_graph->getDisplayedDataMapRef()) {
                names.insert(std::make_pair(std::pair<int, int>(MetaGraphDM::DX_VIEWDATA, int(i)),
                                            std::string("Data Maps: ") + datamaps[i].getName()));
            }
        }
        auto &shapegraphs = m_meta_graph->getShapeGraphs();
        for (i = 0; i < shapegraphs.size(); i++) {
            if (toplayerclass != MetaGraphDM::DX_VIEWAXIAL ||
                i != m_meta_graph->getDisplayedShapeGraphRef()) {
                names.insert(
                    std::make_pair(std::pair<int, int>(MetaGraphDM::DX_VIEWAXIAL, int(i)),
                                   std::string("Shape Graphs: ") + shapegraphs[i].getName()));
            }
        }
        for (i = 0; i < m_meta_graph->getPointMaps().size(); i++) {
            // note 1: no VGA graph can push to another VGA graph (point onto point
            // transforms)
            if (toplayerclass != MetaGraphDM::DX_VIEWVGA) {
                names.insert(std::make_pair(std::pair<int, int>(MetaGraphDM::DX_VIEWVGA, int(i)),
                                            std::string("Visibility Graphs: ") +
                                                m_meta_graph->getPointMaps()[i].getName()));
            }
        }
        CPushDialog dlg(names);
        dlg.m_origin_layer = QString(origin_layer.c_str());
        dlg.m_origin_attribute = QString(origin_attribute.c_str());
        if (QDialog::Accepted == dlg.exec()) {
            m_communicator = new CMSCommunicator; // dummy value to prevent draw while
                                                  // this operation is in progress
            // now have to separate vga and axial layers again:
            int sel = dlg.m_layer_selection;
            std::pair<int, int> dest = genlib::getMapAtIndex(names, sel)->first;
            //         CWaitCursor c;
            m_meta_graph->pushValuesToLayer(dest.first, dest.second, dlg.m_function,
                                            dlg.m_count_intersections);
            delete m_communicator;
            m_communicator = NULL;
            SetUpdateFlag(NEW_TABLE);
        }
    }
}

void QGraphDoc::OnAddColumn() {
    CRenameObjectDlg dlg(tr("Column")); // using the constructor without a column name sets rename
                                        // column dialog to insert column name mode
    bool success = false;
    while (dlg.exec() == QDialog::Accepted && !success) {
        if (dlg.m_object_name.isEmpty()) {
            QMessageBox::warning(this, tr("Warning"), tr("Column name cannot be empty"),
                                 QMessageBox::Ok, QMessageBox::Ok);
        } else {
            AttributeTable &tab = m_meta_graph->getAttributeTable();
            bool found = false;
            for (int i = 0; i < tab.getNumColumns(); i++) {
                if (tab.getColumnName(i) == dlg.m_object_name.toStdString()) {
                    QMessageBox::warning(this, tr("Notice"),
                                         tr("Sorry, another column already has this "
                                            "name, please choose a unique column name"),
                                         QMessageBox::Ok, QMessageBox::Ok);
                    found = true;
                    break;
                }
            }
            if (!found) {
                success = true;
                break;
            }
        }
    }
    if (success) {
        auto col = m_meta_graph->addAttribute(dlg.m_object_name.toStdString());
        if (col.has_value())
            m_meta_graph->setDisplayedAttribute(col.value());
        else
            m_meta_graph->setDisplayedAttribute(-1);
        SetUpdateFlag(QGraphDoc::NEW_DATA);
        // Tell the views to update their menus
        SetRedrawFlag(VIEW_ALL, QGraphDoc::REDRAW_GRAPH, QGraphDoc::NEW_COLUMN);
    }
}

void QGraphDoc::OnRenameColumn() {
    AttributeTable *tab = &(m_meta_graph->getAttributeTable());
    int col = m_meta_graph->getDisplayedAttribute();
    // -1 is reference number, -2 is displaying nothing (-2 shouldn't happen but
    // is)
    if (col == -1 || col == -2 || m_meta_graph->isAttributeLocked(col)) {
        QMessageBox::warning(this, tr("Warning"), tr("Cannot rename locked column"),
                             QMessageBox::Ok, QMessageBox::Ok);
        return;
    }

    int newcol = RenameColumn(tab, col);
    if (newcol != -1) {
        m_meta_graph->setDisplayedAttribute(newcol);
        SetUpdateFlag(QGraphDoc::NEW_DATA);
        SetRedrawFlag(VIEW_ALL, QGraphDoc::REDRAW_GRAPH, QGraphDoc::NEW_COLUMN);
    }
}

int QGraphDoc::RenameColumn(AttributeTable *tab, int col) {
    QString colname = QString(tab->getColumnName(col).c_str());
    CRenameObjectDlg dlg("Column", colname); // using the column name sets the
                                             // dialog to replace column name mode
    bool success = false;
    while (dlg.exec() == QDialog::Accepted && !success && dlg.m_object_name != colname) {
        std::string newColName = dlg.m_object_name.toStdString();
        if (tab->hasColumn(newColName)) {
            QMessageBox::warning(this, tr("Notice"),
                                 tr("Sorry, another column already has this name, "
                                    "please choose a unique column name"),
                                 QMessageBox::Ok, QMessageBox::Ok);
        } else {
            tab->renameColumn(tab->getColumnName(col), newColName);
            return tab->getColumnIndex(newColName);
        }
    }
    return -1;
}

void QGraphDoc::OnColumnProperties() {
    AttributeTable *tab = &(m_meta_graph->getAttributeTable());
    LayerManagerImpl *layers = &(getLayers());
    int col = m_meta_graph->getDisplayedAttribute();

    CColumnPropertiesDlg dlg(tab, layers, m_meta_graph->getSelSet(), col);

    dlg.exec();
}

///////////////////////////////////////////////////////////////////////////////////////

void QGraphDoc::OnUpdateColumn() {
    int col = m_meta_graph->getDisplayedAttribute();
    // -1 is reference number, -2 is displaying nothing (-2 shouldn't happen but
    // is)
    if (col == -1 || col == -2 || m_meta_graph->isAttributeLocked(col)) {
        QMessageBox::warning(this, tr("Warning"), tr("Cannot edit locked column"), QMessageBox::Ok,
                             QMessageBox::Ok);
        return;
    }

    PointMapDM *pointmap = NULL;
    ShapeMapDM *shapemap = NULL;
    int vc = m_meta_graph->getViewClass();
    if (vc & MetaGraphDM::DX_VIEWVGA) {
        pointmap = &(m_meta_graph->getDisplayedPointMap());
    } else if (vc & MetaGraphDM::DX_VIEWAXIAL) {
        shapemap = &(m_meta_graph->getDisplayedShapeGraph());
    } else if (vc & MetaGraphDM::DX_VIEWDATA) {
        shapemap = &(m_meta_graph->getDisplayedDataMap());
    }

    if (ReplaceColumnContents(pointmap, shapemap, col)) {
        m_meta_graph->setDisplayedAttribute(col);
        SetUpdateFlag(QGraphDoc::NEW_DATA);
        SetRedrawFlag(VIEW_ALL, QGraphDoc::REDRAW_GRAPH, QGraphDoc::NEW_DATA);
    }
}

// Either shapemap or pointmap should be NULL:
bool QGraphDoc::ReplaceColumnContents(PointMapDM *pointmap, ShapeMapDM *shapemap, int col) {
    SalaObj program_context;
    SalaGrf graph;
    if (pointmap != NULL) {
        graph.map.point = &pointmap->getInternalMap();
        program_context = SalaObj(SalaObj::S_POINTMAPOBJ, graph);
    } else if (shapemap != NULL) {
        SalaGrf graph;
        graph.map.shape = &shapemap->getInternalMap();
        program_context = SalaObj(SalaObj::S_SHAPEMAPOBJ, graph);
    } else {
        QMessageBox::warning(this, tr("Warning"),
                             tr("Replace column contents requires either a shape "
                                "map or visibility graph"),
                             QMessageBox::Ok, QMessageBox::Ok);
        return false;
    }

    AttributeTable *table = program_context.getTable();

    // insert dialog is a misnomer now!
    CInsertColumnDlg dlg(table, col); // Using a column number sets it to use the
                                      // replace text rather than select text

    bool error = true;
    while (error && QDialog::Accepted == dlg.exec()) {
        error = false;
        size_t n = dlg.m_formula_text.length();
        char *text = new char[n + 1];
        if (n == 0) {
            QMessageBox::warning(this, tr("Notice"), tr("Please enter a formula"), QMessageBox::Ok,
                                 QMessageBox::Ok);
            error = true;
        } else {
            strcpy(text, dlg.m_formula_text.c_str());
            std::istringstream stream(text);
            SalaProgram proggy(program_context);
            if (!proggy.parse(stream)) {
                QString msg = QString("There was an error parsing your formula:\n\n") +
                              proggy.getLastErrorMessage().c_str();
                QMessageBox::warning(this, tr("Warning"), msg, QMessageBox::Ok, QMessageBox::Ok);
                error = true;
            } else {
                // just check you really are viewing the layers:
                bool retvar;
                if (dlg.m_selection_only) {
                    retvar = proggy.runupdate(col, pointmap ? pointmap->getSelSet()
                                                            : shapemap->getSelSet());
                } else {
                    retvar = proggy.runupdate(col);
                }
                if (!retvar) {
                    QString msg = QString("There was an error running your formula:\n\n") +
                                  proggy.getLastErrorMessage().c_str();
                    QMessageBox::warning(this, tr("Warning"), msg, QMessageBox::Ok,
                                         QMessageBox::Ok);
                    error = true;
                }
            }
        }
        if (!error) {
            table->getColumn(col).setFormula(text);
        }
        delete[] text;
    }
    return !error;
}

void QGraphDoc::OnEditQuery() {
    PointMapDM *pointmap = NULL;
    ShapeMapDM *shapemap = NULL;
    int vc = m_meta_graph->getViewClass();
    if (vc & MetaGraphDM::DX_VIEWVGA) {
        pointmap = &(m_meta_graph->getDisplayedPointMap());
    } else if (vc & MetaGraphDM::DX_VIEWAXIAL) {
        shapemap = &(m_meta_graph->getDisplayedShapeGraph());
    } else if (vc & MetaGraphDM::DX_VIEWDATA) {
        shapemap = &(m_meta_graph->getDisplayedDataMap());
    }

    if (SelectByQuery(pointmap, shapemap)) {
        SetRedrawFlag(VIEW_ALL, QGraphDoc::REDRAW_GRAPH, QGraphDoc::NEW_DATA);
    }
}

// Either shapemap or pointmap should be NULL:
bool QGraphDoc::SelectByQuery(PointMapDM *pointmap, ShapeMapDM *shapemap) {
    SalaObj program_context;
    SalaGrf graph;
    if (pointmap != NULL) {
        graph.map.point = &pointmap->getInternalMap();
        program_context = SalaObj(SalaObj::S_POINTMAPOBJ, graph);
    } else if (shapemap != NULL) {
        SalaGrf graph;
        graph.map.shape = &shapemap->getInternalMap();
        program_context = SalaObj(SalaObj::S_SHAPEMAPOBJ, graph);
    } else {
        QMessageBox::warning(this, tr("Warning"),
                             tr("Replace column contents requires either a shape "
                                "map or visibility graph"),
                             QMessageBox::Ok, QMessageBox::Ok);
        return false;
    }

    AttributeTable *table = program_context.getTable();
    // insert dialog is a misnomer now!
    CInsertColumnDlg dlg(table, -1); // -1 sets it to use the select text rather than replace text
    bool error = true;
    while (error && QDialog::Accepted == dlg.exec()) {
        error = false;
        std::string multibytetext(((MainWindow *)m_mainFrame)->m_formula_cache.toStdString());
        char *text = new char[multibytetext.length() + 1];
        strcpy(text, multibytetext.c_str());
        std::istringstream stream(text);
        SalaProgram proggy(program_context);
        if (!proggy.parse(stream)) {
            QString msg = QString("There was an error parsing your formula:\n") +
                          QString(proggy.getLastErrorMessage().c_str());
            QMessageBox::warning(this, tr("Error"), msg, QMessageBox::Ok, QMessageBox::Ok);
            error = true;
        } else {
            // just check you really are viewing the layers:
            bool retvar;
            std::vector<int> selset;
            if (dlg.m_selection_only) {
                retvar = proggy.runselect(selset,
                                          pointmap ? pointmap->getSelSet() : shapemap->getSelSet());
            } else {
                retvar = proggy.runselect(selset);
            }
            if (!retvar) {
                QString msg = QString("There was an error running your formula:\n") +
                              QString(proggy.getLastErrorMessage().c_str());
                QMessageBox::warning(this, tr("Error"), msg, QMessageBox::Ok, QMessageBox::Ok);
                error = true;
            } else {
                // make the selection using the selset:
                if (pointmap) {
                    pointmap->setCurSel(selset);
                } else {
                    // note, shape maps have been working with rowids directly:
                    shapemap->setCurSel(selset);
                }
            }
        }
        delete[] text;
        if (!error) {
            return true;
        }
    }
    return false;
}

void QGraphDoc::OnEditSelectToLayer() {
    if ((m_meta_graph->getViewClass() & (MetaGraphDM::DX_VIEWAXIAL | MetaGraphDM::DX_VIEWDATA)) &&
        m_meta_graph->isSelected()) {

        CRenameObjectDlg dlg("Layer"); // note, without specifying existing layer name, this
                                       // defaults to "New layer" behaviour
        if (QDialog::Accepted == dlg.exec()) {

            auto layer_name = dlg.m_object_name.toStdString();
            if (layer_name.empty()) {
                layer_name = "Untitled";
            }

            bool retvar = false;
            if (m_meta_graph->getViewClass() & (MetaGraphDM::DX_VIEWAXIAL)) {
                retvar = m_meta_graph->getDisplayedShapeGraph().selectionToLayer(layer_name);
            } else {
                retvar = m_meta_graph->getDisplayedDataMap().selectionToLayer(layer_name);
            }
            if (retvar) {
                SetRedrawFlag(VIEW_ALL, REDRAW_GRAPH, NEW_DATA);
                SetUpdateFlag(QGraphDoc::NEW_TABLE);
            } else {
                QMessageBox::warning(this, tr("Warning"),
                                     tr("Couldn't create new layer.\nPlease note there "
                                        "is a limit of 64 layers per map."),
                                     QMessageBox::Ok, QMessageBox::Ok);
            }
        }
    }
}

///////////////////////////////////////////////////////////////////////

void QGraphDoc::OnRemoveColumn() {
    // just check you really are viewing the layers:
    int col = m_meta_graph->getDisplayedAttribute();
    // -1 is reference number, -2 is displaying nothing (-2 shouldn't happen but
    // is)
    if (col == -1 || col == -2 || m_meta_graph->isAttributeLocked(col)) {
        QMessageBox::warning(this, tr("Warning"), tr("Cannot remove locked column"),
                             QMessageBox::Ok, QMessageBox::Ok);
    } else if (QMessageBox::Yes == QMessageBox::question(this, tr("Notice"),
                                                         tr("Are you sure you want to delete the "
                                                            "currently displayed column?"),
                                                         QMessageBox::Yes | QMessageBox::No,
                                                         QMessageBox::No)) {
        // note this -1 simply means shift back one
        m_meta_graph->setDisplayedAttribute(col - 1);
        m_meta_graph->removeAttribute(col);
        SetUpdateFlag(QGraphDoc::NEW_DATA);
        SetRedrawFlag(VIEW_ALL, QGraphDoc::REDRAW_GRAPH, QGraphDoc::NEW_COLUMN);
    }
}

////////////////////////////////////////////////////////////////////////////////

void QGraphDoc::OnFileProperties() {
    if (!m_meta_graph) {
        QMessageBox::warning(this, tr("Warning"), tr("No graph"), QMessageBox::Ok, QMessageBox::Ok);
    } else {
        CFilePropertiesDlg dlg;
        // editables
        dlg.m_title = QString(m_meta_graph->getFileProperties().getTitle().c_str());
        dlg.m_location = QString(m_meta_graph->getFileProperties().getLocation().c_str());
        dlg.m_description = QString(m_meta_graph->getFileProperties().getDescription().c_str());
        // non-editables
        dlg.m_author = QString(m_meta_graph->getFileProperties().getPerson().c_str());
        dlg.m_create_date = QString(m_meta_graph->getFileProperties().getDate().c_str());
        dlg.m_create_program = QString(m_meta_graph->getFileProperties().getProgram().c_str());
        dlg.m_organization = QString(m_meta_graph->getFileProperties().getOrganization().c_str());
        if (m_meta_graph && m_meta_graph->getVersion() != -1) {
            QString info;
            info.asprintf("%d", m_meta_graph->getVersion());
            dlg.m_file_version = info;
        } else {
            dlg.m_file_version = tr("<Unsaved>");
        }
        if (QDialog::Accepted == dlg.exec()) {
            m_meta_graph->getFileProperties().setTitle(dlg.m_title.toStdString());
            m_meta_graph->getFileProperties().setLocation(dlg.m_location.toStdString());
            m_meta_graph->getFileProperties().setDescription(dlg.m_description.toStdString());
        }
    }
}

void QGraphDoc::OnViewShowGrid() {
    if (m_meta_graph->getShowGrid()) {
        m_meta_graph->setShowGrid(false);
    } else {
        m_meta_graph->setShowGrid(true);
    }
    SetRedrawFlag(VIEW_ALL, QGraphDoc::REDRAW_GRAPH, QGraphDoc::NEW_DEPTHMAPVIEW_SETUP);
}

void QGraphDoc::OnViewSummary() {
    CAttributeSummary dlg(this);
    dlg.exec();
}

void QGraphDoc::OnToolsPointConvShapeMap() {
    // CWaitCursor wait;
    m_meta_graph->getDisplayedPointMap().getInternalMap().mergeFromShapeMap(
        m_meta_graph->getDisplayedDataMap().getInternalMap());
    m_meta_graph->setViewClass(MetaGraphDM::DX_SHOWVGATOP);
    SetUpdateFlag(QGraphDoc::NEW_TABLE);
    SetRedrawFlag(VIEW_ALL, REDRAW_GRAPH, NEW_DATA);
}

// this is unlink from a set of points!
void QGraphDoc::OnToolsAxialConvShapeMap() {
    if (m_meta_graph->getDataMaps().empty()) {
        QMessageBox::warning(this, tr("Warning"), tr("No data source layers for unlink points"),
                             QMessageBox::Ok, QMessageBox::Ok);
        return;
    }

    std::vector<std::string> names;
    for (size_t i = 0; i < m_meta_graph->getDataMaps().size(); i++) {
        names.push_back(std::string("Data Maps: ") + m_meta_graph->getDataMaps()[i].getName());
    }

    // choose shape map...
    CLayerChooserDlg dlg(names);
    dlg.m_text = tr("Please select source layer for unlink points");

    if (dlg.exec()) {
        // CWaitCursor wait;
        m_meta_graph->getDisplayedShapeGraph().unlinkFromShapeMap(
            m_meta_graph->getDataMaps()[dlg.m_layer].getInternalMap());
        m_meta_graph->setViewClass(MetaGraphDM::DX_SHOWAXIALTOP);
        SetUpdateFlag(QGraphDoc::NEW_TABLE);
        SetRedrawFlag(VIEW_ALL, REDRAW_GRAPH, NEW_DATA);
    }
}

void QGraphDoc::OnToolsLineLoadUnlinks() {
    // Get file from user
    QFileDialog::Options options;
    QString outfile = QFileDialog::getOpenFileName(
        0, tr("Import Unlink IDs"), "", tr("Text files (*.txt)\nAll files (*.*)"), 0, options);
    if (outfile.isEmpty())
        return;

    std::ifstream stream(outfile.toLatin1());
    if (stream.fail()) {
        QMessageBox::warning(this, tr("Warning"),
                             tr("There was an error opening the file.\nPlease "
                                "check the file is not already open"),
                             QMessageBox::Ok, QMessageBox::Ok);
        return;
    }
    // Check to see if they want to use another key column for the unlink:
    CAttributeChooserDlg dlg(m_meta_graph->getDisplayedShapeGraph().getAttributeTable());
    dlg.m_text = tr("Please select the attribute the file uses as the key for the unlinks");
    if (dlg.exec() == QDialog::Accepted) {
        // Run the process
        auto &map = m_meta_graph->getDisplayedShapeGraph();
        if (!map.getInternalMap().unlinkShapeSet(stream, dlg.m_attribute)) {
            QMessageBox::warning(this, tr("Warning"),
                                 tr("There was an error reading the unlink file.\n"
                                    "A tab delimited set of unlink IDs is expected"),
                                 QMessageBox::Ok, QMessageBox::Ok);
            return;
        }
        int conn_col = map.getAttributeTable().getColumnIndex("Connectivity");
        if (map.getDisplayedAttribute() == conn_col) {
            map.invalidateDisplayedAttribute();
            map.setDisplayedAttribute(conn_col); // <- reflect changes to connectivity counts
        }
        m_meta_graph->setViewClass(MetaGraphDM::DX_SHOWAXIALTOP);
        SetUpdateFlag(QGraphDoc::NEW_DATA);
        SetRedrawFlag(VIEW_ALL, REDRAW_GRAPH, NEW_DATA);
    }
}

void QGraphDoc::OnConvertMapShapes() {
    if (m_meta_graph && m_meta_graph->viewingShapes()) {
        CConvertShapesDlg dlg;
        if (QDialog::Accepted == dlg.exec()) {
            int viewclass = m_meta_graph->getViewClass();
            auto &map = m_meta_graph->getDisplayedDataMap();
            auto selSet = dlg.m_selected_only ? std::make_optional(map.getSelSet()) : std::nullopt;
            if (viewclass & MetaGraphDM::DX_VIEWDATA) {
                map.getInternalMap().convertPointsToPolys(dlg.m_radius, selSet);
            } else if (viewclass & MetaGraphDM::DX_VIEWAXIAL) {
                map.getInternalMap().convertPointsToPolys(dlg.m_radius, selSet);
            } else {
                QMessageBox::warning(this, tr("Warning"),
                                     tr("Cannot perform conversion on this map type"),
                                     QMessageBox::Ok, QMessageBox::Ok);
            }
        }
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////

QT_END_NAMESPACE
