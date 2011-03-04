#include "inputpresenter.h"

#include "multilineedit.h"

#include "action.h"
#include "actioncollection.h"
#include "bufferview.h"
#include "client.h"
#include "iconloader.h"
#include "ircuser.h"
#include "networkmodel.h"
#include "qtui.h"
#include "qtuisettings.h"
#include "tabcompleter.h"
#include <QPainter>
#include <QApplication>

const int leftMargin = 3;

InputPresenter::InputPresenter(MultiLineEdit *input, QObject *parent)
  : AbstractItemPresenter(parent),
    _inputLine(input),
    _networkId(0)
{
  _inputLine->installEventFilter(this);

  _inputLine->setMinHeight(1);
  _inputLine->setMaxHeight(5);
  _inputLine->setMode(MultiLineEdit::MultiLine);
  _inputLine->setPasteProtectionEnabled(true);

  new TabCompleter(_inputLine);

  UiStyleSettings fs("Fonts");
  fs.notify("UseCustomInputWidgetFont", this, SLOT(setUseCustomFont(QVariant)));
  fs.notify("InputWidget", this, SLOT(setCustomFont(QVariant)));
  if(fs.value("UseCustomInputWidgetFont", false).toBool())
    setCustomFont(fs.value("InputWidget", QFont()));

  UiSettings s("InputWidget");

#ifdef HAVE_KDE
  s.notify("EnableSpellCheck", this, SLOT(setEnableSpellCheck(QVariant)));
  setEnableSpellCheck(s.value("EnableSpellCheck", false));
#endif

  s.notify("EnableEmacsMode", this, SLOT(setEnableEmacsMode(QVariant)));
  setEnableEmacsMode(s.value("EnableEmacsMode", false));

  s.notify("EnablePerChatHistory", this, SLOT(setEnablePerChatHistory(QVariant)));
  setEnablePerChatHistory(s.value("EnablePerChatHistory", false));

  s.notify("MaxNumLines", this, SLOT(setMaxLines(QVariant)));
  setMaxLines(s.value("MaxNumLines", 5));

  s.notify("EnableScrollBars", this, SLOT(setScrollBarsEnabled(QVariant)));
  setScrollBarsEnabled(s.value("EnableScrollBars", true));

  s.notify("EnableMultiLine", this, SLOT(setMultiLineEnabled(QVariant)));
  setMultiLineEnabled(s.value("EnableMultiLine", true));

  ActionCollection *coll = QtUi::actionCollection();

  Action *activateInputline = coll->add<Action>("FocusInputLine");
  connect(activateInputline, SIGNAL(triggered()), SLOT(setFocus()));
  activateInputline->setText(tr("Focus Input Line"));
  activateInputline->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_L));

  connect(inputLine(), SIGNAL(currentCharFormatChanged(QTextCharFormat)), this, SLOT(currentCharFormatChanged(QTextCharFormat)));
  connect(inputLine(), SIGNAL(textEntered(QString)), this, SLOT(textEntered(QString)));
}

InputPresenter::~InputPresenter() {
}

void InputPresenter::setUseCustomFont(const QVariant &v) {
  if(v.toBool()) {
    UiStyleSettings fs("Fonts");
    setCustomFont(fs.value("InputWidget"));
  } else
    setCustomFont(QFont());
}

void InputPresenter::setCustomFont(const QVariant &v) {
  QFont font = v.value<QFont>();
  if(font.family().isEmpty())
    font = QApplication::font();
  // TODO check wether changes propagate through inputLine->prop sync
//  // we don't want font styles as this conflics with mirc code richtext editing
//  setFontBold(false);
//  setFontItalic(false);
//  setFontUnderline(false);
//  setFontStrikeout(false);
  _inputLine->setCustomFont(font);
}

void InputPresenter::setEnableSpellCheck(const QVariant &v) {
  _inputLine->setSpellCheckEnabled(v.toBool());
}

void InputPresenter::setEnableEmacsMode(const QVariant &v) {
  _inputLine->setEmacsMode(v.toBool());
}

void InputPresenter::setEnablePerChatHistory(const QVariant &v) {
  _perChatHistory = v.toBool();
}

void InputPresenter::setMaxLines(const QVariant &v) {
  _inputLine->setMaxHeight(v.toInt());
}

void InputPresenter::setScrollBarsEnabled(const QVariant &v) {
  _inputLine->setScrollBarsEnabled(v.toBool());
}

void InputPresenter::setMultiLineEnabled(const QVariant &v) {
  _inputLine->setMode(v.toBool()? MultiLineEdit::MultiLine : MultiLineEdit::SingleLine);
}

