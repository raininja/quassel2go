/***************************************************************************
 *   Copyright (C) 2005-2010 by the Quassel Project                        *
 *   devel@quassel-irc.org                                                 *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) version 3.                                           *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#include "mainwin.h"

#ifdef HAVE_KDE
#  include <KAction>
#  include <KActionCollection>
#  include <KHelpMenu>
#  include <KMenuBar>
#  include <KShortcutsDialog>
#  include <KStatusBar>
#  include <KToggleFullScreenAction>
#  include <KToolBar>
#  include <KWindowSystem>
#endif

#ifdef Q_WS_X11
#  include <QX11Info>
#endif

#include "aboutdlg.h"
#include "awaylogfilter.h"
#include "awaylogview.h"
#include "mobileaction.h"
#include "actioncollection.h"
#include "bufferhotlistfilter.h"
#include "buffermodel.h"
#include "mobilebufferviewwindow.h"
#include "bufferviewoverlay.h"
#include "bufferviewoverlayfilter.h"
#include "bufferwidget.h"
#include "channellistdlg.h"
#include "chatlinemodel.h"
#include "chatscene.h"
#include "chatmonitorfilter.h"
#include "chatmonitorview.h"
#include "chatview.h"
#include "client.h"
#include "clientbacklogmanager.h"
#include "clientbufferviewconfig.h"
#include "clientbufferviewmanager.h"
#include "clientignorelistmanager.h"
#include "columnhandleitem.h"
#include "coreconfigwizard.h"
#include "coreconnectdlg.h"
#include "coreconnection.h"
#include "coreconnectionstatusmodel.h"
#include "coreinfodlg.h"
#include "contextmenuactionprovider.h"
#include "debugbufferviewoverlay.h"
#include "debuglogwidget.h"
#include "debugmessagemodelfilter.h"
#include "flatproxymodel.h"
#include "iconloader.h"
#include "irclistmodel.h"
#include "ircconnectionwizard.h"
#include "legacysystemtray.h"
#include "msgprocessorstatusmodel.h"
#include "nicklistwidget.h"
#include "mobileuiapplication.h"
#include "qtuimessageprocessor.h"
#include "qtuisettings.h"
#include "qtuistyle.h"
#include "settingsdlg.h"
#include "settingspagedlg.h"
#include "statusnotifieritem.h"
#include "toolbaractionprovider.h"
#include "topicmodel.h"

#ifndef HAVE_KDE
#  ifdef HAVE_PHONON
#    include "phononnotificationbackend.h"
#  endif
#  include "systraynotificationbackend.h"
#  include "taskbarnotificationbackend.h"
#  include "maemonotificationbackend.h"
#else /* HAVE_KDE */
#  include "knotificationbackend.h"
#endif /* HAVE_KDE */

#ifdef HAVE_SSL
#  include "sslinfodlg.h"
#endif

#ifdef HAVE_INDICATEQT
#include "indicatornotificationbackend.h"
#endif

#include "settingspages/aliasessettingspage.h"
#include "settingspages/appearancesettingspage.h"
#include "settingspages/backlogsettingspage.h"
#include "settingspages/bufferviewsettingspage.h"
#include "settingspages/chatmonitorsettingspage.h"
#include "settingspages/chatviewsettingspage.h"
#include "settingspages/connectionsettingspage.h"
#include "settingspages/coreaccountsettingspage.h"
#include "settingspages/coreconnectionsettingspage.h"
#include "settingspages/highlightsettingspage.h"
#include "settingspages/identitiessettingspage.h"
#include "settingspages/ignorelistsettingspage.h"
#include "settingspages/inputwidgetsettingspage.h"
#include "settingspages/itemviewsettingspage.h"
#include "settingspages/networkssettingspage.h"
#include "settingspages/notificationssettingspage.h"
#include "settingspages/topicwidgetsettingspage.h"

#ifndef HAVE_KDE
#  include "settingspages/shortcutssettingspage.h"
#endif

#include <QGraphicsProxyWidget>
#include <QGraphicsLinearLayout>
#include <QDeclarativeView>
#include <QDeclarativeComponent>
#include <QDeclarativeEngine>
#include <QDeclarativeContext>
#include <QDeclarativeItem>
#include <QGLWidget>
#include "qmlchatview.h"
#include "qmlcontextobject.h"
#include "qmlinputwidget.h"
#include "qmlnickwidget.h"
#include "qmlthemeimageprovider.h"
#include "qmlscrollmodel.h"

MainWin::MainWin(QWidget *parent)
#ifdef HAVE_KDE
  : KMainWindow(parent),
    _kHelpMenu(new KHelpMenu(this, KGlobal::mainComponent().aboutData())),
    #else
  : QMainWindow(parent),
    #endif
    _msgProcessorStatusModel(new MsgProcessorStatusModel(this)),
    _coreConnectionStatusModel(new CoreConnectionStatusModel(Client::coreConnection(), this)),
    _titleSetter(this),
    _declarativeView(0),
    _qmlContextObject(0),
    _bufferWidget(0),
    _nickListWidget(0),
    _chatMonitorView(0),
    _topicModel(0),
    _awayLog(0),
    _authDlg(new CoreConnectAuthDlg(0, this)),
    _layoutLoaded(false),
    _activeBufferViewIndex(-1)
{
  setAttribute(Qt::WA_DeleteOnClose, false);  // we delete the mainwin manually

#ifdef Q_WS_MAEMO_5
  setAttribute(Qt::WA_Maemo5StackedWindow);
#endif

  QtUiSettings uiSettings;
  QString style = uiSettings.value("Style", QString()).toString();
  if(!style.isEmpty()) {
    QApplication::setStyle(style);
  }

  QApplication::setQuitOnLastWindowClosed(false);

  setWindowTitle("Quassel IRC");
  setWindowIconText("Quassel IRC");
  updateIcon();

  connect(_authDlg, SIGNAL(accepted()), this, SLOT(userAuthenticationAccepted()));
}

