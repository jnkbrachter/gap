/****************************************************************************
**
*W  string.h                    GAP source                   Martin Schönert
**
**
*Y  Copyright (C)  1996,  Lehrstuhl D für Mathematik,  RWTH Aachen,  Germany
*Y  (C) 1998 School Math and Comp. Sci., University of St Andrews, Scotland
*Y  Copyright (C) 2002 The GAP Group
**
**  This file declares the functions which mainly deal with strings.
**
**  A *string* is a  list that  has no  holes, and  whose  elements  are  all
**  characters.  For the full definition of strings see chapter  "Strings" in
**  the {\GAP} manual.  Read also "More about Strings" about the  string flag
**  and the compact representation of strings.
**
**  Strings in compact representation  can be accessed and handled through
**  the  macros     'NEW_STRING',   `CHARS_STRING'  (and   'CSTR_STRING'),
**  'GET_LEN_STRING',   `SET_LEN_STRING', `GROW_STRING',  'GET_ELM_STRING'
**  and `SET_ELM_STRING'.
**  
**  This  package also contains the   list  function  for ranges, which   are
**  installed in the appropriate tables by 'InitString'.
*/

#ifndef GAP_STRING_H
#define GAP_STRING_H

#ifdef  INCLUDE_DECLARATION_PART
#endif

#include <string.h>  /* for memcpy */

/****************************************************************************
**

*F * * * * * * * * * * * * * * character functions  * * * * * * * * * * * * *
*/

/****************************************************************************
**


*V  ObjsChar[<chr>] . . . . . . . . . . . . . . . . table of character values
**
**  'ObjsChar' contains all the character values.  That way we do not need to
**  allocate new bags for new characters.
*/
extern Obj ObjsChar [256];


/****************************************************************************
**

*F * * * * * * * * * * * * * * * string functions * * * * * * * * * * * * * *
*/

/****************************************************************************
**
*F  SIZEBAG_STRINGLEN( <len> ) . . . . size of Bag for string of length <len>
**  
*/
#define SIZEBAG_STRINGLEN(len)         ((len) + 1 + sizeof(UInt))

/****************************************************************************
**
*F  CSTR_STRING( <list> ) . . . . . . . . . . . . . . .  C string of a string
*F  CHARS_STRING( <list> ) . . . . . . . . . . . . . .   same pointer 
**
**  'CSTR_STRING'  returns the (address  of the)  C  character string of  the
**  string <list>. Note that the string as C string is truncated before the
**  first null character. Try to avoid this and use CHARS_STRING.
**
**  Note that 'CSTR_STRING' is a macro, so do not call it with arguments that
**  have sideeffects.
*/
#define CSTR_STRING(list)            ((Char*)ADDR_OBJ(list) + sizeof(UInt))
#define CHARS_STRING(list)           ((UChar*)ADDR_OBJ(list) + sizeof(UInt))

/****************************************************************************
**
*F  GET_LEN_STRING( <list> )  . . . . . . . . . . . . . .  length of a string
**
**  'GET_LEN_STRING' returns the length of the string <list>, as a C integer.
**
**  Note that  'GET_LEN_STRING' is a macro, so  do not call it with arguments
**  that have sideeffects.
*/
#define GET_LEN_STRING(list)            (*((UInt*)ADDR_OBJ(list)))

/****************************************************************************
**
*F  SET_LEN_STRING( <list>, <len> ) . . . . . . . . . set length of a string
**
**  'SET_LEN_STRING' sets length of the string <list> to C integer <len>.
**
**  Note that  'SET_LEN_STRING' is a macro, so  do not call it with arguments
**  that have sideeffects.
*/
#define SET_LEN_STRING(list,len)     (*((UInt*)ADDR_OBJ(list)) = (UInt)(len))

/****************************************************************************
**
*F  NEW_STRING( <len> ) . . . . . . . . . . . . . . . . . . make a new string
**
**  'NEW_STRING' returns a new string with room for <len> characters. It also
**  sets its length to len. 
**
*/
extern Obj NEW_STRING(Int len);

/****************************************************************************
**
*F  GROW_STRING(<list>, <len>) . . . .  make sure a string is large enough
**
**  'GROW_STRING' grows  the string <list>  if necessary to ensure that it
**  has room for at least <len> elements.
**
**  Note that 'GROW_STRING' is a macro, so do not call it with arguments that
**  have sideeffects.
*/
#define GROW_STRING(list,len)   ( ((len) + sizeof(UInt) < SIZE_OBJ(list)) ? \
                                 0L : GrowString(list,len) )

extern  Int             GrowString (
            Obj                 list,
            UInt                need );

/****************************************************************************
**
*F  SHRINK_STRING(<list>) . . . . . . . . .  shrink a string to minimal size
**
**  'SHRINK_STRING' gives back not needed memory allocated by string.
**
**  Note that 'SHRINK_STRING' is a macro, so do not call it with arguments that
**  have sideeffects.
*/
#define SHRINK_STRING(list)   ResizeBag((list),\
                            (SIZEBAG_STRINGLEN(GET_LEN_STRING((list)))));

/****************************************************************************
**
*F  GET_ELM_STRING( <list>, <pos> ) . . . . . . select an element of a string
**
**  'GET_ELM_STRING'  returns the  <pos>-th  element  of  the string  <list>.
**  <pos> must be  a positive integer  less than  or  equal to  the length of
**  <list>.
**
**  Note that 'GET_ELM_STRING' is a  macro, so do not  call it with arguments
**  that have sideeffects.
*/
#define GET_ELM_STRING(list,pos)        (ObjsChar[ \
                         (((UInt1*)ADDR_OBJ(list))[(pos) + sizeof(UInt) - 1])])

