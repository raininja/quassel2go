/***************************************************************************
 *   Copyright (C) 2005-08 by the Quassel IRC Team                         *
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

#ifndef BUFFERVIEWSETTINGSPAGE_H
#define BUFFERVIEWSETTINGSPAGE_H

#include "settingspage.h"
#include "ui_bufferviewsettingspage.h"
#include "ui_buffervieweditdlg.h"

class BufferViewConfig;

class BufferViewSettingsPage : public SettingsPage {
  Q_OBJECT

public:
  BufferViewSettingsPage(QWidget *parent = 0);
  ~BufferViewSettingsPage();

public slots:
  void save();
  void load();
  void reset();

private slots:
  void coreConnectionStateChanged(bool state);

  void addBufferView(BufferViewConfig *config);
  void addBufferView(int bufferViewId);
  void newBufferView(const QString &bufferViewName);
  void updateBufferView();

  void on_addBufferView_clicked();
  void on_renameBufferView_clicked();
  
private:
  Ui::BufferViewSettingsPage ui;

  // mappings for bufferViewId to position in the list and vice versa
  QHash<int, int> _viewToListPos;
  QHash<int, int> _listPosToView;

  // list of bufferviews to create
  QList<BufferViewConfig *> _newBufferViews;

  // Hash of pointers to cloned bufferViewConfigs holding the changes
  QHash<BufferViewConfig *, BufferViewConfig *> _changedBufferViews;

  int listPos(BufferViewConfig *config);
  int bufferViewId(int listPos);
  BufferViewConfig *cloneConfig(BufferViewConfig *config);
  BufferViewConfig *configForDisplay(BufferViewConfig *config);
};


/**************************************************************************
 * BufferViewEditDlg
 *************************************************************************/
class BufferViewEditDlg : public QDialog {
  Q_OBJECT

public:
  BufferViewEditDlg(const QString &old, const QStringList &existing = QStringList(), QWidget *parent = 0);

  inline QString bufferViewName() const { return ui.bufferViewEdit->text(); }

private slots:
  void on_bufferViewEdit_textChanged(const QString &);
  
private:
  Ui::BufferViewEditDlg ui;

  QStringList existing;
};



#endif // BUFFERVIEWSETTINGSPAGE_H