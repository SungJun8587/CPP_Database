
//***************************************************************************
// DBSchemaToExcel.cpp: implementation of the CDBSchemaToExcel class.
//
//***************************************************************************

#include "pch.h"
#include "DBSchemaToExcel.h"

//***************************************************************************
// Construction/Destruction
//***************************************************************************

CDBSchemaToExcel::CDBSchemaToExcel(const CVector<DBModel::TableRef>& dbTables) : _dbTables(dbTables)
{ 
}

//***************************************************************************
//
void CDBSchemaToExcel::ToHeaderStyle(const std::string& start_cell, const std::string& end_cell)
{
	_excel.SetRowHeight(1, 20);

	auto borderRange = _excel.CreateRange(start_cell, end_cell);

	// 배경색 설정
	xlnt::fill header_fill;
	header_fill = xlnt::pattern_fill().type(xlnt::pattern_fill_type::solid);
	header_fill = header_fill.pattern_fill().foreground(xlnt::color::black());
	header_fill = header_fill.pattern_fill().background(xlnt::color::black());

	// 테두리 설정
	xlnt::border cell_border;
	xlnt::border::border_property outer_prop;
	outer_prop.style(xlnt::border_style::thin);

	cell_border.side(xlnt::border_side::bottom, outer_prop);
	cell_border.side(xlnt::border_side::start, outer_prop);
	cell_border.side(xlnt::border_side::top, outer_prop);
	cell_border.side(xlnt::border_side::end, outer_prop);

	// 폰트 스타일 설정
	xlnt::font title_font;
	title_font.name("Gulim");					// 폰트 패밀리
	title_font.bold(true);						// 폰트 굵게
	title_font.size(10);						// 폰트 크기
	title_font.color(xlnt::color::white());		// 폰트 색깔

	// 정렬 설정
	xlnt::alignment center_align;
	center_align.horizontal(xlnt::horizontal_alignment::center);
	center_align.vertical(xlnt::vertical_alignment::center);

	borderRange.border(cell_border);
	borderRange.fill(header_fill);
	borderRange.font(title_font);
	borderRange.alignment(center_align);
}

//***************************************************************************
//
void CDBSchemaToExcel::AddExcelTableInfo()
{
	int32 currentRow = 1;

	// 활성화 되어있는 기존 시트명('Sheet1')을 다른 시트명('TABLE')로 변경
	_excel.RenameSheet("TABLE");
	ToHeaderStyle("A1", "I1");

	_excel.WriteCell(currentRow, 1, "SCHEMA_NAME");
	_excel.WriteCell(currentRow, 2, "TABLE_NAME");
	_excel.WriteCell(currentRow, 3, "IDENTITY");
	_excel.WriteCell(currentRow, 4, "CHARACTER_SET");
	_excel.WriteCell(currentRow, 5, "COLLATION");
	_excel.WriteCell(currentRow, 6, "ENGINE");
	_excel.WriteCell(currentRow, 7, "TABLE_COMMENT");
	_excel.WriteCell(currentRow, 8, "CREATE_DATE");
	_excel.WriteCell(currentRow, 9, "MODIFY_DATE");

	for( DBModel::TableRef& dbTable : _dbTables )
	{
		currentRow++;
		_excel.WriteCell(currentRow, 1, dbTable->_schemaName);
		_excel.WriteCell(currentRow, 2, dbTable->_tableName);
		_excel.WriteCell(currentRow, 3, dbTable->_auto_increment_value);
		_excel.WriteCell(currentRow, 4, dbTable->_characterset);
		_excel.WriteCell(currentRow, 5, dbTable->_collation);
		_excel.WriteCell(currentRow, 6, dbTable->_storageEngine);
		_excel.WriteCell(currentRow, 7, dbTable->_tableComment, true);
		_excel.WriteCell(currentRow, 8, dbTable->_createDate);
		_excel.WriteCell(currentRow, 9, dbTable->_modifyDate);
		_excel.SetRowHeight(currentRow, 16);
	}

	_excel.SetAllCellTextFormat(3);

	_excel.SetColumnWidth(1, ExcelColumnWidth::cnSchemaName);
	_excel.SetColumnWidth(2, ExcelColumnWidth::cnObjectName);
	_excel.SetColumnWidth(3, ExcelColumnWidth::cnDefaultWidth);
	_excel.SetColumnWidth(4, ExcelColumnWidth::cnCharacterSet);
	_excel.SetColumnWidth(5, ExcelColumnWidth::cnCollation);
	_excel.SetColumnWidth(6, ExcelColumnWidth::cnEngine);
	_excel.SetColumnWidth(7, ExcelColumnWidth::cnComment);
	_excel.SetColumnWidth(8, ExcelColumnWidth::cnDateTime);
	_excel.SetColumnWidth(9, ExcelColumnWidth::cnDateTime);
}

