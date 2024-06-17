# SPDX-FileCopyrightText: 2021 Petros Koutsolampros
#
# SPDX-License-Identifier: GPL-3.0-or-later

target_sources(acanthis
    PUBLIC
        licenseagreement.h
        settings/generalpage.h
        settings/interfacepage.h
        settings/settingsdialog.h
    PRIVATE
        licenseagreement.cpp
        settings/generalpage.cpp
        settings/interfacepage.cpp
        settings/settingsdialog.cpp
)
