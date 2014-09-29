/*
 *  Manjaro Settings Manager
 *  Ramon Buldó <ramon@manjaro.org>
 *
 *  Copyright (C) 2007 Free Software Foundation, Inc.
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "EnabledLocalesModel.h"

#include <QtCore/QFile>
#include <QtCore/QTextStream>
#include <QtDBus/QDBusInterface>

#include <QDebug>

EnabledLocalesModel::EnabledLocalesModel(QObject *parent) :
    QAbstractListModel(parent)
{

}


int EnabledLocalesModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return locales_.size();
}


int EnabledLocalesModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return roleNames().size();
}


QVariant EnabledLocalesModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() >= locales_.count()) {
        return QVariant();
    }
    const LocaleItem &localeItem = locales_[index.row()];

    Locale systemLocale = Locale();
    Locale locale(localeItem.key().toLatin1());

    /* Get language and country in current system locale */
    UnicodeString uDisplayLanguage;
    UnicodeString uDisplayCountry;
    locale.getDisplayLanguage(systemLocale, uDisplayLanguage);
    locale.getDisplayCountry(systemLocale, uDisplayCountry);

    /* Capitalize language and country */
    UErrorCode status;
    BreakIterator *titleIterator = BreakIterator::createTitleInstance(systemLocale, status);
    uDisplayLanguage = uDisplayLanguage.toTitle(titleIterator);
    uDisplayCountry = uDisplayCountry.toTitle(titleIterator);

    QString displayLanguage = unicodeStringToQString(uDisplayLanguage);
    QString displayCountry = unicodeStringToQString(uDisplayCountry);

    switch (role) {
    case Qt::DisplayRole:
        return QString("%1 - %2 (%3)").arg(displayLanguage).arg(displayCountry).arg(localeItem.key());
    case KeyRole:
        return localeItem.key();
    case CountryRole:
        return displayCountry;
    case LanguageRole:
        return displayLanguage;
    case AddressRole:
        return address();
    case CollateRole:
        return collate();
    case CtypeRole:
        return ctype();
    case IdentificationRole:
        return identification();
    case LangRole:
        return lang();
    case LanguageLcRole:
        return language();
    case MeasurementRole:
        return measurement();
    case MonetaryRole:
        return monetary();
    case MessagesRole:
        return messages();
    case NameRole:
        return name();
    case NumericRole:
        return numeric();
    case PaperRole:
        return paper();
    case TelephoneRole:
        return telephone();
    case TimeRole:
        return time();
    }

    return QVariant();
}


bool EnabledLocalesModel::insertLocale(int row, int count, const LocaleItem &localeItem)
{
    /* Locale already in the list */
    if (locales_.contains(localeItem)) {
        return false;
    }

    beginInsertRows(QModelIndex(), row, row + count - 1);

    for (int c=0; c < count; c++) {
        locales_.insert(row, localeItem);
    }

    endInsertRows();
    return true;
}


bool EnabledLocalesModel::removeLocale(int row, int count)
{
    /* Don't delete last locale */
    if (rowCount(QModelIndex()) == 1) {
        return false;
    }

    beginRemoveRows(QModelIndex(), row, row + count - 1);

    for (int c=0; c < count; c++) {
        locales_.removeAt(row);
    }

    endRemoveRows();
    return true;
}


void EnabledLocalesModel::setAddress(const QModelIndex &index)
{
    if (address_ != index.data(EnabledLocalesModel::KeyRole).toString()) {
        address_ = index.data(EnabledLocalesModel::KeyRole).toString();
        QVector<int> roles { AddressRole };
        emit dataChanged(index, index, roles);
    }
}


void EnabledLocalesModel::setCollate(const QModelIndex &index)
{
    if (collate_ != index.data(EnabledLocalesModel::KeyRole).toString()) {
        collate_ = index.data(EnabledLocalesModel::KeyRole).toString();
        QVector<int> roles { CollateRole };
        emit dataChanged(index, index, roles);
    }
}


void EnabledLocalesModel::setCtype(const QModelIndex &index)
{
    if (ctype_ != index.data(EnabledLocalesModel::KeyRole).toString()) {
        ctype_ = index.data(EnabledLocalesModel::KeyRole).toString();
        QVector<int> roles { CtypeRole };
        emit dataChanged(index, index, roles);
    }
}


void EnabledLocalesModel::setIdentification(const QModelIndex &index)
{
    if (identification_ != index.data(EnabledLocalesModel::KeyRole).toString()) {
        identification_ = index.data(EnabledLocalesModel::KeyRole).toString();
        QVector<int> roles { IdentificationRole };
        emit dataChanged(index, index, roles);
    }
}


void EnabledLocalesModel::setLang(const QModelIndex &index)
{
    if (lang_ != index.data(EnabledLocalesModel::KeyRole).toString()) {
        lang_ = index.data(EnabledLocalesModel::KeyRole).toString();
        QVector<int> roles { LangRole };
        emit dataChanged(index, index, roles);
    }
}


