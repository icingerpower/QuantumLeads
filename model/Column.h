#ifndef COLUMN_H
#define COLUMN_H

#include <QString>
#include <QHash>

class Column
{
public:
    static const QList<Column> ALL_COLUMNS;
    static const QHash<QString, const Column *> ALL_COLUMNS_BY_ID;
    static const Column COL_PHONE;
    static const Column COL_EMAIL;
    static const Column COL_WEBSITE_1;
    static const Column COL_WEBSITE_2;
    static const Column COL_PERSON_FIRST_NAME;
    static const Column COL_PERSON_LAST_NAME;
    static const Column COL_PERSON_TITLE;
    static const Column COL_N_EMPLOYE;
    static const Column COL_INDUSTRY;
    static const Column COL_LINKEDIN_URL_COMPANY;
    static const Column COL_LINKEDIN_URL_PERSON;
    static const Column COL_COMPANY;
    static const Column COL_COMPANY_STREET;
    static const Column COL_COMPANY_CITY;
    static const Column COL_COMPANY_STATE;
    static const Column COL_COMPANY_POSTAL_CODE;
    static const Column COL_COMPANY_COUNTRY;
    static const Column COL_YEARLY_INCOME;
    static const int GET_COL_INDEX(const QString &id);

    Column(const QString &id, const QString &name);

    const QString &id() const;
    const QString &name() const;

private:
    QString m_id;
    QString m_name;
};

#endif // COLUMN_H