//***************************************************************************
//
void CDBSchemaToExcel::AddExcelTableColumnInfo()
{
	int32 currentRow = 1;

	_excel.AddSheet("TABLE_COLUMN");
	ToHeaderStyle("A1", "Q1");

	_excel.WriteCell(currentRow, 1, "SCHEMA_NAME");
	_excel.WriteCell(currentRow, 2, "TABLE_NAME");
	_excel.WriteCell(currentRow, 3, "COLUMN_SEQ");
	_excel.WriteCell(currentRow, 4, "COLUMN_NAME");
	_excel.WriteCell(currentRow, 5, "DATATYPE");
	_excel.WriteCell(currentRow, 6, "MAX_LENGTH");
	_excel.WriteCell(currentRow, 7, "PRECISION");
	_excel.WriteCell(currentRow, 8, "SCALE");
	_excel.WriteCell(currentRow, 9, "DATATYPEDESC");
	_excel.WriteCell(currentRow, 10, "IS_NULLABLE");
	_excel.WriteCell(currentRow, 11, "DEFAULT_DEFINITION");
	_excel.WriteCell(currentRow, 12, "IS_IDENTITY");
	_excel.WriteCell(currentRow, 13, "SEED_VALUE");
	_excel.WriteCell(currentRow, 14, "INC_VALUE");
	_excel.WriteCell(currentRow, 15, "CHARACTER_SET");
	_excel.WriteCell(currentRow, 16, "COLLATION");
	_excel.WriteCell(currentRow, 17, "COLUMN_COMMENT");

	for( DBModel::TableRef& dbTable : _dbTables )
	{
		for( DBModel::ColumnRef& dbColumn : dbTable->_columns )
		{
			currentRow++;
			_excel.WriteCell(currentRow, 1, dbColumn->_schemaName);
			_excel.WriteCell(currentRow, 2, dbColumn->_tableName);
			_excel.WriteCell(currentRow, 3, dbColumn->_seq);
			_excel.WriteCell(currentRow, 4, dbColumn->_columnName);
			_excel.WriteCell(currentRow, 5, dbColumn->_datatype);
			_excel.WriteCell(currentRow, 6, std::to_string(dbColumn->_maxLength));
			_excel.WriteCell(currentRow, 7, std::to_string(dbColumn->_precision));
			_excel.WriteCell(currentRow, 8, std::to_string(dbColumn->_scale));
			_excel.WriteCell(currentRow, 9, dbColumn->_datatypedesc);
			_excel.WriteCell(currentRow, 10, std::to_string(dbColumn->_nullable));
			_excel.WriteCell(currentRow, 11, dbColumn->_defaultConstraintName);
			_excel.WriteCell(currentRow, 12, dbColumn->_identitydesc);
			_excel.WriteCell(currentRow, 13, std::to_string(dbColumn->_seedValue));
			_excel.WriteCell(currentRow, 14, std::to_string(dbColumn->_incrementValue));
			_excel.WriteCell(currentRow, 15, dbColumn->_characterset);
			_excel.WriteCell(currentRow, 16, dbColumn->_collation);
			_excel.WriteCell(currentRow, 17, dbColumn->_columnComment, true);
			_excel.SetRowHeight(currentRow, 16);
		}
	}

	_excel.SetColumnWidth(1, ExcelColumnWidth::cnSchemaName);
	_excel.SetColumnWidth(2, ExcelColumnWidth::cnObjectName);
	_excel.SetColumnWidth(3, ExcelColumnWidth::cnSequence);
	_excel.SetColumnWidth(4, ExcelColumnWidth::cnColumnName);
	_excel.SetColumnWidth(5, ExcelColumnWidth::cnDataType);
	_excel.SetColumnWidth(6, ExcelColumnWidth::cnDefaultWidth);
	_excel.SetColumnWidth(7, ExcelColumnWidth::cnDefaultWidth);
	_excel.SetColumnWidth(8, ExcelColumnWidth::cnDefaultWidth);
	_excel.SetColumnWidth(9, ExcelColumnWidth::cnDataTypeDesc);
	_excel.SetColumnWidth(10, ExcelColumnWidth::cnDefaultWidth);
	_excel.SetColumnWidth(11, ExcelColumnWidth::cnDefaultConstraintValue);
	_excel.SetColumnWidth(12, ExcelColumnWidth::cnDefaultWidth);
	_excel.SetColumnWidth(13, ExcelColumnWidth::cnDefaultWidth);
	_excel.SetColumnWidth(14, ExcelColumnWidth::cnDefaultWidth);
	_excel.SetColumnWidth(15, ExcelColumnWidth::cnCharacterSet);
	_excel.SetColumnWidth(16, ExcelColumnWidth::cnCollation);
	_excel.SetColumnWidth(17, ExcelColumnWidth::cnComment);
}

