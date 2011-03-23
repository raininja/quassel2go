#ifndef MOBILEBUFFERVIEWWINDOW_H
#define MOBILEBUFFERVIEWWINDOW_H

#include <QAction>
#include <QMainWindow>
#include "mobilebufferview.h"

class MobileBufferViewWindow : public QMainWindow
{
    Q_OBJECT
  Q_PROPERTY(bool active READ isActive WRITE setActive STORED true)
public:
    explicit MobileBufferViewWindow(QWidget *parent = 0);


    QAction *toggleVisibleAction() const;
    inline bool isActive() const { return _active; }
    void setActive(bool active = true);

    MobileBufferView *view() {
      return _view;
    }

signals:
    void requestActivation(int selfId);

public slots:
    void updateTitle();

private slots:
    void activateClicked();

private:
    MobileBufferView *_view;
    QAction *_action;
    bool _active;
};

#endif // MOBILEBUFFERVIEWWINDOW_H
