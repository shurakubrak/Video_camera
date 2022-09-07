#pragma once
#include <string>
#include <sqlite3.h>

class sqlite_db_t
{
public:
	std::string dbname;
	sqlite3* connection;
	sqlite3_stmt* stmt;
	int rc;
	std::string sql_str;
	int row_number;
	int column_number;
	std::string field;

	sqlite_db_t()
	{
		stmt = NULL;
		connection = NULL;
		dbname = "";
		sql_str = "";
		row_number = 0;
		column_number = 0;
		rc = 0;
	};
	~sqlite_db_t() {}

	bool open_db();
	void close_db();
	bool exec_sql(bool SIU = false);
	void first_rec();
	bool next_rec();
	void last_rec();
	std::string field_by_name(std::string field_name);
    static bool atob(std::string str);
};

