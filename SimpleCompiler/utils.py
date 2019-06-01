# -*- coding: utf-8 -*-

from enum import Enum
import random, re

'''
    token list
'''

keyword = (
    'char', 'int', 'float', 'break',
    'const', 'return', 'void', 'continue',
    'do', 'while', 'if', 'else', 'for'
)

delimiter = ('{', '}', ';', ',')

operator = (
    '(', ')', '[', ']', '!', '*', '/', '%', '+', '-', 
    '<', '<=', '>', '>=', '==', '!=', '&&', '||', '='
)


'''
    type enums
'''

TokenType = Enum('TokenType', (
    'Keyword', 'Delimiter', 'Operator', 'Var', 'Char', 'Int', 'Float', 'String'
))

VarType = Enum('VarType', ('Void', 'Char', 'Int', 'Float', 'Function'))

ParseType = Enum('ParseType', (
    'Parse_OK',
    # token
    'Parse_Except_Value',
    'Parse_Char_Need_End_Sign',
    'Parse_Char_Escape_Error',
    'Parse_String_Need_End_Sign',
    'Parse_String_Escape_Error',
    'Parse_Int_Number_Error',
    'Parse_Float_Number_Error',
    'Parse_SNotation_Number_Error',
    # program
    'Parse_Token_Error',
    'Parse_Not_Find_Back_Bracket',
    'Parse_Definition_Except_Var',
    'Parse_Definition_Except_Splitter',
    'Parse_Definition_Type_Mismatch',
    'Parse_Definition_Type_Can_Not_Void',
    'Parse_Block_Need_Semicolon',
    'Parse_Block_Except_Value',
    'Parse_Fuction_Miss_Splitter',
    'Parse_Function_Need_Left_Parenthesis',
    'Parse_Function_Need_Right_Parenthesis',
    'Parse_Control_Need_Keyword',
    'Parse_Control_Need_Brackets',
    'Parse_Control_Need_Semicolon',
    'Parse_Control_Except_Block',
    'Parse_Operand_Number_Is_Not_Right',
    'Parse_Operand_Near_By_Operand',
    'Parse_Operator_Near_By_Operator',
    'Parse_Left_Parenthesis_Near_By_Operand',
    'Parse_Right_Parenthesis_Near_By_Operator',
    'Parse_Expression_Except_Effective_Value',
    'Prase_Expression_Equal_Sign_Left_Value_Error',
    # analysis
    'Parse_Program_Error',
    'Parse_Not_Find_Main_Function',
    # assembler
    'Generate_Need_Effective_Type',
    'Generate_Var_Is_Not_Defined',
    'Generate_Expression_Right_Not_Assignable',
    'Generate_Expression_Need_Effective_Type',
))


'''
    base function
'''

def isWhitespace(ch):
    if ch == None: return False
    if isinstance(ch, int):
        ch = chr(ch)
    return ch.isspace()


def isAlpha(ch):
    if ch == None: return False
    if isinstance(ch, int):
        ch = chr(ch)
    return ch.isalpha() or ch == '_'


def isNumberic(ch):
    if ch == None: return False
    if isinstance(ch, int):
        ch = chr(ch)
    return ch.isdigit()


def isDelimiter(ch):
    return ch in delimiter


def mayOperator(ch):
    if ch == None: return False
    for i in operator:
        if ch == i[0]:
            return True
    return False


def isVar(str):
    if not isAlpha(str[0]):
        return False
    for ch in str[1:]:
        if not (isAlpha(ch) or isNumberic(ch)):
            return False
    return True

def isNumber(str):
    return True if re.compile(r'^[-+]?([0-9]+|[0-9]*\.[0-9]+)([eE][-+]?[0-9]+)?$').match(str) else False

def isString(str): # bugs, but enough in there!
    if str[0] != '\"' or str[-1] != '\"':
        return False
    return True

def get_line(string, line):
    lines = string.splitlines()
    if line < 1 or line > len(lines):
        return None
    return lines[line - 1]


'''
    token value
'''

