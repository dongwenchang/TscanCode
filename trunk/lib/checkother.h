/*
 * Cppcheck - A tool for static C/C++ code analysis
 * Copyright (C) 2007-2012 Daniel Marjamäki and Cppcheck team.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 * The above software in this distribution may have been modified by THL A29 Limited (“Tencent Modifications”).
 * All Tencent Modifications are Copyright (C) 2015 THL A29 Limited.
 */

//---------------------------------------------------------------------------
#ifndef CheckOtherH
#define CheckOtherH
//---------------------------------------------------------------------------

#include "config.h"
#include "check.h"
#include "settings.h"


class Token;
class Function;
class Variable;


/** @brief Various small checks */

class CPPCHECKLIB CheckOther : public Check {
public:
    /** @brief This constructor is used when registering the CheckClass */
    CheckOther() : Check(myName())
    { }

    /** @brief This constructor is used when running checks. */
    CheckOther(const Tokenizer *tokenizer, const Settings *settings, ErrorLogger *errorLogger)
        : Check(myName(), tokenizer, settings, errorLogger)
    { }

	/** @brief Run checks against the normal token list */
	void runChecks(const Tokenizer *tokenizer, const Settings *settings, ErrorLogger *errorLogger) {
		CheckOther checkOther(tokenizer, settings, errorLogger);
		// TSC:from TSC 20130712 logic type break into logic;compute;suspicious
		if(getCheckConfig()->compute)
		{
		
			/*void f( int a[]) std::cout << sizeof(a) / sizeof(int) << std::endl */
			if(getCheckConfig()->SizeofForArrayParameter)
				checkOther.checkSizeofForArrayParameter();

			/* sizeof() a const number */
			if(getCheckConfig()->SizeofForNumericParameter)
				checkOther.checkSizeofForNumericParameter();

			/* sizeof(pointer) */
			if(getCheckConfig()->SizeofForPointerSize)
				checkOther.checkSizeofForPointerSize();

			/*check sizeof(sizeof())*/
			if(getCheckConfig()->sizeofsizeof)
				checkOther.sizeofsizeof();
			
			/*int ivar = -2; unsigned int uvar = 2;return ivar / uvar;*/
			if(getCheckConfig()->UnsignedDivision)
				checkOther.checkUnsignedDivision();

			/*a=5;a<<-1*/
			if(getCheckConfig()->NegativeBitwiseShift)
				checkOther.checkNegativeBitwiseShift();

			/* if( var%5 >6) */
			if(getCheckConfig()->ModuloAlwaysTrueFalse)
				checkOther.checkModuloAlwaysTrueFalse();
			
		}
		//,tokenizer->getSourceFilePath()+" checkother_compute ",checklogfile)

		//LOG(;,"\n",checklogfile)
		//TIMELOG(
		if(getCheckConfig()->Suspicious)
		{
			
				/*Suspect ";"*/
			if(getCheckConfig()->SuspiciousSemicolon)
				checkOther.checkSuspiciousSemicolon();

			/* *p = "/usr" + '/'; */
			if(getCheckConfig()->strPlusChar)
				checkOther.strPlusChar();
				/* assert have "=" */
			if(getCheckConfig()->AssignmentInAssert)
				checkOther.checkAssignmentInAssert();
				
		}
		//,"checkother_suspicious ",checklogfile)

		//TIMELOG(
		if(getCheckConfig()->logic)
		{
		
			
			/*Branch condition same if (a) { b = 1; } else if (a) { b = 2; }*/
			if(getCheckConfig()->DuplicateIf)
				checkOther.checkDuplicateIf();

			/*Branch same*/
			if(getCheckConfig()->DuplicateBranch)
				checkOther.checkDuplicateBranch();

			/* if (a && a)，if( a>a) */
			if(getCheckConfig()->DuplicateExpression)
				checkOther.checkDuplicateExpression();

			/* bool within & >>*/
			if(getCheckConfig()->clarifyCondition)
				checkOther.clarifyCondition();

			/* break missing */
			//if(getCheckConfig()->RedundantAssignmentInSwitch&&false)
			//	checkOther.checkRedundantAssignmentInSwitch();
		
			
			/* bool compare with int */
			if(getCheckConfig()->ComparisonOfBoolExpressionWithInt)
				checkOther.checkComparisonOfBoolExpressionWithInt();

	
			//from TSC 20141024 move  checkComparisonOfBoolWithInt to runChecks .eg. int i=xx; bool bi=(bool)i; if(true==bi)
			/* bool Compare with int */
		
			if(getCheckConfig()->ComparisonOfBoolWithInt)
				checkOther.checkComparisonOfBoolWithInt();
            

			/* bool b = compare2(6);bool a = compare1(4); if(b > a){} */
			if(getCheckConfig()->ComparisonOfBoolWithBool)
				checkOther.checkComparisonOfBoolWithBool();
			
		}	
		//,"checkother_logic ",checklogfile)

#ifdef TSC_IGNORE_LOWCHECK
		;

#else
		checkOther.warningOldStylePointerCast();
		checkOther.invalidPointerCast();
		checkOther.checkUnsignedDivision();
		checkOther.checkCharVariable();
		checkOther.strPlusChar();
		checkOther.sizeofsizeof();
		checkOther.sizeofCalculation();
		checkOther.checkRedundantAssignment();
		checkOther.checkRedundantAssignmentInSwitch();
		checkOther.checkAssignmentInAssert();
		checkOther.checkSizeofForArrayParameter();
		checkOther.checkSizeofForPointerSize();
		checkOther.checkSizeofForNumericParameter();
		checkOther.checkSelfAssignment();
		checkOther.checkDuplicateIf();
		checkOther.checkDuplicateBranch();
		checkOther.checkDuplicateExpression();
		checkOther.checkUnreachableCode();
		checkOther.checkSuspiciousSemicolon();
		checkOther.checkVariableScope();
		checkOther.clarifyCondition();   // not simplified because ifAssign
		checkOther.checkComparisonOfBoolExpressionWithInt();
		checkOther.checkSignOfUnsignedVariable();  // don't ignore casts (#3574)
		checkOther.checkIncompleteArrayFill();
		checkOther.checkSuspiciousStringCompare();
#endif
	}

