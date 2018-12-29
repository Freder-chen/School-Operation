#-*- coding: UTF-8 -*-

import re
import ssl
import json
import time
import urllib
import requests
import numpy as np
from bs4 import BeautifulSoup

from common import hds
from common import print_dict_to_json


path = '../../data' # data save path
ssl._create_default_https_context = ssl._create_unverified_context # MacOS need


def get_book_details(url):
    try:
        req = urllib.request.Request(url, headers=hds[np.random.randint(0, len(hds))])
        source_code = urllib.request.urlopen(req).read()
        plain_text = str(source_code, 'utf-8')
    except (urllib.error.HTTPError, urllib.error.URLError) as e:
        print(e)
    details_dict = {}
    soup = BeautifulSoup(plain_text, "lxml")

    # peaple number
    details_dict['people_num'] = soup.find('a', {'class': 'rating_people'}).find('span').get_text().strip()
    details_dict['rating'] = soup.find('strong', {'class': 'rating_num'}).get_text().strip()

    # author, publisher, published year, price
    info_item = soup.find('div', {'id': 'info'})
    info_str = ' '.join(str(info_item).split())
    try:
        details_dict['author'] = re.search(r'作者:</span> <a href="\S*">(.*?)</a>', info_str).group(1).strip()
    except:
        details_dict['author'] = ''
    details_dict['publisher'] = re.search(r'出版社:</span>(.*?)<br/>', info_str).group(1).strip()
    details_dict['published_year'] = re.search(r'出版年:</span>(.*?)<br/>', info_str).group(1).strip()
    details_dict['price'] = re.search(r'定价:</span>(.*?)<br/>', info_str).group(1).strip()

    # tags
    tags_item = soup.find('div', {'id': 'db-tags-section'}).find('div', {'class': 'indent'})
    tag_list = []
    for tag in tags_item.findAll('span'):
        tag_list.append(tag.get_text().strip())
    details_dict['tags'] = ','.join(tag_list)

    # return details_dict to json
    return details_dict


def douban_book_top250_spider():
    book_lists = []
    page_num = 0
    try_times = 0

    while True:
        url = "http://book.douban.com/top250?start={}".format(str(page_num * 25))
        time.sleep(np.random.rand() * 5)

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
        list_soup = soup.find('div', {'class': 'article'})

        try_times += 1;
        if list_soup == None and try_times < 200:
            continue
        elif list_soup == None or len(list_soup.findAll('table')) < 1:
            break # Break when no informatoin got after 200 times requesting

        for index, book_item in enumerate(list_soup.findAll('table')):
            book_dict = {}
            book_dict['img_url'] = book_item.find('img').get('src')

            hd_item = book_item.find('div', {'class': 'pl2'})
            book_dict['title'] = ' '.join(hd_item.find('a').get_text().split())
            book_dict['book_url'] = hd_item.find('a').get('href')
            book_dict['book_id'] = re.search(r'subject/(.*)/', book_dict['book_url']).group(1)

            # book details
            try:
                book_dict['book_details'] = get_book_details(book_dict['book_url'])
            except:
                book_dict['book_details'] = {}
                print('--- book detrails error')
            book_lists.append(book_dict)
            print('    Downloading Information From block {}'.format(index + 1))
        try_times = 0 # set 0 when got valid information
        page_num += 1
        print('Downloading Information From Page %d' % page_num)
    return book_lists


if __name__=='__main__':
    book_lists = douban_book_top250_spider()
    print_dict_to_json(book_lists, '{}/book_top250.json'.format(path))
    # get_book_details(url)
