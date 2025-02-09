#include <QObject>

#include "Column.h"

const Column Column::COL_PHONE{"phone", QObject::tr("Phone")};
const Column Column::COL_EMAIL{"email", QObject::tr("Email")};
const Column Column::COL_WEBSITE_1{"website1", QObject::tr("Website 1")};
const Column Column::COL_WEBSITE_2{"website2", QObject::tr("Website 2")};
const Column Column::COL_PERSON_FIRST_NAME{"First name", QObject::tr("First name")};
const Column Column::COL_PERSON_LAST_NAME{"Last name", QObject::tr("Last name")};
const Column Column::COL_PERSON_TITLE{"Title", QObject::tr("Person Title")};
const Column Column::COL_N_EMPLOYE{"nEmploye", QObject::tr("Number of employe")};
const Column Column::COL_INDUSTRY{"industry", QObject::tr("Industry")};
const Column Column::COL_LINKEDIN_URL_COMPANY{"LinkedinUrl", QObject::tr("LinkedIn Url")};
const Column Column::COL_LINKEDIN_URL_PERSON{"LinkedinUrlPerson", QObject::tr("LinkedIn Url Person")};
const Column Column::COL_COMPANY{"CompanyName", QObject::tr("Company name")};
const Column Column::COL_COMPANY_STREET{"CompanyStreet", QObject::tr("Company street")};
const Column Column::COL_COMPANY_CITY{"CompanyCity", QObject::tr("Company city")};
const Column Column::COL_COMPANY_STATE{"CompanyState", QObject::tr("Company state")};
const Column Column::COL_COMPANY_POSTAL_CODE{"CompanyPostalCode", QObject::tr("Company postal code")};
const Column Column::COL_COMPANY_COUNTRY{"CompanyCountry", QObject::tr("Country")};
const Column Column::COL_YEARLY_INCOME{"yearlyIncome", QObject::tr("Yearly income")};
const QList<Column> Column::ALL_COLUMNS{
    Column::COL_PHONE
    , Column::COL_EMAIL
    , Column::COL_WEBSITE_1
    , Column::COL_WEBSITE_2
    , Column::COL_PERSON_FIRST_NAME
    , Column::COL_PERSON_LAST_NAME
    , Column::COL_PERSON_TITLE
    , Column::COL_N_EMPLOYE
    , Column::COL_INDUSTRY
    , Column::COL_LINKEDIN_URL_COMPANY
    , Column::COL_LINKEDIN_URL_PERSON
    , Column::COL_COMPANY
    , Column::COL_COMPANY_STREET
    , Column::COL_COMPANY_CITY
    , Column::COL_COMPANY_STATE
    , Column::COL_COMPANY_POSTAL_CODE
    , Column::COL_COMPANY_COUNTRY
    , Column::COL_YEARLY_INCOME
};
const QHash<QString,  const Column *> Column::ALL_COLUMNS_BY_ID
= []() -> QHash<QString, const Column *>{
    QHash<QString, const Column *> allColumnsById;
    for (const auto &column : ALL_COLUMNS)
    {
        allColumnsById[column.id()] = &column;
    }
    return allColumnsById;
}();

const int Column::GET_COL_INDEX(const QString &id)
{
    int index = -1;
    for (int i=0; i<ALL_COLUMNS.size(); ++i)
    {
        if (ALL_COLUMNS[i].id() == id)
        {
            return i;
        }
    }
    return index;
}

Column::Column(const QString &id, const QString &name)
{
    m_id = id;
    m_name = name;
}

const QString &Column::id() const
{
    return m_id;
}

const QString &Column::name() const
{
    return m_name;
}