void MainWin::init() {
  connect(Client::instance(), SIGNAL(networkCreated(NetworkId)), SLOT(clientNetworkCreated(NetworkId)));
  connect(Client::instance(), SIGNAL(networkRemoved(NetworkId)), SLOT(clientNetworkRemoved(NetworkId)));
  connect(Client::messageModel(), SIGNAL(rowsInserted(const QModelIndex &, int, int)),
          SLOT(messagesInserted(const QModelIndex &, int, int)));
  connect(GraphicalUi::contextMenuActionProvider(), SIGNAL(showChannelList(NetworkId)), SLOT(showChannelList(NetworkId)));
  connect(GraphicalUi::contextMenuActionProvider(), SIGNAL(showIgnoreList(QString)), SLOT(showIgnoreList(QString)));

  connect(Client::coreConnection(), SIGNAL(startCoreSetup(QVariantList)), SLOT(showCoreConfigWizard(QVariantList)));
  connect(Client::coreConnection(), SIGNAL(connectionErrorPopup(QString)), SLOT(handleCoreConnectionError(QString)));
  connect(Client::coreConnection(), SIGNAL(userAuthenticationRequired(CoreAccount *, bool *, QString)), SLOT(userAuthenticationRequired(CoreAccount *, bool *, QString)));
  connect(Client::coreConnection(), SIGNAL(handleNoSslInClient(bool*)), SLOT(handleNoSslInClient(bool *)));
  connect(Client::coreConnection(), SIGNAL(handleNoSslInCore(bool*)), SLOT(handleNoSslInCore(bool *)));
#ifdef HAVE_SSL
  connect(Client::coreConnection(), SIGNAL(handleSslErrors(const QSslSocket *, bool *, bool *)), SLOT(handleSslErrors(const QSslSocket *, bool *, bool *)));
#endif

  statusBar()->hide();

  // Order is sometimes important
  setupActions();
  setupBufferWidget();
  setupMenus();
  setupTopicWidget();
  setupNickWidget();
  setupInputWidget();
  setupChatMonitor();
  setupViewMenuTail();
  setupSystray();
  setupTitleSetter();
  setupHotList();

  // try setup qml... (needs _bufferWidget already initialized)
  QmlChatView::setBufferWidget(_bufferWidget);
  qmlRegisterType<ChatView>();
  qmlRegisterType<BufferWidget>();
  qmlRegisterType<TopicModel>();
  qmlRegisterType<QAbstractItemModel>();
  qmlRegisterType<QmlScrollModel>();
  qmlRegisterType<QmlChatView>("org.quassel", 0, 1, "QuasselChatView");
  qmlRegisterType<QmlInputWidget>("org.quassel", 0, 1, "QuasselInputWidget");
  qmlRegisterType<QmlNickWidget>("org.quassel", 0, 1, "QuasselNickWidget");
  _declarativeView = new QDeclarativeView(this);
  QmlThemeImageProvider *imgProvider = new QmlThemeImageProvider(this);
  _declarativeView->engine()->addImageProvider(QLatin1String("quassel"), imgProvider);
#ifdef Q_WS_MAEMO_5
  // default settings are fine
#else
  // desktop version appears to perform better this way (even though not as good as qmlviewerq??):
  _declarativeView->setViewport(new QGLWidget());
#endif
  _declarativeView->setResizeMode(QDeclarativeView::SizeRootObjectToView);
  _qmlContextObject = new QmlContextObject(_declarativeView);
  _qmlContextObject->setBufferWidget(_bufferWidget);
  _declarativeView->rootContext()->setContextObject(_qmlContextObject);
  _declarativeView->rootContext()->setContextProperty("ctxt", _qmlContextObject);
  _declarativeView->rootContext()->setContextProperty("coreConnection", Client::coreConnection());
  _declarativeView->rootContext()->setContextProperty("coreConnectionStatus", _coreConnectionStatusModel);
  _declarativeView->rootContext()->setContextProperty("backlogManager", Client::backlogManager());
  _declarativeView->rootContext()->setContextProperty("backlogManagerStatus", _msgProcessorStatusModel);
  _declarativeView->rootContext()->setContextProperty("topicModel", _topicModel);
  _declarativeView->setSource(QUrl("qrc:/qml/Main.qml"));
  setCentralWidget(_declarativeView);

  connect(_qmlContextObject, SIGNAL(fullScreenChanged(bool)), this, SLOT(setFullscreen(bool)));
//  connect(_nickListWidget, SIGNAL(currentNickModelChanged(QAbstractItemModel*)),
//          _qmlContextObject, SLOT(setChannelUsersModel(QAbstractItemModel*)));
//  connect(_nickListWidget, SIGNAL(currentNickRootIndexChanged(QModelIndex)),
//          _qmlContextObject, SLOT(setChannelUsersRootIndex(QModelIndex)));

  connect(_qmlContextObject, SIGNAL(requestZoomIn()),
          _bufferWidget, SLOT(zoomIn()));
  connect(_qmlContextObject, SIGNAL(requestZoomOut()),
          _bufferWidget, SLOT(zoomOut()));

  connect(_qmlContextObject, SIGNAL(requestSearch()), QtUi::actionCollection("General")->action("ToggleSearchBar"), SLOT(trigger()));


  //#ifndef HAVE_KDE
  //#  ifdef HAVE_PHONON
  //  QtUi::registerNotificationBackend(new PhononNotificationBackend(this));
  //#  endif
  //#  ifndef QT_NO_SYSTEMTRAYICON
  //  QtUi::registerNotificationBackend(new SystrayNotificationBackend(this));
  //#  endif

  //  // mobile quassel doesn't have taskbar for notifications
  //  //QtUi::registerNotificationBackend(new TaskbarNotificationBackend(this));

#if defined(Q_WS_MAEMO_5) || defined(Q_WS_HILDON)
  QtUi::registerNotificationBackend(new MaemoNotificationBackend(this));
#endif

  //#else /* HAVE_KDE */
  //  QtUi::registerNotificationBackend(new KNotificationBackend(this));
  //#endif /* HAVE_KDE */

  //#ifdef HAVE_INDICATEQT
  //  QtUi::registerNotificationBackend(new IndicatorNotificationBackend(this));
  //#endif

  // we assume that at this point, all configurable actions are defined!
  QtUi::loadShortcuts();

  connect(bufferWidget(), SIGNAL(currentChanged(BufferId)), SLOT(currentBufferChanged(BufferId)));

  setDisconnectedState();  // Disable menus and stuff

#ifdef HAVE_KDE
  setAutoSaveSettings();
#endif

  // restore mainwin state
  QtUiSettings s;
  restoreStateFromSettings(s);

  CoreConnection *conn = Client::coreConnection();
  if(!conn->connectToCore()) {
    // No autoconnect selected (or no accounts)
    showCoreConnectionDlg();
  }
}

MainWin::~MainWin() {
}

void MainWin::quit() {
  QtUiSettings s;
  saveStateToSettings(s);
  saveLayout();
  QApplication::quit();
}

void MainWin::saveStateToSettings(UiSettings &s) {
  s.setValue("MainWinSize", _normalSize);
  s.setValue("MainWinPos", _normalPos);
  s.setValue("MainWinState", saveState());
  s.setValue("MainWinGeometry", saveGeometry());
  s.setValue("MainWinMinimized", isMinimized());
  s.setValue("MainWinMaximized", isMaximized());
  s.setValue("MainWinHidden", !isVisible());
  BufferId lastBufId = Client::bufferModel()->currentBuffer();
  if(lastBufId.isValid())
    s.setValue("LastUsedBufferId", lastBufId.toInt());

#ifdef HAVE_KDE
  saveAutoSaveSettings();
#endif
}

void MainWin::restoreStateFromSettings(UiSettings &s) {
  _normalSize = s.value("MainWinSize", size()).toSize();
  _normalPos = s.value("MainWinPos", pos()).toPoint();
  bool maximized = s.value("MainWinMaximized", false).toBool();

#ifndef HAVE_KDE
  restoreGeometry(s.value("MainWinGeometry").toByteArray());

  if(maximized) {
    // restoreGeometry() fails if the windows was maximized, so we resize and position explicitly
    resize(_normalSize);
    move(_normalPos);
  }

  // we don't allow mainwin layout customization for now
  // restoreState(s.value("MainWinState").toByteArray());

#else
  move(_normalPos);
#endif

  if(s.value("MainWinHidden").toBool() && QtUi::haveSystemTray())
    QtUi::hideMainWidget();
  else if(s.value("MainWinMinimized").toBool())
    showMinimized();
  else if(maximized)
    showMaximized();
  else
    show();
}

void MainWin::updateIcon() {
#ifdef Q_WS_MAC
  const int size = 128;
#else
  const int size = 48;
#endif

  QPixmap icon;
  if(Client::isConnected())
    icon = DesktopIcon("quassel", size);
  else
    icon = DesktopIcon("quassel_inactive", size);
  setWindowIcon(icon);
  qApp->setWindowIcon(icon);
}

