#pragma once
#include <QObject>
#include <QScreen>
#include <QString>
#include <QRect>

class Monitor : public QObject {
    Q_OBJECT
    Q_PROPERTY(QString name READ name CONSTANT)
    Q_PROPERTY(QString identifier READ identifier CONSTANT)
    Q_PROPERTY(QRect geometry READ geometry CONSTANT)
    Q_PROPERTY(int width READ width CONSTANT)
    Q_PROPERTY(int height READ height CONSTANT)
    Q_PROPERTY(bool isPrimary READ isPrimary CONSTANT)

public:
    explicit Monitor(QScreen *screen, QObject *parent = nullptr);
    ~Monitor();

    QString name() const;
    QString identifier() const;
    QRect geometry() const;
    int width() const;
    int height() const;
    bool isPrimary() const;

    static QString screenIdentifier(QScreen *screen);

signals:
    void geometryChanged();
    void destroyed(Monitor *monitor);

private:
    QScreen *m_screen;
    QString m_name;
    QString m_identifier;
    QRect m_geometry;
    bool m_isPrimary;
};
