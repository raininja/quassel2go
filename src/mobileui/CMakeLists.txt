# Builds the mobileui module

set(QT_DONT_USE_QTGUI 0)
set(QT_USE_QTNETWORK 1)
if(HAVE_DBUS)
  set(QT_USE_QTDBUS 1)
endif(HAVE_DBUS)
if(HAVE_WEBKIT)
  set(QT_USE_QTWEBKIT 1)
  set(QT_USE_XMLPATTERNS 1)
endif(HAVE_WEBKIT)
include(${QT_USE_FILE})

set(SOURCES
    aboutdlg.cpp
    awaylogfilter.cpp
    awaylogview.cpp
    bufferwidget.cpp
    channellistdlg.cpp
    chatitem.cpp
    chatline.cpp
    chatlinemodel.cpp
    chatlinemodelitem.cpp
    chatmonitorfilter.cpp
    chatmonitorview.cpp
    chatscene.cpp
    chatview.cpp
    chatviewsearchbar.cpp
    chatviewsearchcontroller.cpp
    chatviewsettings.cpp
    columnhandleitem.cpp
    coreconfigwizard.cpp
    coreconnectdlg.cpp
    coreconnectionstatusmodel.cpp
    coreinfodlg.cpp
    debugbufferviewoverlay.cpp
    debugconsole.cpp
    debuglogwidget.cpp
    debugmessagemodelfilter.cpp
    inputpresenter.cpp
    inputnickpresenter.cpp
    inputwidget.cpp
    ircconnectionwizard.cpp
    itemviewkinetictapsuppressor.cpp
    legacysystemtray.cpp
    mainpage.cpp
    mainwin.cpp
    markerlineitem.cpp
    mobileaction.cpp
    mobilebufferview.cpp
    mobilebufferviewwindow.cpp
    msgprocessorstatusmodel.cpp
    nicklistwidget.cpp
    qtui.cpp
    qmlchatview.cpp
    qmlcontextobject.cpp
    qmlinputwidget.cpp
    qmlnickwidget.cpp
    qmlsectionproxymodel.cpp
    qmlthemeimageprovider.cpp
    qmlscrollmodel.cpp
    mobileuiapplication.cpp
    qtuimessageprocessor.cpp
    qtuisettings.cpp
    qtuistyle.cpp
    scrollareakineticscroller.cpp
    settingsdlg.cpp
    settingspagedlg.cpp
    simplenetworkeditor.cpp
    systemtray.cpp
    systraynotificationbackend.cpp
    taskbarnotificationbackend.cpp
    maemonotificationbackend.cpp
    maemo5notification.cpp
    titlesetter.cpp
    topicmodel.cpp
    topicwidget.cpp
    webpreviewitem.cpp)

set(MOC_HDRS
    aboutdlg.h
    awaylogfilter.h
    awaylogview.h
    bufferwidget.h
    channellistdlg.h
    chatitem.h
    chatlinemodel.h
    chatmonitorfilter.h
    chatmonitorview.h
    chatscene.h
    chatview.h
    chatviewsearchbar.h
    chatviewsearchcontroller.h
    columnhandleitem.h
    coreconfigwizard.h
    coreconnectdlg.h
    coreconnectionstatusmodel.h
    coreinfodlg.h
    debugbufferviewoverlay.h
    debugconsole.h
    debuglogwidget.h
    debugmessagemodelfilter.h
    inputpresenter.h
    inputnickpresenter.h
    inputwidget.h
    ircconnectionwizard.h
    itemviewkinetictapsuppressor.h
    legacysystemtray.h
    mainpage.h
    mainwin.h
    markerlineitem.h
    mobileaction.h
    mobilebufferview.h
    mobilebufferviewwindow.h
    msgprocessorstatusmodel.h
    nicklistwidget.h
    qtui.h
    qmlchatview.h
    qmlcontextobject.h
    qmlinputwidget.h
    qmlnickwidget.h
    qmlsectionproxymodel.h
    qmlscrollmodel.h
    qmlthemeimageprovider.h
    mobileuiapplication.h
    qtuimessageprocessor.h
    qtuistyle.h
    scrollareakineticscroller.h
    settingsdlg.h
    settingspagedlg.h
    simplenetworkeditor.h
    systemtray.h
    systraynotificationbackend.h
    taskbarnotificationbackend.h
    maemonotificationbackend.h
    titlesetter.h
    topicmodel.h
    topicwidget.h)

set(HEADERS
    chatline.h
    chatlinemodelitem.h
    chatviewsettings.h
    qtuisettings.h
    webpreviewitem.h)