//***************************************************************************
//
void CDBSchemaToExcel::AddExcelConstraintsInfo()
{
	int32 currentRow = 1;

	_excel.AddSheet("TABLE_CONSTRAINTS");
	ToHeaderStyle("A1", "H1");

	_excel.WriteCell(currentRow, 1, "SCHEMA_NAME");
	_excel.WriteCell(currentRow, 2, "TABLE_NAME");
	_excel.WriteCell(currentRow, 3, "CONSTRAINT_NAME");
	_excel.WriteCell(currentRow, 4, "CONSTRAINT_TYPE");
	_excel.WriteCell(currentRow, 5, "CONSTRAINT_TYPE_DESC");
	_excel.WriteCell(currentRow, 6, "CONST_VALUE");
	_excel.WriteCell(currentRow, 7, "IS_SYSTEM_NAMED");
	_excel.WriteCell(currentRow, 8, "IS_STATUS");

	for( DBModel::TableRef& dbTable : _dbTables )
	{
		for( DBModel::ConstraintRef& dbConstraint : dbTable->_constraints )
		{
			currentRow++;
			_excel.WriteCell(currentRow, 1, dbConstraint->_schemaName);
			_excel.WriteCell(currentRow, 2, dbConstraint->_tableName);
			_excel.WriteCell(currentRow, 3, dbConstraint->_constName);
			_excel.WriteCell(currentRow, 4, dbConstraint->_constType);
			_excel.WriteCell(currentRow, 5, dbConstraint->_constTypeDesc);
			_excel.WriteCell(currentRow, 6, dbConstraint->_constValue);
			_excel.WriteCell(currentRow, 7, std::to_string(dbConstraint->_systemNamed));
			_excel.WriteCell(currentRow, 8, std::to_string(dbConstraint->_status));
			_excel.SetRowHeight(currentRow, 16);
		}
	}

	_excel.SetColumnWidth(1, ExcelColumnWidth::cnSchemaName);
	_excel.SetColumnWidth(2, ExcelColumnWidth::cnObjectName);
	_excel.SetColumnWidth(3, ExcelColumnWidth::cnConstraintName);
	_excel.SetColumnWidth(4, ExcelColumnWidth::cnDefaultWidth + 6);
	_excel.SetColumnWidth(5, ExcelColumnWidth::cnDefaultWidth + 14);
	_excel.SetColumnWidth(6, ExcelColumnWidth::cnConstraintValue);
	_excel.SetColumnWidth(7, ExcelColumnWidth::cnSystemNamed);
	_excel.SetColumnWidth(8, ExcelColumnWidth::cnDefaultWidth);
}

//***************************************************************************
//
void CDBSchemaToExcel::AddExcelIndexInfo()
{
	int32 currentRow = 1;

	_excel.AddSheet("TABLE_INDEX");
	ToHeaderStyle("A1", "J1");

	_excel.WriteCell(currentRow, 1, "SCHEMA_NAME");
	_excel.WriteCell(currentRow, 2, "TABLE_NAME");
	_excel.WriteCell(currentRow, 3, "INDEX_NAME");
	_excel.WriteCell(currentRow, 4, "INDEX_TYPE");
	_excel.WriteCell(currentRow, 5, "IS_PRIMARYKEY");
	_excel.WriteCell(currentRow, 6, "IS_UNIQUE");
	_excel.WriteCell(currentRow, 7, "IS_SYSTEM_NAMED");
	_excel.WriteCell(currentRow, 8, "COLUMN_SEQ");
	_excel.WriteCell(currentRow, 9, "COLUMN_NAME");
	_excel.WriteCell(currentRow, 10, "COLUMN_SORT");

	for( DBModel::TableRef& dbTable : _dbTables )
	{
		for( DBModel::IndexRef& dbIndex : dbTable->_indexes )
		{
			for( DBModel::IndexColumnRef& dbIndexColumn : dbIndex->_columns )
			{
				currentRow++;
				_excel.WriteCell(currentRow, 1, dbIndex->_schemaName);
				_excel.WriteCell(currentRow, 2, dbIndex->_tableName);
				_excel.WriteCell(currentRow, 3, dbIndex->_indexName);
				_excel.WriteCell(currentRow, 4, dbIndex->_type);
				_excel.WriteCell(currentRow, 5, std::to_string(dbIndex->_primaryKey));
				_excel.WriteCell(currentRow, 6, std::to_string(dbIndex->_uniqueKey));
				_excel.WriteCell(currentRow, 7, std::to_string(dbIndex->_systemNamed));
				_excel.WriteCell(currentRow, 8, dbIndexColumn->_seq);
				_excel.WriteCell(currentRow, 9, dbIndexColumn->_columnName);
				_excel.WriteCell(currentRow, 10, ToString(dbIndexColumn->_sort));
				_excel.SetRowHeight(currentRow, 16);
			}
		}
	}

	_excel.SetColumnWidth(1, ExcelColumnWidth::cnSchemaName);
	_excel.SetColumnWidth(2, ExcelColumnWidth::cnObjectName);
	_excel.SetColumnWidth(3, ExcelColumnWidth::cnConstraintName);
	_excel.SetColumnWidth(4, ExcelColumnWidth::cnIndexType);
	_excel.SetColumnWidth(5, ExcelColumnWidth::cnDefaultWidth + 2);
	_excel.SetColumnWidth(6, ExcelColumnWidth::cnDefaultWidth);
	_excel.SetColumnWidth(7, ExcelColumnWidth::cnSystemNamed);
	_excel.SetColumnWidth(8, ExcelColumnWidth::cnSequence);
	_excel.SetColumnWidth(9, ExcelColumnWidth::cnColumnName);
	_excel.SetColumnWidth(10, ExcelColumnWidth::cnDefaultWidth);
}

