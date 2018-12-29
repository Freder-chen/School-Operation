import os

from douban_book_top250 import douban_book_top250_spider
from douban_movie_top250 import douban_movie_top250_spider
from douban_movie_comment import *
from douban_book import *
from common import print_dict_to_json


def book_by_tag_spider(path):
    book_path = '{}/book_by_tag'.format(path)
    if not os.path.exists(book_path):
        os.makedirs(book_path)
    
    # get tag list
    # https://book.douban.com/tag/?view=type&icn=index-sorttags-all

    book_tag_lists = ['名著', '小说', 
                      '历史', '心理学', '哲学', '传记', '思想',
                      '爱情', '成长', 
                      '经济学', '投资', '创业', '广告', '股票',
                      '科普', '互联网', '编程', '科学', '算法', '神经网络', '程序', 'web']
    for index, tag_name in enumerate(book_tag_lists):
        filepath = '{}/book_{}.json'.format(book_path, tag_name)
        if os.path.exists(filepath): continue
        book_lists = book_spider(tag_name)
        print_dict_to_json(book_lists, filepath)
        print('Downloading Tag From Index {} {}'.format(index + 1, tag_name))


def book_top250_spider(path):
    book_lists = douban_book_top250_spider()
    print_dict_to_json(book_lists, '{}/book_top250.json'.format(path))


def movie_top250_spider(path):
    movie_lists = douban_movie_top250_spider()
    print_dict_to_json(movie_lists, '{}/movie_top250.json'.format(path))


def movie_comment_spider(path):
    comment_path = '{}/movie_full_comment'.format(path)
    if not os.path.exists(comment_path):
        os.makedirs(comment_path)

    movie_id_list = get_movie_top250_id_list()
    print('Downloading movie list')

    for index, movie_id in enumerate(movie_id_list):
        filepath = '{}/movie_{}_comment.json'.format(comment_path, movie_id)
        if os.path.exists(filepath): continue
        comment_list = movie_commen_spider(movie_id)
        print_dict_to_json(comment_list, filepath)
        print('Downloading Comment From Index {}'.format(index + 1))


if __name__ == '__main__':
    path = '../data' # data save path

    book_top250_spider(path)
    movie_top250_spider(path)
    movie_comment_spider(path)
    book_by_tag_spider(path)
