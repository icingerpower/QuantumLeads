#include <QObject>

#include "ColumnMissingException.h"

void ColumnMissingException::raise() const
{
    throw *this;
}

ColumnMissingException *ColumnMissingException::clone() const
{
    return new ColumnMissingException{*this};
}

const QString &ColumnMissingException::error() const
{
    return m_error;
}

void ColumnMissingException::setError(const QString &error)
{
    m_error = error;
}

QString ColumnMissingException::title() const
{
    return QObject::tr("Column missing");
}
