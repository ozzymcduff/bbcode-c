/*
 *  test_bbcode.c
 *  bbCode
 *
 *  Created by Mathieu on 2008-05-01.
 *  Copyright 2008 Gewalli. All rights reserved.
 *
 */
#include "test_bbcode.h"
#include "replace_bbcode.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

bool test_replace(int num,char* val,char* exp)
{
  char* repl = bbcode_replace(val);
  if (strcmp(repl,exp))
  {
    printf("TEST %d: '%s'\nEXPT %d: '%s'\n", num, repl, num, exp);
    return false;
  }
  return true;
}

bool test_readline(char* buffer, int max, FILE * pFile)
{
  if (!feof(pFile))
  {
    fgets (buffer , max , pFile);
    int buffer_len = strlen(buffer);
    if(buffer[buffer_len-1]=='\n')
      buffer[buffer_len-1]=(int)NULL; // remove newline
    return true;
  }
  else
  {
    return false;
  }
}

bool test_bbcode1(char* file)
{  
 // printf("%c - %c: %d - %d, %c - %c: %d - %d\n",'a','z','a','z','å','ö','å','ö');
  
   char test [100];
   char expect [100];
   FILE * pFile;
   int testNum = 0;
   pFile = fopen (file,"r"); 
   if (pFile!=NULL)
   {
     while (!feof(pFile)) 
     {
       if (!test_readline(test,100,pFile)) { printf("problem reading test line!!"); exit(EXIT_FAILURE); }
       if (!test_readline(expect,100,pFile)) { printf("problem reading expect line!!"); exit(EXIT_FAILURE); }
       bool success = test_replace(++testNum, test, expect);
       printf("Test %d %s.\n", testNum, (success?"success":"failure"));
     }
     fclose (pFile);
   }else{ printf("Unable to open file\n"); }
   return true;
} 

bool test_bbcode2()
{
  printf("tag: %s\n",bbcode_get_htmltag(BB_CODE));  
  return true;
}

bool bbcode_testfixture(char* file)
{
  bool retval = true;
  retval&=test_bbcode1(file);
  retval&=test_bbcode2();
  return retval;
}
