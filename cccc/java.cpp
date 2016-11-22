/*
 * A n t l r  T r a n s l a t i o n  H e a d e r
 *
 * Terence Parr, Will Cohen, and Hank Dietz: 1989-1999
 * Purdue University Electrical Engineering
 * With AHPCRC, University of Minnesota
 * ANTLR Version 1.33MR20
 *
 *   ../pccts/bin/antlr -CC -k 2 -gd -ge -rl 5000 -w1 -e3 -ft Jtokens.h java.g
 *
 */

#define ANTLR_VERSION	13320
#include "pcctscfg.h"
#include "pccts_stdio.h"
#include "Jtokens.h"

#define zzTRACE_RULES
#include "AParser.h"
#include "cccc.h"
#include "cccc_utl.h"
#include "cccc_opt.h"

  // the objects which PCCTS creates for ASTs as the #0 variable etc
// have type "pointer to ASTBase", which means they need to be cast
// to a pointer to my variant of AST if I want to call my AST 
// methods on them
#define MY_AST(X) ( (AST*) X)
#ifndef zzTRACE_RULES
#define zzTRACE_RULES
#endif
#include "AParser.h"
#include "JParser.h"
#include "DLexerBase.h"
#include "ATokPtr.h"
#ifndef PURIFY
#define PURIFY(r,s) memset((char *) &(r),'\0',(s));
#endif

void
JParser::compilationUnit(void)
{
	zzRULE;
	ANTLRTokenPtr eof=NULL;
	zzTRACEIN("compilationUnit");
	
	// We want to keep track of the scope we are operating in
	string scope;
	{
		if ( (LA(1)==PACKAGE) ) {
			packageDefinition( scope );
		}
		else {
			if ( (setwd1[LA(1)]&0x1) ) {
			}
			else {FAIL(1,err1,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk); goto fail;}
		}
	}
	{
		while ( (LA(1)==IMPORT) ) {
			importDefinition();
		}
	}
	{
		while ( (setwd1[LA(1)]&0x2) ) {
			typeDefinition( scope );
		}
	}
	zzmatch(Eof); labase++;
	
	if ( !guessing ) {
		eof = (ANTLRTokenPtr)LT(1);
	}
	
	if ( !guessing ) {
	
	ps->record_other_extent(1, eof->getLine(),"<file scope items>");
	}
 consume();
	zzTRACEOUT("compilationUnit");
	return;
fail:
	if ( guessing ) zzGUESS_FAIL;
	syn(zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk);
	resynch(setwd1, 0x4);
	zzTRACEOUT("compilationUnit");
}

void
JParser::packageDefinition( string& scope )
{
	zzRULE;
	zzTRACEIN("packageDefinition");
	zzmatch(PACKAGE); labase++;
	 consume();
	identifier2( scope );
	zzmatch(SEMI); labase++;
	 consume();
	zzTRACEOUT("packageDefinition");
	return;
fail:
	if ( guessing ) zzGUESS_FAIL;
	syn(zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk);
	resynch(setwd1, 0x8);
	zzTRACEOUT("packageDefinition");
}

void
JParser::importDefinition(void)
{
	zzRULE;
	zzTRACEIN("importDefinition");
	zzmatch(IMPORT); labase++;
	 consume();
	identifierStar();
	zzmatch(SEMI); labase++;
	 consume();
	zzTRACEOUT("importDefinition");
	return;
fail:
	if ( guessing ) zzGUESS_FAIL;
	syn(zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk);
	resynch(setwd1, 0x10);
	zzTRACEOUT("importDefinition");
}

void
JParser::typeDefinition( string& parentScope )
{
	zzRULE;
	ANTLRTokenPtr m=NULL;
	zzTRACEIN("typeDefinition");
	Visibility v;
	if ( (setwd1[LA(1)]&0x20)
 ) {
		modifiers( v );
		{
			if ( (LA(1)==CLASS) ) {
				classDefinition( parentScope );
			}
			else {
				if ( (LA(1)==INTERFACE) ) {
					interfaceDefinition( parentScope );
				}
				else {FAIL(1,err2,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk); goto fail;}
			}
		}
	}
	else {
		if ( (LA(1)==SEMI) ) {
			zzmatch(SEMI); labase++;
			 consume();
		}
		else {FAIL(1,err3,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk); goto fail;}
	}
	zzTRACEOUT("typeDefinition");
	return;
fail:
	if ( guessing ) zzGUESS_FAIL;
	syn(zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk);
	resynch(setwd1, 0x40);
	zzTRACEOUT("typeDefinition");
}

void
JParser::declaration(void)
{
	zzRULE;
	zzTRACEIN("declaration");
	
	Visibility v=vDONTKNOW;
	int startLine=LT(1)->getLine();
	modifiers( v );
	typeSpec();
	variableDefinitions( d1,d2,db,v,startLine );
	zzTRACEOUT("declaration");
	return;
fail:
	if ( guessing ) zzGUESS_FAIL;
	syn(zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk);
	resynch(setwd1, 0x80);
	zzTRACEOUT("declaration");
}

void
JParser::modifiers( Visibility& v )
{
	zzRULE;
	zzTRACEIN("modifiers");
	{
		while ( (setwd2[LA(1)]&0x1) ) {
			modifier( v );
		}
	}
	zzTRACEOUT("modifiers");
	return;
fail:
	if ( guessing ) zzGUESS_FAIL;
	syn(zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk);
	resynch(setwd2, 0x2);
	zzTRACEOUT("modifiers");
}

void
JParser::typeSpec(void)
{
	zzRULE;
	zzTRACEIN("typeSpec");
	if ( (LA(1)==IDENT)
 ) {
		classTypeSpec();
	}
	else {
		if ( (setwd2[LA(1)]&0x4) ) {
			builtInTypeSpec();
		}
		else {FAIL(1,err4,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk); goto fail;}
	}
	zzTRACEOUT("typeSpec");
	return;
fail:
	if ( guessing ) zzGUESS_FAIL;
	syn(zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk);
	resynch(setwd2, 0x8);
	zzTRACEOUT("typeSpec");
}

void
JParser::classTypeSpec(void)
{
	zzRULE;
	ANTLRTokenPtr lb=NULL;
	zzTRACEIN("classTypeSpec");
	identifier();
	{
		while ( (LA(1)==LBRACK) ) {
			zzmatch(LBRACK); labase++;
			
			if ( !guessing ) {
						lb = (ANTLRTokenPtr)LT(1);
			}
			 consume();
			zzmatch(RBRACK); labase++;
			 consume();
		}
	}
	zzTRACEOUT("classTypeSpec");
	return;
fail:
	if ( guessing ) zzGUESS_FAIL;
	syn(zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk);
	resynch(setwd2, 0x10);
	zzTRACEOUT("classTypeSpec");
}

void
JParser::builtInTypeSpec(void)
{
	zzRULE;
	ANTLRTokenPtr lb=NULL;
	zzTRACEIN("builtInTypeSpec");
	builtInType();
	{
		while ( (LA(1)==LBRACK) ) {
			zzmatch(LBRACK); labase++;
			
			if ( !guessing ) {
						lb = (ANTLRTokenPtr)LT(1);
			}
			 consume();
			zzmatch(RBRACK); labase++;
			 consume();
		}
	}
	zzTRACEOUT("builtInTypeSpec");
	return;
fail:
	if ( guessing ) zzGUESS_FAIL;
	syn(zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk);
	resynch(setwd2, 0x20);
	zzTRACEOUT("builtInTypeSpec");
}

void
JParser::type(void)
{
	zzRULE;
	zzTRACEIN("type");
	if ( (LA(1)==IDENT) ) {
		identifier();
	}
	else {
		if ( (setwd2[LA(1)]&0x40)
 ) {
			builtInType();
		}
		else {FAIL(1,err5,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk); goto fail;}
	}
	zzTRACEOUT("type");
	return;
fail:
	if ( guessing ) zzGUESS_FAIL;
	syn(zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk);
	resynch(setwd2, 0x80);
	zzTRACEOUT("type");
}

void
JParser::builtInType(void)
{
	zzRULE;
	zzTRACEIN("builtInType");
	if ( (LA(1)==KW_VOID) ) {
		zzmatch(KW_VOID); labase++;
		 consume();
	}
	else {
		if ( (LA(1)==KW_BOOLEAN) ) {
			zzmatch(KW_BOOLEAN); labase++;
			 consume();
		}
		else {
			if ( (LA(1)==KW_BYTE) ) {
				zzmatch(KW_BYTE); labase++;
				 consume();
			}
			else {
				if ( (LA(1)==KW_CHAR) ) {
					zzmatch(KW_CHAR); labase++;
					 consume();
				}
				else {
					if ( (LA(1)==KW_SHORT)
 ) {
						zzmatch(KW_SHORT); labase++;
						 consume();
					}
					else {
						if ( (LA(1)==KW_INT) ) {
							zzmatch(KW_INT); labase++;
							 consume();
						}
						else {
							if ( (LA(1)==KW_FLOAT) ) {
								zzmatch(KW_FLOAT); labase++;
								 consume();
							}
							else {
								if ( (LA(1)==KW_LONG) ) {
									zzmatch(KW_LONG); labase++;
									 consume();
								}
								else {
									if ( (LA(1)==KW_DOUBLE) ) {
										zzmatch(KW_DOUBLE); labase++;
										 consume();
									}
									else {FAIL(1,err6,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk); goto fail;}
								}
							}
						}
					}
				}
			}
		}
	}
	zzTRACEOUT("builtInType");
	return;
fail:
	if ( guessing ) zzGUESS_FAIL;
	syn(zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk);
	resynch(setwd3, 0x1);
	zzTRACEOUT("builtInType");
}

void
JParser::identifier(void)
{
	zzRULE;
	zzTRACEIN("identifier");
	zzmatch(IDENT); labase++;
	 consume();
	{
		while ( (LA(1)==DOT)
 ) {
			zzmatch(DOT); labase++;
			 consume();
			zzmatch(IDENT); labase++;
			 consume();
		}
	}
	zzTRACEOUT("identifier");
	return;
fail:
	if ( guessing ) zzGUESS_FAIL;
	syn(zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk);
	resynch(setwd3, 0x2);
	zzTRACEOUT("identifier");
}

