// SPDX-FileCopyrightText: 2011-2012 Tasos Varoudis
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "graphdoc.h"
#include "indexWidget.h"
#include "mainwindowmoduleregistry.hpp"
#include "settings.h"
#include "treeWindow.h"
#include "views/mapview.h"

#include <QButtonGroup>
#include <QComboBox>
#include <QMainWindow>

class ItemTreeEntry {
  public:
    ItemTreeEntry() {
        m_type = -1;
        m_cat = -1;
        m_subcat = -1;
    }
    ItemTreeEntry(char t, short c, short sc) {
        m_type = t;
        m_cat = c;
        m_subcat = sc;
    }
    char m_type;
    short m_cat;
    short m_subcat;
};

class QDepthmapView;
class QGraphDoc;

QT_BEGIN_NAMESPACE class QAction;
class QMenu;
class QMdiArea;
class QMdiSubWindow;
class QSignalMapper;
class QToolButton;
QT_END_NAMESPACE

const int MaxRecentFiles = 5;

enum { FOCUSGRAPH = 1001, AllTransactionsDone = 1002 };

class QmyEvent : public QEvent {
  public:
    void *wparam;
    int lparam;
    QmyEvent(Type type, void *wp, int lp);
};

class MainWindow : public QMainWindow {
    Q_OBJECT

  public:
    MainWindow(const QString &fileToLoad, Settings &settings);
    // Graph analysis options
    QString m_formula_cache;
    Options m_options;
    QRgb m_foreground;
    QRgb m_background;

    MainWindowModuleRegistry mainWindowPluginRegistry;

    QGraphDoc *activeMapDoc();

    bool m_simpleVersion; // bool that replaces compile defines

    void RedoPlotViewMenu(QGraphDoc *pDoc);
    void updateToolbar();
    void update3DToolbar();
    void showContextMenu(QPoint &point);
    void UpdateStatus(QString s1, QString s2, QString s3);
    void updateGLWindows(bool datasetChanged, bool recentreView);
    void loadFile(QString fileName);

    void chooseAttributeOnIndex(int attributeIdx);

  protected:
    QGraphDoc *m_treeDoc;
    void closeEvent(QCloseEvent *event);
    bool eventFilter(QObject *object, QEvent *e);
    virtual void actionEvent(QActionEvent *event);

