# -*- coding: utf-8 -*-

from utils import *
from simpletoken import *


class ParseProgram(object):

    def __init__(self):
        super(ParseProgram, self).__init__()
        self.__symbol_table = SymbolTable()
        self.__error_number = 0
        self.__error_log = ''

    def __push_error(self, type, token):
        self.__error_number += 1
        self.__error_log += 'Error: {} in line {}\n\t{} => {}\n'.format(
            type, token.line, get_line(self.string, token.line), token.value)
        # goto next line
        t = self.__get_token()
        while t != None and t.line == token.line:
            self.p += 1
            t = self.__get_token()

    def __return_error(self, type):
        self.p -= 1
        return type

    def __get_tokens(self, string):
        pt = ParseToken()
        pt.parse_tokens(string)
        if pt.get_error_number() > 0:
            self.__error_number = pt.get_error_number()
            self.__error_log += pt.get_error_log()
            return None
        return pt.get_tokens()

    def __get_token(self):
        if (self.p >= len(self.tokens)): return None
        return self.tokens[self.p]

    def __get_next_token(self):
        if (self.p >= len(self.tokens)): return None
        token = self.__get_token()
        self.p += 1
        return token

    def __parse_expression(self, father, is_condition=False):
        operand_stack = []
        operator_stack = []

        def __get_operator_number():
            ret = 0
            for op in operator_stack:
                if op != '(':
                    ret += 1
            return ret

        def __get_priority(lstr, rstr):
            operator_priority = {
                '(': 0, ')': 0,
                '=': 1,
                '&&': 2, '||': 2,
                '==': 3, '!=': 3,
                '<': 4, '>': 4, '<=': 4, '>=': 4,
                '+': 5, '-': 5,
                '*': 6, '/': 6, '%': 6,
            }
            return operator_priority[lstr] - operator_priority[rstr]

        while self.p < len(self.tokens):
            token = self.__get_next_token()
            if token and token.value == ';': break
            if TokenValue.is_var(token) or TokenValue.is_const(token):
                if len(operand_stack) != __get_operator_number():
                    return self.__return_error(ParseType.Parse_Operand_Near_By_Operand)
                # TO-DO: if array or func
                name = token.value
                token = self.__get_token()
                if token and token.value == '(':
                    self.p += 1; elist = []
                    token = self.__get_token()
                    if token and token.value == ')':
                        self.p += 1
                    else:
                        while True:
                            ret = self.__parse_expression(self.__symbol_table, is_condition=True)
                            if ret != ParseType.Parse_OK:
                                return ret
                            elist.append(self.__symbol_table.pop_symbol())
                            # print(elist)
                            token = self.__get_next_token()
                            if not (token and token.value == ','):
                                self.p -= 1; break
                    fvar = VarValue(name=name, type=VarType.Function, value=elist)
                    operand_stack.append(ExpressionValue(fvar))

                else:
                    operand_stack.append(ExpressionValue(name))
            elif token and token.value == '(':
                if len(operand_stack) != __get_operator_number():
                    return self.__return_error(ParseType.Parse_Left_Parenthesis_Near_By_Operand)
                operator_stack.append(token.value)
            elif token and token.value == ')':
                if len(operand_stack) != __get_operator_number() + 1:
                    return self.__return_error(ParseType.Parse_Right_Parenthesis_Near_By_Operator)
                while True:
                    if not operator_stack:
                        if is_condition:
                            while operator_stack:
                                if len(operand_stack) < 2:
                                    return self.__return_error(ParseType.Parse_Operand_Number_Is_Not_Right)
                                operand_stack.append(ExpressionValue(
                                    operator_stack.pop(), right=operand_stack.pop(), left=operand_stack.pop()))
                            father.add_symbol(operand_stack[0])
                            return ParseType.Parse_OK
                        return self.__return_error(ParseType.Parse_Not_Find_Back_Bracket)
                    op = operator_stack.pop()
                    if op != '(':
                        operand_stack.append(ExpressionValue(
                            op, right=operand_stack.pop(), left=operand_stack.pop()))
                    else:
                        break
            elif token and token.value == '!':
                pass # TO-DO
            elif TokenValue.is_operator(token):
                if len(operand_stack) != __get_operator_number() + 1:
                    return self.__return_error(ParseType.Parse_Operator_Near_By_Operator)
                while operator_stack:
                    op = operator_stack.pop()
                    if __get_priority(op, token.value) >= 0:
                        if token.value == '=' and operand_stack[-2].is_leaf():
                            return self.__return_error(ParseType.Prase_Expression_Equal_Sign_Left_Value_Error)
                        operand_stack.append(ExpressionValue(
                            op, right=operand_stack.pop(), left=operand_stack.pop()))
                    else:
                        operator_stack.append(op)
                        break
                operator_stack.append(token.value)
            else:
                if is_condition and token and token.value == ',':
                    self.p -= 1
                    break
                return self.__return_error(ParseType.Parse_Expression_Except_Effective_Value)
        while operator_stack:
            if len(operand_stack) < 2:
                return self.__return_error(ParseType.Parse_Operand_Number_Is_Not_Right)
            operand_stack.append(ExpressionValue(
                operator_stack.pop(), right=operand_stack.pop(), left=operand_stack.pop()))
        # if len(operand_stack) == 1:
        father.add_symbol(operand_stack[0])
        # elif len(operand_stack) > 1:
            # print(operand_stack)
        return ParseType.Parse_OK

    def __parse_definition(self, father):
        token = self.__get_next_token()
        assert TokenValue.is_variable_type(token), 'this token is not variable type{}.'.format(token.type)
        if TokenValue.is_void(token):
            return self.__return_error(ParseType.Parse_Definition_Type_Can_Not_Void)
        type = VarValue.get_var_type(token and token.value)
        while True:
            token = self.__get_next_token()
            if not TokenValue.is_var(token):
                return self.__return_error(ParseType.Parse_Definition_Except_Var)
            var = VarValue(name=token.value, type=type)
            token = self.__get_next_token()
            # TO-DO: array definition
            if token and token.value == '[':
                pass
            if token and token.value == '=':
                token = self.__get_next_token()
                if (type == VarType.Char and TokenValue.is_char(token))  \
                or (type == VarType.Int and TokenValue.is_int(token)     \
                or (type == VarType.Float and TokenValue.is_float(token))):
                    var.set_value(token.value)
                    token = self.__get_next_token()
                else:
                    return self.__return_error(ParseType.Parse_Definition_Type_Mismatch)
            if token and token.value == ',':
                father.add_symbol(var)
                continue
            elif token and token.value == ';':
                father.add_symbol(var)
                break
            else:
                return self.__return_error(ParseType.Parse_Definition_Except_Splitter)
        return ParseType.Parse_OK

    def __parse_if_else(self, father):
        token = self.__get_next_token()
        assert token and token.value == 'if', 'this token is not \'if\''
        ifvalue = IfElseValue()
        token = self.__get_next_token()
        if token.value != '(':
            return self.__return_error(ParseType.Parse_Control_Need_Brackets)
        ret = self.__parse_expression(ifvalue.block, is_condition=True)
        if ret != ParseType.Parse_OK:
            return ret
        token = self.__get_token()
        if token.value != '{':
            return self.__return_error(ParseType.Parse_Control_Except_Block)
        ret = self.__parse_block(ifvalue.block)
        if ret != ParseType.Parse_OK:
            return ret
        token = self.__get_token()
        if token and token.value == 'else':
            self.p += 1
            token = self.__get_token()
            if token and token.value == '{':
                ret = self.__parse_block(ifvalue.else_block)
            elif token and token.value == 'if':
                ret = self.__parse_if_else(ifvalue.else_block)
            else:
                return self.__return_error(ParseType.Parse_Control_Except_Block)
            if ret != ParseType.Parse_OK:
                return ret
        father.add_symbol(ifvalue)
        return ParseType.Parse_OK

    def __parse_while(self, father):
        token = self.__get_next_token()
        assert token and token.value == 'while', 'this token is not \'while\''
        whilevalue = WhileValue()
        token = self.__get_next_token()
        if token.value != '(':
            return self.__return_error(ParseType.Parse_Control_Need_Brackets)
        ret = self.__parse_expression(whilevalue.block, is_condition=True)
        if ret != ParseType.Parse_OK:
            return ret
        token = self.__get_token()
        if token.value != '{':
            return self.__return_error(ParseType.Parse_Control_Except_Block)
        ret = self.__parse_block(whilevalue.block)
        if ret != ParseType.Parse_OK:
            return ret
        father.add_symbol(whilevalue)
        return ParseType.Parse_OK

    def __parse_do_while(self, father):
        token = self.__get_next_token()
        assert token and token.value == 'do', 'this token is not \'do-while\''
        dowhilevalue = DoWhileValue()
        token = self.__get_token()
        if token.value != '{':
            return self.__return_error(ParseType.Parse_Control_Except_Block)
        ret = self.__parse_block(dowhilevalue.block)
        if ret != ParseType.Parse_OK:
            return ret
        token = self.__get_next_token()
        if token.value != 'while':
            return self.__return_error(ParseType.Parse_Control_Need_Keyword)
        token = self.__get_next_token()
        if token.value != '(':
            return self.__return_error(ParseType.Parse_Control_Need_Brackets)
        ret = self.__parse_expression(dowhilevalue.block, is_condition=True)
        if ret != ParseType.Parse_OK:
            return ret
        token = self.__get_next_token()
        if token.value != ';':
            return self.__return_error(ParseType.Parse_Control_Need_Semicolon)
        father.add_symbol(dowhilevalue)
        return ParseType.Parse_OK

    def __parse_for(self, father):
        token = self.__get_next_token()
        assert token and token.value == 'for', 'this token is not \'for\''
        forvalue = ForValue()
        token = self.__get_next_token()
        if token.value != '(':
            return self.__return_error(ParseType.Parse_Control_Need_Brackets)
        ret = self.__parse_expression(forvalue.block)
        if ret != ParseType.Parse_OK:
            return ret
        ret = self.__parse_expression(forvalue.block)
        if ret != ParseType.Parse_OK:
            return ret
        ret = self.__parse_expression(forvalue.block, is_condition=True)
        if ret != ParseType.Parse_OK:
            return ret
        token = self.__get_token()
        if token.value != '{':
            return self.__return_error(ParseType.Parse_Control_Except_Block)
        ret = self.__parse_block(forvalue.block)
        if ret != ParseType.Parse_OK:
            return ret
        father.add_symbol(forvalue)
        return ParseType.Parse_OK


    def __parse_return(self, father):
        token = self.__get_next_token()
        assert token and token.value == 'return', 'this token is not \'return\''
        ret = self.__parse_expression(self.__symbol_table)
        if ret != ParseType.Parse_OK:    
            return ret
        father.add_symbol(ReturnValue(self.__symbol_table.pop_symbol()))
        return ParseType.Parse_OK

    def __parse_block(self, father):
        token = self.__get_next_token()
        assert token and token.value == '{', 'this token is not {'
        while True:
            p = self.p
            token = self.__get_token()
            if token and token.value == '}':
                self.p += 1
                break
            # definition
            if token and TokenValue.is_variable_type(token):
                ret = self.__parse_definition(father)
            # expression, function call
            elif token and TokenValue.is_var(token):
                ret = self.__parse_expression(father)
            # control: if-else, while, for
            elif token and token.value == 'if':
                ret = self.__parse_if_else(father)
            elif token and token.value == 'while':
                ret = self.__parse_while(father)
            elif token and token.value == 'do':
                ret = self.__parse_do_while(father)
            elif token and token.value == 'for':
                ret = self.__parse_for(father)
            # return, break, continue
            elif token and token.value == 'return':
                ret = self.__parse_return(father)
            elif token and token.value == 'break':
                self.p += 1
                token = self.__get_token()
                if token and token.value == ';':
                    self.p += 1
                    father.add_symbol(BreakValue())
                    ret = ParseType.Parse_OK
                else:
                    p = self.p
                    ret = ParseType.Parse_Block_Need_Semicolon
            elif token and token.value == 'continue':
                self.p += 1
                token = self.__get_token()
                if token and token.value == ';':
                    self.p += 1
                    father.add_symbol(ContinueValue())
                    ret = ParseType.Parse_OK
                else:
                    p = self.p
                    ret = ParseType.Parse_Block_Need_Semicolon
            elif token and token.value == ';':
                father.add_symbol(ExpressionValue())
                self.p += 1
                ret = ParseType.Parse_OK
            else:
                return self.__return_error(ParseType.Parse_Block_Except_Value)
            if ret != ParseType.Parse_OK:
                self.__push_error(ret, self.tokens[self.p])
        return ParseType.Parse_OK

    def __parse_statment(self, father):
        token = self.__get_next_token()
        assert TokenValue.is_variable_type(token), 'this token is not variable type{}.'.format(token.type)
        type = VarValue.get_var_type(token and token.value)
        token = self.__get_next_token()
        if not TokenValue.is_var(token):
            return self.__return_error(ParseType.Parse_Definition_Except_Var)
        name = token and token.value
        token = self.__get_next_token()
        if token and token.value == '(': # function
            function = FunctionValue(name, type)
            token = self.__get_next_token()
            if not (token and token.value == ')'):
                while True:
                    if TokenValue.is_void(token):
                        return self.__return_error(ParseType.Parse_Definition_Type_Can_Not_Void)
                    if not TokenValue.is_variable_type(token):
                        return self.__return_error(ParseType.Parse_Definition_Type_Mismatch)
                    argt = token.value
                    token = self.__get_next_token()
                    if not TokenValue.is_var(token):
                        return self.__return_error(ParseType.Parse_Definition_Except_Var)
                    name = token.value
                    function.add_param(VarValue(name, argt))
                    token = self.__get_next_token()
                    if token and token.value == ')': break
                    if not (token and token.value == ','):
                        return self.__return_error(ParseType.Parse_Definition_Except_Splitter)
                    token = self.__get_next_token()
            token = self.__get_next_token()
            if token and token.value == ';':
                father.add_symbol(function)
                return ParseType.Parse_OK
            elif token and token.value == '{':
                self.p -= 1
                ret = self.__parse_block(function.block)
                father.add_symbol(function)
                return ret
            else:
                return self.__return_error(ParseType.Parse_Fuction_Miss_Splitter)
        else: # var
            if type == VarType.Void:
                return self.__return_error(ParseType.Parse_Definition_Type_Can_Not_Void)
            while True:
                var = VarValue(name=name, type=type)
                if token and token.value == '=':
                    token = self.__get_next_token()
                    if (type == VarType.Char and TokenValue.is_char(token))  \
                    or (type == VarType.Int and TokenValue.is_int(token)     \
                    or (type == VarType.Float and TokenValue.is_float(token))):
                        var.set_value(token.value)
                        token = self.__get_next_token()
                    else:
                        return self.__return_error(ParseType.Parse_Definition_Type_Mismatch)
                if token and token.value == ',':
                    father.add_symbol(var)
                    token = self.__get_next_token()
                    if not TokenValue.is_var(token):
                        return self.__return_error(ParseType.Parse_Definition_Except_Var)
                    name = token and token.value
                    token = self.__get_next_token()
                    continue
                elif token and token.value == ';':
                    father.add_symbol(var)
                    return ParseType.Parse_OK
                else:
                    return self.__return_error(ParseType.Parse_Definition_Except_Splitter)

    def __parse_program(self):
        token = self.__get_token()
        if TokenValue.is_variable_type(token):
            return self.__parse_statment(self.__symbol_table)
        # expression, function call
        elif token and TokenValue.is_var(token):
            ret = self.__parse_expression(self.__symbol_table)
        # control: if-else, while, for
        elif token and token.value == 'if':
            ret = self.__parse_if_else(self.__symbol_table)
        elif token and token.value == 'while':
            ret = self.__parse_while(self.__symbol_table)
        elif token and token.value == 'do':
            ret = self.__parse_do_while(self.__symbol_table)
        elif token and token.value == 'for':
            ret = self.__parse_for(self.__symbol_table)
        else:
            self.p += 1
            ret = ParseType.Parse_Block_Except_Value
        return ret

    def parse_program(self, string):
        self.string = string
        self.tokens = self.__get_tokens(string)
        if self.tokens == None:
            return
        self.p = 0
        while self.p < len(self.tokens):
            ret = self.__parse_program()
            if ret != ParseType.Parse_OK:
                token = self.__get_token()
                if token == None:
                    self.p -= 1
                    token = self.__get_token()
                self.__push_error(ret, token)

    def get_error_log(self):
        return self.__error_log

    def get_error_number(self):
        return self.__error_number

    def get_symbol_table(self):
        return self.__symbol_table.get_symbol_table()
    

if __name__ == '__main__':
    with open('./test/test.c', 'rt') as f:
        string = f.read()
    
    pp = ParseProgram()
    pp.parse_program(string)
    if pp.get_error_number() > 0:
        print(pp.get_error_log())
    else:
        for i in pp.get_symbol_table():
            print(i)