bool InputPresenter::eventFilter(QObject *watched, QEvent *event) {
  if(event->type() != QEvent::KeyPress)
    return false;

  QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);

  // keys from BufferView should be sent to (and focus) the input line
  BufferView *view = qobject_cast<BufferView *>(watched);
  if(view) {
    if(keyEvent->text().length() == 1 && !(keyEvent->modifiers() & (Qt::ControlModifier ^ Qt::AltModifier)) ) { // normal key press
      QChar c = keyEvent->text().at(0);
      if(c.isLetterOrNumber() || c.isSpace() || c.isPunct() || c.isSymbol()) {
        inputLine()->setFocus();
        QCoreApplication::sendEvent(inputLine(), keyEvent);
        return true;
      }
    }
    return false;
  } else if(watched == _inputLine) {
    if(keyEvent->matches(QKeySequence::Find)) {
      QAction *act = GraphicalUi::actionCollection()->action("ToggleSearchBar");
      if(act) {
        act->toggle();
        return true;
      }
    }
    return false;
  }
  return false;
}

void InputPresenter::currentChanged(const QModelIndex &current, const QModelIndex &previous) {
  BufferId currentBufferId = current.data(NetworkModel::BufferIdRole).value<BufferId>();
  BufferId previousBufferId = previous.data(NetworkModel::BufferIdRole).value<BufferId>();

  if (_perChatHistory) {
    //backup
    historyMap[previousBufferId].history = inputLine()->history();
    historyMap[previousBufferId].tempHistory = inputLine()->tempHistory();
    historyMap[previousBufferId].idx = inputLine()->idx();
    historyMap[previousBufferId].inputLine = inputLine()->html();

    //restore
    inputLine()->setHistory(historyMap[currentBufferId].history);
    inputLine()->setTempHistory(historyMap[currentBufferId].tempHistory);
    inputLine()->setIdx(historyMap[currentBufferId].idx);
    inputLine()->setHtml(historyMap[currentBufferId].inputLine);
    inputLine()->moveCursor(QTextCursor::End,QTextCursor::MoveAnchor);

    // FIXME this really should be in MultiLineEdit (and the const int on top removed)
    QTextBlockFormat format = inputLine()->textCursor().blockFormat();
    format.setLeftMargin(leftMargin); // we want a little space between the frame and the contents
    inputLine()->textCursor().setBlockFormat(format);
  }
}

BufferInfo InputPresenter::currentBufferInfo() const {
  return selectionModel()->currentIndex().data(NetworkModel::BufferInfoRole).value<BufferInfo>();
};

void InputPresenter::textEntered(const QString &text) {
  Client::userInput(currentBufferInfo(), text);

  // TODO check wether inputLine->property sync works
//  setFontBold(false);
//  setFontUnderline(false);
//  setFontItalic(false);

  QTextCharFormat fmt;
  fmt.setFontWeight(QFont::Normal);
  fmt.setFontUnderline(false);
  fmt.setFontItalic(false);
  fmt.clearForeground();
  fmt.clearBackground();
  inputLine()->setCurrentCharFormat(fmt);

#ifdef HAVE_KDE
  // Set highlighter back to active in case it was deactivated by too many errors.
  if(_inputLine->highlighter())
    _inputLine->highlighter()->setActive(true);
#endif
}

void InputPresenter::mergeFormatOnSelection(const QTextCharFormat &format) {
  QTextCursor cursor = inputLine()->textCursor();
  cursor.mergeCharFormat(format);
  inputLine()->mergeCurrentCharFormat(format);
}

void InputPresenter::setFormatOnSelection(const QTextCharFormat &format) {
  QTextCursor cursor = inputLine()->textCursor();
  cursor.setCharFormat(format);
  inputLine()->setCurrentCharFormat(format);
}

QTextCharFormat InputPresenter::getFormatOfWordOrSelection() {
  QTextCursor cursor = inputLine()->textCursor();
  return cursor.charFormat();
}

void InputPresenter::currentCharFormatChanged(const QTextCharFormat &format) {
  emit fontChanged(format.font());
}

void InputPresenter::setFontBold(bool bold)
{
  QTextCharFormat fmt;
  fmt.setFontWeight(bold ? QFont::Bold : QFont::Normal);
  mergeFormatOnSelection(fmt);
}

void InputPresenter::setFontUnderline(bool underline)
{
  QTextCharFormat fmt;
  fmt.setFontUnderline(underline);
  mergeFormatOnSelection(fmt);
}

void InputPresenter::setFontItalic(bool italic)
{
  QTextCharFormat fmt;
  fmt.setFontItalic(italic);
  mergeFormatOnSelection(fmt);
}

const QFont InputPresenter::currentFont() const
{
  return _inputLine->currentCharFormat().font();
}

bool InputPresenter::boldText() const
{
  return currentFont().bold();
}
bool InputPresenter::underlineText() const
{
  return currentFont().underline();
}
bool InputPresenter::italicText() const
{
  return currentFont().italic();
}
