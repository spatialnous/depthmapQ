// SPDX-FileCopyrightText: 2011-2012 Tasos Varoudis
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "depthmapX/graphdoc.hpp"

#include "salalib/agents/agent.hpp"
#include "salalib/agents/agentprogram.hpp"

#include <QOpenGLWidget>
#include <QPoint>
#include <QRect>
#include <QSize>

#include <QOpenGLFunctions>

#define ID_ADD_AGENT 32947
#define ID_3D_PAN 32948
#define ID_3D_ZOOM 32949
#define ID_3D_ROT 32950
#define ID_3D_PAUSE 32951
#define ID_3D_PLAY_LOOP 32981
#define ID_3D_FILLED 32983

/////////////////////////////////////////////////////////////////////////////

// CScreenAgent

struct CMannequinTemplate {
    bool m_init;
    float m_unit;
    bool m_male;
    float m_points[10][3];
    CMannequinTemplate();
    virtual ~CMannequinTemplate();
    void Init(float unit, bool male);
    void Destroy();
};

struct QMannequin {
    float m_zrot; // facing direction
    Point2f m_startloc;
    Point2f m_lastloc;
    Point2f m_nextloc;
    int m_frame;
    bool m_left;
    bool m_paused;
    // these are used when replaying a trace
    bool m_active;
    bool m_playback;
    double m_time;
    //
    int m_agent_id;
    int m_trace_id;
    //
    QMannequin(const Point2f &lastloc = Point2f(), int id = -1, bool playback = false);
    QMannequin(const QMannequin &man);
    QMannequin &operator=(const QMannequin &man);
    void draw(CMannequinTemplate &templ, bool drawtrails, bool highlight);
    void frame();
    void advance(const Point2f &nextloc);
    // big long array to record trails!
    float m_points[150];
    int m_pointstart;
    int m_pointcount;
};

struct Trace {
    double starttime;
    double endtime;
    std::vector<Event2f> events;
};

struct C3DPixelData {
    Point2f m_point;
    int m_value;
    C3DPixelData(const Point2f &p = Point2f()) {
        m_point = p;
        m_value = -1;
    }
};

/////////////////////////////////////////////////////////////////////////////

class Q3DView : public QOpenGLWidget, protected QOpenGLFunctions {
    Q_OBJECT

    // Attributes
  public:
    Q3DView(QWidget *parent = NULL,
            QGraphDoc *doc = NULL); // protected constructor used by dynamic creation
    ~Q3DView();
    QSize sizeHint() const;

    QGraphDoc *pDoc;

    unsigned int *m_nTimerID;
    QRect m_oldRect;
    float m_fRadius;
    QPainter *m_pDC;
    //
    bool m_quick_draw;
    std::mutex m_draw_mutex;
    bool m_animating;
    bool m_drawtrails;
    bool m_fill;
    double m_track; // camera track -- used in playloop
    //
    //
    Region4f m_region;
    float *m_points;
    float m_rect[4][3];
    std::map<int, C3DPixelData> m_pixels;
    //
    int m_pointcount;
    //
    int m_mouse_mode;
    int m_mouse_mode_on;
    bool m_right_mouse;
    int m_key_mode_on;
    int m_keydown;
    QPoint m_mouse_origin;
    //
    float m_panx;
    float m_pany;
    float m_rotx;
    float m_roty;
    float m_zoom;
    //
    // use to initialise mannequintemplate:
    CMannequinTemplate m_male_template;
    CMannequinTemplate m_female_template;
    std::vector<QMannequin> m_mannequins;
    std::vector<Agent> m_agents;
    std::vector<Trace> m_traces;
    AgentProgram m_agent_program;
    //
    // used to keep track of internal time for all agents
    double m_global_time;
    //
    // Operations
  public:
    // void Init();
    void CreateRGBPalette();
    void Reshape(int x, int y);
    void DrawScene();
    void ReloadLineData();
    void ReloadPointData();
    void SetModelMat();
    void CreateAgent(QPoint point);

    void Rot(QSize diff);
    void Pan(QSize diff);
    void Zoom(QSize diff);
    void PlayLoop();

    // My message map functions
    int OnRedraw(int wParam, int lParam);
    void OnLButtonDown(unsigned int nFlags, QPoint point);
    void OnRecentreView();
    void On3dPan();
    void On3dRot();
    void On3dZoom();
    void OnAddAgent();
    void OnMouseMove(unsigned int nFlags, QPoint point);
    void OnLButtonUp(unsigned int nFlags, QPoint point);
    void OnAgentTrails();
    void OnToolsAgentLoadProgram();
    void OnKeyDown(unsigned int nChar, unsigned int nRepCnt, unsigned int nFlags);
    void OnKeyUp(unsigned int nChar, unsigned int nRepCnt, unsigned int nFlags);
    void OnPlayLoop();
    void On3dFilled();
    bool OnMouseWheel(unsigned int nFlags, short zDelta, QPointF pt);
    void OnRButtonDown(unsigned int nFlags, QPoint point);
    void OnRButtonUp(unsigned int nFlags, QPoint point);
    void OnToolsImportTraces();
    void OnToolsAgentsPause();
    void OnToolsAgentsStop();
    void OnToolsAgentsPlay();

  public slots:
    void timerSlot();

  protected:
    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();
    virtual void timerEvent(QTimerEvent *event);
    // virtual void paintEvent(QPaintEvent *event);
    virtual void keyPressEvent(QKeyEvent *event);
    virtual void keyReleaseEvent(QKeyEvent *event);
    // virtual void resizeEvent(QResizeEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);
    virtual void wheelEvent(QWheelEvent *event);
    //  virtual void contextMenuEvent(QContextMenuEvent *event);
    virtual void closeEvent(QCloseEvent *event);
};