/****************************************************************************
**
*F  SET_ELM_STRING( <list>, <pos>, <val> ) . . . . set a character of a string
**
**  'SET_ELM_STRING'  sets the  <pos>-th  character  of  the string  <list>.
**  <val> must be a character and <list> stay a string after the assignment.
**
**  Note that 'SET_ELM_STRING' is a  macro, so do not  call it with arguments
**  that have sideeffects.
*/
#define SET_ELM_STRING(list,pos,val)      (((UInt1*)ADDR_OBJ(list))\
[(pos) + sizeof(UInt) - 1] = *((UInt1*)ADDR_OBJ(val)))

/****************************************************************************
**
*F  COPY_CHARS( <str>, <charpnt>, <n> ) . . . copies <n> chars, starting
**  from character pointer <charpnt>, to beginning of string
**  
**  This is a   macro. It assumes  that the  data  area in  <str> is  large
**  enough. It does not add a terminating null character and not change the
**  length of the string.
**
*/
#define COPY_CHARS(str,pnt,n)         (memcpy(CHARS_STRING(str), pnt, n));

/****************************************************************************
**
*F  PrintString( <list> ) . . . . . . . . . . . . . . . . . .  print a string
**
**  'PrintString' prints the string with the handle <list>.
**
**  No  linebreaks are allowed,  if one must be  inserted  anyhow, it must be
** escaped by a backslash '\', which is done in 'Pr'.  */
extern void PrintString (
    Obj                 list );


/****************************************************************************
**
*F  PrintString1( <list> )  . . . . . . . . . . .  print a string for 'Print'
**
**  'PrintString1' prints the string  constant  in  the  format  used  by  the
**  'Print' and 'PrintTo' function.
*/
extern void PrintString1 (
            Obj                 list );


/****************************************************************************
**

*F  IS_STRING( <obj> )  . . . . . . . . . . . . test if an object is a string
**
**  'IS_STRING' returns 1  if the object <obj>  is a string  and 0 otherwise.
**  It does not change the representation of <obj>.
**
**  Note that 'IS_STRING' is a  macro, so do not call  it with arguments that
**  have sideeffects.
*/
#define IS_STRING(obj)  ((*IsStringFuncs[ TNUM_OBJ( obj ) ])( obj ))

extern  Int             (*IsStringFuncs [LAST_REAL_TNUM+1]) ( Obj obj );


/****************************************************************************
**
*F  IS_STRING_REP( <list> ) . . . . . . . .  check if <list> is in string rep
*/
#define IS_STRING_REP(list)  \
  (T_STRING <= TNUM_OBJ(list) && TNUM_OBJ(list) <= T_STRING_SSORT+IMMUTABLE)


/****************************************************************************
**
*F  IsString( <obj> ) . . . . . . . . . . . . . test if an object is a string
**
**  'IsString' returns 1 if the object <obj> is a string and 0 otherwise.  It
**  does not change the representation of <obj>.
*/
extern Int IsString (
            Obj                 obj );


/****************************************************************************
**
*F  ConvString( <string> )  . . convert a string to the string representation
**
**  'ConvString' converts the string <list> to the string representation.
*/
extern void ConvString (
            Obj                 string );


/****************************************************************************
**
*F  IsStringConv( <obj> ) . . . . . test if an object is a string and convert
**
**  'IsStringConv'   returns 1  if   the object <obj>  is   a  string,  and 0
**  otherwise.   If <obj> is a  string it  changes  its representation to the
**  string representation.
*/
extern Int IsStringConv (
            Obj                 obj );


/****************************************************************************
**

*F  C_NEW_STRING( <string>, <len>, <cstring> )  . . . . . . create GAP string
*/
/* ???
#define C_NEW_STRING(string,len,cstr) \
  do { \
    string = NEW_STRING( len ); \
    SyStrncat( CSTR_STRING(string), cstr, len ); \
  } while ( 0 );
???  */
#define C_NEW_STRING(string,len,cstr) \
  do { \
    string = NEW_STRING( len ); \
    memcpy( CHARS_STRING(string), (cstr), (len) ); \
  } while ( 0 );

Obj MakeString(Char *cstr);
Obj MakeString2(Char *cstr1, Char *cstr2);
Obj MakeString3(Char *cstr1, Char *cstr2, Char *cstr3);
Obj MakeImmString(Char *cstr);
Obj MakeImmString2(Char *cstr1, Char *cstr2);
Obj MakeImmString3(Char *cstr1, Char *cstr2, Char *cstr3);
Obj ConvImmString(Obj str);


/****************************************************************************
**
*F  SINT_CHAR(a)
**
**  'SINT_CHAR' converts the character a (a UInt1) into a signed (C) integer.
*/
#define SINT_CHAR(a)    (((UInt1)a)<128 ? (Int)a : (Int)a-256)

/****************************************************************************
**
*F  CHAR_SINT(n)
**
**  'CHAR_SINT' converts the signed (C) integer n into an (UInt1) character.
*/
#define CHAR_SINT(n)    (UInt1)(n>=0 ? n : n+256)

/****************************************************************************
**

*F * * * * * * * * * * * * * initialize package * * * * * * * * * * * * * * *
*/

/****************************************************************************
**

*F  InitInfoString()  . . . . . . . . . . . . . . . . table of init functions
*/
StructInitInfo * InitInfoString ( void );


#endif // GAP_STRING_H

/****************************************************************************
**

*E  string.h  . . . . . . . . . . . . . . . . . . . . . . . . . . . ends here
*/