	/** @brief Run checks against the simplified token list */
	void runSimplifiedChecks(const Tokenizer *tokenizer, const Settings *settings, ErrorLogger *errorLogger) {
		CheckOther checkOther(tokenizer, settings, errorLogger);	
		if(getCheckConfig()->compute)
		{	
			/* var/0 */
			if(getCheckConfig()->ZeroDivision)
				checkOther.checkZeroDivision();

			/* bool bValue = true;bValue++ */
			if(getCheckConfig()->IncrementBoolean)
				checkOther.checkIncrementBoolean();
		}

		if(getCheckConfig()->Suspicious)
		{
						/* void foo(bool *p) {p = false;} */
			if(getCheckConfig()->AssignBoolToPointer)
				checkOther.checkAssignBoolToPointer();

			/* memset(p, sizeof(p), 0) */
			if(getCheckConfig()->MemsetZeroBytes)
				checkOther.checkMemsetZeroBytes();
		}
		
		if(getCheckConfig()->logic)
		{
			/* Condition opposite */
			if(getCheckConfig()->oppositeInnerCondition)
				checkOther.oppositeInnerCondition();


			/* bool a&b */
			if(getCheckConfig()->BitwiseOnBoolean)
				checkOther.checkBitwiseOnBoolean();

			/* always ture or false */
			if(getCheckConfig()->IncorrectLogicOperator)
				checkOther.checkIncorrectLogicOperator();

			//from TSC 20141024 move  checkComparisonOfBoolWithInt to runChecks .eg. int i=xx; bool bi=(bool)i; if(true==bi)
			/* bool Compare with int */
			/*
			if(getCheckConfig()->ComparisonOfBoolWithInt)
				checkOther.checkComparisonOfBoolWithInt();
            */
			/*switch check*/
			//if(getCheckConfig()->SwitchCaseFallThrough&&false)
				//checkOther.checkSwitchCaseFallThrough();

		}
		

#ifdef TSC_IGNORE_LOWCHECK
		;

#else
		// Checks
		checkOther.oppositeInnerCondition();
		checkOther.clarifyCalculation();
		checkOther.clarifyStatement();
		checkOther.checkConstantFunctionParameter();
		checkOther.checkIncompleteStatement();

        checkOther.invalidFunctionUsage();
        checkOther.checkZeroDivision();
        checkOther.checkMathFunctions();
        checkOther.checkCCTypeFunctions();

        checkOther.checkIncorrectLogicOperator();
        checkOther.checkMisusedScopedObject();
        checkOther.checkComparisonOfFuncReturningBool();
        checkOther.checkComparisonOfBoolWithBool();
        checkOther.checkMemsetZeroBytes();
        checkOther.checkIncorrectStringCompare();
        checkOther.checkIncrementBoolean();
        checkOther.checkComparisonOfBoolWithInt();
        checkOther.checkSwitchCaseFallThrough();
        checkOther.checkAlwaysTrueOrFalseStringCompare();
        checkOther.checkModuloAlwaysTrueFalse();

        checkOther.checkAssignBoolToPointer();
        checkOther.checkBitwiseOnBoolean();
        checkOther.checkInvalidFree();
        checkOther.checkDoubleFree();
        checkOther.checkRedundantCopy();
        checkOther.checkNegativeBitwiseShift();
#endif
    }