  private slots:
    void updateActiveWindows();
    void updateSubWindowTitles(QString newTitle);
    void updateWindowMenu();
    void setActiveSubWindow(QWidget *window);
    void OnSelchangingTree(QTreeWidgetItem *item, int col);
    void OnSelchangingList();
    void OnFileNew();
    void OnFileImport();
    void OnFileOpen();
    void OnFileClose();
    void OnFileSave();
    void OnFileSaveAs();
    void OnFileProperties();
    void OnFilePrint();
    void OnFilePrintPreview();
    void OnFilePrintSetup();
    void OnEditUndo();
    void OnEditCopyData();
    void OnEditCopy();
    void OnEditSave();
    void OnEditClear();
    void OnEditQuery();
    void OnViewZoomsel();
    void OnEditSelectToLayer();
    void OnAppAbout();
    void OnLayerNew();
    void OnLayerDelete();
    void OnLayerConvert();
    void OnLayerConvertDrawing();
    void OnConvertMapShapes();
    void OnFileExport();
    void OnFileExportMapGeometry();
    void OnFileExportLinks();
    void OnAxialConnectionsExportAsDot();
    void OnAxialConnectionsExportAsPairCSV();
    void OnSegmentConnectionsExportAsPairCSV();
    void OnPointmapExportConnectionsAsCSV();
    void OnAddColumn();
    void OnRenameColumn();
    void OnUpdateColumn();
    void OnRemoveColumn();
    void OnColumnProperties();
    void OnPushToLayer();
    void OnToolsMakeGraph();
    void OnToolsUnmakeGraph();
    void OnToolsImportVGALinks();
    void OnToolsGenerateIsovistsFromFile();
    void OnToolsIsovistpath();
    void OnToolsAgentLoadProgram();
    void OnToolsRunAxa();
    void OnToolsPD();
    void OnToolsAPD();
    void OnToolsMakeFewestLineMap();
    void OnToolsAxialConvShapeMap();
    void OnToolsLineLoadUnlinks();
    void OnToolsRunSeg();
    void OnToolsTopomet();
    void OnToolsTPD();
    void OnToolsMPD();
    void OnToolsPointConvShapeMap();
    void OnToolsOptions();
    void OnViewCentreView();
    void OnViewShowGrid();
    void OnViewSummary();
    void OnViewColourRange();
    void OnHelpBugs();
    void OnHelpManual();
    void OnHelpTutorials();
    void OnHelpSalaManual();
    void OnEditGrid();
    void OnWindowMap();
    void OnViewTable();
    void OnWindow3dView();
    void OnWindowGLView();
    void OnViewScatterplot();
    void OnToolsRun();
    void OnToolsAgentRun();
    // MapView message
    void zoomModeTriggered(QAction *triggerAction);
    void FillModeTriggered();
    void LineModeTriggered(QAction *triggerAction);
    void isoModeTriggered(int isovistModeType);
    void joinTriggered(QAction *triggerAction);
    void zoomButtonTriggered();
    void FillButtonTriggered();
    void LineButtonTriggered();
    void isoButtonTriggered();
    void joinButtonTriggered();
    void openRecentFile(QString fileName);
    void StepDepthTriggered();
    void AxialMapTriggered();
    void SelectPenTriggered();
    void DragButtonTriggered();
    void SelectButtonTriggered();
    void OnSelchangeViewSelector_X(int);
    void OnSelchangeViewSelector_Y(int);
    void OninvertColor();
    void OnzoomTo();
    // PlotView message
    void OntoggleColor();
    void OntoggleOrg();
    void OnviewTrend();
    void OnYX();
    void OnRtwo();
    // Menu Update slots
    void updateViewMenu();
    void updateAttributesMenu();
    void updateMapMenu();
    void updateEditMenu();
    void updateFileMenu();
    void updateVisibilitySubMenu();
    void updateStepDepthSubMenu();
    void updateSegmentStepDepthSubMenu();
    void updateAgentToolsSubMenu();
    void updateSegmentSubMenu();
    void updateAxialSubMenu();
    // 3D View ToolBar slots
    void OnToolsImportTraces();
    void OnAddAgent();
    void OnToolsAgentsPlay();
    void OnToolsAgentsPause();
    void OnToolsAgentsStop();
    void OnAgentTrails();
    void On3dRot();
    void On3dPan();
    void On3dZoom();
    void OnPlayLoop();
    void On3dFilled();

  private:
    int OnFocusGraph(QGraphDoc *pDoc, int lParam);
    void setCurrentFile(const QString &fileName);
    void updateRecentFileActions(const QStringList &files);
    QString strippedName(const QString &fullFileName);

    void createActions();
    void createMenus();
    void createToolBars();
    void createStatusBar();

    // Settings Files
    Settings &mSettings;
    void readSettings();
    void writeSettings();

    bool m_defaultMapWindowIsLegacy;

    QWidget *setupAttributesListWidget();
    MapView *createMapView();
    MapView *activeMapView();
    QMdiSubWindow *findMapView(const QString &fileName);
    //////////////////////////////////////////////////////
    //	treeContorl
    QVector<QIcon> m_tree_icon;
    std::map<int, std::string> m_view_map_entries;

    std::vector<bool> m_attribute_locked;
    std::map<QTreeWidgetItem *, ItemTreeEntry> m_treegraphmap;
    std::map<QTreeWidgetItem *, ItemTreeEntry> m_treedrawingmap;
    QTreeWidgetItem *m_topgraph;
    QTreeWidgetItem *m_backgraph;
    QTreeWidgetItem *m_treeroots[5];

