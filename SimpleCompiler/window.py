# -*- coding: utf-8 -*-

import os
import tkinter as tk
from simpletoken import ParseToken
from simpleprogram import ParseProgram
from simple_assembler import GenerateAssembler


class window(tk.Tk):

    def __init__(self):
        super(window, self).__init__()
        self.title('Simple Compiler')
        self.geometry('1000x600')
        self.__add_context()

    def get_in_content(self):
        return self.iText.get('1.0', tk.END)

    def set_in_content(self, string):
        self.iText.delete('1.0', tk.END)
        self.iText.insert('1.0', string)

    def set_out_content(self, string):
        self.oText.delete('1.0', tk.END)
        self.oText.insert('1.0', string)

    def set_err_coutent(self, string):
        self.eText.delete('1.0', tk.END)
        self.eText.insert('1.0', string)

    def __add_context(self):
        # root
        frame = tk.Frame(self)
        frame.pack(fill="both",expand=1)

        frame.columnconfigure(0, weight=1)
        frame.grid_rowconfigure(0, weight=1)
        frame.grid_rowconfigure(1, weight=10)

        # menu
        menu_frame = tk.Frame(frame)
        menu_frame.grid(row=0, column=0, sticky="nsew")

        reload_button = tk.Button(menu_frame, text='reload', width=10, height=1, command=self.__hit_reload_button)
        reload_button.pack(side='left')
        token_button = tk.Button(menu_frame, text='token', width=10, height=1, command=self.__hit_token_button)
        token_button.pack(side='left')
        program_button = tk.Button(menu_frame, text='program', width=10, height=1, command=self.__hit_program_button)
        program_button.pack(side='left')
        assembler_button = tk.Button(menu_frame, text='assembler', width=10, height=1, command=self.__hit_assembler_button)
        assembler_button.pack(side='left')
        do_button = tk.Button(menu_frame, text='do', width=10, height=1, command=self.__hit_do_button)
        do_button.pack(side='left')
        
        # content
        content_frame = tk.Frame(frame)
        content_frame.grid(row=1, column=0, sticky="nsew")

        content_frame.rowconfigure(0, weight=1)
        content_frame.grid_columnconfigure(0, weight=1)
        content_frame.grid_columnconfigure(1, weight=1)

        self.iText = tk.Text(content_frame, highlightcolor='white', borderwidth=2, relief="solid")
        self.iText.grid(row=0, column=0, sticky="nsew")
        self.oText = tk.Text(content_frame, highlightcolor='white', borderwidth=2, relief="solid")
        self.oText.grid(row=0, column=1, sticky="nsew")

    def __hit_reload_button(self):
        self.set_in_content(self.__get_test())

    def __hit_token_button(self):
        istr = self.iText.get('1.0', tk.END)
        self.set_out_content(self.__get_token_out(istr))

    def __hit_program_button(self):
        istr = self.iText.get('1.0', tk.END)
        self.set_out_content(self.__get_program_out(istr))

    def __hit_assembler_button(self):
        istr = self.iText.get('1.0', tk.END)
        self.set_out_content(self.__get_assembler_out(istr))

    def __hit_do_button(self):
        istr = self.iText.get('1.0', tk.END)
        self.set_out_content(self.__get_do_out(istr))

    def __get_test(self):
        with open('./test/test.c', 'rt') as f:
            string = f.read()
        return string

    def __get_token_out(self, string):
        pt = ParseToken()
        pt.parse_tokens(string)
        return pt.get_error_log() if pt.get_error_number() > 0 \
            else '\n'.join(str(i) for i in pt.get_tokens())

    def __get_program_out(self, string):
        pp = ParseProgram()
        pp.parse_program(string)
        return pp.get_error_log() if pp.get_error_number() > 0 \
            else '\n'.join(str(i) for i in pp.get_symbol_table())

    def __get_assembler_out(self, string):
        ga = GenerateAssembler()
        ga.traverse(string)
        return ga.get_error_log() if ga.get_error_number() > 0 \
            else ga.get_assembler()

    def __get_do_out(self, string):
        ga = GenerateAssembler()
        ga.traverse(string)
        if ga.get_error_number() > 0:
            return ga.get_error_log()
        else:
            ass = ga.get_assembler()
            with open('./test/test.s', 'w') as file:
                file.write(ass)
            os.system('as -o ./test/test.o ./test/test.s')
            os.system('ld -e _main -macosx_version_min 10.14 -lSystem -o ./test/test ./test/test.o')
            return os.popen('./test/test').read()


if __name__ == '__main__':
    # with open('./test/test.c', 'rt') as f:
    #     string = f.read()
    win = window()
    # win.set_in_content(string)
    win.mainloop()