void MainWin::setupActions() {
  ActionCollection *coll = QtUi::actionCollection("General", tr("General"));
  // File

  QAction *act = new MobileAction(SmallIcon("network-connect"), tr("&Connect Core"),
                                  coll, this, SLOT(showCoreConnectionDlg()));
  coll->addAction("ConnectCore", act);

  act = new MobileAction(SmallIcon("network-disconnect"), tr("&Disconnect Core"), coll,
                         Client::instance(), SLOT(disconnectFromCore()));
  coll->addAction("DisconnectCore", act);

  act = new MobileAction(SmallIcon("help-about"), tr("Core &Info"), coll,
                         this, SLOT(showCoreInfoDlg()));
  coll->addAction("CoreInfo", act);

  act = new MobileAction(SmallIcon("configure"), tr("Configure &Networks"), coll,
                         this, SLOT(on_actionConfigureNetworks_triggered()));
  coll->addAction("ConfigureNetworks", act);

  // View
  act = new MobileAction(SmallIcon("configure"), tr("&Configure Chat Lists"), coll,
                         this, SLOT(on_actionConfigureViews_triggered()));
  coll->addAction("ConfigureBufferViews", act);

  coll->addAction("ToggleSearchBar", new MobileAction(SmallIcon("edit-find"), tr("Show &Search Bar"), coll,
                                                      0, 0, QKeySequence::Find))->setCheckable(true);

  coll->addAction("ShowAwayLog", new MobileAction(tr("Show Away Log"), coll,
                                                  this, SLOT(showAwayLog())));

  coll->addAction("ToggleChatMonitor", new MobileAction(tr("Show Chat Monitor"), coll,
                                                        0, 0))->setCheckable(true);

#ifdef HAVE_KDE
  QAction *fullScreenAct = KStandardAction::fullScreen(this, SLOT(toggleFullscreen()), this, coll);
#else
  QAction *fullScreenAct = new MobileAction(SmallIcon("view-fullscreen"), tr("&Full Screen Mode"), coll,
                                            this, SLOT(toggleFullscreen()), QKeySequence(Qt::Key_F11));
  fullScreenAct->setCheckable(true);
#endif
  coll->addAction("ToggleFullscreen", fullScreenAct);

  // Settings
#ifdef Q_WS_MAC
  QAction *configureQuasselAct = new MobileAction(SmallIcon("configure"), tr("&Configure Quassel"), coll,
                                                  this, SLOT(showSettingsDlg()));
  configureQuasselAct->setMenuRole(QAction::PreferencesRole);
#else
  QAction *configureQuasselAct = new MobileAction(SmallIcon("configure"), tr("&Configure Quassel"), coll,
                                                  this, SLOT(showSettingsDlg()), QKeySequence(Qt::Key_F7));
#endif
  coll->addAction("ConfigureQuassel", configureQuasselAct);

  // Help
  QAction *aboutAct = new MobileAction(SmallIcon("quassel"), tr("&About"), coll,
                                       this, SLOT(showAboutDlg()));
  aboutAct->setMenuRole(QAction::AboutRole);
  coll->addAction("About", aboutAct);


  coll->addAction("DebugNetworkModel", new MobileAction(SmallIcon("tools-report-bug"), tr("Debug &NetworkModel"), coll,
                                                        this, SLOT(on_actionDebugNetworkModel_triggered())));
  coll->addAction("DebugBufferViewOverlay", new MobileAction(SmallIcon("tools-report-bug"), tr("Debug &BufferViewOverlay"), coll,
                                                             this, SLOT(on_actionDebugBufferViewOverlay_triggered())));
  coll->addAction("DebugMessageModel", new MobileAction(SmallIcon("tools-report-bug"), tr("Debug &MessageModel"), coll,
                                                        this, SLOT(on_actionDebugMessageModel_triggered())));
  coll->addAction("DebugHotList", new MobileAction(SmallIcon("tools-report-bug"), tr("Debug &HotList"), coll,
                                                   this, SLOT(on_actionDebugHotList_triggered())));
  coll->addAction("DebugLog", new MobileAction(SmallIcon("tools-report-bug"), tr("Debug &Log"), coll,
                                               this, SLOT(on_actionDebugLog_triggered())));
  coll->addAction("ReloadStyle", new MobileAction(SmallIcon("view-refresh"), tr("Reload Stylesheet"), coll,
                                                  QtUi::style(), SLOT(reload()), QKeySequence::Refresh));

  // Navigation
  coll = QtUi::actionCollection("Navigation", tr("Navigation"));

  coll->addAction("JumpHotBuffer", new MobileAction(tr("Jump to hot chat"), coll,
                                                    this, SLOT(on_jumpHotBuffer_triggered()), QKeySequence(Qt::META + Qt::Key_A)));

  // Jump keys
#ifdef Q_WS_MAC
  const int bindModifier = Qt::ControlModifier | Qt::AltModifier;
  const int jumpModifier = Qt::ControlModifier;
#else
  const int bindModifier = Qt::ControlModifier;
  const int jumpModifier = Qt::AltModifier;
#endif

  coll->addAction("BindJumpKey0", new MobileAction(tr("Set Quick Access #0"), coll, this, SLOT(bindJumpKey()),
                                                   QKeySequence(bindModifier + Qt::Key_0)))->setProperty("Index", 0);
  coll->addAction("BindJumpKey1", new MobileAction(tr("Set Quick Access #1"), coll, this, SLOT(bindJumpKey()),
                                                   QKeySequence(bindModifier + Qt::Key_1)))->setProperty("Index", 1);
  coll->addAction("BindJumpKey2", new MobileAction(tr("Set Quick Access #2"), coll, this, SLOT(bindJumpKey()),
                                                   QKeySequence(bindModifier + Qt::Key_2)))->setProperty("Index", 2);
  coll->addAction("BindJumpKey3", new MobileAction(tr("Set Quick Access #3"), coll, this, SLOT(bindJumpKey()),
                                                   QKeySequence(bindModifier + Qt::Key_3)))->setProperty("Index", 3);
  coll->addAction("BindJumpKey4", new MobileAction(tr("Set Quick Access #4"), coll, this, SLOT(bindJumpKey()),
                                                   QKeySequence(bindModifier + Qt::Key_4)))->setProperty("Index", 4);
  coll->addAction("BindJumpKey5", new MobileAction(tr("Set Quick Access #5"), coll, this, SLOT(bindJumpKey()),
                                                   QKeySequence(bindModifier + Qt::Key_5)))->setProperty("Index", 5);
  coll->addAction("BindJumpKey6", new MobileAction(tr("Set Quick Access #6"), coll, this, SLOT(bindJumpKey()),
                                                   QKeySequence(bindModifier + Qt::Key_6)))->setProperty("Index", 6);
  coll->addAction("BindJumpKey7", new MobileAction(tr("Set Quick Access #7"), coll, this, SLOT(bindJumpKey()),
                                                   QKeySequence(bindModifier + Qt::Key_7)))->setProperty("Index", 7);
  coll->addAction("BindJumpKey8", new MobileAction(tr("Set Quick Access #8"), coll, this, SLOT(bindJumpKey()),
                                                   QKeySequence(bindModifier + Qt::Key_8)))->setProperty("Index", 8);
  coll->addAction("BindJumpKey9", new MobileAction(tr("Set Quick Access #9"), coll, this, SLOT(bindJumpKey()),
                                                   QKeySequence(bindModifier + Qt::Key_9)))->setProperty("Index", 9);

  coll->addAction("JumpKey0", new MobileAction(tr("Quick Access #0"), coll, this, SLOT(onJumpKey()),
                                               QKeySequence(jumpModifier + Qt::Key_0)))->setProperty("Index", 0);
  coll->addAction("JumpKey1", new MobileAction(tr("Quick Access #1"), coll, this, SLOT(onJumpKey()),
                                               QKeySequence(jumpModifier + Qt::Key_1)))->setProperty("Index", 1);
  coll->addAction("JumpKey2", new MobileAction(tr("Quick Access #2"), coll, this, SLOT(onJumpKey()),
                                               QKeySequence(jumpModifier + Qt::Key_2)))->setProperty("Index", 2);
  coll->addAction("JumpKey3", new MobileAction(tr("Quick Access #3"), coll, this, SLOT(onJumpKey()),
                                               QKeySequence(jumpModifier + Qt::Key_3)))->setProperty("Index", 3);
  coll->addAction("JumpKey4", new MobileAction(tr("Quick Access #4"), coll, this, SLOT(onJumpKey()),
                                               QKeySequence(jumpModifier + Qt::Key_4)))->setProperty("Index", 4);
  coll->addAction("JumpKey5", new MobileAction(tr("Quick Access #5"), coll, this, SLOT(onJumpKey()),
                                               QKeySequence(jumpModifier + Qt::Key_5)))->setProperty("Index", 5);
  coll->addAction("JumpKey6", new MobileAction(tr("Quick Access #6"), coll, this, SLOT(onJumpKey()),
                                               QKeySequence(jumpModifier + Qt::Key_6)))->setProperty("Index", 6);
  coll->addAction("JumpKey7", new MobileAction(tr("Quick Access #7"), coll, this, SLOT(onJumpKey()),
                                               QKeySequence(jumpModifier + Qt::Key_7)))->setProperty("Index", 7);
  coll->addAction("JumpKey8", new MobileAction(tr("Quick Access #8"), coll, this, SLOT(onJumpKey()),
                                               QKeySequence(jumpModifier + Qt::Key_8)))->setProperty("Index", 8);
  coll->addAction("JumpKey9", new MobileAction(tr("Quick Access #9"), coll, this, SLOT(onJumpKey()),
                                               QKeySequence(jumpModifier + Qt::Key_9)))->setProperty("Index", 9);

  // Buffer navigation
  coll->addAction("NextBufferView", new MobileAction(SmallIcon("go-next-view"), tr("Activate Next Chat List"), coll,
                                                     this, SLOT(nextBufferView()), QKeySequence(QKeySequence::Forward)));
  coll->addAction("PreviousBufferView", new MobileAction(SmallIcon("go-previous-view"), tr("Activate Previous Chat List"), coll,
                                                         this, SLOT(previousBufferView()), QKeySequence::Back));
  coll->addAction("NextBuffer", new MobileAction(SmallIcon("go-down"), tr("Go to Next Chat"), coll,
                                                 this, SLOT(nextBuffer()), QKeySequence(Qt::ALT + Qt::Key_Down)));
  coll->addAction("PreviousBuffer", new MobileAction(SmallIcon("go-up"), tr("Go to Previous Chat"), coll,
                                                     this, SLOT(previousBuffer()), QKeySequence(Qt::ALT + Qt::Key_Up)));
}