    void MakeTree();
    void MakeGraphTree();
    void MakeDrawingTree();
    void ClearGraphTree();
    void MakeAttributeList();
    void SetAttributeChecks();
    void SetDrawingTreeChecks();
    void SetGraphTreeChecks();

    ////////////////////////////////////////////////////////////

    QMdiArea *mdiArea;
    QSignalMapper *windowMapper;
    IndexWidget *m_indexWidget;
    AttribWindow *m_attrWindow;

    QLabel *g_size;
    QLabel *g_pos_curr;
    QLabel *g_info_curr;

    QMenu *fileMenu;
    QMenu *editMenu;
    QMenu *mapMenu;
    QMenu *exportSubMenu;
    QMenu *attributesMenu;
    QMenu *toolsMenu;
    QMenu *visibilitySubMenu;
    QMenu *stepDepthSubMenu;
    QMenu *agentToolsSubMenu;
    QMenu *axialSubMenu;
    QMenu *segmentSubMenu;
    QMenu *segmentStepDepthSubMenu;
    QMenu *viewMenu;
    QMenu *windowMenu;
    QMenu *helpMenu;

    QToolBar *fileToolBar;
    QToolBar *editToolBar;
    QToolBar *plotToolBar;
    QToolBar *thirdViewToolBar;
    QToolButton *fillColorToolButton;
    QToolButton *zoomToolButton;
    QToolButton *lineToolButton;
    QToolButton *newisoToolButton;
    QToolButton *JoinToolButton;
    QToolButton *SelectButton;
    QToolButton *DragButton;
    QToolButton *SelectPenButton;
    QToolButton *AxialMapButton;
    QToolButton *StepDepthButton;
    QToolButton *attr_add_button;
    QToolButton *attr_del_button;

    // File Menu Actions
    QAction *newAct;
    QAction *openAct;
    QAction *closeAct;
    QAction *saveAct;
    QAction *saveAsAct;
    QAction *propertiesAct;
    QAction *printAct;
    QAction *printPreviewAct;
    QAction *printSetupAct;
    QAction *recentFileActs[MaxRecentFiles];
    QAction *separatorAct;
    QAction *exitAct;

    // Edit Menu Actions
    QAction *undoAct;
    QAction *copyDataAct;
    QAction *copyScreenAct;
    QAction *exportScreenAct;
    QAction *clearAct;
    QAction *selectByQueryAct;
    QAction *selectionToLayerAct;

    // Map Menu Actions
    QAction *mapNewAct;
    QAction *deleteAct;
    QAction *convertActiveMapAct;
    QAction *convertDrawingMapAct;
    QAction *convertMapShapesAct;
    QAction *importAct;
    QAction *exportAct;
    QAction *exportGeometryAct;
    QAction *exportLinksAct;
    QAction *exportAxialConnectionsDotAct;
    QAction *exportAxialConnectionsPairAct;
    QAction *exportSegmentConnectionsPairAct;
    QAction *exportPointmapConnectionsPairAct;

    // Attributes Menu Actions
    QAction *renameColumnAct;
    QAction *columnPropertiesAct;

    // Tools Menu Actions
    QAction *makeVisibilityGraphAct;
    QAction *unmakeVisibilityGraphAct;
    QAction *importVGALinksAct;
    QAction *generateIsovistsAct;
    QAction *makeIsovistPathAct;
    QAction *runVisibilityGraphAnalysisAct;
    QAction *visibilityStepAct;
    QAction *metricStepAct;
    QAction *angularStepAct;
    QAction *convertDataMapLinesAct;
    QAction *runAgentAnalysisAct;
    QAction *loadAgentProgramAct;
    QAction *runGraphAnaysisAct;
    QAction *stepDepthAct;
    QAction *reduceToFewestLineMapAct;
    QAction *convertDataMapPointsAct;
    QAction *loadUnlinksFromFileAct;
    QAction *runAngularSegmentAnalysisAct;
    QAction *runTopologicalOrMetricAnalysisAct;
    QAction *segmentAngularStepAct;
    QAction *topologicalStepAct;
    QAction *segmentMetricStepAct;
    QAction *optionsAct;

