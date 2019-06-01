# -*- coding: utf-8 -*-

class IdCounter(object):
    
    __id_counter = 0
    __digit_number = 0

    def __init__(self, digit_number):
        self.__digit_number = digit_number

    def get_next(self):
        self.__id_counter += 1
        return self.get()

    def get(self):
        return ('{'+':0{}'.format(self.__digit_number)+'}').format(self.__id_counter)