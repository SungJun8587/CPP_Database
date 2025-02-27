
//***************************************************************************
// DBSchemaToSync.h : interface for the Database Schema Synchronizer.
//
//***************************************************************************

#ifndef __DBSCHEMATOSYNC_H__
#define __DBSCHEMATOSYNC_H__

#pragma once

class CDBSchemaToSync
{
	enum UpdateStep : uint8
	{
		DropIndex,
		DropForeignKey,
		AlterColumn,
		AddColumn,
		CreateTable,
		DefaultConstraint,
		CreateIndex,
		CreateForeignKey,
		DropColumn,
		DropTable,
		DropStoredProcecure,
		CreateStoredProcecure,
		DropFunction,
		CreateFunction,

		Max
	};

	enum ColumnFlag : uint8
	{
		Type = 1 << 0,
		Nullable = 1 << 1,
		Identity = 1 << 2,
		Default = 1 << 3,
		Length = 1 << 4,
	};

public:
	CDBSchemaToSync(CBaseODBC& conn,
		const CVector<DBModel::TableRef>& dbTables, const CVector<DBModel::ProcedureRef>& dbProcedures, const CVector<DBModel::FunctionRef>& dbFunctions,
		const CVector<DBModel::TableRef>& xmlTables, const CVector<DBModel::ProcedureRef>& xmlProcedures, const CVector<DBModel::FunctionRef>& xmlFunctions,
		const CSet<_tstring>& xmlRemovedTables);
	~CDBSchemaToSync();

	EDBClass	GetDBClass() { return _dbClass; }

	void		Synchronize();

private:
	void		CompareDBModel();
	void		CompareTables(DBModel::TableRef dbTable, DBModel::TableRef xmlTable);
	void		CompareColumns(DBModel::TableRef dbTable, DBModel::ColumnRef dbColumn, DBModel::ColumnRef xmlColumn);
	void		CompareStoredProcedures();
	void		CompareFunctions();
	void		ExecuteUpdateQueries();

private:
	EDBClass	_dbClass;
	CBaseODBC&	_dbConn;

	CVector<DBModel::TableRef>			_xmlTables;
	CVector<DBModel::ProcedureRef>		_xmlProcedures;
	CVector<DBModel::FunctionRef>		_xmlFunctions;
	CSet<_tstring>						_xmlRemovedTables;

	CVector<DBModel::TableRef>			_dbTables;
	CVector<DBModel::ProcedureRef>		_dbProcedures;
	CVector<DBModel::FunctionRef>		_dbFunctions;

	CSet<_tstring>						_dependentIndexes;
	CSet<_tstring>						_dependentReferenceKeys;
	CVector<_tstring>					_updateQueries[UpdateStep::Max];
};

#endif // ndef __DBSCHEMATOSYNC_H__