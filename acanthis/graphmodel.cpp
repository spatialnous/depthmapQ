// SPDX-FileCopyrightText: 2021 Petros Koutsolampros
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "graphmodel.h"

#include <QVariant>

GraphModel::GraphModel(std::string filename) : m_filename(filename) {
    m_metaGraph = std::unique_ptr<MetaGraph>(new MetaGraph(filename));
    m_metaGraph->readFromFile(filename);
}