    /** To check the dead code in a program, which is unaccessible due to the counter-conditions check in nested-if statements **/
    void oppositeInnerCondition();

    /** @brief Clarify calculation for ".. a * b ? .." */
    void clarifyCalculation();

    /** @brief Suspicious condition (assignment+comparison) */
    void clarifyCondition();

    /** @brief Suspicious statement like '*A++;' */
    void clarifyStatement();

    /** @brief Are there C-style pointer casts in a c++ file? */
    void warningOldStylePointerCast();

    /** @brief Check for pointer casts to a type with an incompatible binary data representation */
    void invalidPointerCast();

    /**
     * @brief Invalid function usage (invalid radix / overlapping data)
     *
     * %Check that given function parameters are valid according to the standard
     * - wrong radix given for strtol/strtoul
     * - overlapping data when using sprintf/snprintf
     */
    void invalidFunctionUsage();

    /** @brief %Check for unsigned division */
    void checkUnsignedDivision();

    /** @brief %Check scope of variables */
    void checkVariableScope();
    void lookupVar(const Token *tok, const Variable* var);

    /** @brief %Check for constant function parameter */
    void checkConstantFunctionParameter();

    /** @brief Using char variable as array index / as operand in bit operation */
    void checkCharVariable();

    /** @brief Incomplete statement. A statement that only contains a constant or variable */
    void checkIncompleteStatement();

    /** @brief str plus char (unusual pointer arithmetic) */
    void strPlusChar();

    /** @brief %Check zero division*/
    void checkZeroDivision();

    /** @brief %Check for parameters given to math function that do not make sense*/
    void checkMathFunctions();

    /** @brief %Check for parameters given to cctype function that do make error*/
    void checkCCTypeFunctions();

    /** @brief %Check for 'sizeof sizeof ..' */
    void sizeofsizeof();

    /** @brief %Check for calculations inside sizeof */
    void sizeofCalculation();

    /** @brief copying to memory or assigning to a variablen twice */
    void checkRedundantAssignment();

    /** @brief %Check for assigning to the same variable twice in a switch statement*/
    void checkRedundantAssignmentInSwitch();

    /** @brief %Check for switch case fall through without comment */
    void checkSwitchCaseFallThrough();

    /** @brief %Check for assigning a variable to itself*/
    void checkSelfAssignment();

    /** @brief %Check for assignment to a variable in an assert test*/
    void checkAssignmentInAssert();

    /** @brief %Check for testing for mutual exclusion over ||*/
    void checkIncorrectLogicOperator();

    /** @brief %Check for objects that are destroyed immediately */
    void checkMisusedScopedObject();

    /** @brief %Check for comparison of function returning bool*/
    void checkComparisonOfFuncReturningBool();

    /** @brief %Check for comparison of variable of type bool*/
    void checkComparisonOfBoolWithBool();

    /** @brief %Check for filling zero bytes with memset() */
    void checkMemsetZeroBytes();

    /** @brief %Check for using sizeof with array given as function argument */
    void checkSizeofForArrayParameter();

    /** @brief %Check for using sizeof of a variable when allocating it */
    void checkSizeofForPointerSize();

    /** @brief %Check for using sizeof with numeric given as function argument */
    void checkSizeofForNumericParameter();

    /** @brief %Check for using bad usage of strncmp and substr */
    void checkIncorrectStringCompare();

    /** @brief %Check for comparison of a string literal with a char* variable */
    void checkSuspiciousStringCompare();

    /** @brief %Check for using postfix increment on bool */
    void checkIncrementBoolean();

    /** @brief %Check for suspicious comparison of a bool and a non-zero (and non-one) value (e.g. "if (!x==4)") */
    void checkComparisonOfBoolWithInt();

    /** @brief %Check for suspicious code where multiple if have the same expression (e.g "if (a) { } else if (a) { }") */
    void checkDuplicateIf();

