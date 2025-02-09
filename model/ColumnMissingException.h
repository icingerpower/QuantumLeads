#ifndef COLMUNMISSINGEXCEPTION_H
#define COLMUNMISSINGEXCEPTION_H

#include <QException>

class ColumnMissingException : public QException
{
public:
    void raise() const override;
    ColumnMissingException *clone() const override;
    QString title() const;
    const QString &error() const;

    void setError(const QString &error);


private:
    QString m_error;
};

#endif // COLMUNMISSINGEXCEPTION_H