//***************************************************************************
//
void CDBSchemaToExcel::AddExcelForeignKeyInfo()
{
	int32 currentRow = 1;

	_excel.AddSheet("TABLE_FOREIGNKEY");
	ToHeaderStyle("A1", "M1");

	_excel.WriteCell(currentRow, 1, "SCHEMA_NAME");
	_excel.WriteCell(currentRow, 2, "TABLE_NAME");
	_excel.WriteCell(currentRow, 3, "FOREIGNKEY_NAME");
	_excel.WriteCell(currentRow, 4, "IS_DISABLED");
	_excel.WriteCell(currentRow, 5, "IS_NOT_TRUSTED");
	_excel.WriteCell(currentRow, 6, "FKEY_TABLE_NAME");
	_excel.WriteCell(currentRow, 7, "FKEY_COLUMN_NAME");
	_excel.WriteCell(currentRow, 8, "RKEY_SCHEMA_NAME");
	_excel.WriteCell(currentRow, 9, "RKEY_TABLE_NAME");
	_excel.WriteCell(currentRow, 10, "RKEY_COLUMN_NAME");
	_excel.WriteCell(currentRow, 11, "UPDATE_RULE");
	_excel.WriteCell(currentRow, 12, "DELETE_RULE");
	_excel.WriteCell(currentRow, 13, "IS_SYSTEM_NAMED");

	for( DBModel::TableRef& dbTable : _dbTables )
	{
		for( DBModel::ForeignKeyRef& dbForeignKey : dbTable->_foreignKeys )
		{
			int columnIndex = 0;
			for( DBModel::IndexColumnRef& dbForeignKeyColumn : dbForeignKey->_foreignKeyColumns )
			{
				currentRow++;
				_excel.WriteCell(currentRow, 1, dbForeignKey->_schemaName);
				_excel.WriteCell(currentRow, 2, dbForeignKey->_tableName);
				_excel.WriteCell(currentRow, 3, dbForeignKey->_foreignKeyName);
				_excel.WriteCell(currentRow, 4, std::to_string(dbForeignKey->_isDisabled));
				_excel.WriteCell(currentRow, 5, std::to_string(dbForeignKey->_isNotTrusted));
				_excel.WriteCell(currentRow, 6, dbForeignKey->_foreignKeyTableName);
				_excel.WriteCell(currentRow, 7, dbForeignKeyColumn->_columnName);
				_excel.WriteCell(currentRow, 8, dbForeignKey->_referenceKeySchemaName);
				_excel.WriteCell(currentRow, 9, dbForeignKey->_referenceKeyTableName);
				_excel.WriteCell(currentRow, 10, dbForeignKey->_referenceKeyColumns[columnIndex++]->_columnName);
				_excel.WriteCell(currentRow, 11, dbForeignKey->_updateRule);
				_excel.WriteCell(currentRow, 12, dbForeignKey->_deleteRule);
				_excel.WriteCell(currentRow, 13, std::to_string(dbForeignKey->_systemNamed));
				_excel.SetRowHeight(currentRow, 16);
			}
		}
	}

	_excel.SetColumnWidth(1, ExcelColumnWidth::cnSchemaName);
	_excel.SetColumnWidth(2, ExcelColumnWidth::cnObjectName);
	_excel.SetColumnWidth(3, ExcelColumnWidth::cnConstraintName);
	_excel.SetColumnWidth(4, ExcelColumnWidth::cnDefaultWidth);
	_excel.SetColumnWidth(5, ExcelColumnWidth::cnDefaultWidth + 2);
	_excel.SetColumnWidth(6, ExcelColumnWidth::cnObjectName);
	_excel.SetColumnWidth(7, ExcelColumnWidth::cnColumnName);
	_excel.SetColumnWidth(8, ExcelColumnWidth::cnSchemaName + 4);
	_excel.SetColumnWidth(9, ExcelColumnWidth::cnObjectName);
	_excel.SetColumnWidth(10, ExcelColumnWidth::cnColumnName);
	_excel.SetColumnWidth(11, ExcelColumnWidth::cnDefaultWidth);
	_excel.SetColumnWidth(12, ExcelColumnWidth::cnDefaultWidth);
	_excel.SetColumnWidth(13, ExcelColumnWidth::cnSystemNamed);
}