void MainWin::setupMenus() {
  ActionCollection *coll = QtUi::actionCollection("General");

  menuBar()->addAction(coll->action("ConnectCore"));
  menuBar()->addAction(coll->action("DisconnectCore"));
  // menuBar()->addAction(coll->action("CoreInfo"));

  //menuBar()->addAction(coll->action("ConfigureNetworks"));

  //menuBar()->addAction(coll->action("ConfigureBufferViews"));

  menuBar()->addAction(coll->action("ToggleChatMonitor"));
  menuBar()->addAction(coll->action("ToggleSearchBar"));

  menuBar()->addAction(coll->action("ShowAwayLog"));

  menuBar()->addAction(coll->action("ConfigureQuassel"));

  menuBar()->addAction(coll->action("About"));

  // TODO: maybe add to about dialogue...
  // _helpDebugMenu = _helpMenu->addMenu(SmallIcon("tools-report-bug"), tr("Debug"));
  // _helpDebugMenu->addAction(coll->action("DebugNetworkModel"));
  // _helpDebugMenu->addAction(coll->action("DebugBufferViewOverlay"));
  // _helpDebugMenu->addAction(coll->action("DebugMessageModel"));
  // _helpDebugMenu->addAction(coll->action("DebugHotList"));
  // _helpDebugMenu->addAction(coll->action("DebugLog"));
  // _helpDebugMenu->addSeparator();
  // _helpDebugMenu->addAction(coll->action("ReloadStyle"));

}

void MainWin::setupBufferWidget() {
  _bufferWidget = new BufferWidget(0);
  _bufferWidget->setModel(Client::bufferModel());
  _bufferWidget->setSelectionModel(Client::bufferModel()->standardSelectionModel());
  //_bufferWidget->show();
  //setCentralWidget(_bufferWidget);
}

void MainWin::addBufferView(int bufferViewConfigId) {
  addBufferView(Client::bufferViewManager()->clientBufferViewConfig(bufferViewConfigId));
}

void MainWin::addBufferView(ClientBufferViewConfig *config) {
  if(!config)
    return;

  config->setLocked(true);

  //create the view and initialize it's filter
  MobileBufferViewWindow *view = new MobileBufferViewWindow(this);

  view->setWindowFlags(view->windowFlags() | Qt::Window);
#if defined(Q_WS_MAEMO_5)
  view->setAttribute(Qt::WA_Maemo5StackedWindow);
#endif

  view->view()->setFilteredModel(Client::bufferModel(), config);
  // view->installEventFilter(_inputWidget); // for key presses

  menuBar()->addAction(view->toggleVisibleAction() );
  connect(view, SIGNAL(requestActivation(int)), this, SLOT(setActiveBufferView(int)));

  Client::bufferModel()->synchronizeView(view->view());

  view->setVisible(_layoutLoaded);

  _bufferViews.append(view);

  if(!activeBufferView())
    nextBufferView();
}

void MainWin::removeBufferView(int bufferViewConfigId) {
  QVariant actionData;
  MobileBufferViewWindow *view;
  foreach(QAction *action, menuBar()->actions()) {
    actionData = action->data();
    if(!actionData.isValid())
      continue;

    view = qobject_cast<MobileBufferViewWindow *>(action->parent());
    if(view && actionData.toInt() == bufferViewConfigId) {
      removeAction(action);
      Client::bufferViewOverlay()->removeView(view->view()->config()->bufferViewId() );
      _bufferViews.removeAll(view);

      if(view->isActive()) {
        setActiveBufferView(-1);
        nextBufferView();
      }

      view->deleteLater();
    }
  }
}