    /** @brief %Check for suspicious code where if and else branch are the same (e.g "if (a) b = true; else b = true;") */
    void checkDuplicateBranch();

    /** @brief %Check for suspicious code with the same expression on both sides of operator (e.g "if (a && a)") */
    void checkDuplicateExpression();

    /** @brief %Check for suspicious code that compares string literals for equality */
    void checkAlwaysTrueOrFalseStringCompare();

    /** @brief %Check for suspicious usage of modulo (e.g. "if(var % 4 == 4)") */
    void checkModuloAlwaysTrueFalse();

    /** @brief %Check for code that gets never executed, such as duplicate break statements */
    void checkUnreachableCode();

    /** @brief assigning bool to pointer */
    void checkAssignBoolToPointer();

    /** @brief %Check for testing sign of unsigned variable */
    void checkSignOfUnsignedVariable();

    /** @brief %Check for using bool in bitwise expression */
    void checkBitwiseOnBoolean();

    /** @brief %Check for comparing a bool expression with an integer other than 0 or 1 */
    void checkComparisonOfBoolExpressionWithInt();

    /** @brief %Check for suspicious use of semicolon */
    void checkSuspiciousSemicolon();

    /** @brief %Check for free() operations on invalid memory locations */
    void checkInvalidFree();
    void invalidFreeError(const Token *tok);

    /** @brief %Check for double free or double close operations */
    void checkDoubleFree();
    void doubleFreeError(const Token *tok, const std::string &varname);

    /** @brief %Check for code creating redundant copies */
    void checkRedundantCopy();

    /** @brief %Check for bitwise operation with negative right operand */
    void checkNegativeBitwiseShift();

    /** @brief %Check for buffers that are filled incompletely with memset and similar functions */
    void checkIncompleteArrayFill();

private:
    // Error messages..
   
   void oppositeInnerConditionError(const Token *tok,const Token *tok1);
	// TSC:from TSC 20131106
	void oppositeInnerConditionError2(const Token *tok,const Token *tok1);
    void clarifyCalculationError(const Token *tok, const std::string &op);
    void clarifyConditionError(const Token *tok, bool assign, bool boolop);
    void clarifyStatementError(const Token* tok);
    void sizeofsizeofError(const Token *tok);
    void sizeofCalculationError(const Token *tok, bool inconclusive);
    void cstyleCastError(const Token *tok);
    void invalidPointerCastError(const Token* tok, const std::string& from, const std::string& to, bool inconclusive);
    void dangerousUsageStrtolError(const Token *tok, const std::string& funcname);
    void sprintfOverlappingDataError(const Token *tok, const std::string &varname);
    void udivError(const Token *tok, bool inconclusive);
    void passedByValueError(const Token *tok, const std::string &parname);
    void constStatementError(const Token *tok, const std::string &type);
    void charArrayIndexError(const Token *tok);
    void charBitOpError(const Token *tok);
    void variableScopeError(const Token *tok, const std::string &varname);
    void strPlusCharError(const Token *tok);
    void zerodivError(const Token *tok);
    void mathfunctionCallError(const Token *tok, const unsigned int numParam = 1);
    void cctypefunctionCallError(const Token *tok, const std::string &functionName, const std::string &value);
    void redundantAssignmentError(const Token *tok1, const Token* tok2, const std::string& var);
    void redundantAssignmentInSwitchError(const Token *tok1, const Token *tok2, const std::string &var);
    void redundantCopyError(const Token *tok1, const Token* tok2, const std::string& var);
    void redundantCopyInSwitchError(const Token *tok1, const Token* tok2, const std::string &var);
    void redundantBitwiseOperationInSwitchError(const Token *tok, const std::string &varname);
    void switchCaseFallThrough(const Token *tok);
    void selfAssignmentError(const Token *tok, const std::string &varname);
    void assignmentInAssertError(const Token *tok, const std::string &varname);
    void incorrectLogicOperatorError(const Token *tok, const std::string &condition, bool always);
    void redundantConditionError(const Token *tok, const std::string &text);
    void misusedScopeObjectError(const Token *tok, const std::string &varname);
    void comparisonOfFuncReturningBoolError(const Token *tok, const std::string &expression);
    void comparisonOfTwoFuncsReturningBoolError(const Token *tok, const std::string &expression1, const std::string &expression2);
    void comparisonOfBoolWithBoolError(const Token *tok, const std::string &expression);
    void memsetZeroBytesError(const Token *tok, const std::string &varname);
    void sizeofForArrayParameterError(const Token *tok);
    void sizeofForPointerError(const Token *tok, const std::string &varname);
    void sizeofForNumericParameterError(const Token *tok);
    void incorrectStringCompareError(const Token *tok, const std::string& func, const std::string &string);
    void incorrectStringBooleanError(const Token *tok, const std::string& string);
    void incrementBooleanError(const Token *tok);
    void comparisonOfBoolWithIntError(const Token *tok, const std::string &expression, bool n0o1);
    void comparisonOfBoolWithInvalidComparator(const Token *tok, const std::string &expression);
    void duplicateIfError(const Token *tok1, const Token *tok2);
    void duplicateBranchError(const Token *tok1, const Token *tok2);
    void duplicateExpressionError(const Token *tok1, const Token *tok2, const std::string &op);
    void alwaysTrueFalseStringCompareError(const Token *tok, const std::string& str1, const std::string& str2);
    void alwaysTrueStringVariableCompareError(const Token *tok, const std::string& str1, const std::string& str2);
    void suspiciousStringCompareError(const Token* tok, const std::string& var);
    void duplicateBreakError(const Token *tok, bool inconclusive);
    void unreachableCodeError(const Token* tok, bool inconclusive);
    void assignBoolToPointerError(const Token *tok);
    void unsignedLessThanZeroError(const Token *tok, const std::string &varname, bool inconclusive);
    void pointerLessThanZeroError(const Token *tok, bool inconclusive);
    void unsignedPositiveError(const Token *tok, const std::string &varname, bool inconclusive);
    void pointerPositiveError(const Token *tok, bool inconclusive);
    void bitwiseOnBooleanError(const Token *tok, const std::string &varname, const std::string &op);
    void comparisonOfBoolExpressionWithIntError(const Token *tok, bool n0o1);
    void SuspiciousSemicolonError(const Token *tok);
    void doubleCloseDirError(const Token *tok, const std::string &varname);
    void moduloAlwaysTrueFalseError(const Token* tok, const std::string& maxVal);
    void negativeBitwiseShiftError(const Token *tok);
    void redundantCopyError(const Token *tok, const std::string &varname);
    void incompleteArrayFillError(const Token* tok, const std::string& buffer, const std::string& function, bool boolean);

