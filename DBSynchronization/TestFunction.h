
//***************************************************************************
// TestFunction.h : interface for the Test Functions.
//
//***************************************************************************

#ifndef __TESTFUNCTION_H__
#define __TESTFUNCTION_H__

void TestDBInfo(CDBQueryProcess dbProcess);
void TestMSSQLTableIndexFragmentationCheck(CDBQueryProcess dbProcess);
void TestMSSQLIndexOptionProcess(CDBQueryProcess dbProcess);
void TestMYSQLCharacterSetCollationEngine(CDBQueryProcess dbProcess);
void TestMYSQLTableFragmentationCheck(CDBQueryProcess dbProcess);
void TestRenameObject(CDBQueryProcess dbProcess);
void TestComment(CDBQueryProcess dbProcess);
void TestORACLEIndexFragmentationCheck(CDBQueryProcess dbProcess);

#endif // ndef __TESTFUNCTION_H__
