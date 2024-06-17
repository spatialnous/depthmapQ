// SPDX-FileCopyrightText: 2017 Petros Koutsolampros
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "coreapplication.h"

#include "agl/view/aglmapviewport.h"
#include "aqmapviewmodel.h"
#include "settingsimpl.h"

#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QtQuick/QQuickView>

int CoreApplication::exec() {
    SettingsImpl settings(new DefaultSettingsFactory);

    if (!settings.readSetting(SettingTag::licenseAccepted, false).toBool()) {
        //        auto dummy = MainWindowFactory::getLicenseDialog();
        //        dummy->setModal(true);
        //        dummy->setWindowTitle(TITLE_BASE);
        //        dummy->exec();
        //        if (dummy->result() == QDialog::Rejected) {
        //            return 0;
        //        }
        settings.writeSetting(SettingTag::licenseAccepted, true);
    }

    auto args = arguments();
    std::string fileToLoad = m_fileToLoad;
    if (args.length() == 2) {
        fileToLoad = args[1].toStdString();
    }

    setOrganizationName("acanthis");
    setOrganizationDomain("acanth.is");
    setApplicationName("acanthis");

    int versionMajor = 1;
    int versionMinor = 0;

    qmlRegisterType<AGLMapViewport>("acanthis", versionMajor, versionMinor, "AGLMapViewport");
    //    qmlRegisterType<GraphViewModel>("acanthis", versionMajor, versionMinor, "GraphViewModel");
    qmlRegisterType<AQMapViewModel>("acanthis", versionMajor, versionMinor, "AQMapViewModel");
    qmlRegisterSingletonType<DocumentManager>("acanthis", versionMajor, versionMinor,
                                              "DocumentManager",
                                              [&](QQmlEngine *, QJSEngine *) -> QObject * {
                                                  return new DocumentManager;
                                                  // the QML engine takes ownership of the singleton
                                              });

    qmlRegisterUncreatableType<GraphModel>(
        "acanthis", versionMajor, versionMinor, "GraphModel",
        QLatin1String("Cannot create objects of type GraphModel"));

    qmlRegisterSingletonType(QUrl("qrc:///scenegraph/Theme.qml"), "acanthis", versionMajor,
                             versionMinor, "Theme");

    QQmlApplicationEngine engine;

    QJSValue jsMetaObject = engine.newQMetaObject(&GraphViewModel::staticMetaObject);
    engine.globalObject().setProperty(GraphViewModel::staticMetaObject.className(), jsMetaObject);
    engine.rootContext()->setContextProperty("qtversion", QString(qVersion()));

    engine.load(QUrl(QStringLiteral("qrc:/scenegraph/MainWindow.qml")));
    //    mMainWindow->setResizeMode(QQuickView::SizeRootObjectToView);
    //    engine->show();

    //    mMainWindow = MainWindowFactory::getMainWindow(fileToLoad, settings);
    //    mMainWindow->setResizeMode(QQuickView::SizeRootObjectToView);
    //    mMainWindow->setSource(QUrl("qrc:///scenegraph/main.qml"));
    //    mMainWindow->show();

    QGuiApplication::setWindowIcon(QIcon(":/images/acanthis.svg"));

    return QGuiApplication::exec();
}
