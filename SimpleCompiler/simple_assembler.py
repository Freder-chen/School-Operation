# -*- coding: utf-8 -*-

from utils import *
from simpleprogram import ParseProgram


class GenerateAssembler(object):
    '''Interpreter'''

    def __init__(self):
        super(GenerateAssembler, self).__init__()

        self.__var_map = VarMap()
        self.__jump_index = 0
        self.__str_index = 0
        self.__stock_index = -48

        self.__data_assembler = '\t.section\t__DATA,__data\n'
        self.__data_number = 0

        self.__text_assembler = '\t.section\t__TEXT,__text,regular,pure_instructions\n'
        self.__text_number = 0

        self.__str_assembler = '\t.section  __TEXT,__cstring,cstring_literals\n'
        self.__str_number = 0

        self.__error_number = 0
        self.__error_log = ''

    def __add_data(self, command):
        if self.__data_number > 0:
            self.__data_assembler += '\n'
        self.__data_number += 1
        self.__data_assembler += command

    def __add_text(self, command):
        if self.__text_number > 0:
            self.__text_assembler += '\n'
        self.__text_number += 1
        self.__text_assembler += command

    def __add_str(self, command):
        if self.__str_number > 0:
            self.__str_assembler += '\n'
        self.__str_number += 1
        self.__str_assembler += command

    def __push_error(self, ptype, symbol):
        self.__error_number += 1
        self.__error_log += 'Error: {} in {}\n\t{}\n'.format(ptype, type(symbol), str(symbol))
    
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

    def __get_jump_lable(self):
        ret = 'lable_{}'.format(self.__jump_index)
        self.__jump_index += 1
        return ret

    def __get_str_lable(self):
        ret = 'L_.str_{}'.format(self.__str_index)
        self.__str_index += 1
        return ret

    def __tranverse_function(self, symbol):

        def __add_command(comm, form='\t'):
            nonlocal command
            command += '{}{}\n'.format(form, comm)

        def __sizeof(t):
            if t is VarType.Char:    ret = 1
            elif t is VarType.Int:   ret = 4
            elif t is VarType.Float: ret = 4
            else:                    ret = None
            return ret

        def __mov(t):
            if t is VarType.Char:    ret = 'movb'
            elif t is VarType.Int:   ret = 'movl'
            elif t is VarType.Float: ret = 'movss'
            else:                    ret = None
            return ret

        def __register(index):
            register = ['eax', 'ebx', 'ecx', 'edx', 'esi', 'edi']
            return register[index]

        def __tranverse_var(symbol):
            if symbol.value:
                dest_index = self.__var_map.find(symbol.name)
                __add_command('{}\t${}, {}(%rbp)'.format(__mov(symbol.type), symbol.value, dest_index))
            return ParseType.Parse_OK

        def __tranverse_expression(symbol):

            def __get_expression(symbol):
                nonlocal register_index
                if symbol.is_leaf():
                    v = symbol.value
                    # print(v)
                    if type(v) is VarValue:
                        # print('1', v)
                        # save register
                        sindex = self.__stock_index + 4
                        params_num = len(v.value)
                        for i in range(register_index):
                            __add_command('movl\t{}, {}(%rbp)'.format(__register(i), sindex - 4 * i))
                        if params_num > 0:
                            for i in range(params_num - 1, -1, -1):
                                if type(v.value[i].value) is str and isString(v.value[i].value):
                                    slable = self.__get_str_lable()
                                    self.__add_str('{}:\n\t.asciz\t{}\n'.format(slable, v.value[i].value))
                                    __add_command('leaq\t{}(%rip), %rdi'.format(slable))
                                    # __add_command('movl %eax, -48(%rbp)')
                                    __add_command('movb\t$0, %al')
                                else:
                                    ret = __tranverse_expression(v.value[i])
                                    if ret != ParseType.Parse_OK:
                                        return ret
                                    if __register(0) != __register(-i-1):
                                        __add_command('movl\t%{}, %{}'.format(__register(0), __register(-i-1)))
                        __add_command('callq\t_{}'.format(v.name))
                        if register_index > 0:
                            __add_command('movl %{}, %{}'.format(__register(0), __register(register_index)))
                        for i in range(register_index):
                            __add_command('movl\t{}(%rbp), %{}'.format(sindex - 4 * i, __register(i)))
                        register_index += 1
                        return ParseType.Parse_OK
                    elif isVar(v):
                        src = self.__var_map.find(symbol.value)
                        if src == None:
                            return ParseType.Generate_Var_Is_Not_Defined
                        src = '{}(%rbp)'.format(src)
                    elif isNumber(v):
                        src = '${}'.format(symbol.value)
                    else:
                        print(symbol)
                        return ParseType.Generate_Expression_Need_Effective_Type
                    __add_command('movl\t{}, %{}'.format(src, __register(register_index)))
                    register_index += 1
                    return ParseType.Parse_OK
                if symbol.value == '=':
                    if not symbol.left.is_leaf():
                        return ParseType.Generate_Expression_Right_Not_Assignable
                    ret = __get_expression(symbol.right)
                    if ret != ParseType.Parse_OK:
                        return ret
                    dest_index = self.__var_map.find(symbol.left.value)
                    if dest_index == None:
                        return ParseType.Generate_Var_Is_Not_Defined
                    __add_command('movl\t%{}, {}(%rbp)'.format(__register(register_index - 1), dest_index))
                    return ParseType.Parse_OK 
                ret = __get_expression(symbol.left)
                if ret != ParseType.Parse_OK:
                    return ret
                ret = __get_expression(symbol.right)
                if ret != ParseType.Parse_OK:
                    return ret
                if symbol.value in ['+', '-', '*']:
                    dic = {'+': 'addl', '-': 'subl', '*': 'imull'}
                    __add_command('{}\t%{}, %{}'.format(dic[symbol.value], __register(register_index - 1), __register(register_index - 2)))
                elif symbol.value == '/':
                    if register_index == 2:
                        __add_command('cltd')
                        __add_command('idivl\t%{}'.format(__register(1)))
                    if register_index > 2:
                        sindex = self.__stock_index + 4
                        __add_command('movl\t%{}, {}(%rbp)'.format(__register(0), sindex))
                        __add_command('movl\t%{}, {}(%rbp)'.format(__register(1), sindex - 4))
                        __add_command('movl\t%{}, %{}'.format(__register(register_index - 2), __register(0)))
                        __add_command('cltd')
                        __add_command('movl\t%{}, %{}'.format(__register(register_index - 1), __register(1)))
                        __add_command('idivl\t%{}'.format(__register(1)))
                        __add_command('movl\t%{}, %{}'.format(__register(0), __register(register_index - 2)))
                        __add_command('movl\t{}(%rbp), %{}'.format(sindex, __register(0)))
                        if not register_index == 3:
                            __add_command('movl {}(%rbp), %{}'.format(sindex - 4, __register(1)))
                elif symbol.value in ['>', '>=', '<', '<=', '==', '!=']:
                    dic = {'>': 'setg', '>=': 'setge', '<': 'setl', '<=': 'setle', '==': 'sete', '!=': 'setne'}
                    __add_command('cmpl\t%{}, %{}'.format(__register(register_index - 1), __register(register_index - 2)))
                    __add_command('{}\t%dl'.format(dic[symbol.value]))
                    __add_command('andb\t$1, %dl')
                    __add_command('movzbl\t%dl, %{}'.format(__register(register_index - 2)))
                elif symbol.value in ['&&', '||']:
                    dic = {'&&': 'je', '||': 'jne'}
                    jlabel = self.__get_jump_lable()
                    __add_command('cmpl\t$0, %{}'.format(__register(register_index - 2)))
                    __add_command('{}\t{}'.format(dic[symbol.value], jlabel))
                    __add_command('cmpl\t$0, %{}'.format(__register(register_index - 1)))
                    __add_command('setne\t%dl')
                    __add_command('{}:'.format(jlabel), form='')
                    __add_command('andb\t$1, %dl')
                    __add_command('movzbl\t%dl, %{}'.format(__register(register_index - 2)))
                register_index -= 1
                return ParseType.Parse_OK
            
            register_index = 0
            ret = __get_expression(symbol)
            return ret

        def __tranverse_return(symbol):
            __add_command('addq\t $48, %rsp')
            ret = __tranverse_expression(symbol.expression)
            if ret != ParseType.Parse_OK:
                return ret
            __add_command('popq\t%rbp')
            __add_command('retq')
            return ParseType.Parse_OK

        def __tranverse_if_else(symbol):
            symbol_table = symbol.block.get_symbol_table()
            ret = __tranverse_expression(symbol_table[0])
            if ret != ParseType.Parse_OK:
                return ret
            __add_command('cmpl $0, %{}'.format(__register(0)))
            jlabel = self.__get_jump_lable()
            __add_command('je {}'.format(jlabel))
            __traverse_block(symbol_table[1:])
            else_table = symbol.else_block.get_symbol_table()
            if not else_table == []:
                out_label = self.__get_jump_lable()
                __add_command('jmp {}'.format(out_label))
            __add_command('{}: '.format(jlabel), form='')
            if not else_table == []:
                __traverse_block(else_table)
                __add_command('{}: '.format(out_label), form='')
            return ParseType.Parse_OK

        def __tranverse_while(symbol):
            symbol_table = symbol.block.get_symbol_table()
            jlabel = self.__get_jump_lable()
            __add_command('{}: '.format(jlabel), form='')
            ret = __tranverse_expression(symbol_table[0])
            if ret != ParseType.Parse_OK:
                return ret
            __add_command('cmpl $0, %{}'.format(__register(0)))
            out_label = self.__get_jump_lable()
            __add_command('jle {}'.format(out_label))
            __traverse_block(symbol_table[1:])
            __add_command('jmp {}'.format(jlabel))
            __add_command('{}: '.format(out_label), form='')
            return ParseType.Parse_OK

        def __tranverse_for(symbol):
            symbol_table = symbol.block.get_symbol_table()
            ret = __tranverse_expression(symbol_table[0])
            if ret != ParseType.Parse_OK:
                return ret
            jlabel = self.__get_jump_lable()
            __add_command('{}: '.format(jlabel), form='')
            ret = __tranverse_expression(symbol_table[1])
            if ret != ParseType.Parse_OK:
                return ret
            __add_command('cmpl $0, %{}'.format(__register(0)))
            out_label = self.__get_jump_lable()
            __add_command('jle {}'.format(out_label))
            __traverse_block(symbol_table[3:])
            ret = __tranverse_expression(symbol_table[2])
            if ret != ParseType.Parse_OK:
                return ret
            __add_command('jmp {}'.format(jlabel))
            __add_command('{}: '.format(out_label), form='')
            return ParseType.Parse_OK

        def __traverse_block(symbol_table):
            var_num = 0
            for symbol in symbol_table:
                if type(symbol) is VarValue:
                    self.__var_map.put(symbol.name, self.__stock_index + 4)
                    self.__stock_index += 4
                    var_num += 1
                    ret = __tranverse_var(symbol)
                elif type(symbol) is ReturnValue:
                    ret = __tranverse_return(symbol)
                elif type(symbol) is ExpressionValue:
                    ret = __tranverse_expression(symbol)
                elif type(symbol) is IfElseValue:
                    ret = __tranverse_if_else(symbol)
                elif type(symbol) is WhileValue:
                    ret = __tranverse_while(symbol)
                elif type(symbol) is ForValue:
                    ret = __tranverse_for(symbol)
                else:
                    ret = ParseType.Generate_Need_Effective_Type
                if ret != ParseType.Parse_OK:
                    print(symbol)
                    self.__push_error(ret, symbol)
            index_list = self.__var_map.pop(var_num)
            if index_list != []:
                self.__stock_index = min(index_list)

        assert type(symbol) is FunctionValue, '\'{}\' is not a function.'.format(symbol)
        command = '\t.globl _{}\n_{}:\n'.format(symbol.name, symbol.name)
        __add_command('pushq\t%rbp')
        __add_command('movq\t%rsp, %rbp')
        __add_command('subq\t$48, %rsp')
        params_num = symbol.get_params_num()
        symbol_table = symbol.get_block_table()
        for i, symbol in enumerate(symbol_table[:params_num]):
            self.__stock_index += 4
            self.__var_map.put(symbol.name, self.__stock_index)
            __add_command('movl\t%{}, {}(%rbp)'.format(__register(-i-1), self.__stock_index))
        __traverse_block(symbol_table[params_num:])
        self.__var_map.pop(params_num)
        self.__add_text(command)
        return ParseType.Parse_OK

    def __traverse_statement(self, symbol):

        def __trans_type(stype):
            if   stype is VarType.Char:  ret = 'byte'
            elif stype is VarType.Int:   ret = 'long'
            elif stype is VarType.Float: ret = 'long'
            else:                        ret = None
            return ret

        assert type(symbol) is VarValue, '\'{}\' is not a definition.'.format(symbol)
        name = symbol.name
        value = symbol.value if symbol.value else 0
        stype = __trans_type(symbol.type)
        if stype is None:
            return ParseType.Generate_Need_Effective_Type
        self.__add_data('\t.globl _{}\n_{}:\n\t.{}\t{}\n'.format(name, name, stype, value))
        return ParseType.Parse_OK

    def __traverse(self, symbol):
        if type(symbol) is VarValue:
            ret = self.__traverse_statement(symbol)
        elif type(symbol) is FunctionValue:
            ret = self.__tranverse_function(symbol)
        else:
            # print(symbol, type(symbol))
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

    def get_assembler(self):
        ret = self.__text_assembler
        if self.__data_number > 0:
            ret += '\n' + self.__data_assembler
        if self.__str_number > 0:
            ret += '\n' + self.__str_assembler
        return ret


if __name__ == '__main__':
    with open('./test/test.c', 'rt') as f:
        string = f.read()
    ic = GenerateAssembler()
    ic.traverse(string)
    if ic.get_error_number() > 0:
        print(ic.get_error_log())
    else:
        print(ic.get_assembler())
