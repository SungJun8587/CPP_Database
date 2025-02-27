
//***************************************************************************
// DBSchemaToExcel.h : interface for the CDBSchemaToExcel class.
//
//***************************************************************************

#ifndef __DBSCHEMATOEXCEL_H__
#define __DBSCHEMATOEXCEL_H__

#pragma once

#ifndef __CUSTOMALLOCATOR_H__
#include <Memory/CustomAllocator.h>
#endif

#ifndef __XLNTUTIL_H__
#include <Excel/XlntUtil.h>
#endif

namespace ExcelColumnWidth
{
	const int cnDefaultWidth = 16;
	const int cnSchemaName = 18;
	const int cnObjectName = 30;
	const int cnColumnName = 25;
	const int cnSequence = 20;
	const int cnParamMode = 22;
	const int cnParamName = 25;
	const int cnConstraintName = 38;
	const int cnIndexType = 33;
	const int cnPartitionName = 25;
	const int cnConstraintValue = 50;
	const int cnDefaultConstraintValue = 24;
	const int cnSystemNamed = 22;
	const int cnComment = 70;
	const int cnDataType = 15;
	const int cnDataTypeDesc = 21;
	const int cnEngine = 18;
	const int cnCharacterSet = 20;
	const int cnCollation = 26;
	const int cnDateTime = 21;
}

class CDBSchemaToExcel
{
public:
	CDBSchemaToExcel(const CVector<DBModel::TableRef>& dbTables);

	void        ToHeaderStyle(const std::string& start_cell, const std::string& end_cell);

	void		AddExcelTableInfo();
	void		AddExcelTableColumnInfo();
	void		AddExcelConstraintsInfo();
	void		AddExcelIndexInfo();
	void		AddExcelForeignKeyInfo();
	void		AddExcelCheckConstraintsInfo();

	void		AddExcelMSSQLTableIndexOptionInfo();
	void		AddExcelMSSQLDefaultConstraintsInfo();

	void		AddExcelORACLEIdentityColumnInfo();

	void		SaveExcelFile(const EDBClass dbClass, const _tstring path);

private:
	CVector<DBModel::TableRef>	_dbTables;
	Xlnt::CXlntUtil				_excel;
};

#endif // ndef __DBSCHEMATOEXCEL_H__