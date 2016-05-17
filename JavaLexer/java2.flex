/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Copyright (C) 1998-2015  Gerwin Klein <lsf@jflex.de>                    *
 * All rights reserved.                                                    *
 *                                                                         *
 * License: BSD                                                            *
 *                                                                         *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/* Java 1.2 language lexer specification */

/* Use together with unicode.flex for Unicode preprocesssing */
/* and java12.cup for a Java 1.2 parser                      */

/* Note that this lexer specification is not tuned for speed.
   It is in fact quite slow on integer and floating point literals, 
   because the input is read twice and the methods used to parse
   the numbers are not very fast. 
   For a production quality application (e.g. a Java compiler) 
   this could be optimized */


import java_cup.runtime.*;

%%

%public
%class Scanner2
%implements sym

%unicode

%line
%column

%cup
%cupdebug

%{
  StringBuilder string = new StringBuilder();
  
  private Symbol symbol(int type) {
    return new JavaSymbol(type, yyline+1, yycolumn+1);
  }

  private Symbol symbol(int type, Object value) {
    return new JavaSymbol(type, yyline+1, yycolumn+1, value);
  }

  /** 
   * assumes correct representation of a long value for 
   * specified radix in scanner buffer from <code>start</code> 
   * to <code>end</code> 
   */
  private long parseLong(int start, int end, int radix) {
    long result = 0;
    long digit;

    for (int i = start; i < end; i++) {
      digit  = Character.digit(yycharat(i),radix);
      result*= radix;
      result+= digit;
    }

    return result;
  }
%}

/* main character classes */
LineTerminator = \r|\n|\r\n
InputCharacter = [^\r\n]

WhiteSpace = {LineTerminator} | [ \t\f]

/* comments */
Comment = {TraditionalComment} | {EndOfLineComment} | 
          {DocumentationComment}