//***************************************************************************
//
void CDBSchemaToExcel::AddExcelCheckConstraintsInfo()
{
	int32 currentRow = 1;

	_excel.AddSheet("TABLE_CHECK_CONSTRAINT");
	ToHeaderStyle("A1", "E1");

	_excel.WriteCell(currentRow, 1, "SCHEMA_NAME");
	_excel.WriteCell(currentRow, 2, "TABLE_NAME");
	_excel.WriteCell(currentRow, 3, "CONST_NAME");
	_excel.WriteCell(currentRow, 4, "CHECK_VALUE");
	_excel.WriteCell(currentRow, 5, "IS_SYSTEM_NAMED");

	for( DBModel::TableRef& dbTable : _dbTables )
	{
		for( DBModel::CheckConstraintRef& dbCheckConstraint : dbTable->_checkConstraints )
		{
			currentRow++;
			_excel.WriteCell(currentRow, 1, dbCheckConstraint->_schemaName);
			_excel.WriteCell(currentRow, 2, dbCheckConstraint->_tableName);
			_excel.WriteCell(currentRow, 3, dbCheckConstraint->_checkConstName);
			_excel.WriteCell(currentRow, 4, dbCheckConstraint->_checkValue);
			_excel.WriteCell(currentRow, 5, std::to_string(dbCheckConstraint->_systemNamed));
			_excel.SetRowHeight(currentRow, 16);
		}
	}

	_excel.SetColumnWidth(1, ExcelColumnWidth::cnSchemaName);
	_excel.SetColumnWidth(2, ExcelColumnWidth::cnObjectName);
	_excel.SetColumnWidth(3, ExcelColumnWidth::cnConstraintName);
	_excel.SetColumnWidth(4, ExcelColumnWidth::cnConstraintValue);
	_excel.SetColumnWidth(5, ExcelColumnWidth::cnSystemNamed);
}

