#pragma once
#include <QObject>
#include <QString>
#include <QColor>
#include <QMap>

class TomlParser : public QObject {
    Q_OBJECT
public:
    explicit TomlParser(QObject *parent = nullptr);

    bool load(const QString &filePath);
    QString getString(const QString &section, const QString &key, const QString &defaultValue = QString()) const;
    int getInt(const QString &section, const QString &key, int defaultValue = 0) const;
    double getDouble(const QString &section, const QString &key, double defaultValue = 0.0) const;
    bool getBool(const QString &section, const QString &key, bool defaultValue = false) const;
    QStringList getStringList(const QString &section, const QString &key) const;
    QColor getColor(const QString &section, const QString &key, const QColor &defaultValue = QColor()) const;

    QStringList sections() const;

private:
    QMap<QString, QMap<QString, QString>> m_data;
    QString m_currentSection;
};
