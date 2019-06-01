from utils import *
from simpleprogram import ParseProgram


class GenerateIntermediateCode(object):

    def __init__(self):
        super(GenerateIntermediateCode, self).__init__()
        self.__intermediate_code_table = IntermediateCodeTable()
        self.__error_number = 0
        self.__error_log = ''

    def __push_error(self, ptype, symbol):
        self.__error_number += 1
        self.__error_log += 'Error: {} in {}\n\t{}\n'.format(ptype, type(symbol), str(symbol))

    def __insert_code(self, op, arg1='', arg2='', result=''):
        self.__intermediate_code_table.insert(op, arg1, arg2, result)

    def __set_code_item(self, index, op=None, arg1=None, arg2=None, result=None):
        self.__intermediate_code_table.set_intermediate_item(index, op, arg1, arg2, result)

    def __get_symbol_table(self, string):
        pp = ParseProgram()
        pp.parse_program(string)
        if pp.get_error_number() > 0:
            self.__error_number = pp.get_error_number()
            self.__error_log += pp.get_error_log()
            return None
        return pp.get_symbol_table()

    def __get_symbol(self):
        if (self.p >= len(self.symbol_table)): return None
        return self.symbol_table[self.p]

    def __get_next_symbol(self):
        if (self.p >= len(self.symbol_table)): return None
        symbol = self.__get_symbol()
        self.p += 1
        return symbol

    def __traverse_definition(self, symbol):
        assert type(symbol) is VarValue, '\'{}\' is not a definition.'.format(symbol)
        if symbol.value == None:
            return ParseType.Parse_OK
        # if value is not one
        self.__insert_code('=', symbol.value, '', symbol.name)
        return ParseType.Parse_OK

    def __traverse_expression(self, symbol, is_bool=False):

        def __trans(i):
            return 'T{}'.format(i)

        def __get_expression(symbol):
            nonlocal index, slist
            assert type(symbol) is ExpressionValue, '\'{}\' is not a expression.'.format(symbol)
            if symbol.is_leaf():
                slist.append(symbol.value)
                return ParseType.Parse_OK
            ret = __get_expression(symbol.right)
            if ret != ParseType.Parse_OK:
                return ret
            ret = __get_expression(symbol.left)
            if ret != ParseType.Parse_OK:
                return ret
            if symbol.value == '=':
                v = slist.pop()
                self.__insert_code(symbol.value, slist.pop(), '', v)
                slist.append(v)
            else:
                v = __trans(index)
                self.__insert_code(symbol.value, slist.pop(), slist.pop(), v)
                slist.append(v)
            index += 1
            return ParseType.Parse_OK

        index = 0; slist = []
        ret = __get_expression(symbol)
        if is_bool:
            return ret, slist[0]
        return ret


    def __traverse_if_else(self, symbol):
        assert type(symbol) is IfElseValue, '\'{}\' is not a ifelse.'.format(symbol)
        condition = symbol.block.get_symbol(0)
        ret, value = self.__traverse_expression(condition, is_bool=True)
        if ret != ParseType.Parse_OK:
            return ret
        self.__insert_code('jnz', value, '', self.get_intermediate_num() + 3)
        self.__insert_code('j')
        item_index = self.get_intermediate_num() - 1
        for sym in symbol.block.get_symbol_table()[1:]:
            self.__traverse(sym)
        self.__set_code_item(item_index, result=self.get_intermediate_num()+1)
        # else
        if symbol.else_block.is_empty():
            return ParseType.Parse_OK
        else:
            self.__insert_code('j')
            item_index = self.get_intermediate_num() - 1
            for sym in symbol.else_block.get_symbol_table():
                self.__traverse(sym)
            self.__set_code_item(item_index, result=self.get_intermediate_num()+1)
        return ParseType.Parse_OK

    def __traverse_while(self, symbol):
        assert type(symbol) is WhileValue, '\'{}\' is not a while.'.format(symbol)
        condition = symbol.block.get_symbol(0)
        begin_index = self.get_intermediate_num() + 1
        ret, value = self.__traverse_expression(condition, is_bool=True)
        if ret != ParseType.Parse_OK:
            return ret
        self.__insert_code('jnz', value, '', self.get_intermediate_num() + 3)
        self.__insert_code('j')
        item_index = self.get_intermediate_num() - 1
        for sym in symbol.block.get_symbol_table()[1:]:
            self.__traverse(sym)
        self.__insert_code('j', '', '', begin_index)
        self.__set_code_item(item_index, result=self.get_intermediate_num()+1)
        return ParseType.Parse_OK

    def __traverse_dowhile(self, symbol):
        assert type(symbol) is DoWhileValue, '\'{}\' is not a dowhile.'.format(symbol)
        begin_index = self.get_intermediate_num() + 1
        for sym in symbol.block.get_symbol_table()[:-1]:
            self.__traverse(sym)
        condition = symbol.block.get_symbol(-1)
        ret, value = self.__traverse_expression(condition, is_bool=True)
        if ret != ParseType.Parse_OK:
            return ret
        self.__insert_code('jnz', value, '', begin_index)
        return ParseType.Parse_OK

    def __traverse_for(self, symbol):
        assert type(symbol) is ForValue, '\'{}\' is not a for.'.format(symbol)
        # init
        ret = self.__traverse_expression(symbol.block.get_symbol(0))
        if ret != ParseType.Parse_OK:
            return ret
        begin_index = self.get_intermediate_num() + 1
        # condition
        ret, value = self.__traverse_expression(symbol.block.get_symbol(1), is_bool=True)
        if ret != ParseType.Parse_OK:
            return ret
        self.__insert_code('jnz', value)
        item2others_index = self.get_intermediate_num() - 1
        self.__insert_code('j')
        item2end_index = self.get_intermediate_num() - 1
        # per_loop
        per_loop_index = self.get_intermediate_num() + 1
        ret = self.__traverse_expression(symbol.block.get_symbol(2))
        if ret != ParseType.Parse_OK:
            return ret
        self.__insert_code('j', '', '', begin_index)
        self.__set_code_item(item2others_index, result=self.get_intermediate_num()+1)
        for sym in symbol.block.get_symbol_table()[3:]:
            self.__traverse(sym)
        self.__insert_code('j', '', '', per_loop_index)
        self.__set_code_item(item2end_index, result=self.get_intermediate_num()+1)
        return ParseType.Parse_OK

    def __traverse(self, symbol):
        if type(symbol) is VarValue:
            ret = self.__traverse_definition(symbol)
        elif type(symbol) is ExpressionValue:
            ret = self.__traverse_expression(symbol)
        elif type(symbol) is IfElseValue:
            ret = self.__traverse_if_else(symbol)
        elif type(symbol) is WhileValue:
            ret = self.__traverse_while(symbol)
        elif type(symbol) is DoWhileValue:
            ret = self.__traverse_dowhile(symbol)
        elif type(symbol) is ForValue:
            ret = self.__traverse_for(symbol)
        else:
            ret = ParseType.Parse_Except_Value
        return ret

    def traverse(self, string):
        self.string = string
        self.symbol_table = self.__get_symbol_table(string)
        if self.symbol_table == None:
            return
        self.p = 0
        while self.p < len(self.symbol_table):
            symbol = self.__get_next_symbol()
            ret = self.__traverse(symbol)
            if ret != ParseType.Parse_OK:
                self.__push_error(ret, symbol)

    def get_error_log(self):
        return self.__error_log

    def get_error_number(self):
        return self.__error_number

    def get_intermediate_code(self):
        return self.__intermediate_code_table.get_intermediate_code()

    def get_intermediate_num(self):
        return self.__intermediate_code_table.get_intermediate_num()


if __name__ == '__main__':
    with open('test.c', 'rt') as f:
        string = f.read()
    ic = GenerateIntermediateCode()
    ic.traverse(string)
    if ic.get_error_number() > 0:
        print(ic.get_error_log())
    else:
        for index, code in enumerate(ic.get_intermediate_code()):
            print(index + 1, code)
