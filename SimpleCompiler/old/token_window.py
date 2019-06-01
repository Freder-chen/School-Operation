# -*- coding: utf-8 -*-

import tkinter as tk
from simpletoken import *

class window(tk.Tk):

    def __init__(self):
        super(window, self).__init__()
        self.title('my win')
        self.geometry('800x500')
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
        self.button = tk.Button(self, text='编译', width=15, height=2, command=self.__hit_button)
        self.button.pack(side='top')

        frame = tk.Frame(self)
        frame.pack(side='bottom')

        frame.grid_columnconfigure(0, weight=1)
        frame.grid_columnconfigure(1, weight=1)
        frame.grid_rowconfigure(0, weight=1)
        frame.grid_rowconfigure(1, weight=8)

        self.iText = tk.Text(frame, highlightcolor='white', borderwidth=2, relief="solid")
        self.iText.grid(row=0, column=0, sticky="nsew")
        self.oText = tk.Text(frame, highlightcolor='white', borderwidth=2, relief="solid")
        self.oText.grid(row=0, column=1, sticky="nsew")
        self.eText = tk.Text(frame, highlightcolor='white', borderwidth=2, relief="solid")
        self.eText.grid(row=1, columnspan=2, sticky="nsew")

    def __hit_button(self):
        istr = self.iText.get('1.0', tk.END)
        pw = ParseToken()
        stu = pw.parse_tokens(istr)
        out = ''
        for i in pw.get_stack():
            out += '({}, {})\n'.format(i.type, i.value)
        self.set_out_content(out)
        self.set_err_coutent(pw.get_error_log())

if __name__ == '__main__':
    with open('Lextestfile_c.txt', 'rt') as f:
        string = f.read()
    win = window()
    win.set_in_content(string)
    win.mainloop()