//***************************************************************************
//
void CDBSchemaToExcel::AddExcelMSSQLTableIndexOptionInfo()
{
	int32 currentRow = 1;

	_excel.AddSheet("TABLE_INDEX_OPTION");
	ToHeaderStyle("A1", "W1");

	_excel.WriteCell(currentRow, 1, "SCHEMA_NAME");
	_excel.WriteCell(currentRow, 2, "TABLE_NAME");
	_excel.WriteCell(currentRow, 3, "INDEX_NAME");
	_excel.WriteCell(currentRow, 4, "IS_PRIMARYKEY");
	_excel.WriteCell(currentRow, 5, "IS_UNIQUE");
	_excel.WriteCell(currentRow, 6, "IS_DISABLED");
	_excel.WriteCell(currentRow, 7, "IS_PADDED");
	_excel.WriteCell(currentRow, 8, "FILLFACTOR");
	_excel.WriteCell(currentRow, 9, "IS_IGNORE_DUP_KEY");
	_excel.WriteCell(currentRow, 10, "IS_ALLOW_ROW_LOCKS");
	_excel.WriteCell(currentRow, 11, "IS_ALLOW_PAGE_LOCKS");
	_excel.WriteCell(currentRow, 12, "IS_HASFILTER");
	_excel.WriteCell(currentRow, 13, "FILTER_DEFINITION");
	_excel.WriteCell(currentRow, 14, "COMPRESSION_DELAY");
	_excel.WriteCell(currentRow, 15, "OPTIMIZE_FOR_SEQUENTIAL_KEY");
	_excel.WriteCell(currentRow, 16, "IS_STATISTICS_NORECOMPUTE");
	_excel.WriteCell(currentRow, 17, "IS_STATISTICS_INCREMENTAL");
	_excel.WriteCell(currentRow, 18, "DATA_COMPRESSION");
	_excel.WriteCell(currentRow, 19, "DATA_COMPRESSION_DESC");
	_excel.WriteCell(currentRow, 20, "XML_COMPRESSION");
	_excel.WriteCell(currentRow, 21, "XML_COMPRESSION_DESC");
	_excel.WriteCell(currentRow, 22, "DATA_SPACENAME");
	_excel.WriteCell(currentRow, 23, "DATA_SPACENAME_DESC");

	for( DBModel::TableRef& dbTable : _dbTables )
	{
		for( DBModel::IndexOptionRef& dbIndexOption : dbTable->_indexOptions )
		{
			currentRow++;
			_excel.WriteCell(currentRow, 1, dbIndexOption->_schemaName);
			_excel.WriteCell(currentRow, 2, dbIndexOption->_tableName);
			_excel.WriteCell(currentRow, 3, dbIndexOption->_indexName);
			_excel.WriteCell(currentRow, 4, std::to_string(dbIndexOption->_primaryKey));
			_excel.WriteCell(currentRow, 5, std::to_string(dbIndexOption->_uniqueKey));
			_excel.WriteCell(currentRow, 6, std::to_string(dbIndexOption->_isDisabled));
			_excel.WriteCell(currentRow, 7, std::to_string(dbIndexOption->_isPadded));
			_excel.WriteCell(currentRow, 8, std::to_string(dbIndexOption->_fillFactor));
			_excel.WriteCell(currentRow, 9, std::to_string(dbIndexOption->_ignoreDupKey));
			_excel.WriteCell(currentRow, 10, std::to_string(dbIndexOption->_allowRowLocks));
			_excel.WriteCell(currentRow, 11, std::to_string(dbIndexOption->_allowPageLocks));
			_excel.WriteCell(currentRow, 12, std::to_string(dbIndexOption->_hasFilter));
			_excel.WriteCell(currentRow, 13, dbIndexOption->_filterDefinition);
			_excel.WriteCell(currentRow, 14, std::to_string(dbIndexOption->_compressionDelay));
			_excel.WriteCell(currentRow, 15, dbIndexOption->_optimizeForSequentialKey);
			_excel.WriteCell(currentRow, 16, std::to_string(dbIndexOption->_statisticsNoRecompute));
			_excel.WriteCell(currentRow, 17, std::to_string(dbIndexOption->_statisticsIncremental));
			_excel.WriteCell(currentRow, 18, std::to_string(dbIndexOption->_dataCompression));
			_excel.WriteCell(currentRow, 19, dbIndexOption->_dataCompressionDesc);
			_excel.WriteCell(currentRow, 20, std::to_string(dbIndexOption->_xmlCompression));
			_excel.WriteCell(currentRow, 21, dbIndexOption->_xmlCompressionDesc);
			_excel.WriteCell(currentRow, 22, dbIndexOption->_fileGroupOrPartitionScheme);
			_excel.WriteCell(currentRow, 23, dbIndexOption->_fileGroupOrPartitionSchemeName);

			_excel.SetRowHeight(currentRow, 16);
		}
	}

	_excel.SetColumnWidth(1, ExcelColumnWidth::cnSchemaName);
	_excel.SetColumnWidth(2, ExcelColumnWidth::cnObjectName);
	_excel.SetColumnWidth(3, ExcelColumnWidth::cnConstraintName);
	_excel.SetColumnWidth(4, ExcelColumnWidth::cnDefaultWidth + 1);
	_excel.SetColumnWidth(5, ExcelColumnWidth::cnDefaultWidth + 1);
	_excel.SetColumnWidth(6, ExcelColumnWidth::cnDefaultWidth);
	_excel.SetColumnWidth(7, ExcelColumnWidth::cnDefaultWidth);
	_excel.SetColumnWidth(8, ExcelColumnWidth::cnDefaultWidth + 1);
	_excel.SetColumnWidth(9, ExcelColumnWidth::cnDefaultWidth + 7);
	_excel.SetColumnWidth(10, ExcelColumnWidth::cnDefaultWidth + 9);
	_excel.SetColumnWidth(11, ExcelColumnWidth::cnDefaultWidth + 9);
	_excel.SetColumnWidth(12, ExcelColumnWidth::cnDefaultWidth);
	_excel.SetColumnWidth(13, ExcelColumnWidth::cnDefaultWidth + 4);
	_excel.SetColumnWidth(14, ExcelColumnWidth::cnDefaultWidth + 4);
	_excel.SetColumnWidth(15, ExcelColumnWidth::cnDefaultWidth + 19);
	_excel.SetColumnWidth(16, ExcelColumnWidth::cnDefaultWidth + 17);
	_excel.SetColumnWidth(17, ExcelColumnWidth::cnDefaultWidth + 17);
	_excel.SetColumnWidth(18, ExcelColumnWidth::cnDefaultWidth + 7);
	_excel.SetColumnWidth(19, ExcelColumnWidth::cnDefaultWidth + 13);
	_excel.SetColumnWidth(20, ExcelColumnWidth::cnDefaultWidth + 7);
	_excel.SetColumnWidth(21, ExcelColumnWidth::cnDefaultWidth + 13);
	_excel.SetColumnWidth(22, ExcelColumnWidth::cnDefaultWidth + 7);
	_excel.SetColumnWidth(23, ExcelColumnWidth::cnDefaultWidth + 11);
}