    void getErrorMessages(ErrorLogger *errorLogger, const Settings *settings) const {
        CheckOther c(0, settings, errorLogger);

        // error
        c.assignBoolToPointerError(0);
        c.sprintfOverlappingDataError(0, "varname");
        c.udivError(0, false);
        c.zerodivError(0);
        c.mathfunctionCallError(0);
        c.misusedScopeObjectError(NULL, "varname");
        c.comparisonOfFuncReturningBoolError(0, "func_name");
        c.comparisonOfTwoFuncsReturningBoolError(0, "func_name1", "func_name2");
        c.comparisonOfBoolWithBoolError(0, "var_name");
        c.sizeofForArrayParameterError(0);
        c.sizeofForPointerError(0, "varname");
        c.sizeofForNumericParameterError(0);
        c.doubleFreeError(0, "varname");
        c.invalidPointerCastError(0, "float", "double", false);
        c.negativeBitwiseShiftError(0);

        //performance
        c.redundantCopyError(0, "varname");
        c.redundantCopyError(0, 0, "var");
        c.redundantAssignmentError(0, 0, "var");

        // style/warning
        c.oppositeInnerConditionError(0,0);
        c.cstyleCastError(0);
        c.dangerousUsageStrtolError(0, "strtol");
        c.passedByValueError(0, "parametername");
        c.constStatementError(0, "type");
        c.charArrayIndexError(0);
        c.charBitOpError(0);
        c.variableScopeError(0, "varname");
        c.strPlusCharError(0);
        c.sizeofsizeofError(0);
        c.sizeofCalculationError(0, false);
        c.redundantAssignmentInSwitchError(0, 0, "var");
        c.redundantCopyInSwitchError(0, 0, "var");
        c.switchCaseFallThrough(0);
        c.selfAssignmentError(0, "varname");
        c.assignmentInAssertError(0, "varname");
        c.incorrectLogicOperatorError(0, "foo > 3 && foo < 4", true);
        c.redundantConditionError(0, "If x > 10 the condition x > 11 is always true.");
        c.memsetZeroBytesError(0, "varname");
        c.clarifyCalculationError(0, "+");
        c.clarifyConditionError(0, true, false);
        c.clarifyStatementError(0);
        c.incorrectStringCompareError(0, "substr", "\"Hello World\"");
        c.suspiciousStringCompareError(0, "foo");
        c.incorrectStringBooleanError(0, "\"Hello World\"");
        c.incrementBooleanError(0);
        c.comparisonOfBoolWithIntError(0, "varname", true);
        c.duplicateIfError(0, 0);
        c.duplicateBranchError(0, 0);
        c.duplicateExpressionError(0, 0, "&&");
        c.alwaysTrueFalseStringCompareError(0, "str1", "str2");
        c.alwaysTrueStringVariableCompareError(0, "varname1", "varname2");
        c.duplicateBreakError(0, false);
        c.unreachableCodeError(0, false);
        c.unsignedLessThanZeroError(0, "varname", false);
        c.unsignedPositiveError(0, "varname", false);
        c.pointerLessThanZeroError(0, false);
        c.pointerPositiveError(0, false);
        c.bitwiseOnBooleanError(0, "varname", "&&");
        c.comparisonOfBoolExpressionWithIntError(0, true);
        c.SuspiciousSemicolonError(0);
        c.cctypefunctionCallError(0, "funname", "value");
        c.moduloAlwaysTrueFalseError(0, "1");
        c.incompleteArrayFillError(0, "buffer", "memset", false);
    }

