// SPDX-FileCopyrightText: 2021 Petros Koutsolampros
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "maplayer.h"

#include "salalib/mgraph.h"

#include <QObject>

// This is a representation of the MetaGraph (the file itself) meant
// to be displayed over multiple views (viewports, lists etc.)
class GraphViewModel;
class GraphModel : public QObject {
    Q_OBJECT

    std::string m_filename;

  public:
    GraphModel(std::string filename);

    MetaGraph &getMetaGraph() const { return *m_metaGraph; }
    bool hasMetaGraph() const { return m_metaGraph.get() != nullptr; }

    std::string getFilenameStr() { return m_filename; }
    Q_INVOKABLE QString getFilename() { return QString::fromStdString(m_filename); }

  private:
    std::unique_ptr<MetaGraph> m_metaGraph = nullptr;
};
