#include "baseclass.h"

biosmabe::BaseClass::BaseClass(int year, int month, int day)
{
	m_year = year;
	m_month = month;
	m_day = day;
}
 
void biosmabe::BaseClass::SetDate(int year, int month, int day)
{
	m_year = year;
	m_month = month;
	m_day = day;
}
 
int biosmabe::BaseClass::getYear()
{
	return m_year;
}

int biosmabe::BaseClass::getMonth()
{
	return m_month;
}

int biosmabe::BaseClass::getDay()
{
	return m_day;
}