class TokenValue(object):

    def __init__(self, type=None, value=None, line=None):
        self.type = type
        self.value = value
        self.line = line

    def __str__(self):
        return '({}, {})'.format(self.type, self.value)

    @staticmethod
    def is_char(token):
        return token.type == TokenType.Char

    @staticmethod
    def is_int(token):
        return token.type == TokenType.Int

    @staticmethod
    def is_float(token):
        return token.type == TokenType.Float    

    @staticmethod
    def is_const(token):
        return TokenValue.is_string(token) or token.type in [TokenType.Char, TokenType.Int, TokenType.Float]

    @staticmethod
    def is_string(token):
        return token.type is TokenType.String

    @staticmethod
    def is_var(token):
        return token.type is TokenType.Var

    @staticmethod
    def is_assignment(token):
        return is_operator(token) and token.value == '='

    @staticmethod
    def is_operator(token):
        return token.type == TokenType.Operator

    @staticmethod
    def is_semicolon(token):
        return token.type == TokenType.Delimiter and token.value == ';'

    @staticmethod
    def is_void(token):
        return token.type == TokenType.Keyword and token.value == 'void'

    @staticmethod
    def is_variable_type(token):
        return token.type == TokenType.Keyword and (token.value in ['char', 'int', 'float', 'void'])

'''
    interface
'''

class ISymbolValue(object):

    def __init__(self, name):
        self.name = name

    def set_name(self, name):
        self.name = name

    def get_name(self):
        return self.name


class IControlValue(object):
    
    def __init__(self):
        super(IControlValue, self).__init__()
        self.block = BlockValue()


class ITableValue(object):

    def __init__(self, symbol_table):
        self.__symbol_table = symbol_table

    def add_symbol(self, value):
        self.__symbol_table.append(value)
    
    def pop_symbol(self):
        return self.__symbol_table.pop()

    def get_symbol(self, index):
        return self.__symbol_table[index]

    def get_symbol_table(self):
        return self.__symbol_table

    def is_empty(self):
        return len(self.get_symbol_table()) == 0


'''
    Value
'''

class VarValue(ISymbolValue):

    def __init__(self, name, type, value=None):
        super(VarValue, self).__init__(name)
        self.type = type
        self.value = value

    def __str__(self):
        if self.type == VarType.Function:
            return '({}, {}, {})'.format(self.type, self.name, [str(i) for i in self.value])
        return '({}, {}, {})'.format(self.type, self.name, self.value)

    def get_type(self):
        return self.type

    def get_value(self):
        return self.value

    def set_value(self, value):
        assert value != None
        self.value = value

    @staticmethod
    def get_var_type(v):
        if v == 'char':
            return VarType.Char
        elif v == 'int':
            return VarType.Int
        elif v == 'float':
            return VarType.Float
        elif v == 'void':
            return VarType.Void
        else:
            return None


class ExpressionValue(object):

    def __init__(self, value=None, left=None, right=None):
        super(ExpressionValue, self).__init__()
        self.value = value
        self.left = left
        self.right = right

    def __str__(self):
        if self.value == None:
            return 'None'
        elif self.left != None and self.left != None:
            return '({}, {}, {})'.format(self.value, self.left, self.right)
        else:
            return '{}'.format(self.value)

    def is_leaf(self):
        return self.left == None and self.right == None


class BlockValue(ITableValue):
    
    def __init__(self):
        super(BlockValue, self).__init__(list())

    def __str__(self):
        return '(' + ', '.join(map(lambda x: str(x), self.get_symbol_table())) + ')'


class IfElseValue(IControlValue):
    
    def __init__(self):
        super(IfElseValue, self).__init__()
        self.else_block = BlockValue() # it's a block value
        # condition, others

    def __str__(self):
        return '(if: {}, else: {})'.format(self.block, self.else_block)


class WhileValue(IControlValue):

    def __init__(self):
        super(WhileValue, self).__init__()
        # condition, others

    def __str__(self):
        return '(while: {})'.format(self.block)


class DoWhileValue(IControlValue):

    def __init__(self):
        super(DoWhileValue, self).__init__()
        # others, condition

    def __str__(self):
        return '(dowhile: {})'.format(self.block)


