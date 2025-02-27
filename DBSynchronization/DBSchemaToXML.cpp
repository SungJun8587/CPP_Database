
//***************************************************************************
// DBSchemaToXML.cpp: implementation of the CDBSchemaToXML class.
//
//***************************************************************************

#include "pch.h"
#include "DBSchemaToXML.h"

//***************************************************************************
// Construction/Destruction
//***************************************************************************

CDBSchemaToXML::CDBSchemaToXML(const CVector<DBModel::TableRef>& dbTables, const CVector<DBModel::ProcedureRef>& dbProcedures, const CVector<DBModel::FunctionRef>& dbFunctions)
	: _dbTables(dbTables), _dbProcedures(dbProcedures), _dbFunctions(dbFunctions) 
{ 
}

//***************************************************************************
//
void CDBSchemaToXML::ParseXmlToDB(const EDBClass dbClass, const _tstring path)
{
	CXMLNode		root;

	ASSERT_CRASH(_xmlUtil.ParseFromFile(path, OUT root));

	std::vector<CXMLNode> tables = root.FindChildren(_T("Table"));
	for( CXMLNode& table : tables )
	{
		DBModel::TableRef t = MakeShared<DBModel::Table>(dbClass);
		t->_schemaName = table.GetStringAttr(_T("schemaname"));
		t->_tableName = table.GetStringAttr(_T("name"));
		t->_tableComment = table.GetStringAttr(_T("desc"));
		t->_auto_increment_value = table.GetStringAttr(_T("auto_increment_value"));

		std::vector<CXMLNode> columns = table.FindChildren(_T("Column"));
		for( CXMLNode& column : columns )
		{
			DBModel::ColumnRef c = MakeShared<DBModel::Column>(dbClass);
			c->_tableName = t->_tableName;
			c->_seq = column.GetStringAttr(_T("seq"));
			c->_tableName = column.GetStringAttr(_T("name"));
			c->_columnComment = column.GetStringAttr(_T("desc"));
			c->_datatypedesc = column.GetStringAttr(_T("type"));

			EDataType columnType = StringToDBDataType(c->_datatypedesc.c_str(), OUT c->_maxLength);
			ASSERT_CRASH(columnType != EDataType::NONE);

			c->_datatype = _tstring(ToString(columnType));
			c->_nullable = !column.GetBoolAttr(_T("notnull"), false);

			const TCHAR* identityStr = column.GetStringAttr(_T("identity"));
			if( ::_tcslen(identityStr) > 0 )
			{
				_tregex pt(_T("(\\d+),(\\d+)"));
				_tcmatch match;
				ASSERT_CRASH(std::regex_match(identityStr, OUT match, pt));
				c->_identity = true;
				c->_seedValue = _ttoi(match[1].str().c_str());
				c->_incrementValue = _ttoi(match[2].str().c_str());
			}

			c->_defaultDefinition = column.GetStringAttr(_T("default"));
			t->_columns.push_back(c);
		}

		std::vector<CXMLNode> indexes = table.FindChildren(_T("Index"));
		for( CXMLNode& index : indexes )
		{
			DBModel::IndexRef i = MakeShared<DBModel::Index>(dbClass);
			i->_tableName = t->_tableName;
			i->_tableName = index.GetStringAttr(_T("name"));

			const TCHAR* kindStr = index.GetStringAttr(_T("kind"));
			i->_kind = StringToDBIndexKind(kindStr);

			const TCHAR* typeStr = index.GetStringAttr(_T("type"));
			i->_type = typeStr;

			i->_primaryKey = index.FindChild(_T("PrimaryKey")).IsValid();
			i->_uniqueKey = index.FindChild(_T("UniqueKey")).IsValid();
			i->_systemNamed = index.FindChild(_T("SystemNamed")).IsValid();

			std::vector<CXMLNode> columns = index.FindChildren(_T("Column"));
			for( CXMLNode& column : columns )
			{
				DBModel::IndexColumnRef c = MakeShared<DBModel::IndexColumn>();
				c->_seq = column.GetInt32Attr(_T("seq"), 0);
				c->_columnName = column.GetStringAttr(_T("name"));
				const TCHAR* indexOrderStr = column.GetStringAttr(_T("order"));
				if( ::_tcsicmp(indexOrderStr, _T("DESC")) == 0 )
					c->_sort = static_cast<EIndexSort>(2);
				else c->_sort = static_cast<EIndexSort>(1);
				i->_columns.push_back(c);
			}

			t->_indexes.push_back(i);
		}

		std::vector<CXMLNode> foreignKeys = table.FindChildren(_T("ForeignKey"));
		for( CXMLNode& foreignKey : foreignKeys )
		{
			DBModel::ForeignKeyRef fk = MakeShared<DBModel::ForeignKey>(dbClass);
			fk->_tableName = t->_tableName;
			fk->_foreignKeyName = foreignKey.GetStringAttr(_T("name"));
			fk->_updateRule = foreignKey.GetStringAttr(_T("update_rule"));
			fk->_deleteRule = foreignKey.GetStringAttr(_T("delete_rule"));

			CXMLNode foreignKeyTable = foreignKey.FindChild(_T("ForeignKeyTable"));
			fk->_foreignKeyTableName = foreignKeyTable.GetStringAttr(_T("name"));
			std::vector<CXMLNode> foreignKeyColumns = foreignKeyTable.FindChildren(_T("Column"));
			for( CXMLNode& foreignKeyColumn : foreignKeyColumns )
			{
				DBModel::IndexColumnRef c = MakeShared<DBModel::IndexColumn>();
				c->_columnName = foreignKeyColumn.GetStringAttr(_T("name"));
				fk->_foreignKeyColumns.push_back(c);
			}

			CXMLNode referenceKeyTable = foreignKey.FindChild(_T("ReferenceKeyTable"));
			fk->_referenceKeyTableName = referenceKeyTable.GetStringAttr(_T("name"));
			std::vector<CXMLNode> referenceKeyColumns = referenceKeyTable.FindChildren(_T("Column"));
			for( CXMLNode& referenceKeyColumn : referenceKeyColumns )
			{
				DBModel::IndexColumnRef c = MakeShared<DBModel::IndexColumn>();
				c->_columnName = referenceKeyColumn.GetStringAttr(_T("name"));
				fk->_referenceKeyColumns.push_back(c);
			}
			t->_foreignKeys.push_back(fk);
		}
		_xmlTables.push_back(t);
	}

	std::vector<CXMLNode> procedures = root.FindChildren(_T("Procedure"));
	for( CXMLNode& procedure : procedures )
	{
		DBModel::ProcedureRef p = MakeShared<DBModel::Procedure>();
		p->_schemaName = procedure.GetStringAttr(_T("schemaname"));
		p->_procName = procedure.GetStringAttr(_T("name"));
		p->_procComment = procedure.GetStringAttr(_T("desc"));
		p->_body = procedure.FindChild(_T("body")).GetStringValue();

		std::vector<CXMLNode> params = procedure.FindChildren(_T("Param"));
		for( CXMLNode& param : params )
		{
			DBModel::ProcParamRef procParam = MakeShared<DBModel::ProcParam>();
			procParam->_paramId = param.GetStringAttr(_T("seq"));
			procParam->_paramMode = StringToDBParamMode(param.GetStringAttr(_T("mode")));
			procParam->_paramName = param.GetStringAttr(_T("name"));
			procParam->_paramComment = param.GetStringAttr(_T("desc"));
			procParam->_datatypedesc = param.GetStringAttr(_T("type"));
			p->_parameters.push_back(procParam);
		}
		_xmlProcedures.push_back(p);
	}

	std::vector<CXMLNode> functions = root.FindChildren(_T("Function"));
	for( CXMLNode& function : functions )
	{
		DBModel::FunctionRef f = MakeShared<DBModel::Function>();
		f->_schemaName = function.GetStringAttr(_T("schemaname"));
		f->_funcName = function.GetStringAttr(_T("name"));
		f->_funcComment = function.GetStringAttr(_T("desc"));
		f->_body = function.FindChild(_T("body")).GetStringValue();

		std::vector<CXMLNode> params = function.FindChildren(_T("Param"));
		for( CXMLNode& param : params )
		{
			DBModel::FuncParamRef funcParam = MakeShared<DBModel::FuncParam>();
			funcParam->_paramId = param.GetStringAttr(_T("seq"));
			funcParam->_paramMode = StringToDBParamMode(param.GetStringAttr(_T("mode")));
			funcParam->_paramName = param.GetStringAttr(_T("name"));
			funcParam->_paramComment = param.GetStringAttr(_T("desc"));
			funcParam->_datatypedesc = param.GetStringAttr(_T("type"));
			f->_parameters.push_back(funcParam);
		}
		_xmlFunctions.push_back(f);
	}

	std::vector<CXMLNode> removedTables = root.FindChildren(_T("RemovedTable"));
	for( CXMLNode& removedTable : removedTables )
	{
		_xmlRemovedTables.insert(removedTable.GetStringAttr(_T("name")));
	}
}