void
JParser::typeSpec2( string& typeString, bool& isBuiltIn )
{
	zzRULE;
	zzTRACEIN("typeSpec2");
	if ( (LA(1)==IDENT) ) {
		classTypeSpec2( typeString );
		if ( !guessing ) {
		isBuiltIn=false;
		}
	}
	else {
		if ( (setwd3[LA(1)]&0x4) ) {
			builtInTypeSpec2( typeString );
			if ( !guessing ) {
			isBuiltIn=true;
			}
		}
		else {FAIL(1,err7,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk); goto fail;}
	}
	zzTRACEOUT("typeSpec2");
	return;
fail:
	if ( guessing ) zzGUESS_FAIL;
	syn(zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk);
	resynch(setwd3, 0x8);
	zzTRACEOUT("typeSpec2");
}

void
JParser::classTypeSpec2( string& typeString )
{
	zzRULE;
	ANTLRTokenPtr lb=NULL;
	zzTRACEIN("classTypeSpec2");
	identifier2( typeString );
	{
		while ( (LA(1)==LBRACK) ) {
			zzmatch(LBRACK); labase++;
			
			if ( !guessing ) {
						lb = (ANTLRTokenPtr)LT(1);
			}
			 consume();
			zzmatch(RBRACK); labase++;
			
			if ( !guessing ) {
			typeString.append("[]");
			}
 consume();
		}
	}
	zzTRACEOUT("classTypeSpec2");
	return;
fail:
	if ( guessing ) zzGUESS_FAIL;
	syn(zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk);
	resynch(setwd3, 0x10);
	zzTRACEOUT("classTypeSpec2");
}

void
JParser::builtInTypeSpec2( string& typeString )
{
	zzRULE;
	zzTRACEIN("builtInTypeSpec2");
	ANTLRTokenPtr typeToken=LT(1);
	builtInType();
	if ( !guessing ) {
	typeString=typeToken->getText();
	}
	{
		while ( (LA(1)==LBRACK) ) {
			zzmatch(LBRACK); labase++;
			 consume();
			zzmatch(RBRACK); labase++;
			
			if ( !guessing ) {
			typeString.append("[]");
			}
 consume();
		}
	}
	zzTRACEOUT("builtInTypeSpec2");
	return;
fail:
	if ( guessing ) zzGUESS_FAIL;
	syn(zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk);
	resynch(setwd3, 0x20);
	zzTRACEOUT("builtInTypeSpec2");
}

void
JParser::identifier2( string& scope )
{
	zzRULE;
	ANTLRTokenPtr id1=NULL, id2=NULL;
	zzTRACEIN("identifier2");
	zzmatch(IDENT); labase++;
	
	if ( !guessing ) {
		id1 = (ANTLRTokenPtr)LT(1);
	}
	
	if ( !guessing ) {
	scope=pu->scopeCombine(scope, id1->getText());
	}
 consume();
	{
		while ( (LA(1)==DOT)
 ) {
			zzmatch(DOT); labase++;
			 consume();
			zzmatch(IDENT); labase++;
			
			if ( !guessing ) {
						id2 = (ANTLRTokenPtr)LT(1);
			}
			
			if ( !guessing ) {
			scope=pu->scopeCombine(scope, id2->getText());
			}
 consume();
		}
	}
	zzTRACEOUT("identifier2");
	return;
fail:
	if ( guessing ) zzGUESS_FAIL;
	syn(zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk);
	resynch(setwd3, 0x40);
	zzTRACEOUT("identifier2");
}

void
JParser::identifierStar(void)
{
	zzRULE;
	zzTRACEIN("identifierStar");
	zzGUESS_BLOCK
	zzGUESS
	if ( !zzrv && (LA(1)==IDENT) && (LA(2)==DOT) ) {
		{
			zzmatch(IDENT); labase++;
			 consume();
			{
				while ( (LA(1)==DOT) && (LA(2)==IDENT) ) {
					zzmatch(DOT); labase++;
					 consume();
					zzmatch(IDENT); labase++;
					 consume();
				}
			}
			zzmatch(DOT); labase++;
			 consume();
			zzmatch(STAR); labase++;
			 consume();
		}
		zzGUESS_DONE
		{
			zzmatch(IDENT); labase++;
			 consume();
			{
				while ( (LA(1)==DOT) && (LA(2)==IDENT) ) {
					zzmatch(DOT); labase++;
					 consume();
					zzmatch(IDENT); labase++;
					 consume();
				}
			}
			zzmatch(DOT); labase++;
			 consume();
			zzmatch(STAR); labase++;
			 consume();
		}
	}
	else {
		if ( !zzrv ) zzGUESS_DONE;
		if ( (LA(1)==IDENT) && (setwd3[LA(2)]&0x80) ) {
			zzmatch(IDENT); labase++;
			 consume();
			{
				while ( (LA(1)==DOT)
 ) {
					zzmatch(DOT); labase++;
					 consume();
					zzmatch(IDENT); labase++;
					 consume();
				}
			}
		}
		else {FAIL(2,err8,err9,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk); goto fail;}
	}
	zzTRACEOUT("identifierStar");
	return;
fail:
	if ( guessing ) zzGUESS_FAIL;
	syn(zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk);
	resynch(setwd4, 0x1);
	zzTRACEOUT("identifierStar");
}