void MainWin::bufferViewToggled(bool enabled) {
  if(!enabled && !isVisible()) {
    // hiding the mainwindow triggers a toggle of the bufferview (which pretty much sucks big time)
    // since this isn't our fault and we can't do anything about it, we suppress the resulting calls
    return;
  }
  QAction *action = qobject_cast<QAction *>(sender());
  Q_ASSERT(action);
  MobileBufferViewWindow *view = qobject_cast<MobileBufferViewWindow *>(action->parent());
  Q_ASSERT(view);

  // Make sure we don't toggle backlog fetch for a view we've already removed
  if(!_bufferViews.contains(view))
    return;

  if(enabled)
    Client::bufferViewOverlay()->addView(view->view()->config()->bufferViewId());
  else
    Client::bufferViewOverlay()->removeView(view->view()->config()->bufferViewId());
}

MobileBufferViewWindow *MainWin::allBuffersView() const {
  // "All Buffers" is always the first buffer created
  if(_bufferViews.count() > 0)
    return _bufferViews[0];
  return 0;
}

MobileBufferViewWindow *MainWin::activeBufferView() const {
  if(_activeBufferViewIndex < 0 || _activeBufferViewIndex >= _bufferViews.count())
    return 0;
  MobileBufferViewWindow *view = _bufferViews.at(_activeBufferViewIndex);
  return view->isActive() ? view : 0;
}

bool MainWin::setActiveBufferView(int bufferViewId)
{
  MobileBufferViewWindow *current = activeBufferView();
  if(current) {
    current->setActive(false);
    _activeBufferViewIndex = -1;

    // qml disconnect
    _qmlContextObject->setActiveBufferListModel(0);
    _qmlContextObject->setActiveBufferListSelectionModel(0);
//    disconnect(current->view()->selectionModel(), 0, _qmlContextObject, 0);
//    disconnect(_qmlContextObject, 0, current->view()->selectionModel(), 0);
  }

  if(bufferViewId < 0)
    return false;

  for(int i = 0; i < _bufferViews.count(); i++) {
    MobileBufferViewWindow *view = _bufferViews.at(i);
    if(view->view()->config()->bufferViewId() == bufferViewId/* && !view->isHidden()*/) {
      _activeBufferViewIndex = i;
      view->setActive(true);

      // qml connect
      _qmlContextObject->setActiveBufferListModel(view->view()->model());
      _qmlContextObject->setActiveBufferListSelectionModel(view->view()->selectionModel());
//      connect(view->view()->selectionModel(), SIGNAL(currentChanged(QModelIndex,QModelIndex)),
//              _qmlContextObject, SLOT(setCurrentBufferModelIndex(QModelIndex)));
//      connect(_qmlContextObject, SIGNAL(currentBufferModelIndexChanged(QModelIndex, QItemSelectionModel::SelectionFlags)),
//              view->view()->selectionModel(), SLOT(setCurrentIndex(QModelIndex,QItemSelectionModel::SelectionFlags)));

      return true;
    }
  }

  return false;
}

void MainWin::changeActiveBufferView(int bufferViewId) {
  if(bufferViewId < 0)
    return;

  if(!setActiveBufferView(bufferViewId)) {
    nextBufferView(); // fallback
  }
}

void MainWin::changeActiveBufferView(bool backwards) {
  int curIndex = _activeBufferViewIndex;

  setActiveBufferView(-1);

  if(!_bufferViews.count())
    return;

  int c = _bufferViews.count();
  while(c--) { // yes, this will reactivate the current active one if all others fail
    if(backwards) {
      if(--curIndex < 0)
        curIndex = _bufferViews.count()-1;
    } else {
      if(++curIndex >= _bufferViews.count())
        curIndex = 0;
    }

    if(setActiveBufferView(curIndex))
      return;
  }
}

void MainWin::nextBufferView() {
  changeActiveBufferView(false);
}

void MainWin::previousBufferView() {
  changeActiveBufferView(true);
}

void MainWin::nextBuffer() {
  MobileBufferViewWindow *view = activeBufferView();
  if(view)
    view->view()->nextBuffer();
}

void MainWin::previousBuffer() {
  MobileBufferViewWindow *view = activeBufferView();
  if(view)
    view->view()->previousBuffer();
}

void MainWin::showNotificationsDlg() {
  SettingsPageDlg dlg(new NotificationsSettingsPage(this), this);
  dlg.exec();
}

void MainWin::on_actionConfigureNetworks_triggered() {
  SettingsPageDlg dlg(new NetworksSettingsPage(this), this);
  dlg.exec();
}

void MainWin::on_actionConfigureViews_triggered() {
  SettingsPageDlg dlg(new BufferViewSettingsPage(this), this);
  dlg.exec();
}

void MainWin::setupNickWidget() {
  _nickListWidget = new NickListWidget(this);
  _nickListWidget->setWindowTitle(tr("Nicks"));
  _nickListWidget->setWindowFlags(_nickListWidget->windowFlags() | Qt::Window);
#if defined(Q_WS_MAEMO_5)
  _nickListWidget->setAttribute(Qt::WA_Maemo5StackedWindow);
#endif

  QAction *act = new QAction(tr("Nicks"), this);
  connect(act, SIGNAL(triggered()), _nickListWidget, SLOT(show()));

  act->setObjectName("ShowNickList");
  act->setCheckable(true);
  menuBar()->addAction(act);

  // attach the NickListWidget to the BufferModel and the default selection
  _nickListWidget->setModel(Client::bufferModel());
  _nickListWidget->setSelectionModel(Client::bufferModel()->standardSelectionModel());
}

void MainWin::setupChatMonitor() {
  ChatMonitorFilter *filter = new ChatMonitorFilter(Client::messageModel(), this);
  _chatMonitorView = new ChatMonitorView(filter, this);
  _chatMonitorView->setWindowTitle(tr("Chat Monitor"));
  _chatMonitorView->setWindowFlags(_chatMonitorView->windowFlags() | Qt::Window);
#ifdef Q_WS_MAEMO_5
  _chatMonitorView->setAttribute(Qt::WA_Maemo5StackedWindow);
#endif
  // _chatMonitorView->setFocusProxy(_inputWidget);
  _chatMonitorView->hide();

  QAction *action = QtUi::actionCollection("General")->action("ToggleChatMonitor");
  connect(action, SIGNAL(toggled(bool)), _chatMonitorView, SLOT(setVisible(bool)));
}

void MainWin::setupInputWidget() {
//  // TODO: remove inputwidget
//  _inputWidget = new InputWidget(0);
//  _inputWidget->show();

//  _inputWidget->setModel(Client::bufferModel());
//  _inputWidget->setSelectionModel(Client::bufferModel()->standardSelectionModel());

//  _bufferWidget->setFocusProxy(_inputWidget);

//  _inputWidget->inputLine()->installEventFilter(_bufferWidget);
}

void MainWin::setupTopicWidget() {
  _topicModel = new TopicModel(this);
  _topicModel->setModel(Client::bufferModel());
  _topicModel->setSelectionModel(Client::bufferModel()->standardSelectionModel());

  // TODO: check wether to port TopicWidget settings to QML. then, get rid of
  //       TopicWidget class
}

void MainWin::setupViewMenuTail() {
  // TODO enable full screen? needs exit btn
  //  menuBar()->addAction(QtUi::actionCollection("General")->action("ToggleFullscreen"));
}

void MainWin::setupTitleSetter() {
  _titleSetter.setModel(Client::bufferModel());
  _titleSetter.setSelectionModel(Client::bufferModel()->standardSelectionModel());
}

void MainWin::setupHotList() {
  FlatProxyModel *flatProxy = new FlatProxyModel(this);
  flatProxy->setSourceModel(Client::bufferModel());
  _bufferHotList = new BufferHotListFilter(flatProxy);
}