//***************************************************************************
//
bool CDBSchemaToXML::DBToCreateXml(const _tstring path)
{
	// append xml declaration
	_xmlUtil.XMLDeclaration();

	// append root node
	rapidxml::xml_node<>* root = _xmlUtil.AddNode(&_xmlUtil.GetDocument(), _T("GameDB"));

	for( DBModel::TableRef& dbTable : _dbTables )
	{
		rapidxml::xml_node<>* table = _xmlUtil.AddNode(root, _T("Table"));
		_xmlUtil.AddAttribute(table, _T("schemaname"), dbTable->_schemaName);
		_xmlUtil.AddAttribute(table, _T("name"), dbTable->_tableName);
		_xmlUtil.AddAttribute(table, _T("desc"), dbTable->_tableComment);
		_xmlUtil.AddAttribute(table, _T("auto_increment_value"), dbTable->_auto_increment_value);

		for( DBModel::ColumnRef& dbColumn : dbTable->_columns )
		{
			rapidxml::xml_node<>* column = _xmlUtil.AddNode(table, _T("Column"));
			_xmlUtil.AddAttribute(column, _T("seq"), dbColumn->_seq);
			_xmlUtil.AddAttribute(column, _T("name"), dbColumn->_tableName);
			_xmlUtil.AddAttribute(column, _T("type"), dbColumn->_datatypedesc);
			_xmlUtil.AddAttribute(column, _T("notnull"), !dbColumn->_nullable ? _T("true") : _T("false"));
			if( dbColumn->_defaultDefinition.size() > 0 ) _xmlUtil.AddAttribute(column, _T("default"), dbColumn->_defaultDefinition);
			if( dbColumn->_identity ) _xmlUtil.AddAttribute(column, _T("identity"), dbColumn->_identitydesc);
			_xmlUtil.AddAttribute(column, _T("desc"), dbColumn->_columnComment);
		}

		for( DBModel::IndexRef& dbIndex : dbTable->_indexes )
		{
			rapidxml::xml_node<>* index = _xmlUtil.AddNode(table, _T("Index"));
			_xmlUtil.AddAttribute(index, _T("name"), dbIndex->_tableName);
			_xmlUtil.AddAttribute(index, _T("type"), dbIndex->_type);

			if( dbIndex->_primaryKey ) _xmlUtil.AddNode(index, _T("PrimaryKey"));
			if( dbIndex->_uniqueKey ) _xmlUtil.AddNode(index, _T("UniqueKey"));
			if( dbIndex->_systemNamed ) _xmlUtil.AddNode(index, _T("SystemNamed"));

			for( DBModel::IndexColumnRef& dbIndexColumn : dbIndex->_columns )
			{
				rapidxml::xml_node<>* column = _xmlUtil.AddNode(index, _T("Column"));
				_xmlUtil.AddAttribute(column, _T("seq"), dbIndexColumn->_seq);
				_xmlUtil.AddAttribute(column, _T("name"), dbIndexColumn->_columnName);
				_xmlUtil.AddAttribute(column, _T("order"), dbIndexColumn->GetSortText());
			}
		}

		for( DBModel::ForeignKeyRef& dbForeignKey : dbTable->_foreignKeys )
		{
			rapidxml::xml_node<>* referenceKey = _xmlUtil.AddNode(table, _T("ForeignKey"));
			_xmlUtil.AddAttribute(referenceKey, _T("name"), dbForeignKey->_foreignKeyName);
			_xmlUtil.AddAttribute(referenceKey, _T("update_rule"), dbForeignKey->_updateRule);
			_xmlUtil.AddAttribute(referenceKey, _T("delete_rule"), dbForeignKey->_deleteRule);

			rapidxml::xml_node<>* foreignKeyTable = _xmlUtil.AddNode(referenceKey, _T("ForeignKeyTable"));
			_xmlUtil.AddAttribute(foreignKeyTable, _T("name"), dbForeignKey->_foreignKeyTableName);
			for( DBModel::IndexColumnRef& dbForeignKeyColumn : dbForeignKey->_foreignKeyColumns )
			{
				rapidxml::xml_node<>* column = _xmlUtil.AddNode(foreignKeyTable, _T("Column"));
				_xmlUtil.AddAttribute(column, _T("name"), dbForeignKeyColumn->_columnName);
			}

			rapidxml::xml_node<>* referenceKeyTable = _xmlUtil.AddNode(referenceKey, _T("ReferenceKeyTable"));
			_xmlUtil.AddAttribute(referenceKeyTable, _T("name"), dbForeignKey->_referenceKeyTableName);
			for( DBModel::IndexColumnRef& dbReferenceKeyColumn : dbForeignKey->_referenceKeyColumns )
			{
				rapidxml::xml_node<>* column = _xmlUtil.AddNode(referenceKeyTable, _T("Column"));
				_xmlUtil.AddAttribute(column, _T("name"), dbReferenceKeyColumn->_columnName);
			}
		}
	}

	for( DBModel::ProcedureRef& dbProcedure : _dbProcedures )
	{
		rapidxml::xml_node<>* procedure = _xmlUtil.AddNode(root, _T("Procedure"));
		_xmlUtil.AddAttribute(procedure, _T("schemaname"), dbProcedure->_schemaName);
		_xmlUtil.AddAttribute(procedure, _T("name"), dbProcedure->_procName);
		_xmlUtil.AddAttribute(procedure, _T("desc"), dbProcedure->_procComment);
		for( DBModel::ProcParamRef& dbProcParam : dbProcedure->_parameters )
		{
			rapidxml::xml_node<>* procParam = _xmlUtil.AddNode(procedure, _T("Param"));
			_xmlUtil.AddAttribute(procParam, _T("seq"), dbProcParam->_paramId);
			_xmlUtil.AddAttribute(procParam, _T("mode"), ToString(dbProcParam->_paramMode));
			_xmlUtil.AddAttribute(procParam, _T("name"), dbProcParam->_paramName);
			_xmlUtil.AddAttribute(procParam, _T("type"), dbProcParam->_datatypedesc);
			_xmlUtil.AddAttribute(procParam, _T("desc"), dbProcParam->_paramComment);
		}
		_xmlUtil.AddCDataValue(_T("\n") + dbProcedure->_fullBody + _T("\n"), procedure, _T("Body"));
	}

	for( DBModel::FunctionRef& dbFunction : _dbFunctions )
	{
		rapidxml::xml_node<>* function = _xmlUtil.AddNode(root, _T("Function"));
		_xmlUtil.AddAttribute(function, _T("schemaname"), dbFunction->_schemaName);
		_xmlUtil.AddAttribute(function, _T("name"), dbFunction->_funcName);
		_xmlUtil.AddAttribute(function, _T("desc"), dbFunction->_funcComment);
		for( DBModel::FuncParamRef& dbFuncParam : dbFunction->_parameters )
		{
			rapidxml::xml_node<>* funcParam = _xmlUtil.AddNode(function, _T("Param"));
			_xmlUtil.AddAttribute(funcParam, _T("seq"), dbFuncParam->_paramId);
			_xmlUtil.AddAttribute(funcParam, _T("mode"), ToString(dbFuncParam->_paramMode));
			_xmlUtil.AddAttribute(funcParam, _T("name"), dbFuncParam->_paramName);
			_xmlUtil.AddAttribute(funcParam, _T("type"), dbFuncParam->_datatypedesc);
			_xmlUtil.AddAttribute(funcParam, _T("desc"), dbFuncParam->_paramComment);
		}
		_xmlUtil.AddCDataValue(_T("\n") + dbFunction->_fullBody + _T("\n"), function, _T("Body"));
	}

	_xmlUtil.SaveFile(path);

	return true;
}

