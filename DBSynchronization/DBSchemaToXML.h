
//***************************************************************************
// DBSchemaToXML.h : interface for the CDBSchemaToXML class.
//
//***************************************************************************

#ifndef __DBSCHEMATOXML_H__
#define __DBSCHEMATOXML_H__

#pragma once

#ifndef __CUSTOMALLOCATOR_H__
#include <Memory/CustomAllocator.h>
#endif

#ifndef __RAPIDXMLUTIL_H__
#include <XML/RapidXMLUtil.h>
#endif

class CDBSchemaToXML
{
public:
	CDBSchemaToXML(const CVector<DBModel::TableRef>& dbTables, const CVector<DBModel::ProcedureRef>& dbProcedures, const CVector<DBModel::FunctionRef>& dbFunctions);

	CVector<DBModel::TableRef>&		GetXMLTable()			{ return _xmlTables; }
	CVector<DBModel::ProcedureRef>&	GetXMLProcedure()		{ return _xmlProcedures; }
	CVector<DBModel::FunctionRef>&	GetXMLFunction()		{ return _xmlFunctions; }
	CSet<_tstring>&					GetXMLRemovedTable()	{ return _xmlRemovedTables; }

	void		ParseXmlToDB(const EDBClass dbClass, const _tstring path);
	bool		DBToCreateXml(const _tstring path);

private:
	CVector<DBModel::TableRef>		_dbTables;
	CVector<DBModel::ProcedureRef>	_dbProcedures;
	CVector<DBModel::FunctionRef>	_dbFunctions;

	CVector<DBModel::TableRef>		_xmlTables;
	CVector<DBModel::ProcedureRef>	_xmlProcedures;
	CVector<DBModel::FunctionRef>	_xmlFunctions;
	CSet<_tstring>					_xmlRemovedTables;

	CRapidXMLUtil					_xmlUtil;
};

#endif // ndef __DBSCHEMATOXML_H__