    static std::string myName() {
        return "Other";
    }

    std::string classInfo() const {
        return "Other checks\n"

               // error
               "* Assigning bool value to pointer (converting bool value to address)\n"
               "* division with zero\n"
               "* scoped object destroyed immediately after construction\n"
               "* assignment in an assert statement\n"
               "* sizeof for array given as function argument\n"
               "* sizeof for numeric given as function argument\n"
               "* using sizeof(pointer) instead of the size of pointed data\n"
               "* incorrect length arguments for 'substr' and 'strncmp'\n"
               "* free() or delete of an invalid memory location\n"
               "* double free() or double closedir()\n"
               "* bitwise operation with negative right operand\n"

               //performance
               "* redundant data copying for const variable\n"
               "* subsequent assignment or copying to a variable or buffer\n"

               // style
               "* Find dead code which is unaccessible due to the counter-conditions check in nested if statements\n"
               "* C-style pointer cast in cpp file\n"
               "* casting between incompatible pointer types\n"
               "* redundant if\n"
               "* bad usage of the function 'strtol'\n"
               "* [[CheckUnsignedDivision|unsigned division]]\n"
               "* passing parameter by value\n"
               "* [[IncompleteStatement|Incomplete statement]]\n"
               "* [[charvar|check how signed char variables are used]]\n"
               "* variable scope can be limited\n"
               "* condition that is always true/false\n"
               "* unusal pointer arithmetic. For example: \"abc\" + 'd'\n"
               "* redundant assignment in a switch statement\n"
               "* redundant pre/post operation in a switch statement\n"
               "* redundant bitwise operation in a switch statement\n"
               "* redundant strcpy in a switch statement\n"
               "* look for 'sizeof sizeof ..'\n"
               "* look for calculations inside sizeof()\n"
               "* assignment of a variable to itself\n"
               "* mutual exclusion over || always evaluating to true\n"
               "* Clarify calculation with parentheses\n"
               "* using increment on boolean\n"
               "* comparison of a boolean with a non-zero integer\n"
               "* comparison of a boolean expression with an integer other than 0 or 1\n"
               "* comparison of a function returning boolean value using relational operator\n"
               "* comparison of a boolean value with boolean value using relational operator\n"
               "* suspicious condition (assignment+comparison)\n"
               "* suspicious condition (runtime comparison of string literals)\n"
               "* suspicious condition (string literals as boolean)\n"
               "* suspicious comparison of a string literal with a char* variable\n"
               "* duplicate break statement\n"
               "* unreachable code\n"
               "* testing if unsigned variable is negative\n"
               "* testing is unsigned variable is positive\n"
               "* using bool in bitwise expression\n"
               "* Suspicious use of ; at the end of 'if/for/while' statement.\n"
               "* incorrect usage of functions from ctype library.\n"
               "* Comparisons of modulo results that are always true/false.\n"
               "* Array filled incompletely using memset/memcpy/memmove.\n";
    }

    void checkExpressionRange(const std::list<const Function*> &constFunctions,
                              const Token *start,
                              const Token *end,
                              const std::string &toCheck);

    void complexDuplicateExpressionCheck(const std::list<const Function*> &constFunctions,
                                         const Token *classStart,
                                         const std::string &toCheck,
                                         const std::string &alt);
};
#endif

