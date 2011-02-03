/*
 *  replace_bbcode.c
 *  bbCode
 *
 *  Created by Mathieu on 2008-05-01.
 *  Copyright 2008 Gewalli. All rights reserved.
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "replace_bbcode.h"
#define printd
#undef printd


/*****************************************************************
* string function from gnu lib, should be replacable by memcpy for most usages here
******************************************************************/
char * bbcode_substring (const char *start, const char *end)
{
    // printf("Substring!\n");
    char *result = /*x*/ malloc (end - start + 1);
    char *scan_result = result;
    const char *scan = start;
    
    while (scan < end)
        *scan_result++ = *scan++;
    
    *scan_result = 0;
    return result;
}
/*****************************************************************
* queue functions
******************************************************************/
int bbcode_q_find_last(const char* queue[], const int length, const char* element)
{
    int i;
    for(i = length; i>=0;i--)
    {if (!strcmp(queue[i],element)) return i;}
    return -1;
}

void bbcode_q_add(const char* queue[], int* index, const char* element)
{
    queue[++(*index)]=element;
}
/*****************************************************************
* tag handling
******************************************************************/
static const int NUM_BBTAGS = 5;
static char* BBTAGS[] = { BB_BOLD, BB_ITALIC, BB_QUOTE, BB_CODE, BB_COLOR };
static char* HTMLTAGS[] = { HTML_BOLD, HTML_ITALIC, HTML_QUOTE, HTML_PRE, HTMLSPAN };
int bbcode_find_tag_index(const char* tag)/* */
{
    int i;
    for(i = 0;i<NUM_BBTAGS;i++) {if (!strcmp(BBTAGS[i],tag)) return i;}
    return -1;
}
char* bbcode_get_htmltag(const char* tag)
{
    int idx = bbcode_find_tag_index(tag);
    if (idx>=0)
        return HTMLTAGS[idx];
    return NULL;
}
/*****************************************************************
* color handling
******************************************************************/
static const int NUM_COLORS = 18;
static char* COLORNAMES[] = { 
    "black",   "brown",    "vinröd",
    "red",     "orange",  "yellow",
    "oliv",    "lime",    "green",
    "turkos",  "lightblue", "blue",
    "darkblue", "lila",    "pink",
    "grey",     "silver",  "white"};

static char* COLORCODES[] = { 
    "000000","663300","800000",
    "FF0000","FF9900","FFFF00",
    "808000","00FF00","008000",
    "008080","00FFFF","0000FF",
    "000080","800080","FF00FF",
    "808080","C0C0C0","FFFFFF"};
