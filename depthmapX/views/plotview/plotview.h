// SPDX-FileCopyrightText: 2011-2012 Tasos Varoudis
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include <QWidget>

/////////////////////////////////////////////////////////////////////////////
// PlotView view

// class CHoverWnd;

#include "qpixmap.h"
#include <QPoint>
#include <QRect>
#include <QSize>

#include "GraphDoc.h"
#include "linreg.h"

#define MK_LBUTTON 0x0001
#define MK_RBUTTON 0x0002
#define MK_SHIFT 0x0004
#define MK_CONTROL 0x0008
#define MK_MBUTTON 0x0010

class QPlotView : public QWidget {
    Q_OBJECT

    // Attributes
  public:
    QGraphDoc *pDoc;
    QPlotView();
    QSize sizeHint() const;

    int m_x_axis;
    int m_y_axis;
    int curr_x;
    int curr_y;

    std::vector<AttributeIndexItem> idx_x;
    std::vector<AttributeIndexItem> idx_y;
    void RedoIndices();

    bool m_queued_redraw;
    bool m_view_origin;
    bool m_view_trend_line;
    bool m_view_equation;
    bool m_view_rsquared;
    bool m_view_monochrome;

    LinReg<float> m_regression;

    //
    double dataX(int x);
    int screenX(double x);
    double dataY(int x);
    int screenY(double x);
    //
    QtRegion m_data_bounds;
    QRect m_screen_bounds;
    //
    QPoint m_mouse_point;
    QRect m_drag_rect_a;
    QRect m_drag_rect_b;
    bool m_selecting;
    bool m_drawdragrect;
    QRgb m_background;
    QRgb m_foreground;

    void *m_parent; // MainWindow*

    // Operations
  public:
    void SetAxis(int axis, int col, bool reset);
    void ResetRegression();

    bool Output(QPainter *pDC, QGraphDoc *pDoc, bool screendraw);

    // this is a tells us how many 100ms ticks have passed since the mouse moved
    int Tid_redraw;

    void OnViewTrendLine();
    void OnViewRsquared();
    void OnViewColor();
    void OnViewOrigin();
    void OnViewEquation();
    int OnRedraw(int wParam, int lParam);
    void OnLButtonDown(bool nFlags, QPoint point);
    void OnMouseMove(bool nFlags, QPoint point);
    void OnLButtonUp(bool nFlags, QPoint point);

    // Implementation
  protected:
    virtual bool eventFilter(QObject *object, QEvent *e);
    virtual void keyPressEvent(QKeyEvent *event);
    virtual void paintEvent(QPaintEvent *event);
    virtual void resizeEvent(QResizeEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);
    virtual void closeEvent(QCloseEvent *event);
    virtual void timerEvent(QTimerEvent *event);

  private:
    QPixmap *m_pixmap;
};
