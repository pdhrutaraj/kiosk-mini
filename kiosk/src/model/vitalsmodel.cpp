#include "vitalsmodel.h"

VitalsModel::VitalsModel(QObject *parent) : QObject(parent) {}

void VitalsModel::setSpo2(int v) { emit spo2Changed(v); }
void VitalsModel::setPulseRate(int v) { emit pulseRateChanged(v); }
void VitalsModel::setTemperature(float v){ emit temperatureChanged(v); }
void VitalsModel::setWeight(float v) { emit weightChanged(v); }
void VitalsModel::setHeight(int v) { emit heightChanged(v); }

