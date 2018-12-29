#-*- coding: UTF-8 -*-

import os
import re
import ssl
import time
import urllib
import requests
import numpy as np
from bs4 import BeautifulSoup
from collections import Counter

from common import hds
from common import print_dict_to_json


path = '../../data' # data save path
ssl._create_default_https_context = ssl._create_unverified_context # MacOS need


def movie_commen_spider(movie_id):
    comment_list = []
    page_num = 0
    try_times = 0

    while True:
        url = 'https://movie.douban.com/subject/{}/comments?start={}&limit=20&sort=new_score&status=P'.format(movie_id, page_num * 20)
        time.sleep(np.random.rand() * 3)

        # Download page
        try:
            request = urllib.request.Request(url, headers = hds[page_num % len(hds)])
            source_code = urllib.request.urlopen(request).read()
            plain_text = str(source_code, 'utf-8')   
        except (urllib.error.HTTPError, urllib.error.URLError) as e:
            break # break when return 403 which means no information

        # Parse page
        soup = BeautifulSoup(plain_text, 'lxml')
        list_soup = soup.find('div', {'id': 'comments'})

        try_times += 1;
        if list_soup == None and try_times < 200:
            continue
        elif list_soup == None or len(list_soup) <= 1:
            break # Break when no informatoin got after 200 times requesting

        for comment_item in list_soup.findAll('div', {'class': 'comment-item'}):
            try:
                comment_block = comment_item.find('div', {'class': 'comment'})
                comment = comment_block.find('p').get_text()
            except:
                comment = ''
            comment_list += [comment]
        try_times = 0 # set 0 when got valid information
        page_num += 1
        # print('   Downloading Information From Page %d' % page_num)
    return comment_list


def get_movie_top250_id_list():
    movie_id_list = []
    page_num = 0
    try_times = 0
    while True:
        url = "http://movie.douban.com/top250?start={}".format(str(page_num * 25))
        time.sleep(np.random.rand() * 3)

        # Download page
        try:
            request = urllib.request.Request(url, headers = hds[page_num % len(hds)])
            source_code = urllib.request.urlopen(request).read()
            plain_text = str(source_code, 'utf-8')   
        except (urllib.error.HTTPError, urllib.error.URLError) as e:
            print(e)
            continue

        # Parse page
        soup = BeautifulSoup(plain_text, 'lxml')
        list_soup = soup.find('ol', {'class': 'grid_view'})

        try_times += 1;
        if list_soup == None and try_times < 200:
            continue
        elif list_soup == None or len(list_soup) <= 1:
            break # Break when no informatoin got after 200 times requesting

        for index, movie_item in enumerate(list_soup.findAll('li')):
            movie_url = movie_item.find('div', {'class': 'hd'}).find('a').get('href')
            movie_id = re.search(r'subject/(.*)/', movie_url).group(1)
            movie_id_list.append(movie_id)
        try_times = 0 # set 0 when got valid information
        page_num += 1
        # print('Downloading Information From Page %d' % page_num)
    return movie_id_list


if __name__=='__main__':
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
