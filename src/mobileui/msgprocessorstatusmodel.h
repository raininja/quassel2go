#ifndef MSGPROCESSORSTATUSMODEL_H
#define MSGPROCESSORSTATUSMODEL_H

#include <QObject>

class MsgProcessorStatusModel : public QObject
{
  Q_OBJECT

  Q_PROPERTY(int progressMinimum READ progressMinimum NOTIFY progressRangeChanged)
  Q_PROPERTY(int progressMaximum READ progressMaximum NOTIFY progressRangeChanged)
  Q_PROPERTY(int progressValue READ progressValue NOTIFY progressValueChanged)
  Q_PROPERTY(bool progressActive READ progressActive NOTIFY progressActiveChanged)
public:
  MsgProcessorStatusModel(QObject *parent = 0);

  int progressMinimum() const { return 0; }
  int progressMaximum() const { return _progressMax; }
  int progressValue() const { return _progressValue; }
  bool progressActive() const { return !(_progressMax <= 0 || _progressValue == _progressMax); }

public slots:
  void setProgress(int value, int max);

signals:
  void progressRangeChanged();
  void progressValueChanged();
  void progressActiveChanged();

private:
  int _progressValue;
  int _progressMax;
};

#endif // MSGPROCESSORSTATUSMODEL_H