void MainWin::setupSystray() {
#ifdef HAVE_DBUS
  _systemTray = new StatusNotifierItem(this);
#elif !defined QT_NO_SYSTEMTRAYICON
  _systemTray = new LegacySystemTray(this);
#else
  _systemTray = new SystemTray(this); // dummy
#endif
  _systemTray->init();
}

void MainWin::connectedToCore() {
  Q_CHECK_PTR(Client::bufferViewManager());
  connect(Client::bufferViewManager(), SIGNAL(bufferViewConfigAdded(int)), this, SLOT(addBufferView(int)));
  connect(Client::bufferViewManager(), SIGNAL(bufferViewConfigDeleted(int)), this, SLOT(removeBufferView(int)));
  connect(Client::bufferViewManager(), SIGNAL(initDone()), this, SLOT(loadLayout()));

  setConnectedState();
}

void MainWin::setConnectedState() {
  ActionCollection *coll = QtUi::actionCollection("General");

  coll->action("ConnectCore")->setEnabled(false);
  coll->action("DisconnectCore")->setEnabled(true);
  coll->action("CoreInfo")->setEnabled(true);

  foreach(QAction *action, menuBar()->actions()) {
    if(isRemoteCoreOnly(action))
      action->setVisible(!Client::internalCore());
  }

  disconnect(Client::backlogManager(), SIGNAL(updateProgress(int, int)), _msgProcessorStatusModel, SLOT(setProgress(int, int)));
  disconnect(Client::backlogManager(), SIGNAL(messagesRequested(const QString &)), this, SLOT(showStatusBarMessage(const QString &)));
  disconnect(Client::backlogManager(), SIGNAL(messagesProcessed(const QString &)), this, SLOT(showStatusBarMessage(const QString &)));
  if(!Client::internalCore()) {
    connect(Client::backlogManager(), SIGNAL(updateProgress(int, int)), _msgProcessorStatusModel, SLOT(setProgress(int, int)));
    connect(Client::backlogManager(), SIGNAL(messagesRequested(const QString &)), this, SLOT(showStatusBarMessage(const QString &)));
    connect(Client::backlogManager(), SIGNAL(messagesProcessed(const QString &)), this, SLOT(showStatusBarMessage(const QString &)));
  }

  // TODO show msg in qml or maemo info banner
  // _viewMenu->setEnabled(true);
  if(!Client::internalCore())
    statusBar()->showMessage(tr("Connected to core."));
  else
    statusBar()->clearMessage();

  // _coreConnectionStatusModel->setVisible(!Client::internalCore());
  updateIcon();
  systemTray()->setState(SystemTray::Active);

  if(Client::networkIds().isEmpty()) {
    IrcConnectionWizard *wizard = new IrcConnectionWizard(this, Qt::Sheet);
    wizard->show();
  }
  else {
    // Monolithic always preselects last used buffer - Client only if the connection died
    if(Client::coreConnection()->wasReconnect() || Quassel::runMode() == Quassel::Monolithic) {
      QtUiSettings s;
      BufferId lastUsedBufferId(s.value("LastUsedBufferId").toInt());
      if(lastUsedBufferId.isValid())
        Client::bufferModel()->switchToBuffer(lastUsedBufferId);
    }
  }
}

void MainWin::loadLayout() {
  QtUiSettings s;
  int accountId = Client::currentCoreAccount().accountId().toInt();
  QByteArray state = s.value(QString("MainWinState-%1").arg(accountId)).toByteArray();
  if(state.isEmpty()) {
    // foreach(BufferView *view, _bufferViews)
    //   view->show();
    _layoutLoaded = true;
    return;
  }

  // we don't customize mainwindow layout for now.
  // restoreState(state, accountId);

  int bufferViewId = s.value(QString("ActiveBufferView-%1").arg(accountId), -1).toInt();
  if(bufferViewId >= 0)
    changeActiveBufferView(bufferViewId);

  _layoutLoaded = true;
}

void MainWin::saveLayout() {
  QtUiSettings s;
  int accountId = _bufferViews.count()? Client::currentCoreAccount().accountId().toInt() : 0; // only save if we still have a layout!
  if(accountId > 0) {
    s.setValue(QString("MainWinState-%1").arg(accountId) , saveState(accountId));
    MobileBufferViewWindow *view = activeBufferView();
    s.setValue(QString("ActiveBufferView-%1").arg(accountId), view ? view->view()->config()->bufferViewId() : -1);
  }
}

void MainWin::disconnectedFromCore() {
  // save core specific layout and remove bufferviews;
  saveLayout();
  _layoutLoaded = false;

  QVariant actionData;
  MobileBufferViewWindow *view;
  foreach(view, _bufferViews) {
    view->deleteLater();
  }
  _bufferViews.clear();

  // store last active buffer
  QtUiSettings s;
  BufferId lastBufId = _bufferWidget->currentBuffer();
  if(lastBufId.isValid()) {
    s.setValue("LastUsedBufferId", lastBufId.toInt());
    // clear the current selection
    Client::bufferModel()->standardSelectionModel()->clearSelection();
  }

  setDisconnectedState();
}

void MainWin::setDisconnectedState() {
  ActionCollection *coll = QtUi::actionCollection("General");
  //ui.menuCore->setEnabled(false);
  coll->action("ConnectCore")->setEnabled(true);
  coll->action("DisconnectCore")->setEnabled(false);
  coll->action("CoreInfo")->setEnabled(false);
  //_viewMenu->setEnabled(false);
  // TODO: convert statusbar messages to UI-independent notification
  statusBar()->showMessage(tr("Not connected to core."));
  if(_msgProcessorStatusModel)
    _msgProcessorStatusModel->setProgress(0, 0);
  updateIcon();
  systemTray()->setState(SystemTray::Passive);
}

void MainWin::userAuthenticationRequired(CoreAccount *account, bool *valid, const QString &errorMessage) {
  Q_UNUSED(errorMessage)

  // this is required to be asynchronous to prevent a deadlock
  // on n900/hildon, when the dialog opens
  // while the app-menu is open

  // *valid = (dlg.exec() == QDialog::Accepted);

  _authDlg->setAccount(account);
  _authDlg->show();

  *valid = false;
}

void MainWin::userAuthenticationAccepted()
{
  Client::coreConnection()->connectToCoreWithAccount(Client::coreConnection()->currentAccount());
}

void MainWin::handleNoSslInClient(bool *accepted) {
  QMessageBox box(QMessageBox::Warning, tr("Unencrypted Connection"), tr("<b>Your client does not support SSL encryption</b>"),
                  QMessageBox::Ignore|QMessageBox::Cancel, this);
  box.setInformativeText(tr("Sensitive data, like passwords, will be transmitted unencrypted to your Quassel core."));
  box.setDefaultButton(QMessageBox::Ignore);
  *accepted = box.exec() == QMessageBox::Ignore;
}

void MainWin::handleNoSslInCore(bool *accepted) {
  QMessageBox box(QMessageBox::Warning, tr("Unencrypted Connection"), tr("<b>Your core does not support SSL encryption</b>"),
                  QMessageBox::Ignore|QMessageBox::Cancel, this);
  box.setInformativeText(tr("Sensitive data, like passwords, will be transmitted unencrypted to your Quassel core."));
  box.setDefaultButton(QMessageBox::Ignore);
  *accepted = box.exec() == QMessageBox::Ignore;

}