int bbcode_find_color_index(const char* attr)/* */
{
    int i;
    for(i = 0;i<NUM_COLORS;i++) {if (!strcmp(COLORNAMES[i],attr)) return i;}
    return -1;
}
char* bbcode_get_colorcode(const char* attr)
{
    int idx = bbcode_find_color_index(attr);
    if (idx>=0)
        return COLORCODES[idx];
    return NULL;
}
/*****************************************************************
* debug
******************************************************************/
void require(int val, char* txt)/* */
{
    if (!val)
    {
        printf("Fail message: %s\n",txt);
        exit(EXIT_FAILURE);
    }
}
/*****************************************************************
* replace bb code
*
* possible problems with this:
* output allocated to a fixed length. What happens when someone 
* uses more memory than allocated?
******************************************************************/
char* bbcode_replace(char* text)
{
#ifdef printd
    printf("  Replace bbcode: %s\n",text);
#endif
    char* queue[10] ;
    int queue_index = -1;
    int length = strlen(text);
    char* output = malloc((length+1000) * sizeof(char));
    memset (output,0,(length+1000) * sizeof(char)); // clear the memory
    char c;
    int j;
    char* index = text;
    /*****************************************************************
        * state: None
        ******************************************************************/
None:;
    char* begin_none = index;
    {
        char* indexOfBracket = strstr(index,"[");
        if (indexOfBracket!=NULL)
        {
            index = indexOfBracket;
            char* substr = bbcode_substring(begin_none,indexOfBracket);
#ifdef printd
            printf("  LEFT_BRACKET: substr: '%s', indexOfBracket: '%s'\n",substr,indexOfBracket);
#endif
            strcat(output,substr);
            free(substr);
            goto LeftTMatch;
        }
        else
        {
            strcat(output,begin_none);
            goto End;// return output;
        }
    }
    /*****************************************************************
        * state: LeftTMatch: Found '['
        ******************************************************************/  
LeftTMatch:;//region left t match
        char* begin_tmatch = index;
        {
            c = *index;
            require (c == LEFT_BRACKET,"c != LEFT_BRACKET");
            index ++;
            int indexOfBracket = strcspn(index,"[]");
            if (indexOfBracket==strlen(index))
            {
                goto None;
            }
            else
            {
                char bracket = index[indexOfBracket];
#ifdef printd
                printf("  switch(%c) , indexOfBracket: %d, strlen(index): %d, index: '%s' \n",bracket,indexOfBracket,strlen(index),index);
#endif
                switch (bracket)
                { // switch
                    case LEFT_BRACKET:
                    {
                        char* substr = bbcode_substring(begin_tmatch,index+indexOfBracket);
#ifdef printd
                        printf("  LEFT_BRACKET(2): '%s'\n",substr);
#endif
                        strcat(output,substr);
                        free(substr);
                        index =index + indexOfBracket;
                        goto LeftTMatch;
                    }
                    case RIGHT_BRACKET:
                    {
                        int idx = -1;
#ifdef printd
                        printf("  RIGHT_BRACKET(3): '%s'\n",index);
#endif
                        char* tag = bbcode_substring(index,index+indexOfBracket);
#ifdef printd
                        printf("  (1)tag: '%s'\n",tag);
#endif
                        char* attr = "";
                        
                        int tag_length = strlen(tag);
                        bool endTag = tag_length > 0 && tag[0] == '/';
                        enum bbcode_attribute_type type = empty;
                        if (endTag)
                        {
                            tag = bbcode_substring(tag+1,tag+tag_length); //tag.Substring(1, tag.Length - 1);
#ifdef printd
                            printf("  (2)tag: '%s'\n",tag);
#endif
                            idx = bbcode_q_find_last(queue,queue_index,tag); //queue.LastIndexOf(tag);
                            
                        }
                        else// begin tag only
                        {//=red =>  style="color: red;",  =#FF0000 =>  style="color: #FF0000;"
                            int eqIdx = strcspn(tag,"=");
#ifdef printd
                            printf("  begin tag(1): eqIdx: %d, strlen(tag): %d\n",eqIdx,strlen(tag));
#endif
                            if (eqIdx!=strlen(tag))
                            {
                                attr = bbcode_substring(tag+eqIdx+1,tag+tag_length);
                                type = bbcode_get_attribute_type(attr);
                                tag = bbcode_substring(tag,tag+eqIdx);
#ifdef printd
                                printf("  begin tag(2): attr: %s, type(tag): %d, tag: %s\n",attr,type,tag);
#endif
                            }
                        }
                        
                        int tag_index = bbcode_find_tag_index(tag);
                        if (tag_index!=-1 && !(endTag && idx<0) && type != invalid)
                        {//
                            if (endTag)
                            {
#ifdef printd
                                printf("  endtag(1): Queue index: %d, found idx: %d\n",queue_index,idx);
#endif
                                for ( j = queue_index; j >= idx; j--)
                                {
                                    strcat(output,"</");
                                    strcat(output,HTMLTAGS[bbcode_find_tag_index(queue[j])]);
                                    strcat(output,">");
                                }
                                queue_index = idx-1;// queue.RemoveRange(idx, queue.Count - idx);
                            }
                            else
                            {
                                bbcode_q_add(queue,&queue_index,tag);
                                strcat(output,"<");
                                strcat(output,HTMLTAGS[tag_index]);
                                bbcode_render_attribute(output,tag,attr,type);                
                                strcat(output,">");
                            }
                            index = index + indexOfBracket + 1;
#ifdef printd
                            printf("  New index(1): '%s'\n",index);
#endif     
                        }
                        else
                        {
                            char* substr = bbcode_substring(begin_tmatch,index+indexOfBracket+1);
#ifdef printd
                            printf("  Not well formed: '%s'\n",substr);
#endif
                            strcat(output,substr);
							index = index + indexOfBracket + 1;
#ifdef printd
							printf("  New index(2): '%s'\n",index);
#endif     
                        }
                    }
                        break;
                    default:
                        printf("EXIT: !!\n");
                        exit(EXIT_FAILURE);
                } // switch
            }
            
            goto None;   
        }
        
        //#endregion
End:;
#ifdef printd
        printf("  End: Queue index: %d\n",queue_index);
#endif
        for ( j = queue_index; j >= 0; j--)
        {
            strcat(output,"</");
            strcat(output,HTMLTAGS[bbcode_find_tag_index(queue[j])]);
            strcat(output,">");
            //output.Append("</" + TagDictionary[queue[j]] + '>')
        }
        return output;
}

/*****************************************************************
* some basic validation of attribute
******************************************************************/
enum bbcode_attribute_type bbcode_get_attribute_type(const char* attr)
{
    int attr_len = strlen(attr);
    if (attr_len == 0) return empty;
    
    if (attr[0] == '#')
    {//[A-F0-9]{6}
        if (attr_len != 6)
            return invalid;
        
        bool allLettersAndDigits = true;
        int i;
        for (i = 0; i < attr_len; i++)
        {
            int val = attr[i];
            allLettersAndDigits &= ((val>='a'&&val<='z')||(val>='A'&&val<='Z')||(val>='0'&&val<='9'));
        }
        
        if (!allLettersAndDigits) return invalid;
        
        return colorcode;
    }
    else
    {//[A-Fa-f]
        bool allLetters = true;
        int i;
        for (i = 0; i < attr_len; i++)
        {
            int val = attr[i];
            allLetters &= ((val>='a'&&val<='z')||(val>='A'&&val<='Z'));
        }
        
        if (!allLetters) return invalid;
        
        int coloridx = bbcode_find_color_index(attr);
        if (coloridx < 0) return invalid;
        return colorname;
    }
}

/*****************************************************************
* render the attribute
******************************************************************/
void bbcode_render_attribute(char* output, const char* tag, const char* attr, const int type)
{
    if (!strcmp(tag, BB_COLOR))
    {
        switch (type)
        {
            case invalid: exit(EXIT_FAILURE);
            case colorcode:
            {  
                strcat(output," style=\"color:");
                strcat(output,attr);
                strcat(output,"\" ");
                break;
            }
            case colorname:
            {
                int coloridx = bbcode_find_color_index(attr);
                if (coloridx < 0) 
                    break;
                strcat(output," style=\"color:#");
                strcat(output,COLORCODES[coloridx]);
                strcat(output,"\" ");
            }
            case empty: break;
            default:
                printf("  Type: '%d'\n",type);
                exit(EXIT_FAILURE);
        }
    }
}

