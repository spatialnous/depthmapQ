// SPDX-FileCopyrightText: 2017 Petros Koutsolampros
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "mapview.h"

MapView::MapView(QGraphDoc &pDoc, Settings &settings, QWidget *parent)
    : QOpenGLWidget(parent), m_pDoc(pDoc), m_settings(settings) {}