#ifdef HAVE_SSL

void MainWin::handleSslErrors(const QSslSocket *socket, bool *accepted, bool *permanently) {
  QString errorString = "<ul>";
  foreach(const QSslError error, socket->sslErrors())
    errorString += QString("<li>%1</li>").arg(error.errorString());
  errorString += "</ul>";

  QMessageBox box(QMessageBox::Warning,
                  tr("Untrusted Security Certificate"),
                  tr("<b>The SSL certificate provided by the core at %1 is untrusted for the following reasons:</b>").arg(socket->peerName()),
                  QMessageBox::Cancel, this);
  box.setInformativeText(errorString);
  box.addButton(tr("Continue"), QMessageBox::AcceptRole);
  box.setDefaultButton(box.addButton(tr("Show Certificate"), QMessageBox::HelpRole));

  QMessageBox::ButtonRole role;
  do {
    box.exec();
    role = box.buttonRole(box.clickedButton());
    if(role == QMessageBox::HelpRole) {
      SslInfoDlg dlg(socket, this);
      dlg.exec();
    }
  } while(role == QMessageBox::HelpRole);

  *accepted = role == QMessageBox::AcceptRole;
  if(*accepted) {
    QMessageBox box2(QMessageBox::Warning,
                     tr("Untrusted Security Certificate"),
                     tr("Would you like to accept this certificate forever without being prompted?"),
                     0, this);
    box2.setDefaultButton(box2.addButton(tr("Current Session Only"), QMessageBox::NoRole));
    box2.addButton(tr("Forever"), QMessageBox::YesRole);
    box2.exec();
    *permanently =  box2.buttonRole(box2.clickedButton()) == QMessageBox::YesRole;
  }
}

#endif /* HAVE_SSL */

void MainWin::handleCoreConnectionError(const QString &error) {
  QMessageBox::critical(this, tr("Core Connection Error"), error, QMessageBox::Ok);
}

void MainWin::showCoreConnectionDlg() {
  CoreConnectDlg dlg(this);
  if(dlg.exec() == QDialog::Accepted) {
    AccountId accId = dlg.selectedAccount();
    if(accId.isValid())
      Client::coreConnection()->connectToCore(accId);
  }
}

void MainWin::showCoreConfigWizard(const QVariantList &backends) {
  CoreConfigWizard *wizard = new CoreConfigWizard(Client::coreConnection(), backends, this);

  wizard->show();
}

void MainWin::showChannelList(NetworkId netId) {
  ChannelListDlg *channelListDlg = new ChannelListDlg();

  if(!netId.isValid()) {
    QAction *action = qobject_cast<QAction *>(sender());
    if(action)
      netId = action->data().value<NetworkId>();
  }

  channelListDlg->setAttribute(Qt::WA_DeleteOnClose);
  channelListDlg->setNetwork(netId);
  channelListDlg->show();
}

void MainWin::showIgnoreList(QString newRule) {
  SettingsPageDlg dlg(new IgnoreListSettingsPage(this), this);
  // prepare config dialog for new rule
  if(!newRule.isEmpty())
    qobject_cast<IgnoreListSettingsPage *>(dlg.currentPage())->editIgnoreRule(newRule);
  dlg.exec();
}

void MainWin::showCoreInfoDlg() {
  CoreInfoDlg(this).exec();
}

void MainWin::showAwayLog() {
  if(_awayLog)
    return;
  AwayLogFilter *filter = new AwayLogFilter(Client::messageModel());
  _awayLog = new AwayLogView(filter, 0);
  filter->setParent(_awayLog);
  connect(_awayLog, SIGNAL(destroyed()), this, SLOT(awayLogDestroyed()));
  _awayLog->setAttribute(Qt::WA_DeleteOnClose);
  _awayLog->show();
}

void MainWin::awayLogDestroyed() {
  _awayLog = 0;
}

void MainWin::showSettingsDlg() {
  SettingsDlg *dlg = new SettingsDlg();

  //Category: Interface
  dlg->registerSettingsPage(new AppearanceSettingsPage(dlg));
  dlg->registerSettingsPage(new ChatViewSettingsPage(dlg));
  dlg->registerSettingsPage(new ChatMonitorSettingsPage(dlg));
  dlg->registerSettingsPage(new ItemViewSettingsPage(dlg));
  dlg->registerSettingsPage(new BufferViewSettingsPage(dlg));
  dlg->registerSettingsPage(new InputWidgetSettingsPage(dlg));
  // dlg->registerSettingsPage(new TopicWidgetSettingsPage(dlg));
  dlg->registerSettingsPage(new HighlightSettingsPage(dlg));
  dlg->registerSettingsPage(new NotificationsSettingsPage(dlg));
  dlg->registerSettingsPage(new BacklogSettingsPage(dlg));
  dlg->registerSettingsPage(new ShortcutsSettingsPage(QtUi::actionCollections(), dlg));

  //Category: IRC
  dlg->registerSettingsPage(new ConnectionSettingsPage(dlg));
  dlg->registerSettingsPage(new IdentitiesSettingsPage(dlg));
  dlg->registerSettingsPage(new NetworksSettingsPage(dlg));
  dlg->registerSettingsPage(new AliasesSettingsPage(dlg));
  dlg->registerSettingsPage(new IgnoreListSettingsPage(dlg));

  // Category: Remote Cores
  if(Quassel::runMode() != Quassel::Monolithic) {
    dlg->registerSettingsPage(new CoreAccountSettingsPage(dlg));
    dlg->registerSettingsPage(new CoreConnectionSettingsPage(dlg));
  }

  dlg->show();
}

void MainWin::showAboutDlg() {
  AboutDlg(this).exec();
}

void MainWin::showShortcutsDlg() {
  // not used in mobile ui. moved to settings dialog
  return;

#ifdef HAVE_KDE
  KShortcutsDialog dlg(KShortcutsEditor::AllActions, KShortcutsEditor::LetterShortcutsDisallowed, this);
  foreach(KActionCollection *coll, QtUi::actionCollections())
    dlg.addCollection(coll, coll->property("Category").toString());
  dlg.exec();
#else
  SettingsPageDlg dlg(new ShortcutsSettingsPage(QtUi::actionCollections(), this), this);
  dlg.exec();
#endif
}

void MainWin::toggleFullscreen() {
  if(isFullScreen())
    showNormal();
  else
    showFullScreen();
}

void MainWin::setFullscreen(bool fullScreen) {
  if(isFullScreen() == fullScreen)
    return;

  toggleFullscreen();
}

/********************************************************************************************************/

bool MainWin::event(QEvent *event) {
  if(event->type() == QEvent::WindowActivate) {
    BufferId buffer = Client::bufferModel()->currentBuffer();
    if(buffer.isValid())
      Client::instance()->markBufferAsRead(buffer);
  }
  return QMainWindow::event(event);
}

void MainWin::moveEvent(QMoveEvent *event) {
  if(!(windowState() & Qt::WindowMaximized))
    _normalPos = event->pos();

  QMainWindow::moveEvent(event);
}

void MainWin::resizeEvent(QResizeEvent *event) {
  if(!(windowState() & Qt::WindowMaximized))
    _normalSize = event->size();

  QMainWindow::resizeEvent(event);
}