//***************************************************************************
//
void CDBSchemaToExcel::AddExcelORACLEIdentityColumnInfo()
{
	int32 currentRow = 1;

	_excel.AddSheet("TABLE_IDENTITY_COLUMN");
	ToHeaderStyle("A1", "S1");

	_excel.WriteCell(currentRow, 1, "SCHEMA_NAME");
	_excel.WriteCell(currentRow, 2, "TABLE_NAME");
	_excel.WriteCell(currentRow, 3, "COLUMN_NAME");
	_excel.WriteCell(currentRow, 4, "IDENTITY_COLUMN");
	_excel.WriteCell(currentRow, 5, "DEFAULT_ON_NULL");
	_excel.WriteCell(currentRow, 6, "GENERATION_TYPE");
	_excel.WriteCell(currentRow, 7, "SEQUENCE_NAME");
	_excel.WriteCell(currentRow, 8, "MIN_VALUE");
	_excel.WriteCell(currentRow, 9, "MAX_VALUE");
	_excel.WriteCell(currentRow, 10, "INCREMENT_BY");
	_excel.WriteCell(currentRow, 11, "CYCLE_FLAG");
	_excel.WriteCell(currentRow, 12, "ORDER_FLAG");
	_excel.WriteCell(currentRow, 13, "CACHE_SIZE");
	_excel.WriteCell(currentRow, 14, "LAST_NUMBER");
	_excel.WriteCell(currentRow, 15, "SCALE_FLAG");
	_excel.WriteCell(currentRow, 16, "EXTEND_FLAG");
	_excel.WriteCell(currentRow, 17, "SHARDED_FLAG");
	_excel.WriteCell(currentRow, 18, "SESSION_FLAG");
	_excel.WriteCell(currentRow, 19, "KEEP_VALUE");

	for( DBModel::TableRef& dbTable : _dbTables )
	{
		for( DBModel::IdentityColumnRef& dbIdentityColumn : dbTable->_identityColumns )
		{
			currentRow++;
			_excel.WriteCell(currentRow, 1, dbIdentityColumn->_schemaName);
			_excel.WriteCell(currentRow, 2, dbIdentityColumn->_tableName);
			_excel.WriteCell(currentRow, 3, dbIdentityColumn->_columnName);
			_excel.WriteCell(currentRow, 4, dbIdentityColumn->_identityColumn);
			_excel.WriteCell(currentRow, 5, dbIdentityColumn->_defaultOnNull);
			_excel.WriteCell(currentRow, 6, dbIdentityColumn->_generationType);
			_excel.WriteCell(currentRow, 7, dbIdentityColumn->_sequenceName);
			_excel.WriteCell(currentRow, 8, std::to_string(dbIdentityColumn->_minValue));
			_excel.WriteCell(currentRow, 9, std::to_string(dbIdentityColumn->_maxValue));
			_excel.WriteCell(currentRow, 10, std::to_string(dbIdentityColumn->_incrementBy));
			_excel.WriteCell(currentRow, 11, dbIdentityColumn->_cycleFlag);
			_excel.WriteCell(currentRow, 12, dbIdentityColumn->_orderFlag);
			_excel.WriteCell(currentRow, 13, std::to_string(dbIdentityColumn->_cacheSize));
			_excel.WriteCell(currentRow, 14, std::to_string(dbIdentityColumn->_lastNumber));
			_excel.WriteCell(currentRow, 15, dbIdentityColumn->_scaleFlag);
			_excel.WriteCell(currentRow, 16, dbIdentityColumn->_extendFlag);
			_excel.WriteCell(currentRow, 17, dbIdentityColumn->_shardedFlag);
			_excel.WriteCell(currentRow, 18, dbIdentityColumn->_sessionFlag);
			_excel.WriteCell(currentRow, 19, dbIdentityColumn->_keepValue);

			_excel.SetRowHeight(currentRow, 16);
		}
	}

	_excel.SetAllCellTextFormat(9);
	_excel.SetAllCellTextFormat(14);

	_excel.SetColumnWidth(1, ExcelColumnWidth::cnSchemaName);
	_excel.SetColumnWidth(2, ExcelColumnWidth::cnObjectName);
	_excel.SetColumnWidth(3, ExcelColumnWidth::cnColumnName);
	_excel.SetColumnWidth(4, ExcelColumnWidth::cnDefaultWidth);
	_excel.SetColumnWidth(5, ExcelColumnWidth::cnDefaultWidth);
	_excel.SetColumnWidth(6, ExcelColumnWidth::cnDefaultWidth + 4);
	_excel.SetColumnWidth(7, ExcelColumnWidth::cnDefaultWidth + 4);
	_excel.SetColumnWidth(8, ExcelColumnWidth::cnDefaultWidth);
	_excel.SetColumnWidth(9, ExcelColumnWidth::cnDefaultWidth);
	_excel.SetColumnWidth(10, ExcelColumnWidth::cnDefaultWidth);
	_excel.SetColumnWidth(11, ExcelColumnWidth::cnDefaultWidth);
	_excel.SetColumnWidth(12, ExcelColumnWidth::cnDefaultWidth);
	_excel.SetColumnWidth(13, ExcelColumnWidth::cnDefaultWidth);
	_excel.SetColumnWidth(14, ExcelColumnWidth::cnDefaultWidth);
	_excel.SetColumnWidth(15, ExcelColumnWidth::cnDefaultWidth);
	_excel.SetColumnWidth(16, ExcelColumnWidth::cnDefaultWidth);
	_excel.SetColumnWidth(17, ExcelColumnWidth::cnDefaultWidth);
	_excel.SetColumnWidth(18, ExcelColumnWidth::cnDefaultWidth);
	_excel.SetColumnWidth(19, ExcelColumnWidth::cnDefaultWidth);
}

