// tests.cpp : Defines the entry point for the console application.
//

#include "replace_bbcode.h"
#include "test_bbcode.h"
#include "TCHAR.h"

int _tmain(int argc, _TCHAR* argv[])
{
	bbcode_testfixture("../../test_bbcode.txt");
	return 0;
}

