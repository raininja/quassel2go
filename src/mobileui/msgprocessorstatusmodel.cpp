#include "msgprocessorstatusmodel.h"

MsgProcessorStatusModel::MsgProcessorStatusModel(QObject *parent) :
    QObject(parent),
  _progressValue(0),
  _progressMax(0)
{
}

void MsgProcessorStatusModel::setProgress(int value, int max) {
  if(value != _progressValue) {
    _progressValue = value;
    emit progressValueChanged();
  }
  if(max != _progressMax) {
    _progressMax = max;
    emit progressRangeChanged();
  }
  emit progressActiveChanged();
}
