#include "tomlparser.h"
#include <QFile>
#include <QTextStream>
#include <QRegularExpression>

TomlParser::TomlParser(QObject *parent)
    : QObject(parent)
{
}

bool TomlParser::load(const QString &filePath) {
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return false;
    }

    QTextStream in(&file);
    m_data.clear();
    m_currentSection = "";

    QRegularExpression sectionRegex(R"(^\[([^\]]+)\]$)");
    QRegularExpression keyValueRegex(R"(^([^\s=]+)\s*=\s*(.+)$)");
    QRegularExpression commentRegex(R"(^\s*#)");

    while (!in.atEnd()) {
        QString line = in.readLine().trimmed();

        if (line.isEmpty() || commentRegex.match(line).hasMatch()) {
            continue;
        }

        QRegularExpressionMatch sectionMatch = sectionRegex.match(line);
        if (sectionMatch.hasMatch()) {
            m_currentSection = sectionMatch.captured(1).trimmed();
            if (!m_data.contains(m_currentSection)) {
                m_data[m_currentSection] = QMap<QString, QString>();
            }
            continue;
        }

        QRegularExpressionMatch kvMatch = keyValueRegex.match(line);
        if (kvMatch.hasMatch()) {
            QString key = kvMatch.captured(1).trimmed();
            QString value = kvMatch.captured(2).trimmed();
            value.remove(QRegularExpression(R"(^["']|["']$)"));
            if (m_currentSection.isEmpty()) {
                m_currentSection = "global";
                m_data[m_currentSection] = QMap<QString, QString>();
            }
            m_data[m_currentSection][key] = value;
        }
    }

    file.close();
    return true;
}

QString TomlParser::getString(const QString &section, const QString &key, const QString &defaultValue) const {
    if (m_data.contains(section) && m_data[section].contains(key)) {
        return m_data[section][key];
    }
    return defaultValue;
}

int TomlParser::getInt(const QString &section, const QString &key, int defaultValue) const {
    QString value = getString(section, key);
    if (!value.isEmpty()) {
        bool ok;
        int result = value.toInt(&ok);
        if (ok) return result;
    }
    return defaultValue;
}

double TomlParser::getDouble(const QString &section, const QString &key, double defaultValue) const {
    QString value = getString(section, key);
    if (!value.isEmpty()) {
        bool ok;
        double result = value.toDouble(&ok);
        if (ok) return result;
    }
    return defaultValue;
}

bool TomlParser::getBool(const QString &section, const QString &key, bool defaultValue) const {
    QString value = getString(section, key).toLower();
    if (value == "true" || value == "yes" || value == "1") {
        return true;
    } else if (value == "false" || value == "no" || value == "0") {
        return false;
    }
    return defaultValue;
}

QStringList TomlParser::getStringList(const QString &section, const QString &key) const {
    QString value = getString(section, key);
    if (value.isEmpty()) return QStringList();
    return value.split(",", Qt::SkipEmptyParts);
}

QColor TomlParser::getColor(const QString &section, const QString &key, const QColor &defaultValue) const {
    QString value = getString(section, key);
    if (value.isEmpty()) return defaultValue;
    QColor color(value);
    if (color.isValid()) return color;
    return defaultValue;
}

QStringList TomlParser::sections() const {
    return m_data.keys();
}
