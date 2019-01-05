#-*- coding: UTF-8 -*-

import random
import numpy as np
import tkinter as tk


n = 3 # change n, not change row and col

row, col = 2**n, 2**n
num = 0 # record color num


def get_random_color(num):
    return ['#' + ''.join([random.choice('0123456789ABCDEF') for j in range(6)]) for i in range(num)]


def find_missing(bg2dl, n, x, y):
    n, x, y = int(n), int(x), int(y)
    for i in range(x, x + n):
        for j in range(y, y + n):
            if bg2dl[i][j] != -1:
                return i, j
    return -1, -1 # error


def puttromino(bg2dl, p1, p2, p3, num):
    def _puttromino(bg2dl, p, num):
        bg2dl[int(p[0])][int(p[1])] = num

    _puttromino(bg2dl, p1, num)
    _puttromino(bg2dl, p2, num)
    _puttromino(bg2dl, p3, num)


def tromino(bg2dl, n, x, y):
    global num
    
    if n == 1: return

    # find missing
    mr, mc = find_missing(bg2dl, n, x, y)

    num += 1
    if mr < x + n / 2 and mc < y + n / 2:
        # If missing Tile is in 1st quadrant
        puttromino(bg2dl, (x+n/2, y+n/2-1), (x+n/2, y+n/2), (x+n/2-1, y+n/2), num)
    elif mr >= x + n / 2 and mc < y + n / 2:
        # If missing Tile is in 2st quadrant
        puttromino(bg2dl, (x+n/2-1, y+n/2), (x+n/2, y+n/2), (x+n/2-1, y+n/2-1), num)
    elif mr < x + n / 2 and mc >= y + n / 2:
        # If missing Tile is in 3st quadrant
        puttromino(bg2dl, (x+n/2, y+n/2-1), (x+n/2, y+n/2), (x+n/2-1, y+n/2-1), num)
    elif mr >= x + n / 2 and mc >= y + n / 2:
        # If missing Tile is in 4st quadrant
        puttromino(bg2dl, (x+n/2-1, y+n/2), (x+n/2, y+n/2-1), (x+n/2-1, y+n/2-1), num)
    else:
        num -= 1

    tromino(bg2dl, n/2, x, y)
    tromino(bg2dl, n/2, x, y+n/2)
    tromino(bg2dl, n/2, x+n/2, y)
    tromino(bg2dl, n/2, x+n/2, y+n/2)


def cover_checkboard(bgl):
    bg2dl = np.reshape(bgl, (row, col))
    tromino(bg2dl, 2**n, 0, 0)
    return bg2dl.ravel()


def label_click(e):
    barecolor = 'black'
    # unbind all label
    children = e.widget.master.winfo_children()
    for child in children:
        child.unbind('<Button-1>')
    # change label background
    e.widget.configure(background=barecolor)
    # cover checkboard
    bgl = [-1 if child.cget('bg') != barecolor else 0 for child in children]
    # cover index
    bgl = cover_checkboard(bgl)
    # show
    colors = get_random_color(max(bgl) + 1)
    for i in range(len(children)):
        children[i].configure(text=bgl[i])
        if bgl[i] > 0: children[i].configure(background=colors[bgl[i] % len(colors)])


def add_checkboard(master, row, col):
    for i in range(row):
        for j in range(col):
            label = tk.Label(master, bg='white', width = 3, height= 2)
            label.grid(row=i, column=j, padx=1, pady=1)
            label.bind('<Button-1>', label_click)


def reset_bc(frame):
    global num
    # destroy checkboard
    for li in frame.grid_slaves():
        li.destroy()
    # readd checkboard
    add_checkboard(frame, row, col)
    # reset num
    num = 0


def add_content(window):
    # header
    tk.Label(window, bg='white', text = 'Tromino', height = 3).pack()
    # checkboard
    frame = tk.Frame(window, height=15, width=10, bg='black')
    add_checkboard(frame, row, col)
    frame.pack()
    # button
    tk.Button(window, text='reset', height=2, command=lambda: reset_bc(frame)).pack()


def main():
    window = tk.Tk()

    window.title('A1')
    window.geometry('500x500')
    add_content(window)

    window.mainloop()


if __name__ == '__main__':
    main()
