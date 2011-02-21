#ifndef INPUTPRESENTER_H
#define INPUTPRESENTER_H

#include <QObject>
#include <QTextCharFormat>

#include "abstractitemview.h"
#include "buffermodel.h"
#include "bufferinfo.h"
#include "identity.h"
#include "network.h"
#include <action.h>

#include "abstractitempresenter.h"

class MultiLineEdit;

class InputPresenter : public AbstractItemPresenter
{
    Q_OBJECT

  Q_PROPERTY(bool boldText READ boldText WRITE setFontBold NOTIFY fontChanged)
  Q_PROPERTY(bool underlineText READ underlineText WRITE setFontUnderline NOTIFY fontChanged)
  Q_PROPERTY(bool italicText READ italicText WRITE setFontItalic NOTIFY fontChanged)

  // TODO: Properties (font etc.)
public:
    explicit InputPresenter(MultiLineEdit *inputLine, QObject *parent = 0);
  virtual ~InputPresenter();

  inline MultiLineEdit* inputLine() const { return _inputLine; }

  const QFont &currentFont() const;
  bool boldText() const;
  bool underlineText() const;
  bool italicText() const;

public slots:
  void setFontBold(bool bold);
  void setFontUnderline(bool underline);
  void setFontItalic(bool italic);

signals:
  void fontChanged(const QFont &font);

protected:
  virtual bool eventFilter(QObject *watched, QEvent *event);

protected slots:
  virtual void currentChanged(const QModelIndex &current, const QModelIndex &previous);
  // virtual void rowsAboutToBeRemoved(const QModelIndex &parent, int start, int end);
  // virtual void dataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight);

private slots:
  void setCustomFont(const QVariant &font);
  void setUseCustomFont(const QVariant &);
  void setEnableSpellCheck(const QVariant &);
  void setEnableEmacsMode(const QVariant &);
  void setEnablePerChatHistory(const QVariant &);
  void setMaxLines(const QVariant &);
  void setMultiLineEnabled(const QVariant &);
  void setScrollBarsEnabled(const QVariant &);
  void textEntered(const QString &text);

  void currentCharFormatChanged(const QTextCharFormat &format);

private:
  BufferInfo currentBufferInfo() const;

  MultiLineEdit *_inputLine;

  NetworkId _networkId;
  IdentityId _identityId;
  QMenu *_colorMenu, *_colorFillMenu;

  void mergeFormatOnSelection(const QTextCharFormat &format);
  QTextCharFormat getFormatOfWordOrSelection();
  void setFormatOnSelection(const QTextCharFormat &format);

  bool _perChatHistory;
  struct HistoryState {
    QStringList history;
    QHash<int, QString> tempHistory;
    qint32 idx;
    QString inputLine;
    inline HistoryState() : idx(0) {};
  };

  QMap<BufferId, HistoryState> historyMap;
};

#endif // INPUTPRESENTER_H
