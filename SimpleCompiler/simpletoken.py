# -*- coding: utf-8 -*-

from utils import *

class ParseToken(object):

    def __init__(self):
        self.__tokens = []
        self.__line = 1 # current token's line of parse
        self.__error_log = ''
        self.__error_number = 0

    def __push_error(self, type, string):
        self.__error_number += 1
        self.__error_log += 'Error: {} in line {}\n\t{} => {}\n'.format(
            type, self.__line, get_line(self.string, self.__line), string)

    def __return_error(self, type):
        self.p -= 1
        return type

    def __return_ok(self, type, value):
        self.__tokens.append(TokenValue(type, value, self.__line))
        return ParseType.Parse_OK

    def __get_ch(self):
        if self.p >= len(self.string):
            return None
        return self.string[self.p]

    def __get_next_ch(self):
        if self.p >= len(self.string):
            return None
        ch = self.__get_ch()
        self.p += 1
        return ch

    def __parse_whitespace(self):
        while self.p < len(self.string) and isWhitespace(self.string[self.p]):
            if self.string[self.p] == '\n':
                self.__line += 1
            self.p += 1

    def __parse_char(self):
        p = self.p
        ch = self.__get_next_ch()
        assert ch == '\'', '{} is not \'.'.format(ch)
        ch = self.__get_next_ch()
        if ch == '\\':
            ch = self.__get_next_ch()
            if not ch in '\'\\bfnrt':
                return self.__return_error(ParseType.Parse_Char_Escape_Error)
        ch = self.__get_next_ch()
        if ch != '\'':
            return self.__return_error(ParseType.Parse_Char_Need_End_Sign)
        return self.__return_ok(TokenType.Char, self.string[p : self.p])

    def __parse_string(self):
        p = self.p
        ch = self.__get_next_ch()
        assert ch == '\"', '{} is not \".'.format(ch)

        ch = self.__get_next_ch()
        while ch != '\"':
            if ch == None or ch == '\n':
                return self.__return_error(ParseType.Parse_String_Need_End_Sign)
            if ch == '\\':
                ch = self.__get_next_ch()
                if not ch in '\'\\bfnrt':
                    return self.__return_error(ParseType.Parse_String_Escape_Error)
            ch = self.__get_next_ch()
        return self.__return_ok(TokenType.String, self.string[p : self.p])

    def __parse_number(self):
        isInt = True
        p = self.p
        ch = self.__get_next_ch()
        if ch != '0':
            if not isNumberic(ch):
                return self.__return_error(ParseType.Parse_Int_Number_Error)
            while self.p < len(self.string) and isNumberic(self.string[self.p]):
                ch = self.__get_next_ch()
        ch = self.string[self.p]
        if ch == '.':
            self.p += 1
            isInt = False
            ch = self.__get_next_ch()
            if not isNumberic(ch):
                return self.__return_error(ParseType.Parse_Float_Number_Error)
            while self.p < len(self.string) and isNumberic(self.string[self.p]):
                ch = self.__get_next_ch()
        if ch == 'e' or ch == 'E':
            self.p += 1
            isInt = False
            ch = self.__get_next_ch()
            if ch == '+' or ch == '-':
                ch = self.__get_next_ch()
            if not isNumberic(ch):
                return self.__return_error(ParseType.Parse_SNotation_Number_Error)
            while self.p < len(self.string) and isNumberic(self.string[self.p]):
                ch = self.__get_next_ch()
        return self.__return_ok(TokenType.Int if isInt else TokenType.Float, self.string[p : self.p])

    def __parse_var(self):
        p = self.p
        ch = self.__get_next_ch()
        assert isAlpha(ch), '{} is not alpha'.format(ch)
        while self.p < len(self.string) and (isAlpha(self.string[self.p]) or isNumberic(self.string[self.p])):
            ch = self.__get_next_ch()
        src = self.string[p: self.p]
        return self.__return_ok(TokenType.Keyword if src in keyword else TokenType.Var, src)

    def __parse_operator(self):
        # Comment
        if self.string.startswith('//', self.p):
            while True:
                ch = self.__get_next_ch()
                if ch == None or ch == '\n':
                    self.__line += 1
                    return ParseType.Parse_OK

        # Operator
        for i in operator:
            if self.string.startswith(i, self.p):
                self.p += len(i)
                return self.__return_ok(TokenType.Operator, i)
        self.p += 2
        return self.__return_error(ParseType.Parse_Except_Value)

    def __parse_token(self):
        ch = self.__get_ch()
        # Char
        if ch == '\'':
            return self.__parse_char()
        # String
        elif ch == '\"':
            return self.__parse_string()
        # Int, Float
        elif isNumberic(ch):
            return self.__parse_number()
        # Delimiter
        elif isDelimiter(ch):
            return self.__return_ok(TokenType.Delimiter, self.__get_next_ch())
        # Keyword, Var
        elif isAlpha(ch):
            return self.__parse_var()
        # Operator, Comment
        else:
            return self.__parse_operator()

    def parse_tokens(self, string):
        self.string = string
        self.p = 0
        self.__parse_whitespace()
        while self.p < len(self.string):
            p = self.p
            ret = self.__parse_token()
            if ret != ParseType.Parse_OK:
                self.__push_error(ret, self.string[p : self.p])
            self.__parse_whitespace()

    def get_error_log(self):
        return self.__error_log

    def get_error_number(self):
        return self.__error_number

    def get_tokens(self):
        return self.__tokens


def main():
    with open('./test/test.c', 'rt') as f:
        string = f.read()

    pw = ParseToken()
    pw.parse_tokens(string)

    for i in pw.get_tokens():
        print(i, i.line)
    print(pw.get_error_log())


if __name__ == '__main__':
    main()