void
JParser::modifier( Visibility& v )
{
	zzRULE;
	zzTRACEIN("modifier");
	if ( (LA(1)==PRIVATE) ) {
		zzmatch(PRIVATE); labase++;
		
		if ( !guessing ) {
		v=vPRIVATE;
		}
 consume();
	}
	else {
		if ( (LA(1)==PUBLIC) ) {
			zzmatch(PUBLIC); labase++;
			
			if ( !guessing ) {
			v=vPUBLIC;
			}
 consume();
		}
		else {
			if ( (LA(1)==PROTECTED) ) {
				zzmatch(PROTECTED); labase++;
				
				if ( !guessing ) {
				v=vPROTECTED;
				}
 consume();
			}
			else {
				if ( (LA(1)==STATIC) ) {
					zzmatch(STATIC); labase++;
					 consume();
				}
				else {
					if ( (LA(1)==TRANSIENT)
 ) {
						zzmatch(TRANSIENT); labase++;
						 consume();
					}
					else {
						if ( (LA(1)==FINAL) ) {
							zzmatch(FINAL); labase++;
							 consume();
						}
						else {
							if ( (LA(1)==ABSTRACT) ) {
								zzmatch(ABSTRACT); labase++;
								 consume();
							}
							else {
								if ( (LA(1)==NATIVE) ) {
									zzmatch(NATIVE); labase++;
									 consume();
								}
								else {
									if ( (LA(1)==THREADSAFE) ) {
										zzmatch(THREADSAFE); labase++;
										 consume();
									}
									else {
										if ( (LA(1)==SYNCHRONIZED)
 ) {
											zzmatch(SYNCHRONIZED); labase++;
											 consume();
										}
										else {
											if ( (LA(1)==VOLATILE) ) {
												zzmatch(VOLATILE); labase++;
												 consume();
											}
											else {FAIL(1,err10,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk); goto fail;}
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}
	zzTRACEOUT("modifier");
	return;
fail:
	if ( guessing ) zzGUESS_FAIL;
	syn(zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk);
	resynch(setwd4, 0x2);
	zzTRACEOUT("modifier");
}

void
JParser::classDefinition( string& parentScope )
{
	zzRULE;
	ANTLRTokenPtr id=NULL, sc=NULL, ic=NULL, cb=NULL;
	zzTRACEIN("classDefinition");
	
	int startLine=LT(1)->getLine(); 
	string className;
	zzmatch(CLASS); labase++;
	 consume();
	zzmatch(IDENT); labase++;
	
	if ( !guessing ) {
		id = (ANTLRTokenPtr)LT(1);
	}
	
	if ( !guessing ) {
	
	className= id->getText();
	}
 consume();
	superClassClause( className );
	implementsClause( className );
	classBlock( className );
	if ( !guessing ) {
	
	int endLine=LT(1)->getLine();
	ps->record_module_extent(startLine,endLine,className,"class",
	"definition",utDEFINITION);
	}
	zzTRACEOUT("classDefinition");
	return;
fail:
	if ( guessing ) zzGUESS_FAIL;
	syn(zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk);
	resynch(setwd4, 0x4);
	zzTRACEOUT("classDefinition");
}

void
JParser::superClassClause( const string& className )
{
	zzRULE;
	zzTRACEIN("superClassClause");
	
	int startLine=LT(1)->getLine(); 
	string parentName;
	{
		if ( (LA(1)==EXTENDS) ) {
			zzmatch(EXTENDS); labase++;
			 consume();
			identifier2( parentName );
			if ( !guessing ) {
			
			ps->record_userel_extent(
			startLine,startLine,
			className,"",parentName,"extends",
			// no private inheritance in Java
			vPUBLIC, utINHERITS);
			}
		}
	}
	zzTRACEOUT("superClassClause");
	return;
fail:
	if ( guessing ) zzGUESS_FAIL;
	syn(zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk);
	resynch(setwd4, 0x8);
	zzTRACEOUT("superClassClause");
}

void
JParser::interfaceDefinition( const string& parentScope )
{
	zzRULE;
	ANTLRTokenPtr id=NULL, ie=NULL, cb=NULL;
	zzTRACEIN("interfaceDefinition");
	string interfaceName;
	zzmatch(INTERFACE); labase++;
	 consume();
	zzmatch(IDENT); labase++;
	
	if ( !guessing ) {
		id = (ANTLRTokenPtr)LT(1);
	}
	
	if ( !guessing ) {
	
	interfaceName= id->getText();
	}
 consume();
	interfaceExtends( interfaceName );
	classBlock( interfaceName );
	zzTRACEOUT("interfaceDefinition");
	return;
fail:
	if ( guessing ) zzGUESS_FAIL;
	syn(zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk);
	resynch(setwd4, 0x10);
	zzTRACEOUT("interfaceDefinition");
}

void
JParser::classBlock( const string& className )
{
	zzRULE;
	zzTRACEIN("classBlock");
	zzmatch(LCURLY); labase++;
	 consume();
	{
		for (;;) {
			if ( !((setwd4[LA(1)]&0x20))) break;
			if ( (setwd4[LA(1)]&0x40) ) {
				field( className );
			}
			else {
				if ( (LA(1)==SEMI)
 ) {
					zzmatch(SEMI); labase++;
					 consume();
				}
				else break; /* MR6 code for exiting loop "for sure" */
			}
		}
	}
	zzmatch(RCURLY); labase++;
	 consume();
	zzTRACEOUT("classBlock");
	return;
fail:
	if ( guessing ) zzGUESS_FAIL;
	syn(zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk);
	resynch(setwd4, 0x80);
	zzTRACEOUT("classBlock");
}

void
JParser::interfaceExtends( const string& className )
{
	zzRULE;
	zzTRACEIN("interfaceExtends");
	
	int startLine=LT(1)->getLine(); 
	string parentName;
	{
		if ( (LA(1)==EXTENDS) ) {
			zzmatch(EXTENDS); labase++;
			 consume();
			identifier2( parentName );
			moreSuperclassNames( className,"extends" );
			if ( !guessing ) {
			
			ps->record_userel_extent(
			startLine,startLine,
			className,"",parentName,"extends",
			// no private inheritance in Java
			vPUBLIC, utINHERITS);
			}
		}
	}
	zzTRACEOUT("interfaceExtends");
	return;
fail:
	if ( guessing ) zzGUESS_FAIL;
	syn(zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk);
	resynch(setwd5, 0x1);
	zzTRACEOUT("interfaceExtends");
}

void
JParser::implementsClause( const std::string& className )
{
	zzRULE;
	zzTRACEIN("implementsClause");
	
	int startLine=LT(1)->getLine(); 
	string parentName;
	{
		if ( (LA(1)==IMPLEMENTS) ) {
			zzmatch(IMPLEMENTS); labase++;
			 consume();
			identifier2( parentName );
			moreSuperclassNames( className,"implements" );
		}
	}
	if ( !guessing ) {
	
	ps->record_userel_extent(
	startLine,startLine,
	className,"",parentName,"implements",
	// no private inheritance in Java
	vPUBLIC, utINHERITS);
	}
	zzTRACEOUT("implementsClause");
	return;
fail:
	if ( guessing ) zzGUESS_FAIL;
	syn(zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk);
	resynch(setwd5, 0x2);
	zzTRACEOUT("implementsClause");
}

void
JParser::moreSuperclassNames( const string& className, const string& inheritType )
{
	zzRULE;
	zzTRACEIN("moreSuperclassNames");
	
	int startLine=LT(1)->getLine(); 
	string parentName;
	{
		while ( (LA(1)==COMMA) ) {
			zzmatch(COMMA); labase++;
			 consume();
			identifier2( parentName );
			if ( !guessing ) {
			
			ps->record_userel_extent(
			startLine,startLine,
			className,"",parentName,inheritType,
			// no private inheritance in Java
			vPUBLIC, utINHERITS);
			}
		}
	}
	zzTRACEOUT("moreSuperclassNames");
	return;
fail:
	if ( guessing ) zzGUESS_FAIL;
	syn(zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk);
	resynch(setwd5, 0x4);
	zzTRACEOUT("moreSuperclassNames");
}

void
JParser::field( const string& className )
{
	zzRULE;
	ANTLRTokenPtr mods=NULL, cd=NULL, id=NULL, t=NULL, s3=NULL;
	zzTRACEIN("field");
	
	string scope=className, typeName; 
	bool isBuiltIn;
	Visibility v=vDONTKNOW;
	int startLine=LT(1)->getLine();
	if ( (setwd5[LA(1)]&0x8) && (setwd5[LA(2)]&0x10) ) {
		modifiers( v );
		{
			if ( (LA(1)==IDENT) && 
(LA(2)==LPAREN) ) {
				ctorDefinition( className,v );
			}
			else {
				if ( (LA(1)==CLASS) ) {
					classDefinition( scope );
				}
				else {
					if ( (LA(1)==INTERFACE) ) {
						interfaceDefinition( scope );
					}
					else {
						if ( (setwd5[LA(1)]&0x20) && (setwd5[LA(2)]&0x40) ) {
							typeSpec2( typeName,isBuiltIn );
							typedDeclaration( className,typeName,isBuiltIn,v,startLine );
						}
						else {FAIL(2,err11,err12,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk); goto fail;}
					}
				}
			}
		}
	}
	else {
		if ( (LA(1)==STATIC) && (LA(2)==LCURLY) ) {
			zzmatch(STATIC); labase++;
			 consume();
			compoundStatement();
		}
		else {
			if ( (LA(1)==LCURLY)
 ) {
				compoundStatement();
			}
			else {FAIL(2,err13,err14,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk); goto fail;}
		}
	}
	zzTRACEOUT("field");
	return;
fail:
	if ( guessing ) zzGUESS_FAIL;
	syn(zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk);
	resynch(setwd5, 0x80);
	zzTRACEOUT("field");
}

void
JParser::typedDeclaration( const string& className,const string& typeName,
	 bool isBuiltIn, Visibility v, int startLine )
{
	zzRULE;
	zzTRACEIN("typedDeclaration");
	if ( (LA(1)==IDENT) && (LA(2)==LPAREN) ) {
		methodDefinition( className,typeName,isBuiltIn,v,startLine );
	}
	else {
		if ( (LA(1)==IDENT) && (setwd6[LA(2)]&0x1) ) {
			variableDefinitions( className,typeName,isBuiltIn,v,startLine );
			zzmatch(SEMI); labase++;
			 consume();
		}
		else {FAIL(2,err15,err16,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk); goto fail;}
	}
	zzTRACEOUT("typedDeclaration");
	return;
fail:
	if ( guessing ) zzGUESS_FAIL;
	syn(zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk);
	resynch(setwd6, 0x2);
	zzTRACEOUT("typedDeclaration");
}

void
JParser::methodDefinition( const string& className, const string& returnType, bool rtIsBuiltIn,
	Visibility v, int startLine )
{
	zzRULE;
	ANTLRTokenPtr id=NULL, param=NULL, rt=NULL, tc=NULL, s2=NULL;
	zzTRACEIN("methodDefinition");
	
	string paramList; 
	bool isDefinition=false;
	zzmatch(IDENT); labase++;
	
	if ( !guessing ) {
		id = (ANTLRTokenPtr)LT(1);
	}
	 consume();
	zzmatch(LPAREN); labase++;
	 consume();
	parameterDeclarationList( paramList,className,v );
	zzmatch(RPAREN); labase++;
	 consume();
	returnTypeBrackersOnEndOfMethodHead();
	{
		if ( (LA(1)==THROWS) ) {
			throwsClause();
		}
	}
	{
		if ( (LA(1)==LCURLY) ) {
			compoundStatement();
			if ( !guessing ) {
			isDefinition=true;
			}
		}
		else {
			if ( (LA(1)==SEMI)
 ) {
				zzmatch(SEMI); labase++;
				 consume();
			}
			else {FAIL(1,err17,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk); goto fail;}
		}
	}
	if ( !guessing ) {
	
	int endLine=LT(1)->getLine();
	string methodName= id->getText();
	if(isDefinition==false)
	{
		ps->record_function_extent(startLine,endLine,
		returnType,className,	
		methodName,paramList,
		"declaration",v,
		utDECLARATION);
	}
	else
	{
	ps->record_function_extent(startLine,endLine,
	returnType,className,
	methodName,paramList,
	"definition",v,
	utDEFINITION);
}

			// the return value is effectively an anonymous 
// parameter...
if(rtIsBuiltIn==false)
{
	// In Java, all parameter passing are effectively by
	// reference (I think).
	ps->record_userel_extent(startLine,startLine,
	className,"",returnType,
	"return type",v,utPARBYREF);
}
	}
	zzTRACEOUT("methodDefinition");
	return;
fail:
	if ( guessing ) zzGUESS_FAIL;
	syn(zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk);
	resynch(setwd6, 0x4);
	zzTRACEOUT("methodDefinition");
}

void
JParser::variableDefinitions( const string& className, const string& returnType, bool rtIsBuiltIn,
	Visibility v, int startLine )
{
	zzRULE;
	zzTRACEIN("variableDefinitions");
	variableDeclarator();
	{
		while ( (LA(1)==COMMA) ) {
			zzmatch(COMMA); labase++;
			 consume();
			variableDeclarator();
		}
	}
	if ( !guessing ) {
	
	if(rtIsBuiltIn==false)
	{
		// In Java, all containment relationships are effectively by
		// reference (I think).
		ps->record_userel_extent(startLine,startLine,
		className,"",returnType,
		"member variable",v,utHASBYREF);
	}
	}
	zzTRACEOUT("variableDefinitions");
	return;
fail:
	if ( guessing ) zzGUESS_FAIL;
	syn(zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk);
	resynch(setwd6, 0x8);
	zzTRACEOUT("variableDefinitions");
}

void
JParser::variableDeclarator(void)
{
	zzRULE;
	ANTLRTokenPtr id=NULL, d=NULL, v=NULL;
	zzTRACEIN("variableDeclarator");
	zzmatch(IDENT); labase++;
	
	if ( !guessing ) {
		id = (ANTLRTokenPtr)LT(1);
	}
	 consume();
	declaratorBrackets();
	varInitializer();
	zzTRACEOUT("variableDeclarator");
	return;
fail:
	if ( guessing ) zzGUESS_FAIL;
	syn(zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk);
	resynch(setwd6, 0x10);
	zzTRACEOUT("variableDeclarator");
}

void
JParser::declaratorBrackets(void)
{
	zzRULE;
	ANTLRTokenPtr lb=NULL;
	zzTRACEIN("declaratorBrackets");
	{
		while ( (LA(1)==LBRACK) ) {
			zzmatch(LBRACK); labase++;
			
			if ( !guessing ) {
						lb = (ANTLRTokenPtr)LT(1);
			}
			 consume();
			zzmatch(RBRACK); labase++;
			 consume();
		}
	}
	zzTRACEOUT("declaratorBrackets");
	return;
fail:
	if ( guessing ) zzGUESS_FAIL;
	syn(zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk);
	resynch(setwd6, 0x20);
	zzTRACEOUT("declaratorBrackets");
}

void
JParser::varInitializer(void)
{
	zzRULE;
	zzTRACEIN("varInitializer");
	{
		if ( (LA(1)==ASSIGN) ) {
			zzmatch(ASSIGN); labase++;
			 consume();
			initializer();
		}
	}
	zzTRACEOUT("varInitializer");
	return;
fail:
	if ( guessing ) zzGUESS_FAIL;
	syn(zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk);
	resynch(setwd6, 0x40);
	zzTRACEOUT("varInitializer");
}

void
JParser::arrayInitializer(void)
{
	zzRULE;
	ANTLRTokenPtr lc=NULL;
	zzTRACEIN("arrayInitializer");
	zzmatch(LCURLY); labase++;
	
	if ( !guessing ) {
		lc = (ANTLRTokenPtr)LT(1);
	}
	 consume();
	{
		if ( (setwd6[LA(1)]&0x80) ) {
			initializer();
			{
				while ( (LA(1)==COMMA) && 
(setwd7[LA(2)]&0x1) ) {
					zzmatch(COMMA); labase++;
					 consume();
					initializer();
				}
			}
			{
				if ( (LA(1)==COMMA) ) {
					zzmatch(COMMA); labase++;
					 consume();
				}
			}
		}
	}
	zzmatch(RCURLY); labase++;
	 consume();
	zzTRACEOUT("arrayInitializer");
	return;
fail:
	if ( guessing ) zzGUESS_FAIL;
	syn(zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk);
	resynch(setwd7, 0x2);
	zzTRACEOUT("arrayInitializer");
}

void
JParser::initializer(void)
{
	zzRULE;
	zzTRACEIN("initializer");
	if ( (setwd7[LA(1)]&0x4) ) {
		expression();
	}
	else {
		if ( (LA(1)==LCURLY) ) {
			arrayInitializer();
		}
		else {FAIL(1,err18,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk); goto fail;}
	}
	zzTRACEOUT("initializer");
	return;
fail:
	if ( guessing ) zzGUESS_FAIL;
	syn(zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk);
	resynch(setwd7, 0x8);
	zzTRACEOUT("initializer");
}

void
JParser::ctorDefinition( const string& className, Visibility v )
{
	zzRULE;
	zzTRACEIN("ctorDefinition");
	
	// The following assertion seemed like a good idea
	// at the time, but it fails for nested classes.
	// assert(className==LT(1)->getText());
	int startLine=LT(1)->getLine();
	string paramList;
	ctorHead( paramList,className,v );
	compoundStatement();
	if ( !guessing ) {
	
	int endLine=LT(1)->getLine();
	ps->record_function_extent(startLine,endLine,
	"",className,className,paramList,
	"definition",v,utDEFINITION);
	}
	zzTRACEOUT("ctorDefinition");
	return;
fail:
	if ( guessing ) zzGUESS_FAIL;
	syn(zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk);
	resynch(setwd7, 0x10);
	zzTRACEOUT("ctorDefinition");
}

void
JParser::ctorHead( string& paramList,const string& className, Visibility v )
{
	zzRULE;
	zzTRACEIN("ctorHead");
	zzmatch(IDENT); labase++;
	 consume();
	zzmatch(LPAREN); labase++;
	 consume();
	parameterDeclarationList( paramList,className,v );
	zzmatch(RPAREN); labase++;
	 consume();
	{
		if ( (LA(1)==THROWS) ) {
			throwsClause();
		}
	}
	zzTRACEOUT("ctorHead");
	return;
fail:
	if ( guessing ) zzGUESS_FAIL;
	syn(zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk);
	resynch(setwd7, 0x20);
	zzTRACEOUT("ctorHead");
}

void
JParser::throwsClause(void)
{
	zzRULE;
	zzTRACEIN("throwsClause");
	zzmatch(THROWS); labase++;
	 consume();
	identifier();
	{
		while ( (LA(1)==COMMA)
 ) {
			zzmatch(COMMA); labase++;
			 consume();
			identifier();
		}
	}
	zzTRACEOUT("throwsClause");
	return;
fail:
	if ( guessing ) zzGUESS_FAIL;
	syn(zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk);
	resynch(setwd7, 0x40);
	zzTRACEOUT("throwsClause");
}

void
JParser::returnTypeBrackersOnEndOfMethodHead(void)
{
	zzRULE;
	zzTRACEIN("returnTypeBrackersOnEndOfMethodHead");
	{
		while ( (LA(1)==LBRACK) ) {
			zzmatch(LBRACK); labase++;
			 consume();
			zzmatch(RBRACK); labase++;
			 consume();
		}
	}
	zzTRACEOUT("returnTypeBrackersOnEndOfMethodHead");
	return;
fail:
	if ( guessing ) zzGUESS_FAIL;
	syn(zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk);
	resynch(setwd7, 0x80);
	zzTRACEOUT("returnTypeBrackersOnEndOfMethodHead");
}

void
JParser::parameterDeclarationList( 
		// a string in which we accumulate a string of all parameters
		string& paramList, 
		// the name of the owner class for the method
	 	const string& clientName, 
		// visibility of the method
	 	const Visibility& v      
	 )
{
	zzRULE;
	zzTRACEIN("parameterDeclarationList");
	string paramItem;
	if ( !guessing ) {
	paramList+="(";
	}
	{
		if ( (setwd8[LA(1)]&0x1) ) {
			parameterDeclaration( paramList,clientName,v,true );
			{
				while ( (LA(1)==COMMA) ) {
					zzmatch(COMMA); labase++;
					
					if ( !guessing ) {
					paramList+=",";
					}
 consume();
					parameterDeclaration( paramList,clientName,v,true );
				}
			}
		}
	}
	if ( !guessing ) {
	paramList+=")";
	}
	zzTRACEOUT("parameterDeclarationList");
	return;
fail:
	if ( guessing ) zzGUESS_FAIL;
	syn(zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk);
	resynch(setwd8, 0x2);
	zzTRACEOUT("parameterDeclarationList");
}

void
JParser::parameterDeclaration( 
		// a string in which we accumulate a string of all parameters
		string& paramList, 
		// the name of the owner class for the method
	 	const string& clientName, 
		// visibility of the method
	 	const Visibility& v,      
		// is this rule being used from a parameter list (in which case
		// we want to do record_userel_extent) or from a syntacticly similar
		// catch statement, which is buried in the implementation of a method 
		// and does not interest us
		bool inParameterList
	 )
{
	zzRULE;
	ANTLRTokenPtr pm=NULL, t=NULL, id=NULL, pd=NULL;
	zzTRACEIN("parameterDeclaration");
	
	string mod, typeName, bracks; bool isBuiltIn; 
	int startLine=LT(1)->getLine();
	parameterModifier( mod );
	typeSpec2( typeName,isBuiltIn );
	zzmatch(IDENT); labase++;
	
	if ( !guessing ) {
		id = (ANTLRTokenPtr)LT(1);
	}
	 consume();
	parameterDeclaratorBrackets( bracks );
	if ( !guessing ) {
	
	if(inParameterList)
	{
		paramList+=(mod+typeName+bracks); 
		
				if(isBuiltIn==false)
		{
			// In Java, all parameter passing is effectively by
			// reference (I think).
			ps->record_userel_extent(startLine,startLine,
			clientName,"",typeName,
			"normal parameter",v,utPARBYREF);
		}
	}
	}
	zzTRACEOUT("parameterDeclaration");
	return;
fail:
	if ( guessing ) zzGUESS_FAIL;
	syn(zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk);
	resynch(setwd8, 0x4);
	zzTRACEOUT("parameterDeclaration");
}

void
JParser::parameterDeclaratorBrackets( string& bracks )
{
	zzRULE;
	zzTRACEIN("parameterDeclaratorBrackets");
	{
		while ( (LA(1)==LBRACK) ) {
			zzmatch(LBRACK); labase++;
			 consume();
			zzmatch(RBRACK); labase++;
			
			if ( !guessing ) {
			bracks+="[]";
			}
 consume();
		}
	}
	zzTRACEOUT("parameterDeclaratorBrackets");
	return;
fail:
	if ( guessing ) zzGUESS_FAIL;
	syn(zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk);
	resynch(setwd8, 0x8);
	zzTRACEOUT("parameterDeclaratorBrackets");
}

void
JParser::parameterModifier( string& paramList )
{
	zzRULE;
	ANTLRTokenPtr f=NULL;
	zzTRACEIN("parameterModifier");
	{
		if ( (LA(1)==FINAL)
 ) {
			zzmatch(FINAL); labase++;
			
			if ( !guessing ) {
						f = (ANTLRTokenPtr)LT(1);
			}
			
			if ( !guessing ) {
			paramList+= f->getText();
			}
 consume();
		}
	}
	zzTRACEOUT("parameterModifier");
	return;
fail:
	if ( guessing ) zzGUESS_FAIL;
	syn(zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk);
	resynch(setwd8, 0x10);
	zzTRACEOUT("parameterModifier");
}

void
JParser::compoundStatement(void)
{
	zzRULE;
	zzTRACEIN("compoundStatement");
	zzmatch(LCURLY); labase++;
	 consume();
	{
		while ( (setwd8[LA(1)]&0x20) ) {
			statement();
		}
	}
	zzmatch(RCURLY); labase++;
	 consume();
	zzTRACEOUT("compoundStatement");
	return;
fail:
	if ( guessing ) zzGUESS_FAIL;
	syn(zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk);
	resynch(setwd8, 0x40);
	zzTRACEOUT("compoundStatement");
}

void
JParser::statement(void)
{
	zzRULE;
	ANTLRTokenPtr c=NULL;
	zzTRACEIN("statement");
	zzGUESS_BLOCK
	string scope;
	if ( (LA(1)==IF) ) {
		ifStatement();
	}
	else {
		if ( (LA(1)==FOR) ) {
			forStatement();
		}
		else {
			if ( (LA(1)==WHILE) ) {
				whileStatement();
			}
			else {
				if ( (LA(1)==DO)
 ) {
					doWhileStatement();
				}
				else {
					if ( (LA(1)==BREAK) ) {
						breakStatement();
					}
					else {
						if ( (LA(1)==CONTINUE) ) {
							continueStatement();
						}
						else {
							if ( (LA(1)==RETURN) ) {
								returnStatement();
							}
							else {
								if ( (LA(1)==SWITCH) ) {
									switchStatement();
								}
								else {
									if ( (LA(1)==THROW)
 ) {
										throwStatement();
									}
									else {
										if ( (LA(1)==TRY) ) {
											tryBlock();
										}
										else {
											if ( (LA(1)==SYNCHRONIZED) && (LA(2)==LPAREN) ) {
												syncStatement();
											}
											else {
												if ( (LA(1)==SEMI) ) {
													emptyStatement();
												}
												else {
													if ( (LA(1)==LCURLY) ) {
														compoundStatement();
													}
													else {
														if ( (LA(1)==CLASS)
 ) {
															classDefinition( scope );
														}
														else {
															if ( (LA(1)==FINAL) && (LA(2)==CLASS) ) {
																zzmatch(FINAL); labase++;
																 consume();
																classDefinition( scope );
															}
															else {
																if ( (LA(1)==ABSTRACT) && (LA(2)==CLASS) ) {
																	zzmatch(ABSTRACT); labase++;
																	 consume();
																	classDefinition( scope );
																}
																else {
																	zzGUESS
																	if ( !zzrv && (setwd8[LA(1)]&0x80) && (setwd9[LA(2)]&0x1) ) {
																		{
																			declaration();
																		}
																		zzGUESS_DONE
																		declaration();
																		zzmatch(SEMI); labase++;
																		 consume();
																	}
																	else {
																		if ( !zzrv ) zzGUESS_DONE;
																		if ( (setwd9[LA(1)]&0x2) && (setwd9[LA(2)]&0x4) ) {
																			expression();
																			zzmatch(SEMI); labase++;
																			 consume();
																		}
																		else {
																			if ( !zzrv ) zzGUESS_DONE;
																			if ( (LA(1)==IDENT) && 
(LA(2)==COLON) ) {
																				zzmatch(IDENT); labase++;
																				 consume();
																				zzmatch(COLON); labase++;
																				
																				if ( !guessing ) {
																																								c = (ANTLRTokenPtr)LT(1);
																				}
																				 consume();
																				statement();
																			}
																			else {FAIL(2,err19,err20,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk); goto fail;}
																		}
																	}
																}
															}
														}
													}
												}
											}
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}
	zzTRACEOUT("statement");
	return;
fail:
	if ( guessing ) zzGUESS_FAIL;
	syn(zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk);
	resynch(setwd9, 0x8);
	zzTRACEOUT("statement");
}

void
JParser::ifStatement(void)
{
	zzRULE;
	zzTRACEIN("ifStatement");
	zzmatch(IF); labase++;
	 consume();
	zzmatch(LPAREN); labase++;
	 consume();
	expression();
	zzmatch(RPAREN); labase++;
	 consume();
	statement();
	optElseClause();
	zzTRACEOUT("ifStatement");
	return;
fail:
	if ( guessing ) zzGUESS_FAIL;
	syn(zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk);
	resynch(setwd9, 0x10);
	zzTRACEOUT("ifStatement");
}

void
JParser::forStatement(void)
{
	zzRULE;
	zzTRACEIN("forStatement");
	zzmatch(FOR); labase++;
	 consume();
	zzmatch(LPAREN); labase++;
	 consume();
	forInit();
	zzmatch(SEMI); labase++;
	 consume();
	forCond();
	zzmatch(SEMI); labase++;
	 consume();
	forIter();
	zzmatch(RPAREN); labase++;
	 consume();
	statement();
	zzTRACEOUT("forStatement");
	return;
fail:
	if ( guessing ) zzGUESS_FAIL;
	syn(zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk);
	resynch(setwd9, 0x20);
	zzTRACEOUT("forStatement");
}

void
JParser::whileStatement(void)
{
	zzRULE;
	zzTRACEIN("whileStatement");
	zzmatch(WHILE); labase++;
	 consume();
	zzmatch(LPAREN); labase++;
	 consume();
	expression();
	zzmatch(RPAREN); labase++;
	 consume();
	statement();
	zzTRACEOUT("whileStatement");
	return;
fail:
	if ( guessing ) zzGUESS_FAIL;
	syn(zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk);
	resynch(setwd9, 0x40);
	zzTRACEOUT("whileStatement");
}

void
JParser::doWhileStatement(void)
{
	zzRULE;
	zzTRACEIN("doWhileStatement");
	zzmatch(DO); labase++;
	 consume();
	statement();
	zzmatch(WHILE); labase++;
	 consume();
	zzmatch(LPAREN); labase++;
	 consume();
	expression();
	zzmatch(RPAREN); labase++;
	 consume();
	zzmatch(SEMI); labase++;
	 consume();
	zzTRACEOUT("doWhileStatement");
	return;
fail:
	if ( guessing ) zzGUESS_FAIL;
	syn(zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk);
	resynch(setwd9, 0x80);
	zzTRACEOUT("doWhileStatement");
}

void
JParser::breakStatement(void)
{
	zzRULE;
	zzTRACEIN("breakStatement");
	zzmatch(BREAK); labase++;
	 consume();
	{
		if ( (LA(1)==IDENT) ) {
			zzmatch(IDENT); labase++;
			 consume();
		}
	}
	zzmatch(SEMI); labase++;
	 consume();
	zzTRACEOUT("breakStatement");
	return;
fail:
	if ( guessing ) zzGUESS_FAIL;
	syn(zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk);
	resynch(setwd10, 0x1);
	zzTRACEOUT("breakStatement");
}

void
JParser::continueStatement(void)
{
	zzRULE;
	zzTRACEIN("continueStatement");
	zzmatch(CONTINUE); labase++;
	 consume();
	{
		if ( (LA(1)==IDENT) ) {
			zzmatch(IDENT); labase++;
			 consume();
		}
	}
	zzmatch(SEMI); labase++;
	 consume();
	zzTRACEOUT("continueStatement");
	return;
fail:
	if ( guessing ) zzGUESS_FAIL;
	syn(zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk);
	resynch(setwd10, 0x2);
	zzTRACEOUT("continueStatement");
}

void
JParser::returnStatement(void)
{
	zzRULE;
	zzTRACEIN("returnStatement");
	zzmatch(RETURN); labase++;
	 consume();
	{
		if ( (setwd10[LA(1)]&0x4) ) {
			expression();
		}
	}
	zzmatch(SEMI); labase++;
	 consume();
	zzTRACEOUT("returnStatement");
	return;
fail:
	if ( guessing ) zzGUESS_FAIL;
	syn(zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk);
	resynch(setwd10, 0x8);
	zzTRACEOUT("returnStatement");
}

void
JParser::switchStatement(void)
{
	zzRULE;
	zzTRACEIN("switchStatement");
	zzmatch(SWITCH); labase++;
	 consume();
	zzmatch(LPAREN); labase++;
	 consume();
	expression();
	zzmatch(RPAREN); labase++;
	 consume();
	zzmatch(LCURLY); labase++;
	 consume();
	{
		while ( (setwd10[LA(1)]&0x10) ) {
			casesGroup();
		}
	}
	zzmatch(RCURLY); labase++;
	 consume();
	zzTRACEOUT("switchStatement");
	return;
fail:
	if ( guessing ) zzGUESS_FAIL;
	syn(zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk);
	resynch(setwd10, 0x20);
	zzTRACEOUT("switchStatement");
}

void
JParser::throwStatement(void)
{
	zzRULE;
	zzTRACEIN("throwStatement");
	zzmatch(THROW); labase++;
	 consume();
	expression();
	zzmatch(SEMI); labase++;
	 consume();
	zzTRACEOUT("throwStatement");
	return;
fail:
	if ( guessing ) zzGUESS_FAIL;
	syn(zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk);
	resynch(setwd10, 0x40);
	zzTRACEOUT("throwStatement");
}

void
JParser::syncStatement(void)
{
	zzRULE;
	zzTRACEIN("syncStatement");
	zzmatch(SYNCHRONIZED); labase++;
	 consume();
	zzmatch(LPAREN); labase++;
	 consume();
	expression();
	zzmatch(RPAREN); labase++;
	 consume();
	compoundStatement();
	zzTRACEOUT("syncStatement");
	return;
fail:
	if ( guessing ) zzGUESS_FAIL;
	syn(zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk);
	resynch(setwd10, 0x80);
	zzTRACEOUT("syncStatement");
}

void
JParser::emptyStatement(void)
{
	zzRULE;
	zzTRACEIN("emptyStatement");
	zzmatch(SEMI); labase++;
	 consume();
	zzTRACEOUT("emptyStatement");
	return;
fail:
	if ( guessing ) zzGUESS_FAIL;
	syn(zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk);
	resynch(setwd11, 0x1);
	zzTRACEOUT("emptyStatement");
}

void
JParser::optElseClause(void)
{
	zzRULE;
	zzTRACEIN("optElseClause");
	zzGUESS_BLOCK
	zzGUESS
	if ( !zzrv && (LA(1)==ELSE) && 
(setwd11[LA(2)]&0x2) ) {
		{
			zzmatch(ELSE); labase++;
			 consume();
		}
		zzGUESS_DONE
		zzmatch(ELSE); labase++;
		 consume();
		statement();
	}
	else {
		if ( !zzrv ) zzGUESS_DONE;
		if ( (setwd11[LA(1)]&0x4) && (setwd11[LA(2)]&0x8) ) {
		}
		else {FAIL(2,err21,err22,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk); goto fail;}
	}
	zzTRACEOUT("optElseClause");
	return;
fail:
	if ( guessing ) zzGUESS_FAIL;
	syn(zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk);
	resynch(setwd11, 0x10);
	zzTRACEOUT("optElseClause");
}

void
JParser::casesGroup(void)
{
	zzRULE;
	zzTRACEIN("casesGroup");
	cases();
	caseSList();
	zzTRACEOUT("casesGroup");
	return;
fail:
	if ( guessing ) zzGUESS_FAIL;
	syn(zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk);
	resynch(setwd11, 0x20);
	zzTRACEOUT("casesGroup");
}

void
JParser::cases(void)
{
	zzRULE;
	zzTRACEIN("cases");
	aCase();
	optMoreCases();
	zzTRACEOUT("cases");
	return;
fail:
	if ( guessing ) zzGUESS_FAIL;
	syn(zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk);
	resynch(setwd11, 0x40);
	zzTRACEOUT("cases");
}

void
JParser::optMoreCases(void)
{
	zzRULE;
	zzTRACEIN("optMoreCases");
	zzGUESS_BLOCK
	zzGUESS
	if ( !zzrv && (setwd11[LA(1)]&0x80) && (setwd12[LA(2)]&0x1) ) {
		{
			if ( (LA(1)==CASE) ) {
				zzmatch(CASE); labase++;
				 consume();
			}
			else {
				if ( (LA(1)==DEFAULT) ) {
					zzmatch(DEFAULT); labase++;
					 consume();
				}
				else {FAIL(1,err23,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk); goto fail;}
			}
		}
		zzGUESS_DONE
		aCase();
		optMoreCases();
	}
	else {
		if ( !zzrv ) zzGUESS_DONE;
		if ( (setwd12[LA(1)]&0x2) && 
(setwd12[LA(2)]&0x4) ) {
		}
		else {FAIL(2,err24,err25,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk); goto fail;}
	}
	zzTRACEOUT("optMoreCases");
	return;
fail:
	if ( guessing ) zzGUESS_FAIL;
	syn(zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk);
	resynch(setwd12, 0x8);
	zzTRACEOUT("optMoreCases");
}

void
JParser::aCase(void)
{
	zzRULE;
	zzTRACEIN("aCase");
	if ( (LA(1)==DEFAULT) ) {
		zzmatch(DEFAULT); labase++;
		 consume();
		zzmatch(COLON); labase++;
		 consume();
	}
	else {
		if ( (LA(1)==CASE) ) {
			zzmatch(CASE); labase++;
			 consume();
			expression();
			zzmatch(COLON); labase++;
			 consume();
		}
		else {FAIL(1,err26,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk); goto fail;}
	}
	zzTRACEOUT("aCase");
	return;
fail:
	if ( guessing ) zzGUESS_FAIL;
	syn(zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk);
	resynch(setwd12, 0x10);
	zzTRACEOUT("aCase");
}

void
JParser::caseSList(void)
{
	zzRULE;
	zzTRACEIN("caseSList");
	{
		while ( (setwd12[LA(1)]&0x20) ) {
			statement();
		}
	}
	zzTRACEOUT("caseSList");
	return;
fail:
	if ( guessing ) zzGUESS_FAIL;
	syn(zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk);
	resynch(setwd12, 0x40);
	zzTRACEOUT("caseSList");
}

void
JParser::forInit(void)
{
	zzRULE;
	zzTRACEIN("forInit");
	zzGUESS_BLOCK
	zzGUESS
	if ( !zzrv && (setwd12[LA(1)]&0x80) && (setwd13[LA(2)]&0x1) ) {
		{
			declaration();
		}
		zzGUESS_DONE
		{
			declaration();
		}
	}
	else {
		if ( !zzrv ) zzGUESS_DONE;
		if ( (setwd13[LA(1)]&0x2) && 
(setwd13[LA(2)]&0x4) ) {
			expressionList();
		}
		else {
			if ( !zzrv ) zzGUESS_DONE;
			if ( (LA(1)==SEMI) ) {
			}
			else {FAIL(2,err27,err28,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk); goto fail;}
		}
	}
	zzTRACEOUT("forInit");
	return;
fail:
	if ( guessing ) zzGUESS_FAIL;
	syn(zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk);
	resynch(setwd13, 0x8);
	zzTRACEOUT("forInit");
}

void
JParser::forCond(void)
{
	zzRULE;
	zzTRACEIN("forCond");
	{
		if ( (setwd13[LA(1)]&0x10) ) {
			expression();
		}
	}
	zzTRACEOUT("forCond");
	return;
fail:
	if ( guessing ) zzGUESS_FAIL;
	syn(zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk);
	resynch(setwd13, 0x20);
	zzTRACEOUT("forCond");
}

void
JParser::forIter(void)
{
	zzRULE;
	zzTRACEIN("forIter");
	{
		if ( (setwd13[LA(1)]&0x40) ) {
			expressionList();
		}
	}
	zzTRACEOUT("forIter");
	return;
fail:
	if ( guessing ) zzGUESS_FAIL;
	syn(zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk);
	resynch(setwd13, 0x80);
	zzTRACEOUT("forIter");
}

void
JParser::tryBlock(void)
{
	zzRULE;
	zzTRACEIN("tryBlock");
	zzmatch(TRY); labase++;
	 consume();
	compoundStatement();
	{
		while ( (LA(1)==CATCH) ) {
			handler();
		}
	}
	{
		if ( (LA(1)==FINALLY)
 ) {
			zzmatch(FINALLY); labase++;
			 consume();
			compoundStatement();
		}
	}
	zzTRACEOUT("tryBlock");
	return;
fail:
	if ( guessing ) zzGUESS_FAIL;
	syn(zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk);
	resynch(setwd14, 0x1);
	zzTRACEOUT("tryBlock");
}

void
JParser::handler(void)
{
	zzRULE;
	zzTRACEIN("handler");
	Visibility dv;
	zzmatch(CATCH); labase++;
	 consume();
	zzmatch(LPAREN); labase++;
	 consume();
	parameterDeclaration( d1,d2,dv,false );
	zzmatch(RPAREN); labase++;
	 consume();
	compoundStatement();
	zzTRACEOUT("handler");
	return;
fail:
	if ( guessing ) zzGUESS_FAIL;
	syn(zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk);
	resynch(setwd14, 0x2);
	zzTRACEOUT("handler");
}

void
JParser::expression(void)
{
	zzRULE;
	zzTRACEIN("expression");
	assignmentExpression();
	zzTRACEOUT("expression");
	return;
fail:
	if ( guessing ) zzGUESS_FAIL;
	syn(zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk);
	resynch(setwd14, 0x4);
	zzTRACEOUT("expression");
}

void
JParser::expressionList(void)
{
	zzRULE;
	zzTRACEIN("expressionList");
	expression();
	{
		while ( (LA(1)==COMMA) ) {
			zzmatch(COMMA); labase++;
			 consume();
			expression();
		}
	}
	zzTRACEOUT("expressionList");
	return;
fail:
	if ( guessing ) zzGUESS_FAIL;
	syn(zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk);
	resynch(setwd14, 0x8);
	zzTRACEOUT("expressionList");
}

void
JParser::assignmentExpression(void)
{
	zzRULE;
	zzTRACEIN("assignmentExpression");
	conditionalExpression();
	{
		if ( (setwd14[LA(1)]&0x10) ) {
			{
				if ( (LA(1)==ASSIGN) ) {
					zzmatch(ASSIGN); labase++;
					 consume();
				}
				else {
					if ( (LA(1)==PLUS_ASSIGN) ) {
						zzmatch(PLUS_ASSIGN); labase++;
						 consume();
					}
					else {
						if ( (LA(1)==MINUS_ASSIGN)
 ) {
							zzmatch(MINUS_ASSIGN); labase++;
							 consume();
						}
						else {
							if ( (LA(1)==STAR_ASSIGN) ) {
								zzmatch(STAR_ASSIGN); labase++;
								 consume();
							}
							else {
								if ( (LA(1)==DIV_ASSIGN) ) {
									zzmatch(DIV_ASSIGN); labase++;
									 consume();
								}
								else {
									if ( (LA(1)==MOD_ASSIGN) ) {
										zzmatch(MOD_ASSIGN); labase++;
										 consume();
									}
									else {
										if ( (LA(1)==SR_ASSIGN) ) {
											zzmatch(SR_ASSIGN); labase++;
											 consume();
										}
										else {
											if ( (LA(1)==BSR_ASSIGN)
 ) {
												zzmatch(BSR_ASSIGN); labase++;
												 consume();
											}
											else {
												if ( (LA(1)==SL_ASSIGN) ) {
													zzmatch(SL_ASSIGN); labase++;
													 consume();
												}
												else {
													if ( (LA(1)==BAND_ASSIGN) ) {
														zzmatch(BAND_ASSIGN); labase++;
														 consume();
													}
													else {
														if ( (LA(1)==BXOR_ASSIGN) ) {
															zzmatch(BXOR_ASSIGN); labase++;
															 consume();
														}
														else {
															if ( (LA(1)==BOR_ASSIGN) ) {
																zzmatch(BOR_ASSIGN); labase++;
																 consume();
															}
															else {FAIL(1,err29,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk); goto fail;}
														}
													}
												}
											}
										}
									}
								}
							}
						}
					}
				}
			}
			assignmentExpression();
		}
	}
	zzTRACEOUT("assignmentExpression");
	return;
fail:
	if ( guessing ) zzGUESS_FAIL;
	syn(zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk);
	resynch(setwd14, 0x20);
	zzTRACEOUT("assignmentExpression");
}

void
JParser::conditionalExpression(void)
{
	zzRULE;
	zzTRACEIN("conditionalExpression");
	logicalOrExpression();
	{
		if ( (LA(1)==QUESTION)
 ) {
			zzmatch(QUESTION); labase++;
			 consume();
			conditionalExpression();
			zzmatch(COLON); labase++;
			 consume();
			conditionalExpression();
		}
	}
	zzTRACEOUT("conditionalExpression");
	return;
fail:
	if ( guessing ) zzGUESS_FAIL;
	syn(zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk);
	resynch(setwd14, 0x40);
	zzTRACEOUT("conditionalExpression");
}

void
JParser::logicalOrExpression(void)
{
	zzRULE;
	zzTRACEIN("logicalOrExpression");
	logicalAndExpression();
	{
		while ( (LA(1)==LOR) ) {
			zzmatch(LOR); labase++;
			 consume();
			logicalAndExpression();
		}
	}
	zzTRACEOUT("logicalOrExpression");
	return;
fail:
	if ( guessing ) zzGUESS_FAIL;
	syn(zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk);
	resynch(setwd14, 0x80);
	zzTRACEOUT("logicalOrExpression");
}

void
JParser::logicalAndExpression(void)
{
	zzRULE;
	zzTRACEIN("logicalAndExpression");
	inclusiveOrExpression();
	{
		while ( (LA(1)==LAND) ) {
			zzmatch(LAND); labase++;
			 consume();
			inclusiveOrExpression();
		}
	}
	zzTRACEOUT("logicalAndExpression");
	return;
fail:
	if ( guessing ) zzGUESS_FAIL;
	syn(zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk);
	resynch(setwd15, 0x1);
	zzTRACEOUT("logicalAndExpression");
}

void
JParser::inclusiveOrExpression(void)
{
	zzRULE;
	zzTRACEIN("inclusiveOrExpression");
	exclusiveOrExpression();
	{
		while ( (LA(1)==BOR) ) {
			zzmatch(BOR); labase++;
			 consume();
			exclusiveOrExpression();
		}
	}
	zzTRACEOUT("inclusiveOrExpression");
	return;
fail:
	if ( guessing ) zzGUESS_FAIL;
	syn(zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk);
	resynch(setwd15, 0x2);
	zzTRACEOUT("inclusiveOrExpression");
}

void
JParser::exclusiveOrExpression(void)
{
	zzRULE;
	zzTRACEIN("exclusiveOrExpression");
	andExpression();
	{
		while ( (LA(1)==BXOR) ) {
			zzmatch(BXOR); labase++;
			 consume();
			andExpression();
		}
	}
	zzTRACEOUT("exclusiveOrExpression");
	return;
fail:
	if ( guessing ) zzGUESS_FAIL;
	syn(zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk);
	resynch(setwd15, 0x4);
	zzTRACEOUT("exclusiveOrExpression");
}

void
JParser::andExpression(void)
{
	zzRULE;
	zzTRACEIN("andExpression");
	equalityExpression();
	{
		while ( (LA(1)==BAND)
 ) {
			zzmatch(BAND); labase++;
			 consume();
			equalityExpression();
		}
	}
	zzTRACEOUT("andExpression");
	return;
fail:
	if ( guessing ) zzGUESS_FAIL;
	syn(zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk);
	resynch(setwd15, 0x8);
	zzTRACEOUT("andExpression");
}

void
JParser::equalityExpression(void)
{
	zzRULE;
	zzTRACEIN("equalityExpression");
	relationalExpression();
	relationalPredicate();
	zzTRACEOUT("equalityExpression");
	return;
fail:
	if ( guessing ) zzGUESS_FAIL;
	syn(zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk);
	resynch(setwd15, 0x10);
	zzTRACEOUT("equalityExpression");
}

void
JParser::relationalPredicate(void)
{
	zzRULE;
	zzTRACEIN("relationalPredicate");
	if ( (LA(1)==INSTANCEOF) ) {
		zzmatch(INSTANCEOF); labase++;
		 consume();
		typeSpec();
	}
	else {
		if ( (setwd15[LA(1)]&0x20) ) {
			{
				while ( (setwd15[LA(1)]&0x40) ) {
					{
						if ( (LA(1)==NOT_EQUAL) ) {
							zzmatch(NOT_EQUAL); labase++;
							 consume();
						}
						else {
							if ( (LA(1)==EQUAL)
 ) {
								zzmatch(EQUAL); labase++;
								 consume();
							}
							else {FAIL(1,err30,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk); goto fail;}
						}
					}
					relationalExpression();
				}
			}
		}
		else {FAIL(1,err31,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk); goto fail;}
	}
	zzTRACEOUT("relationalPredicate");
	return;
fail:
	if ( guessing ) zzGUESS_FAIL;
	syn(zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk);
	resynch(setwd15, 0x80);
	zzTRACEOUT("relationalPredicate");
}

void
JParser::relationalExpression(void)
{
	zzRULE;
	zzTRACEIN("relationalExpression");
	shiftExpression();
	{
		while ( (setwd16[LA(1)]&0x1) ) {
			{
				if ( (LA(1)==LESSTHAN) ) {
					zzmatch(LESSTHAN); labase++;
					 consume();
				}
				else {
					if ( (LA(1)==GT) ) {
						zzmatch(GT); labase++;
						 consume();
					}
					else {
						if ( (LA(1)==LE) ) {
							zzmatch(LE); labase++;
							 consume();
						}
						else {
							if ( (LA(1)==GE)
 ) {
								zzmatch(GE); labase++;
								 consume();
							}
							else {FAIL(1,err32,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk); goto fail;}
						}
					}
				}
			}
			shiftExpression();
		}
	}
	zzTRACEOUT("relationalExpression");
	return;
fail:
	if ( guessing ) zzGUESS_FAIL;
	syn(zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk);
	resynch(setwd16, 0x2);
	zzTRACEOUT("relationalExpression");
}

void
JParser::shiftExpression(void)
{
	zzRULE;
	zzTRACEIN("shiftExpression");
	additiveExpression();
	{
		while ( (setwd16[LA(1)]&0x4) ) {
			{
				if ( (LA(1)==SL) ) {
					zzmatch(SL); labase++;
					 consume();
				}
				else {
					if ( (LA(1)==SR) ) {
						zzmatch(SR); labase++;
						 consume();
					}
					else {
						if ( (LA(1)==BSR) ) {
							zzmatch(BSR); labase++;
							 consume();
						}
						else {FAIL(1,err33,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk); goto fail;}
					}
				}
			}
			additiveExpression();
		}
	}
	zzTRACEOUT("shiftExpression");
	return;
fail:
	if ( guessing ) zzGUESS_FAIL;
	syn(zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk);
	resynch(setwd16, 0x8);
	zzTRACEOUT("shiftExpression");
}

void
JParser::additiveExpression(void)
{
	zzRULE;
	zzTRACEIN("additiveExpression");
	multiplicativeExpression();
	{
		while ( (setwd16[LA(1)]&0x10)
 ) {
			{
				if ( (LA(1)==PLUS) ) {
					zzmatch(PLUS); labase++;
					 consume();
				}
				else {
					if ( (LA(1)==MINUS) ) {
						zzmatch(MINUS); labase++;
						 consume();
					}
					else {FAIL(1,err34,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk); goto fail;}
				}
			}
			multiplicativeExpression();
		}
	}
	zzTRACEOUT("additiveExpression");
	return;
fail:
	if ( guessing ) zzGUESS_FAIL;
	syn(zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk);
	resynch(setwd16, 0x20);
	zzTRACEOUT("additiveExpression");
}

void
JParser::multiplicativeExpression(void)
{
	zzRULE;
	zzTRACEIN("multiplicativeExpression");
	unaryExpression();
	{
		while ( (setwd16[LA(1)]&0x40) ) {
			{
				if ( (LA(1)==STAR) ) {
					zzmatch(STAR); labase++;
					 consume();
				}
				else {
					if ( (LA(1)==DIV)
 ) {
						zzmatch(DIV); labase++;
						 consume();
					}
					else {
						if ( (LA(1)==MOD) ) {
							zzmatch(MOD); labase++;
							 consume();
						}
						else {FAIL(1,err35,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk); goto fail;}
					}
				}
			}
			unaryExpression();
		}
	}
	zzTRACEOUT("multiplicativeExpression");
	return;
fail:
	if ( guessing ) zzGUESS_FAIL;
	syn(zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk);
	resynch(setwd16, 0x80);
	zzTRACEOUT("multiplicativeExpression");
}

void
JParser::unaryExpression(void)
{
	zzRULE;
	zzTRACEIN("unaryExpression");
	if ( (LA(1)==INC) ) {
		zzmatch(INC); labase++;
		 consume();
		unaryExpression();
	}
	else {
		if ( (LA(1)==DEC) ) {
			zzmatch(DEC); labase++;
			 consume();
			unaryExpression();
		}
		else {
			if ( (LA(1)==MINUS) ) {
				zzmatch(MINUS); labase++;
				 consume();
				unaryExpression();
			}
			else {
				if ( (LA(1)==PLUS)
 ) {
					zzmatch(PLUS); labase++;
					 consume();
					unaryExpression();
				}
				else {
					if ( (setwd17[LA(1)]&0x1) ) {
						unaryExpressionNotPlusMinus();
					}
					else {FAIL(1,err36,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk); goto fail;}
				}
			}
		}
	}
	zzTRACEOUT("unaryExpression");
	return;
fail:
	if ( guessing ) zzGUESS_FAIL;
	syn(zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk);
	resynch(setwd17, 0x2);
	zzTRACEOUT("unaryExpression");
}

void
JParser::unaryExpressionNotPlusMinus(void)
{
	zzRULE;
	zzTRACEIN("unaryExpressionNotPlusMinus");
	zzGUESS_BLOCK
	if ( (LA(1)==BNOT) ) {
		zzmatch(BNOT); labase++;
		 consume();
		unaryExpression();
	}
	else {
		if ( (LA(1)==LNOT) ) {
			zzmatch(LNOT); labase++;
			 consume();
			unaryExpression();
		}
		else {
			zzGUESS
			if ( !zzrv && (LA(1)==LPAREN) && (setwd17[LA(2)]&0x4) ) {
				{
					zzmatch(LPAREN); labase++;
					 consume();
					builtInTypeSpec();
					zzmatch(RPAREN); labase++;
					 consume();
					unaryExpression();
				}
				zzGUESS_DONE
				{
					zzmatch(LPAREN); labase++;
					 consume();
					builtInTypeSpec();
					zzmatch(RPAREN); labase++;
					 consume();
					unaryExpression();
				}
			}
			else {
				if ( !zzrv ) zzGUESS_DONE;
				zzGUESS
				if ( !zzrv && (LA(1)==LPAREN) && 
(LA(2)==IDENT) ) {
					{
						zzmatch(LPAREN); labase++;
						 consume();
						classTypeSpec();
						zzmatch(RPAREN); labase++;
						 consume();
						unaryExpressionNotPlusMinus();
					}
					zzGUESS_DONE
					{
						zzmatch(LPAREN); labase++;
						 consume();
						classTypeSpec();
						zzmatch(RPAREN); labase++;
						 consume();
						unaryExpressionNotPlusMinus();
					}
				}
				else {
					if ( !zzrv ) zzGUESS_DONE;
					if ( (setwd17[LA(1)]&0x8) && (setwd17[LA(2)]&0x10) ) {
						postfixExpression();
					}
					else {FAIL(2,err37,err38,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk); goto fail;}
				}
			}
		}
	}
	zzTRACEOUT("unaryExpressionNotPlusMinus");
	return;
fail:
	if ( guessing ) zzGUESS_FAIL;
	syn(zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk);
	resynch(setwd17, 0x20);
	zzTRACEOUT("unaryExpressionNotPlusMinus");
}

void
JParser::postfixExpression(void)
{
	zzRULE;
	zzTRACEIN("postfixExpression");
	if ( (setwd17[LA(1)]&0x40) ) {
		primaryExpression();
		{
			for (;;) {
				if ( !((setwd17[LA(1)]&0x80))) break;
				if ( (LA(1)==DOT) ) {
					zzmatch(DOT); labase++;
					 consume();
					{
						if ( (LA(1)==IDENT)
 ) {
							zzmatch(IDENT); labase++;
							 consume();
						}
						else {
							if ( (LA(1)==KW_THIS) ) {
								zzmatch(KW_THIS); labase++;
								 consume();
							}
							else {
								if ( (LA(1)==CLASS) ) {
									zzmatch(CLASS); labase++;
									 consume();
								}
								else {
									if ( (LA(1)==NEW) ) {
										newExpression();
									}
									else {
										if ( (LA(1)==SUPER) ) {
											zzmatch(SUPER); labase++;
											 consume();
											zzmatch(LPAREN); labase++;
											 consume();
											{
												if ( (setwd18[LA(1)]&0x1)
 ) {
													expressionList();
												}
											}
											zzmatch(RPAREN); labase++;
											 consume();
										}
										else {FAIL(1,err39,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk); goto fail;}
									}
								}
							}
						}
					}
				}
				else {
					if ( (LA(1)==LBRACK) && (LA(2)==RBRACK) ) {
						{
							int zzcnt=1;
							do {
								zzmatch(LBRACK); labase++;
								 consume();
								zzmatch(RBRACK); labase++;
								 consume();
							} while ( (LA(1)==LBRACK) );
						}
						zzmatch(DOT); labase++;
						 consume();
						zzmatch(CLASS); labase++;
						 consume();
					}
					else {
						if ( (LA(1)==LBRACK) && (setwd18[LA(2)]&0x2) ) {
							zzmatch(LBRACK); labase++;
							 consume();
							expression();
							zzmatch(RBRACK); labase++;
							 consume();
						}
						else {
							if ( (LA(1)==LPAREN) ) {
								zzmatch(LPAREN); labase++;
								 consume();
								argList();
								zzmatch(RPAREN); labase++;
								 consume();
							}
							else break; /* MR6 code for exiting loop "for sure" */
						}
					}
				}
			}
		}
		{
			if ( (LA(1)==INC)
 ) {
				zzmatch(INC); labase++;
				 consume();
			}
			else {
				if ( (LA(1)==DEC) ) {
					zzmatch(DEC); labase++;
					 consume();
				}
				else {
					if ( (setwd18[LA(1)]&0x4) ) {
					}
					else {FAIL(1,err40,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk); goto fail;}
				}
			}
		}
	}
	else {
		if ( (setwd18[LA(1)]&0x8) ) {
			builtInType();
			{
				while ( (LA(1)==LBRACK) ) {
					zzmatch(LBRACK); labase++;
					 consume();
					zzmatch(RBRACK); labase++;
					 consume();
				}
			}
			zzmatch(DOT); labase++;
			 consume();
			zzmatch(CLASS); labase++;
			 consume();
		}
		else {FAIL(1,err41,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk); goto fail;}
	}
	zzTRACEOUT("postfixExpression");
	return;
fail:
	if ( guessing ) zzGUESS_FAIL;
	syn(zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk);
	resynch(setwd18, 0x10);
	zzTRACEOUT("postfixExpression");
}

void
JParser::primaryExpression(void)
{
	zzRULE;
	zzTRACEIN("primaryExpression");
	if ( (LA(1)==IDENT)
 ) {
		zzmatch(IDENT); labase++;
		 consume();
	}
	else {
		if ( (LA(1)==NEW) ) {
			newExpression();
		}
		else {
			if ( (setwd18[LA(1)]&0x20) ) {
				constant();
			}
			else {
				if ( (LA(1)==SUPER) ) {
					zzmatch(SUPER); labase++;
					 consume();
				}
				else {
					if ( (LA(1)==BTRUE) ) {
						zzmatch(BTRUE); labase++;
						 consume();
					}
					else {
						if ( (LA(1)==BFALSE)
 ) {
							zzmatch(BFALSE); labase++;
							 consume();
						}
						else {
							if ( (LA(1)==KW_THIS) ) {
								zzmatch(KW_THIS); labase++;
								 consume();
							}
							else {
								if ( (LA(1)==PNULL) ) {
									zzmatch(PNULL); labase++;
									 consume();
								}
								else {
									if ( (LA(1)==LPAREN) ) {
										zzmatch(LPAREN); labase++;
										 consume();
										assignmentExpression();
										zzmatch(RPAREN); labase++;
										 consume();
									}
									else {FAIL(1,err42,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk); goto fail;}
								}
							}
						}
					}
				}
			}
		}
	}
	zzTRACEOUT("primaryExpression");
	return;
fail:
	if ( guessing ) zzGUESS_FAIL;
	syn(zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk);
	resynch(setwd18, 0x40);
	zzTRACEOUT("primaryExpression");
}

void
JParser::newExpression(void)
{
	zzRULE;
	zzTRACEIN("newExpression");
	zzmatch(NEW); labase++;
	 consume();
	type();
	{
		if ( (LA(1)==LPAREN) ) {
			zzmatch(LPAREN); labase++;
			 consume();
			argList();
			zzmatch(RPAREN); labase++;
			 consume();
			{
				if ( (LA(1)==LCURLY)
 ) {
					classBlock( d1 );
				}
			}
		}
		else {
			if ( (LA(1)==LBRACK) ) {
				newArrayDeclarator();
				{
					if ( (LA(1)==LCURLY) ) {
						arrayInitializer();
					}
				}
			}
			else {FAIL(1,err43,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk); goto fail;}
		}
	}
	zzTRACEOUT("newExpression");
	return;
fail:
	if ( guessing ) zzGUESS_FAIL;
	syn(zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk);
	resynch(setwd18, 0x80);
	zzTRACEOUT("newExpression");
}

void
JParser::argList(void)
{
	zzRULE;
	zzTRACEIN("argList");
	{
		if ( (setwd19[LA(1)]&0x1) ) {
			expressionList();
		}
		else {
			if ( (LA(1)==RPAREN) ) {
			}
			else {FAIL(1,err44,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk); goto fail;}
		}
	}
	zzTRACEOUT("argList");
	return;
fail:
	if ( guessing ) zzGUESS_FAIL;
	syn(zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk);
	resynch(setwd19, 0x2);
	zzTRACEOUT("argList");
}

void
JParser::newArrayDeclarator(void)
{
	zzRULE;
	zzTRACEIN("newArrayDeclarator");
	zzGUESS_BLOCK
	zzGUESS
	if ( !zzrv && (LA(1)==LBRACK) && 
(setwd19[LA(2)]&0x4) ) {
		{
			zzmatch(LBRACK); labase++;
			 consume();
			{
				if ( (setwd19[LA(1)]&0x8) ) {
					expression();
				}
			}
			zzmatch(RBRACK); labase++;
			 consume();
			zzmatch(LBRACK); labase++;
			 consume();
		}
		zzGUESS_DONE
		zzmatch(LBRACK); labase++;
		 consume();
		{
			if ( (setwd19[LA(1)]&0x10) ) {
				expression();
			}
		}
		zzmatch(RBRACK); labase++;
		 consume();
		newArrayDeclarator();
	}
	else {
		if ( !zzrv ) zzGUESS_DONE;
		if ( (LA(1)==LBRACK) && (setwd19[LA(2)]&0x20) ) {
			zzmatch(LBRACK); labase++;
			 consume();
			{
				if ( (setwd19[LA(1)]&0x40) ) {
					expression();
				}
			}
			zzmatch(RBRACK); labase++;
			 consume();
		}
		else {FAIL(2,err45,err46,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk); goto fail;}
	}
	zzTRACEOUT("newArrayDeclarator");
	return;
fail:
	if ( guessing ) zzGUESS_FAIL;
	syn(zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk);
	resynch(setwd19, 0x80);
	zzTRACEOUT("newArrayDeclarator");
}

void
JParser::constant(void)
{
	zzRULE;
	zzTRACEIN("constant");
	if ( (LA(1)==NUM_INT1)
 ) {
		zzmatch(NUM_INT1); labase++;
		 consume();
	}
	else {
		if ( (LA(1)==NUM_INT1A) ) {
			zzmatch(NUM_INT1A); labase++;
			 consume();
		}
		else {
			if ( (LA(1)==NUM_INT1B) ) {
				zzmatch(NUM_INT1B); labase++;
				 consume();
			}
			else {
				if ( (LA(1)==NUM_INT2) ) {
					zzmatch(NUM_INT2); labase++;
					 consume();
				}
				else {
					if ( (LA(1)==NUM_INT3) ) {
						zzmatch(NUM_INT3); labase++;
						 consume();
					}
					else {
						if ( (LA(1)==NUM_INT4)
 ) {
							zzmatch(NUM_INT4); labase++;
							 consume();
						}
						else {
							if ( (LA(1)==CHARCONST) ) {
								zzmatch(CHARCONST); labase++;
								 consume();
							}
							else {
								if ( (LA(1)==STRINGCONST) ) {
									zzmatch(STRINGCONST); labase++;
									 consume();
								}
								else {
									if ( (LA(1)==NUM_FLOAT) ) {
										zzmatch(NUM_FLOAT); labase++;
										 consume();
									}
									else {FAIL(1,err47,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk); goto fail;}
								}
							}
						}
					}
				}
			}
		}
	}
	zzTRACEOUT("constant");
	return;
fail:
	if ( guessing ) zzGUESS_FAIL;
	syn(zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk);
	resynch(setwd20, 0x1);
	zzTRACEOUT("constant");
}