//***************************************************************************
//
void CDBSchemaToExcel::AddExcelMSSQLDefaultConstraintsInfo()
{
	int32 currentRow = 1;

	_excel.AddSheet("TABLE_DEFAULT_CONSTRAINT");
	ToHeaderStyle("A1", "F1");

	_excel.WriteCell(currentRow, 1, "SCHEMA_NAME");
	_excel.WriteCell(currentRow, 2, "TABLE_NAME");
	_excel.WriteCell(currentRow, 3, "CONST_NAME");
	_excel.WriteCell(currentRow, 4, "COLUMN_NAME");
	_excel.WriteCell(currentRow, 5, "DEFAULT_VALUE");
	_excel.WriteCell(currentRow, 6, "IS_SYSTEM_NAMED");

	for( DBModel::TableRef& dbTable : _dbTables )
	{
		for( DBModel::DefaultConstraintRef& dbDefaultConstraint : dbTable->_defaultConstraints )
		{
			currentRow++;
			_excel.WriteCell(currentRow, 1, dbDefaultConstraint->_schemaName);
			_excel.WriteCell(currentRow, 2, dbDefaultConstraint->_tableName);
			_excel.WriteCell(currentRow, 3, dbDefaultConstraint->_defaultConstName);
			_excel.WriteCell(currentRow, 4, dbDefaultConstraint->_columnName);
			_excel.WriteCell(currentRow, 5, dbDefaultConstraint->_defaultValue);
			_excel.WriteCell(currentRow, 6, std::to_string(dbDefaultConstraint->_systemNamed));
			_excel.SetRowHeight(currentRow, 16);
		}
	}

	_excel.SetColumnWidth(1, ExcelColumnWidth::cnSchemaName);
	_excel.SetColumnWidth(2, ExcelColumnWidth::cnObjectName);
	_excel.SetColumnWidth(3, ExcelColumnWidth::cnConstraintName);
	_excel.SetColumnWidth(4, ExcelColumnWidth::cnColumnName);
	_excel.SetColumnWidth(5, ExcelColumnWidth::cnDefaultConstraintValue);
	_excel.SetColumnWidth(6, ExcelColumnWidth::cnSystemNamed);
}

//***************************************************************************
//
void CDBSchemaToExcel::SaveExcelFile(const EDBClass dbClass, const _tstring path)
{
	try
	{
		AddExcelTableInfo();
		AddExcelTableColumnInfo();
		AddExcelConstraintsInfo();
		AddExcelIndexInfo();
		AddExcelForeignKeyInfo();
		AddExcelCheckConstraintsInfo();

		if( dbClass == EDBClass::MSSQL )
		{
			AddExcelMSSQLTableIndexOptionInfo();
			AddExcelMSSQLDefaultConstraintsInfo();
		}
		else if( dbClass == EDBClass::ORACLE )
		{
			AddExcelORACLEIdentityColumnInfo();
		}

		// 저장
		_excel.SaveAs(path);
	}
	catch( const std::exception& e )
	{
		_tcerr << _T("오류 발생: ") << e.what() << std::endl;
	}
}