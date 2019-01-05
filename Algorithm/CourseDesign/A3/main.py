#-*- coding: UTF-8 -*-

import time
import random
import numpy as np
import tkinter as tk


layer = 5 # can change


def accsum(n):
    return sum([i for i in range(1, n + 1)])


def get_random_num_list(n):
    return [random.randint(1, 2 * n) for i in range(n)]


def get_lchild(index):
    index += 1 # +1 to begin in 1

    # get index of layer
    l = 1
    while index > accsum(l):
        l += 1

    return index + l - 1 # -1 to begin in 0

    
def get_rchild(index):
    return get_lchild(index) + 1


def sum_path(num_list, path):
    return sum([num_list[i] for i in path])


def find_max_path(num_list):
    def _find_max_path(num_list, path):
        global layer

        if len(path) >= layer: return path
        lp = _find_max_path(num_list, path + [get_lchild(path[-1])])
        rp = _find_max_path(num_list, path + [get_rchild(path[-1])])
        path = lp if sum_path(num_list, lp) > sum_path(num_list, rp) else rp

        return path
    
    return _find_max_path(num_list, [0])


def label_click(e):
    e.widget.configure(background='black')


def add_tower(master):
    global layer

    num_list = get_random_num_list(accsum(layer))
    path = find_max_path(num_list)

    index = 0
    row, col = layer, 2 * layer - 1
    for i in range(1, layer + 1):
        for j in range(i):
            tk.Label(master, bg='white', width = 2, height= 1).grid(row=i, column=(layer-i)+2*j, padx=1, pady=1)
    
    for li in master.winfo_children():
        if len(path) > 0 and index == path[0]:
            li.configure(background='green')
            path.remove(index)
        li.configure(text=num_list[index])
        index += 1


def reset_bc(frame, text):
    global layer
    # change layer
    try:
        layer = int(text.get())
    except:
        layer = 5
        text.set('5')

    # destroy checkboard
    for li in frame.grid_slaves():
        li.destroy()
    # readd checkboard
    add_tower(frame)


def add_content(window):
    # header
    tk.Label(window, bg='white', text = 'Tower', height = 3).pack()
    # checkboard
    content_frame = tk.Frame(window, height=10, width=10, bg='black')
    add_tower(content_frame)
    content_frame.pack()
    # tail
    tail_frame = tk.Frame(window, height=10, width=10, bg='white')
    
    text = tk.StringVar()
    tk.Entry(tail_frame, textvariable=text).pack(padx=10, pady=10, side='left')
    text.set('5')
    
    tk.Button(tail_frame, text='reset', height=1, command=lambda: reset_bc(content_frame, text)).pack(padx=10, pady=10, side='right')
    
    tail_frame.pack()


def main():
    window = tk.Tk()

    window.title('A3')
    window.geometry('500x500')
    add_content(window)

    window.mainloop()


if __name__ == '__main__':
    main()
