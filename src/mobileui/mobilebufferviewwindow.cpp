#include <QMenuBar>
#include "mobilebufferviewwindow.h"

MobileBufferViewWindow::MobileBufferViewWindow(QWidget *parent) :
    QMainWindow(parent),
    _action(0),
    _active(false)
{
  _view = new MobileBufferView(this);
  setCentralWidget(_view);

  _action = new QAction(tr("BufferView"), this);
  connect(_action, SIGNAL(triggered()), this, SLOT(show()));

  QAction *act = new QAction(tr("Set Active"), this);
  connect(act, SIGNAL(triggered()), this, SLOT(activateClicked()));
  act->setObjectName("RequestBufferViewActivation");
  menuBar()->addAction(act);

  connect(_view, SIGNAL(updateTitle()), this, SLOT(updateTitle()));
}

QAction * MobileBufferViewWindow::toggleVisibleAction() const
{
  return _action;
}

void MobileBufferViewWindow::updateTitle() {
  QString title = _view->config() ? _view->config()->bufferViewName() : "";
  if(_active)
    title.prepend(QString::fromUtf8("• "));
  setWindowTitle(title);
  _action->setText(title);
}

void MobileBufferViewWindow::setActive(bool active) {
  if(active != _active) {
    _active = active;
    updateTitle();
    if(active)
      raise(); // for tabbed docks
  }
}

void MobileBufferViewWindow::activateClicked()
{
  emit requestActivation(_view->config() ? _view->config()->bufferViewId() : -1);
}