    // View Menu Actions
    QAction *showGridAct;
    QAction *attributeSummaryAct;

    // Window Menu Actions
    QAction *mapAct;
    QAction *scatterPlotAct;
    QAction *tableAct;
    QAction *thirdDViewAct;
    QAction *glViewAct;
    QAction *colourRangeAct;
    QAction *cascadeAct;
    QAction *tileAct;
    QAction *arrangeIconsAct;

    // Help Menu Actions
    QAction *onlineBugsAct;
    QAction *onlineHandbookAct;
    QAction *onlineTutorialsAct;
    QAction *onlineScriptingManualAct;
    QAction *aboutDepthMapAct;

    // depthmapX Contorl
    QAction *addColumAct;
    QAction *updateColumAct;
    QAction *removeColumAct;
    QAction *pushValueAct;
    QAction *invertColorAct;
    QAction *zoomInAct;
    QAction *zoomOutAct;
    QAction *zoomToAct;
    QAction *RecentAct;
    QAction *SetGridAct;
    QAction *STDFillColorAct;
    QAction *ContextFillColorAct;
    QAction *AugmentFillColorAct; // AV test - TV
    QAction *SelectLineAct;
    QAction *SelectPolyLineAct;
    QAction *MakeIosAct;
    QAction *PartialMakeIosAct;
    QAction *JoinAct;
    QAction *JoinUnlinkAct;
    // PlotMap control

    QComboBox *x_coord;
    QComboBox *y_coord;
    QAction *toggleColor;
    QAction *toggleOrg;
    QAction *viewTrend;
    QAction *yx;
    QAction *Rtwo;
    // 3D View ToolBar
    QAction *toolsImportTracesAct;
    QAction *addAgentAct;
    QAction *toolsAgentsPlayAct;
    QAction *toolsAgentsPauseAct;
    QAction *toolsAgentsStopAct;
    QAction *agentTrailsAct;
    QAction *thirdRotAct;
    QAction *thirdPanAct;
    QAction *thirdZoomAct;
    QAction *playLoopAct;
    QAction *thirdFilledAct;

    int m_selected_mapbar_item = -1;

    enum {
        ID_MAPBAR_ZOOM_ITEMS = 2,
        ID_MAPBAR_FILL_ITEMS = 8,
        ID_MAPBAR_DRAW_ITEMS = 10,
        ID_MAPBAR_ISOVIST_ITEMS = 12,
        ID_MAPBAR_JOIN_ITEMS = 15
    };

    enum {
        ID_MAPBAR_ITEM_SELECT = 0,
        ID_MAPBAR_ITEM_MOVE = 1,
        ID_MAPBAR_ITEM_ZOOM_IN = 2,
        ID_MAPBAR_ITEM_ZOOM_OUT = 3,
        ID_MAPBAR_ITEM_FILL = 7,
        ID_MAPBAR_ITEM_SEMIFILL = 8,
        ID_MAPBAR_ITEM_PENCIL = 9,
        ID_MAPBAR_ITEM_LINETOOL = 10,
        ID_MAPBAR_ITEM_POLYGON = 11,
        ID_MAPBAR_ITEM_ISOVIST = 12,
        ID_MAPBAR_ITEM_HALFISOVIST = 13,
        ID_MAPBAR_ITEM_AL2 = 14,
        ID_MAPBAR_ITEM_PD = 15,
        ID_MAPBAR_ITEM_JOIN = 16,
        ID_MAPBAR_ITEM_UNJOIN = 17,
        ID_MAPBAR_ITEM_AUGMENT_FILL = 18 // AV test - TV
    };

    int m_currentIsovistModeType = ID_MAPBAR_ITEM_ISOVIST;
};