set(FORMS
    aboutdlg.ui
    bufferviewwidget.ui
    bufferwidget.ui
    channellistdlg.ui
    chatviewsearchbar.ui
    coreconfigwizardintropage.ui
    coreconfigwizardadminuserpage.ui
    coreconfigwizardstorageselectionpage.ui
    coreconfigwizardsyncpage.ui
    coreconnectauthdlg.ui
    coreconnectionstatuswidget.ui
    coreinfodlg.ui
    debugbufferviewoverlay.ui
    debugconsole.ui
    debuglogwidget.ui
    inputwidget.ui
    msgprocessorstatuswidget.ui
    nicklistwidget.ui
    settingsdlg.ui
    settingspagedlg.ui
    simplenetworkeditor.ui
    topicwidget.ui)

if(HAVE_KDE)
  set(SOURCES ${SOURCES} knotificationbackend.cpp)
  set(MOC_HDRS ${MOC_HDRS} knotificationbackend.h)
else(HAVE_KDE)
  if(HAVE_PHONON)
    set(SOURCES ${SOURCES} phononnotificationbackend.cpp)
    set(MOC_HDRS ${MOC_HDRS} phononnotificationbackend.h)
    set(FORMS ${FORMS} phononnotificationconfigwidget.ui)
    include_directories(${PHONON_INCLUDES})
  endif(HAVE_PHONON)
endif(HAVE_KDE)

if(HAVE_DBUS)
  set(SOURCES ${SOURCES} statusnotifieritem.cpp statusnotifieritemdbus.cpp)
  set(MOC_HDRS ${MOC_HDRS} statusnotifieritem.h statusnotifieritemdbus.h)
  set(FORMS ${FORMS})
  qt4_add_dbus_interface(DBUS ../../interfaces/org.kde.StatusNotifierWatcher.xml statusnotifierwatcher)
  qt4_add_dbus_interface(DBUS ../../interfaces/org.freedesktop.Notifications.xml notificationsclient)
  qt4_add_dbus_adaptor(DBUS ../../interfaces/org.kde.StatusNotifierItem.xml statusnotifieritemdbus.h StatusNotifierItemDBus)  
endif(HAVE_DBUS)

if(HAVE_SSL)
  set(SOURCES ${SOURCES} sslinfodlg.cpp)
  set(MOC_HDRS ${MOC_HDRS} sslinfodlg.h)
  set(FORMS ${FORMS} sslinfodlg.ui)
endif(HAVE_SSL)

if(INDICATEQT_FOUND)
  set(SOURCES ${SOURCES} indicatornotificationbackend.cpp)
  set(MOC_HDRS ${MOC_HDRS} indicatornotificationbackend.h)
  set(FORMS ${FORMS} indicatornotificationconfigwidget.ui)
  include_directories(${INDICATEQT_INCLUDE_DIRS})
endif(INDICATEQT_FOUND)

# embed QML file
set(CLIENT_RCS ${CLIENT_RCS} "mobileui/qml/qml.qrc" PARENT_SCOPE)

foreach(FORM ${FORMS})
  set(FORMPATH ${FORMPATH} ui/${FORM})
endforeach(FORM ${FORMS})

# handle settingspages
include(settingspages/settingspages.inc)
foreach(SP ${SETTINGSPAGES})
  set(SPSRC ${SPSRC} settingspages/${SP}settingspage.cpp)
  set(SPHDR ${SPHDR} settingspages/${SP}settingspage.h)
  set(SPFRM ${SPFRM} settingspages/${SP}settingspage.ui)
endforeach(SP ${SETTINGSPAGES})
foreach(SRC ${SP_SOURCES})
  set(SPSRC ${SPSRC} settingspages/${SRC})
endforeach(SRC ${SP_SOURCES})
foreach(HDR ${SP_HEADERS})
  set(SPHDR ${SPHDR} settingspages/${HDR})
endforeach(HDR ${SP_HEADERS})
foreach(FRM ${SP_FORMS})
  set(SPFRM ${SPFRM} settingspages/${FRM})
endforeach(FRM ${SP_FORMS})

qt4_wrap_cpp(MOC ${MOC_HDRS} ${SPHDR})
qt4_wrap_ui(UI ${FORMPATH} ${SPFRM})

include_directories(${CMAKE_SOURCE_DIR}/src/common
                    ${CMAKE_SOURCE_DIR}/src/client
                    ${CMAKE_SOURCE_DIR}/src/mobileui
                    ${CMAKE_SOURCE_DIR}/src/mobileui/settingspages
                    ${CMAKE_SOURCE_DIR}/src/uisupport
                    ${CMAKE_CURRENT_BINARY_DIR})

add_library(mod_mobileui STATIC ${SOURCES} ${SPSRC} ${MOC} ${DBUS} ${UI} ${HEADERS})
add_dependencies(mod_mobileui mod_common mod_client mod_uisupport)


add_dependencies(mod_mobileui
qml/Button.qml         qml/ChatListView.qml  qml/ChatView.qml  qml/Main.qml
qml/Input.qml     qml/NickListView.qml  qml/ScrollBar.qml
qml/ListSectionItem.qml
qml/ListItem.qml
qml/ToolButton.qml
qml/ProgressWidget.qml
qml/StatusMessageWidget.qml)

