#include <iostream>
#include "sqlite_db.h"

using namespace std;


bool sqlite_db_t::open_db()
{
	if (connection)
		sqlite3_close(connection);
	rc = sqlite3_open(dbname.c_str(), &connection);
	if (rc)
		return false;
	return true;
}
//---------------------------------------------------------------------------

void sqlite_db_t::close_db()
{
	if (stmt)
	{
		sqlite3_finalize(stmt);
		stmt = NULL;
	}
	if (connection)
		sqlite3_close(connection);
	connection = NULL;
}
//-------------------------------------------------------------------------

bool sqlite_db_t::exec_sql(bool SIU)
{
	if (stmt)
		sqlite3_finalize(stmt);
	rc = sqlite3_prepare_v2(connection, sql_str.c_str(), -1, &stmt, 0);
	if (rc != SQLITE_OK)
		return false;


	switch (sqlite3_step(stmt))
	{
	case SQLITE_DONE:
		if (SIU)
			return true;
		else
			return false;
	case SQLITE_ROW:
		row_number = 0;
		return true;
	default:
		return false;
	}
}
//---------------------------------------------------------------------------

void sqlite_db_t::first_rec()
{
	if (sqlite3_reset(stmt) == SQLITE_OK)
		if (sqlite3_step(stmt) == SQLITE_ROW)
			row_number = 0;
}
//---------------------------------------------------------------------------

bool sqlite_db_t::next_rec()
{
	int r = sqlite3_step(stmt);
	switch (r)
	{
	case SQLITE_DONE:
		return false;
	case SQLITE_ROW:
		row_number++;
		break;
	default:
		return false;
	}
	return true;
}
//---------------------------------------------------------------------------

string sqlite_db_t::field_by_name(string field_name)
{
	string strResult = "";
	int type = 0;
	int field_number = sqlite3_column_count(stmt);
	if (field_number)
		for (int i = 0; i < field_number; i++)
			if (sqlite3_column_name(stmt, i) == field_name)
			{
				type = sqlite3_column_type(stmt, i);
				switch (type)
				{
				case SQLITE_INTEGER:
					strResult = to_string(sqlite3_column_int(stmt, i));
					break;
				case SQLITE_FLOAT:
					strResult = to_string(sqlite3_column_double(stmt, i));
					break;
				case SQLITE_TEXT:
					strResult = (char*)sqlite3_column_text(stmt, i);
					break;
				default:
					strResult = "";
				}
			}
	return strResult;
}

bool sqlite_db_t::atob(string str)
{
    if (str == "Y" || str == "y" || str == "Yes" || str == "yes" || str == "1" || str == "t" || str == "T" || str == "True" || str == "true"
        || str == "true" || str == "FALSE")
        return true;
    else
        return false;
}