void EnabledLocalesModel::setLanguage(const QModelIndex &index)
{
    if (language_ != index.data(EnabledLocalesModel::KeyRole).toString()) {
        language_ = index.data(EnabledLocalesModel::KeyRole).toString();
        QVector<int> roles { LanguageLcRole };
        emit dataChanged(index, index, roles);
    }
}


void EnabledLocalesModel::setMeasurement(const QModelIndex &index)
{
    if (measurement_ != index.data(EnabledLocalesModel::KeyRole).toString()) {
        measurement_ = index.data(EnabledLocalesModel::KeyRole).toString();
        QVector<int> roles { MeasurementRole };
        emit dataChanged(index, index, roles);
    }
}


void EnabledLocalesModel::setMonetary(const QModelIndex &index)
{
    if (monetary_ != index.data(EnabledLocalesModel::KeyRole).toString()) {
        monetary_ = index.data(EnabledLocalesModel::KeyRole).toString();
        QVector<int> roles { MonetaryRole };
        emit dataChanged(index, index, roles);
    }
}


void EnabledLocalesModel::setMessages(const QModelIndex &index)
{
    if (messages_ != index.data(EnabledLocalesModel::KeyRole).toString()) {
        messages_ = index.data(EnabledLocalesModel::KeyRole).toString();
        QVector<int> roles { MessagesRole };
        emit dataChanged(index, index, roles);
    }
}


void EnabledLocalesModel::setName(const QModelIndex &index)
{
    if (name_ != index.data(EnabledLocalesModel::KeyRole).toString()) {
        name_ = index.data(EnabledLocalesModel::KeyRole).toString();
        QVector<int> roles { NameRole };
        emit dataChanged(index, index, roles);
    }
}


void EnabledLocalesModel::setNumeric(const QModelIndex &index)
{
    if (numeric_ != index.data(EnabledLocalesModel::KeyRole).toString()) {
        numeric_ = index.data(EnabledLocalesModel::KeyRole).toString();
        QVector<int> roles { NumericRole };
        emit dataChanged(index, index, roles);
    }
}


void EnabledLocalesModel::setPaper(const QModelIndex &index)
{
    if (paper_ != index.data(EnabledLocalesModel::KeyRole).toString()) {
        paper_ = index.data(EnabledLocalesModel::KeyRole).toString();
        QVector<int> roles { PaperRole };
        emit dataChanged(index, index, roles);
    }
}


void EnabledLocalesModel::setTelephone(const QModelIndex &index)
{
    if (telephone_ != index.data(EnabledLocalesModel::KeyRole).toString()) {
        telephone_ = index.data(EnabledLocalesModel::KeyRole).toString();
        QVector<int> roles { TelephoneRole };
        emit dataChanged(index, index, roles);
    }
}


void EnabledLocalesModel::setTime(const QModelIndex &index)
{
    if (time_ != index.data(EnabledLocalesModel::KeyRole).toString()) {
        time_ = index.data(EnabledLocalesModel::KeyRole).toString();
        QVector<int> roles { TimeRole };
        emit dataChanged(index, index, roles);
    }
}


bool EnabledLocalesModel::contains(const LocaleItem &localeItem) const
{
    return (locales_.contains(localeItem));
}


/*
 * Return a QStringlist of enabled locales keys (es_ES.UTF-8, en_US.UTF-8, ...)
 */
QStringList EnabledLocalesModel::locales() const
{
    QStringList locales;
    for (LocaleItem localeItem : locales_) {
        locales << localeItem.key();
    }
    return locales;
}


icu::UnicodeString EnabledLocalesModel::qStringToUnicodeString(const QString &sourceStr)
{
    return UnicodeString(static_cast<const UChar *>(sourceStr.utf16()),
                         sourceStr.length());
}


QString EnabledLocalesModel::unicodeStringToQString(const icu::UnicodeString &sourceStr)
{
    return QString(reinterpret_cast<const QChar *>(sourceStr.getBuffer()),
                   sourceStr.length());
}


QHash<int, QByteArray> EnabledLocalesModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[KeyRole] = "key";
    roles[CountryRole] = "country";
    roles[LanguageRole] = "language";
    roles[AddressRole] = "address";
    roles[CollateRole] = "collate";
    roles[CtypeRole] = "ctype";
    roles[IdentificationRole] = "identification";
    roles[LangRole] = "lang";
    roles[LanguageLcRole] = "languagelc";
    roles[MeasurementRole] = "measurement";
    roles[MonetaryRole] = "monetary";
    roles[MessagesRole] = "messages";
    roles[NameRole] = "name";
    roles[NumericRole] = "numeric";
    roles[PaperRole] = "paper";
    roles[TelephoneRole] = "telephone";
    roles[TimeRole] = "time";
    return roles;
}

int EnabledLocalesModel::findKey(const QString key) const
{
    for (int i = 0; i < rowCount(QModelIndex()); ++i) {
        if (key == locales_.at(i).key()) {
            return i;
        }
    }
    return -1;
}


