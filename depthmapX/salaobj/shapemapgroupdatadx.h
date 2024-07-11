// SPDX-FileCopyrightText: 2000-2010 University College London, Alasdair Turner
// SPDX-FileCopyrightText: 2011-2012 Tasos Varoudis
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "salalib/shapemapgroupdata.h"

#include "genlib/p2dpoly.h"

#include <deque>
#include <string>

class ShapeMapGroupDataDX {

    ShapeMapGroupData m_mapGroupData;
    mutable std::optional<size_t> m_currentLayer;

  public:
    QtRegion m_region; // easier public for now

    ShapeMapGroupDataDX(ShapeMapGroupData &&mapGroupData) : m_mapGroupData(mapGroupData) {
        m_currentLayer = std::nullopt;
    }
    ShapeMapGroupDataDX(const std::string &name = std::string()) : m_mapGroupData(name) {
        m_currentLayer = std::nullopt;
    }
    ShapeMapGroupDataDX(ShapeMapGroupDataDX &&other)
        : m_mapGroupData(other.m_mapGroupData), m_currentLayer(other.m_currentLayer),
          m_region(std::move(other.m_region)) {}
    ShapeMapGroupDataDX &operator=(ShapeMapGroupDataDX &&other) {
        m_mapGroupData = other.m_mapGroupData;
        m_currentLayer = other.m_currentLayer;
        m_region = std::move(other.m_region);
        return *this;
    }
    ShapeMapGroupDataDX(const ShapeMapGroupDataDX &) = delete;
    ShapeMapGroupDataDX &operator=(const ShapeMapGroupDataDX &) = delete;

    void setName(const std::string &name) { m_mapGroupData.name = name; }
    const std::string &getName() const { return m_mapGroupData.name; }

    QtRegion &getRegion() const { return (QtRegion &)m_region; }

    const ShapeMapGroupData &getInternalData() const { return m_mapGroupData; }

    bool hasCurrentLayer() const { return m_currentLayer.has_value(); }
    int getCurrentLayer() const { return m_currentLayer.has_value() ? *m_currentLayer : -1; }
    void invalidateCurrentLayer() const { m_currentLayer = std::nullopt; }
    void setCurrentLayer(int currentLayer) const { m_currentLayer = currentLayer; }

  public:
    bool read(std::istream &stream);
    bool write(std::ofstream &stream);
};
