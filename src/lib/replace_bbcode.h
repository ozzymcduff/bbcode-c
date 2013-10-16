/*
 *  replace_bbcode.h
 *  bbCode
 *
 *  Created by Mathieu on 2008-05-01.
 *  Copyright 2008 Gewalli. All rights reserved.
 *
 */
//#pragma once
#define LEFT_BRACKET '['
#define RIGHT_BRACKET ']'
#define BB_BOLD "b"
#define BB_ITALIC "i"
#define BB_QUOTE "quote"
#define BB_CODE "code"
#define BB_COLOR "color"
#define HTML_BOLD "b"
#define HTML_ITALIC "i"
#define HTML_QUOTE "blockquote"
#define HTML_PRE "pre"
#define HTMLSPAN "span"
enum bbcode_attribute_type
{
  invalid = 0,
  colorcode = 1,
  colorname = 2,
  empty = 3,
};
/*****************************************************************
* replace bb code
******************************************************************/
char* bbcode_replace(char* text);
void require(int val, char* txt);
/*****************************************************************
* some basic validation of attribute
******************************************************************/
enum bbcode_attribute_type bbcode_get_attribute_type(const char* attr);
/*****************************************************************
* render the attribute
******************************************************************/
void bbcode_render_attribute(char* output, const char* tag, const char* attr, const int type);
/*****************************************************************
* string function from gnu lib, should be replacable by memcpy for most usages here
******************************************************************/
char * bbcode_substring (const char *start, const char *end);
/*****************************************************************
* queue functions
******************************************************************/
int bbcode_q_find_last(const char* queue[], const int length, const char* element);
void bbcode_q_add(const char* queue[], int* index, const char* element);
/*****************************************************************
* tag handling
******************************************************************/
int bbcode_find_tag_index(const char* tag);
char* bbcode_get_htmltag(const char* tag);
/*****************************************************************
* color handling
******************************************************************/
int bbcode_find_color_index(const char* attr);
char* bbcode_get_colorcode(const char* attr);