TraditionalComment = "/*" [^*] ~"*/" | "/*" "*"+ "/"
EndOfLineComment = "//" {InputCharacter}* {LineTerminator}?
DocumentationComment = "/*" "*"+ [^/*] ~"*/"

/* identifiers */
Identifier = [:jletter:][:jletterdigit:]*

/* integer literals */
DecIntegerLiteral = 0 | [1-9][0-9]*
DecLongLiteral    = {DecIntegerLiteral} [lL]

HexIntegerLiteral = 0 [xX] 0* {HexDigit} {1,8}
HexLongLiteral    = 0 [xX] 0* {HexDigit} {1,16} [lL]
HexDigit          = [0-9a-fA-F]

OctIntegerLiteral = 0+ [1-3]? {OctDigit} {1,15}
OctLongLiteral    = 0+ 1? {OctDigit} {1,21} [lL]
OctDigit          = [0-7]
    
/* floating point literals */        
FloatLiteral  = ({FLit1}|{FLit2}|{FLit3}) {Exponent}? [fF]
DoubleLiteral = ({FLit1}|{FLit2}|{FLit3}) {Exponent}?

FLit1    = [0-9]+ \. [0-9]* 
FLit2    = \. [0-9]+ 
FLit3    = [0-9]+ 
Exponent = [eE] [+-]? [0-9]+

/* string and character literals */
StringCharacter = [^\r\n\"\\]
SingleCharacter = [^\r\n\'\\]

%state STRING, CHARLITERAL

%%

<YYINITIAL> {

  /* keywords */
  "abstract"                     { return symbol(ABSTRACT, yytext()); }
  "boolean"                      { return symbol(BOOLEAN, yytext()); }
  "break"                        { return symbol(BREAK, yytext()); }
  "byte"                         { return symbol(BYTE, yytext()); }
  "case"                         { return symbol(CASE, yytext()); }
  "catch"                        { return symbol(CATCH, yytext()); }
  "char"                         { return symbol(CHAR, yytext()); }
  "class"                        { return symbol(CLASS, yytext()); }
  "const"                        { return symbol(CONST, yytext()); }
  "continue"                     { return symbol(CONTINUE, yytext()); }
  "do"                           { return symbol(DO, yytext()); }
  "double"                       { return symbol(DOUBLE, yytext()); }
  "else"                         { return symbol(ELSE, yytext()); }
  "extends"                      { return symbol(EXTENDS, yytext()); }
  "final"                        { return symbol(FINAL, yytext()); }
  "finally"                      { return symbol(FINALLY, yytext()); }
  "float"                        { return symbol(FLOAT, yytext()); }
  "for"                          { return symbol(FOR, yytext()); }
  "default"                      { return symbol(DEFAULT, yytext()); }
  "implements"                   { return symbol(IMPLEMENTS, yytext()); }
  "import"                       { return symbol(IMPORT, yytext()); }
  "instanceof"                   { return symbol(INSTANCEOF, yytext()); }
  "int"                          { return symbol(INT, yytext()); }
  "interface"                    { return symbol(INTERFACE, yytext()); }
  "long"                         { return symbol(LONG, yytext()); }
  "native"                       { return symbol(NATIVE, yytext()); }
  "new"                          { return symbol(NEW, yytext()); }
  "goto"                         { return symbol(GOTO, yytext()); }
  "if"                           { return symbol(IF, yytext()); }
  "public"                       { return symbol(PUBLIC, yytext()); }
  "short"                        { return symbol(SHORT, yytext()); }
  "super"                        { return symbol(SUPER, yytext()); }
  "switch"                       { return symbol(SWITCH, yytext()); }
  "synchronized"                 { return symbol(SYNCHRONIZED, yytext()); }
  "package"                      { return symbol(PACKAGE, yytext()); }
  "private"                      { return symbol(PRIVATE, yytext()); }
  "protected"                    { return symbol(PROTECTED, yytext()); }
  "transient"                    { return symbol(TRANSIENT, yytext()); }
  "return"                       { return symbol(RETURN, yytext()); }
  "void"                         { return symbol(VOID, yytext()); }
  "static"                       { return symbol(STATIC, yytext()); }
  "while"                        { return symbol(WHILE, yytext()); }
  "this"                         { return symbol(THIS, yytext()); }
  "throw"                        { return symbol(THROW, yytext()); }
  "throws"                       { return symbol(THROWS, yytext()); }
  "try"                          { return symbol(TRY, yytext()); }
  "volatile"                     { return symbol(VOLATILE, yytext()); }
  "strictfp"                     { return symbol(STRICTFP, yytext()); }
  
  /* the annoation symbol*/
  "@"                            { return symbol(AT, yytext()); }
  
  /* boolean literals */
  "true"                         { return symbol(BOOLEAN_LITERAL, true); }
  "false"                        { return symbol(BOOLEAN_LITERAL, false); }
  
  /* null literal */
  "null"                         { return symbol(NULL_LITERAL, yytext()); }
  
  
  /* separators */
  "("                            { return symbol(LPAREN, yytext()); }
  ")"                            { return symbol(RPAREN, yytext()); }
  "{"                            { return symbol(LBRACE, yytext()); }
  "}"                            { return symbol(RBRACE, yytext()); }
  "["                            { return symbol(LBRACK, yytext()); }
  "]"                            { return symbol(RBRACK, yytext()); }
  ";"                            { return symbol(SEMICOLON, yytext()); }
  ","                            { return symbol(COMMA, yytext()); }
  "."                            { return symbol(DOT, yytext()); }
  " "+                            { return symbol(SPACE,new String(" "));}

  /* operators */
  "="                            { return symbol(EQ, yytext()); }
  ">"                            { return symbol(GT, yytext()); }
  "<"                            { return symbol(LT, yytext()); }
  "!"                            { return symbol(NOT, yytext()); }
  "~"                            { return symbol(COMP, yytext()); }
  "?"                            { return symbol(QUESTION, yytext()); }
  ":"                            { return symbol(COLON, yytext()); }
  "=="                           { return symbol(EQEQ, yytext()); }
  "<="                           { return symbol(LTEQ, yytext()); }
  ">="                           { return symbol(GTEQ, yytext()); }
  "!="                           { return symbol(NOTEQ, yytext()); }
  "&&"                           { return symbol(ANDAND, yytext()); }
  "||"                           { return symbol(OROR, yytext()); }
  "++"                           { return symbol(PLUSPLUS, yytext()); }
  "--"                           { return symbol(MINUSMINUS, yytext()); }
  "+"                            { return symbol(PLUS, yytext()); }
  "-"                            { return symbol(MINUS, yytext()); }
  "*"                            { return symbol(MULT, yytext()); }
  "/"                            { return symbol(DIV, yytext()); }
  "&"                            { return symbol(AND, yytext()); }
  "|"                            { return symbol(OR, yytext()); }
  "^"                            { return symbol(XOR, yytext()); }
  "%"                            { return symbol(MOD, yytext()); }
  "<<"                           { return symbol(LSHIFT, yytext()); }
  ">>"                           { return symbol(RSHIFT, yytext()); }
  ">>>"                          { return symbol(URSHIFT, yytext()); }
  "+="                           { return symbol(PLUSEQ, yytext()); }
  "-="                           { return symbol(MINUSEQ, yytext()); }
  "*="                           { return symbol(MULTEQ, yytext()); }
  "/="                           { return symbol(DIVEQ, yytext()); }
  "&="                           { return symbol(ANDEQ, yytext()); }
  "|="                           { return symbol(OREQ, yytext()); }
  "^="                           { return symbol(XOREQ, yytext()); }
  "%="                           { return symbol(MODEQ, yytext()); }
  "<<="                          { return symbol(LSHIFTEQ, yytext()); }
  ">>="                          { return symbol(RSHIFTEQ, yytext()); }
  ">>>="                         { return symbol(URSHIFTEQ, yytext()); }
  
  /* string literal */
  \"                             { yybegin(STRING);  string.setLength(0);}

  /* character literal */
  \'                             { yybegin(CHARLITERAL); }

  /* numeric literals */

  /* This is matched together with the minus, because the number is too big to 
     be represented by a positive integer. */
  "-2147483648"                  { return symbol(INTEGER_LITERAL, new Integer(Integer.MIN_VALUE)); }
  
  {DecIntegerLiteral}            { return symbol(INTEGER_LITERAL, new Integer(yytext())); }
  {DecLongLiteral}               { return symbol(INTEGER_LITERAL, new Long(yytext().substring(0,yylength()-1))); }
  
  {HexIntegerLiteral}            { return symbol(INTEGER_LITERAL, new Integer((int) parseLong(2, yylength(), 16))); }
  {HexLongLiteral}               { return symbol(INTEGER_LITERAL, new Long(parseLong(2, yylength()-1, 16))); }
 
  {OctIntegerLiteral}            { return symbol(INTEGER_LITERAL, new Integer((int) parseLong(0, yylength(), 8))); }  
  {OctLongLiteral}               { return symbol(INTEGER_LITERAL, new Long(parseLong(0, yylength()-1, 8))); }
  
  {FloatLiteral}                 { return symbol(FLOATING_POINT_LITERAL, new Float(yytext().substring(0,yylength()-1))); }
  {DoubleLiteral}                { return symbol(FLOATING_POINT_LITERAL, new Double(yytext())); }
  {DoubleLiteral}[dD]            { return symbol(FLOATING_POINT_LITERAL, new Double(yytext().substring(0,yylength()-1))); }
  
  /* comments */
  {Comment}                      { /* ignore */ }

  /* whitespace */
  {WhiteSpace}                   { /* ignore */ }

  /* identifiers */ 
  {Identifier}                   { return symbol(IDENTIFIER, yytext()); }  
}

<STRING> {
  \"                             { yybegin(YYINITIAL); return symbol(STRING_LITERAL, "\""+string.toString()+"\""); }
  
  {StringCharacter}+             { string.append( yytext() ); }
  
  /* escape sequences */
  "\\b"                          { string.append( '\b' ); }
  "\\t"                          { string.append( '\t' ); }
  "\\n"+                          { string.append( '\n' ); }
  "\\f"                          { string.append( '\f' ); }
  "\\r"                          { string.append( '\r' ); }
  "\\\""                         { string.append( '\"' ); }
  "\\'"                          { string.append( '\'' ); }
  "\\\\"                         { string.append( '\\' ); }
  \\[0-3]?{OctDigit}?{OctDigit}  { char val = (char) Integer.parseInt(yytext().substring(1),8);
                        				   string.append( val ); }
  
  /* error cases */
  \\.                            { throw new RuntimeException("Illegal escape sequence \""+yytext()+"\""); }
  {LineTerminator}               { throw new RuntimeException("Unterminated string at end of line"); }
}

<CHARLITERAL> {
  {SingleCharacter}\'            { yybegin(YYINITIAL); return symbol(CHARACTER_LITERAL, "\'"+yytext().charAt(0)+"\'"); }
  
  /* escape sequences */
  "\\b"\'                        { yybegin(YYINITIAL); return symbol(CHARACTER_LITERAL, '\b');}
  "\\t"\'                        { yybegin(YYINITIAL); return symbol(CHARACTER_LITERAL, '\t');}
  "\\n"\'+                        { yybegin(YYINITIAL); return symbol(CHARACTER_LITERAL, '\n');}
  "\\f"\'                        { yybegin(YYINITIAL); return symbol(CHARACTER_LITERAL, '\f');}
  "\\r"\'                        { yybegin(YYINITIAL); return symbol(CHARACTER_LITERAL, '\r');}
  "\\\""\'                       { yybegin(YYINITIAL); return symbol(CHARACTER_LITERAL, '\"');}
  "\\'"\'                        { yybegin(YYINITIAL); return symbol(CHARACTER_LITERAL, '\'');}
  "\\\\"\'                       { yybegin(YYINITIAL); return symbol(CHARACTER_LITERAL, '\\'); }
  \\[0-3]?{OctDigit}?{OctDigit}\' { yybegin(YYINITIAL); 
			                              int val = Integer.parseInt(yytext().substring(1,yylength()-1),8);
			                            return symbol(CHARACTER_LITERAL, "\'"+(char)val+"\'"); }
  
  /* error cases */
  \\.                            { throw new RuntimeException("Illegal escape sequence \""+yytext()+"\""); }
  {LineTerminator}               { throw new RuntimeException("Unterminated character literal at end of line"); }
}

/* error fallback */
[^]                              { throw new RuntimeException("Illegal character \""+yytext()+
                                                              "\" at line "+yyline+", column "+yycolumn); }
<<EOF>>                          { return symbol(EOF,new String("")); }