class ForValue(IControlValue):
    
    def __init__(self):
        super(ForValue, self).__init__()
        # init, condition, per_loop, others

    def __str__(self):
        return '(for: {})'.format(self.block)


class BreakValue(object):
    def __init__(self):
        super(BreakValue, self).__init__()

    def __str__(self):
        return 'break'


class ContinueValue(object):
    def __init__(self):
        super(ContinueValue, self).__init__()

    def __str__(self):
        return 'continue'
        

class ReturnValue(object):

    def __init__(self, expression=None):
        super(ReturnValue, self).__init__()
        self.expression = expression

    def __str__(self):
        return '(return: {})'.format(self.expression)
        

class FunctionValue(ISymbolValue):
    
    def __init__(self, name, ret_type):
        super(FunctionValue, self).__init__(name)
        self.ret_type = ret_type
        self.params_num = 0
        self.block = BlockValue()

    def __str__(self):
        symbol_table = self.block.get_symbol_table()
        ret = '({}, {}, {}, '.format(self.ret_type, self.name, self.params_num)
        ret += ', '.join(map(lambda x: str(x), symbol_table[:self.params_num]))
        ret += '\n\t'
        ret += '\n\t'.join(map(lambda x: str(x), symbol_table[self.params_num:]))
        ret += '\n)'
        return ret

    def get_params_num(self):
        return self.params_num

    def add_param(self, var_value):
        if var_value == None: return
        self.block.add_symbol(var_value)
        self.params_num += 1

    def get_block_table(self):
        return self.block.get_symbol_table()


class SymbolTable(ITableValue):

    def __init__(self):
        super(SymbolTable, self).__init__([])

    def find_function(self, name):
        symbol_table = self.get_symbol_table()
        for symbol in symbol_table:
            if type(symbol) is FunctionValue and symbol.name == name:
                return symbol
        return None


class IntermediateCodeItem(object):

    def __init__(self, op, arg1='', arg2='', result=''):
        super(IntermediateCodeItem, self).__init__()
        self.op = op
        self.arg1 = arg1
        self.arg2 = arg2
        self.result = result
    
    def __str__(self):
        return '({}, {}, {}, {})'.format(self.op, self.arg1, self.arg2, self.result)


class IntermediateCodeTable(object):

    def __init__(self):
        super(IntermediateCodeTable, self).__init__()
        self.__intermediate_code = []
        self.__intermediate_num = 0

    def insert(self, op, arg1='', arg2='', result=''):
        self.__intermediate_num += 1
        self.__intermediate_code.append(IntermediateCodeItem(op, arg1, arg2, result))

    def get_intermediate_code(self):
        return self.__intermediate_code

    def get_intermediate_num(self):
        return self.__intermediate_num

    def get_intermediate_item(self, index):
        return self.__intermediate_code[index]

    def set_intermediate_item(self, index, op=None, arg1=None, arg2=None, result=None):
        if op: self.__intermediate_code[index].op = op
        if arg1: self.__intermediate_code[index].arg1 = arg1
        if arg2: self.__intermediate_code[index].arg2 = arg2
        if result: self.__intermediate_code[index].result = result


class VarItem(object):
    
    def __init__(self, name, index):
        super(VarItem, self).__init__()
        self.name = name
        self.index = index

    def __str__(self):
        return '(VarItem, {}, {})'.format(self.name, self.index)


class VarMap(object):

    def __init__(self):
        super(VarMap, self).__init__()
        self.__var_map = []

    def __str__(self):
        ret = ''
        for var in self.__var_map:
            ret += str(var) + '\n'
        return ret

    def put(self, name, index):
        self.__var_map.append(VarItem(name, index))

    def pop(self, number):
        return [self.__var_map.pop().index for i in range(number)]

    def find(self, name):
        for i in self.__var_map[::-1]:
            if i.name == name:
                return i.index
        return None
    
    def contains(self, name):
        return name in [i.name for i in self.__var_map]

    def get_var_map(self):
        return self.__var_map