void MainWin::closeEvent(QCloseEvent *event) {
  QtUiSettings s;
  MobileUiApplication* app = qobject_cast<MobileUiApplication*> qApp;
  Q_ASSERT(app);
  if(!app->isAboutToQuit() && QtUi::haveSystemTray() && s.value("MinimizeOnClose").toBool()) {
    QtUi::hideMainWidget();
    event->ignore();
  } else {
    event->accept();
    quit();
  }
}

void MainWin::messagesInserted(const QModelIndex &parent, int start, int end) {
  Q_UNUSED(parent);

  bool hasFocus = QApplication::activeWindow() != 0;

  for(int i = start; i <= end; i++) {
    QModelIndex idx = Client::messageModel()->index(i, ChatLineModel::ContentsColumn);
    if(!idx.isValid()) {
      qDebug() << "MainWin::messagesInserted(): Invalid model index!";
      continue;
    }
    Message::Flags flags = (Message::Flags)idx.data(ChatLineModel::FlagsRole).toInt();
    if(flags.testFlag(Message::Backlog) || flags.testFlag(Message::Self))
      continue;

    BufferId bufId = idx.data(ChatLineModel::BufferIdRole).value<BufferId>();
    BufferInfo::Type bufType = Client::networkModel()->bufferType(bufId);

    if(hasFocus && bufId == Client::bufferModel()->currentBuffer())
      continue;

    if((flags & Message::Highlight || bufType == BufferInfo::QueryBuffer)
        && !(Client::ignoreListManager() && Client::ignoreListManager()->match(idx.data(MessageModel::MessageRole).value<Message>(),
                                                                               Client::networkModel()->networkName(bufId))))
    {
      QModelIndex senderIdx = Client::messageModel()->index(i, ChatLineModel::SenderColumn);
      QString sender = senderIdx.data(ChatLineModel::EditRole).toString();
      QString contents = idx.data(ChatLineModel::DisplayRole).toString();
      AbstractNotificationBackend::NotificationType type;

      if(bufType == BufferInfo::QueryBuffer && !hasFocus)
        type = AbstractNotificationBackend::PrivMsg;
      else if(bufType == BufferInfo::QueryBuffer && hasFocus)
        type = AbstractNotificationBackend::PrivMsgFocused;
      else if(flags & Message::Highlight && !hasFocus)
        type = AbstractNotificationBackend::Highlight;
      else
        type = AbstractNotificationBackend::HighlightFocused;

      QtUi::instance()->invokeNotification(bufId, type, sender, contents);
    }
  }
}

void MainWin::currentBufferChanged(BufferId buffer) {
  if(buffer.isValid())
    Client::instance()->markBufferAsRead(buffer);

  if(_bufferWidget && _bufferWidget->currentChatView() && _bufferWidget->currentChatView()->scene()) {
    _qmlContextObject->setFirstColumn( _bufferWidget->currentChatView()->scene()->firstColumnHandle() );
    _qmlContextObject->setSecondColumn( _bufferWidget->currentChatView()->scene()->secondColumnHandle() );
  } else {
    _qmlContextObject->setFirstColumn(0);
    _qmlContextObject->setSecondColumn(0);
  }
}

void MainWin::clientNetworkCreated(NetworkId id) {
  const Network *net = Client::network(id);
  connect(net, SIGNAL(updatedRemotely()), this, SLOT(clientNetworkUpdated()));
}

void MainWin::clientNetworkUpdated() {
  const Network *net = qobject_cast<const Network *>(sender());
  if(!net)
    return;

  switch(net->connectionState()) {
  case Network::Initialized:
    // if we have no currently selected buffer, jump to the first connecting statusbuffer
    if(!bufferWidget()->currentBuffer().isValid()) {
      QModelIndex idx = Client::networkModel()->networkIndex(net->networkId());
      if(idx.isValid()) {
        BufferId statusBufferId = idx.data(NetworkModel::BufferIdRole).value<BufferId>();
        Client::bufferModel()->switchToBuffer(statusBufferId);
      }
    }
    break;
  case Network::Disconnected:
    break;
  default:
    ;
  }
}

void MainWin::clientNetworkRemoved(NetworkId id) {
  QAction *action = findChild<QAction *>(QString("NetworkAction-%1").arg(id.toInt()));
  if(!action)
    return;

  action->deleteLater();
}

void MainWin::connectOrDisconnectFromNet() {
  QAction *act = qobject_cast<QAction *>(sender());
  if(!act) return;
  const Network *net = Client::network(act->data().value<NetworkId>());
  if(!net) return;
  if(net->connectionState() == Network::Disconnected) net->requestConnect();
  else net->requestDisconnect();
}

void MainWin::on_jumpHotBuffer_triggered() {
  if(!_bufferHotList->rowCount())
    return;

  QModelIndex topIndex = _bufferHotList->index(0, 0);
  BufferId bufferId = _bufferHotList->data(topIndex, NetworkModel::BufferIdRole).value<BufferId>();
  Client::bufferModel()->switchToBuffer(bufferId);
}

void MainWin::onJumpKey() {
  QAction *action = qobject_cast<QAction *>(sender());
  if(!action || !Client::bufferModel())
    return;
  int idx = action->property("Index").toInt();

  if(_jumpKeyMap.isEmpty())
    _jumpKeyMap = CoreAccountSettings().jumpKeyMap();

  if(!_jumpKeyMap.contains(idx))
    return;

  BufferId buffer = _jumpKeyMap.value(idx);
  if(buffer.isValid())
    Client::bufferModel()->switchToBuffer(buffer);

}

void MainWin::bindJumpKey() {
  QAction *action = qobject_cast<QAction *>(sender());
  if(!action || !Client::bufferModel())
    return;
  int idx = action->property("Index").toInt();

  _jumpKeyMap[idx] = Client::bufferModel()->currentBuffer();
  CoreAccountSettings().setJumpKeyMap(_jumpKeyMap);
}

void MainWin::on_actionDebugNetworkModel_triggered() {
  QTreeView *view = new QTreeView;
  view->setAttribute(Qt::WA_DeleteOnClose);
  view->setWindowTitle("Debug NetworkModel View");
  view->setModel(Client::networkModel());
  view->setColumnWidth(0, 250);
  view->setColumnWidth(1, 250);
  view->setColumnWidth(2, 80);
  view->resize(610, 300);
  view->show();
}

void MainWin::on_actionDebugHotList_triggered() {
  QTreeView *view = new QTreeView;
  view->setAttribute(Qt::WA_DeleteOnClose);
  view->setModel(_bufferHotList);
  view->show();
}

void MainWin::on_actionDebugBufferViewOverlay_triggered() {
  DebugBufferViewOverlay *overlay = new DebugBufferViewOverlay(0);
  overlay->setAttribute(Qt::WA_DeleteOnClose);
  overlay->show();
}

void MainWin::on_actionDebugMessageModel_triggered() {
  QTableView *view = new QTableView(0);
  DebugMessageModelFilter *filter = new DebugMessageModelFilter(view);
  filter->setSourceModel(Client::messageModel());
  view->setModel(filter);
  view->setAttribute(Qt::WA_DeleteOnClose, true);
  view->verticalHeader()->hide();
  view->horizontalHeader()->setStretchLastSection(true);
  view->show();
}

void MainWin::on_actionDebugLog_triggered() {
  DebugLogWidget *logWidget = new DebugLogWidget(0);
  logWidget->show();
}

void MainWin::showStatusBarMessage(const QString &message) {
  statusBar()->showMessage(message, 10000);
}