/*
 * Read uncommented locales from /etc/locale.gen
 */
void EnabledLocalesModel::init()
{
    beginResetModel();
    locales_.clear();
    QFile file("/etc/locale.gen");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "error: failed to open '" << "/etc/locale.gen" << "'!";
        return;
    }

    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine();
        if (line.isEmpty() || line.startsWith("#")) {
            continue;
        }
        locales_ << LocaleItem(line.split(" ", QString::SkipEmptyParts).value(0));
    }
    file.close();
    endResetModel();
}


/*
 * Read system locales using org.freedesktop.locale1 dbus interface
 */
void EnabledLocalesModel::updateSystemLocales()
{
    QDBusInterface dbusInterface("org.freedesktop.locale1",
                                 "/org/freedesktop/locale1",
                                 "org.freedesktop.locale1",
                                 QDBusConnection::systemBus());

    QStringList localeEnviromentVariables = dbusInterface.property("Locale").toStringList();

    QModelIndex defaultIndex = QModelIndex();

    for (QString lev : localeEnviromentVariables) {
        if (lev.startsWith("LANG=")) {
            int defaultLocaleRow = findKey(lev.section('=', 1));
            defaultIndex = index(defaultLocaleRow,0);
        }
    }

    setAddress(defaultIndex);
    setCollate(defaultIndex);
    setCtype(defaultIndex);
    setIdentification(defaultIndex);
    setLang(defaultIndex);
    setLanguage(defaultIndex);
    setMeasurement(defaultIndex);
    setMonetary(defaultIndex);
    setMessages(defaultIndex);
    setName(defaultIndex);
    setNumeric(defaultIndex);
    setPaper(defaultIndex);
    setTelephone(defaultIndex);
    setTime(defaultIndex);

    for (QString lev : localeEnviromentVariables) {
        QString value = lev.section('=', 1);
        if (lev.startsWith("LC_ADDRESS=")) {
            if (value.isEmpty()) {
                setAddress(defaultIndex);
            } else {
                int row = findKey(value);
                setAddress(index(row, 0));
            }
        } else if (lev.startsWith("LC_COLLATE=")) {
            if (value.isEmpty()) {
                setCollate(defaultIndex);
            } else {
                int row = findKey(value);
                setCollate(index(row, 0));
            }
        } else if (lev.startsWith("LC_CTYPE=")) {
            if (value.isEmpty()) {
                setCtype(defaultIndex);
            } else {
                int row = findKey(value);
                setCtype(index(row, 0));
            }
        } else if (lev.startsWith("LC_IDENTIFICATION=")) {
            if (value.isEmpty()) {
                setIdentification(defaultIndex);
            } else {
                int row = findKey(value);
                setIdentification(index(row, 0));
            }
        } else if (lev.startsWith("LANG=")) {
            if (value.isEmpty()) {
                setLang(defaultIndex);
            } else {
                int row = findKey(value);
                setLang(index(row, 0));
            }
        } else if (lev.startsWith("LANGUAGE=")) {
            if (value.isEmpty()) {
                setLanguage(defaultIndex);
            } else {
                int row = findKey(value);
                setLanguage(index(row, 0));
            }
        } else if (lev.startsWith("LC_MEASUREMENT=")) {
            if (value.isEmpty()) {
                setMeasurement(defaultIndex);
            } else {
                int row = findKey(value);
                setMeasurement(index(row, 0));
            }
        } else if (lev.startsWith("LC_MESSAGES=")) {
            if (value.isEmpty()) {
                setMessages(defaultIndex);
            } else {
                int row = findKey(value);
                setMessages(index(row, 0));
            }
        } else if (lev.startsWith("LC_MONETARY=")) {
            if (value.isEmpty()) {
                setMonetary(defaultIndex);
            } else {
                int row = findKey(value);
                setMonetary(index(row, 0));
            }
        } else if (lev.startsWith("LC_NAME=")) {
            if (value.isEmpty()) {
                setName(defaultIndex);
            } else {
                int row = findKey(value);
                setName(index(row, 0));
            }
        } else if (lev.startsWith("LC_NUMERIC=")) {
            if (value.isEmpty()) {
                setNumeric(defaultIndex);
            } else {
                int row = findKey(value);
                setNumeric(index(row, 0));
            }
        } else if (lev.startsWith("LC_PAPER=")) {
            if (value.isEmpty()) {
                setPaper(defaultIndex);
            } else {
                int row = findKey(value);
                setPaper(index(row, 0));
            }
        } else if (lev.startsWith("LC_TELEPHONE=")) {
            if (value.isEmpty()) {
                setTelephone(defaultIndex);
            } else {
                int row = findKey(value);
                setTelephone(index(row, 0));
            }
        } else if (lev.startsWith("LC_TIME=")) {
            if (value.isEmpty()) {
                setTime(defaultIndex);
            } else {
                int row = findKey(value);
                setTime(index(row, 0));
            }
        }
    }
}
