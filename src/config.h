#pragma once
#include <QObject>

class Config : public QObject {
    Q_OBJECT
public:
    Config();
    void load();
};