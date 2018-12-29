#-*- coding: UTF-8 -*-

import re
import ssl
import time
import urllib
import requests
import numpy as np
from bs4 import BeautifulSoup

from common import hds
from common import print_dict_to_json


path = '../../data' # data save path
ssl._create_default_https_context = ssl._create_unverified_context # MacOS need


def get_people_num(url):
    try:
        req = urllib.request.Request(url, headers=hds[np.random.randint(0, len(hds))])
        source_code = urllib.request.urlopen(req).read()
        plain_text = str(source_code, 'utf-8')
    except (urllib.error.HTTPError, urllib.error.URLError) as e:
        print(e)
    soup = BeautifulSoup(plain_text, "lxml")
    people_num = soup.find('a', {'class': 'rating_people'}).find('span').get_text()
    return people_num


def get_movie_details(url):
    try:
        req = urllib.request.Request(url, headers=hds[np.random.randint(0, len(hds))])
        source_code = urllib.request.urlopen(req).read()
        plain_text = str(source_code, 'utf-8')
    except (urllib.error.HTTPError, urllib.error.URLError) as e:
        print(e)
    details_dict = {}
    soup = BeautifulSoup(plain_text, "lxml")
    info_item = soup.find('div', {'id': 'info'})

    # peaple number
    details_dict['people_num'] = soup.find('a', {'class': 'rating_people'}).find('span').get_text()
    details_dict['rating'] = soup.find('strong', {'class': 'rating_num'}).get_text()
    
    # director, screenwriter, starring
    details_list = []
    for detail in info_item.findAll('span', {'class': 'attrs'}):
        details_list.append(detail.get_text())
    details_dict['director'] = details_list[0]
    details_dict['screenwriter'] = details_list[1]
    details_dict['starring'] = details_list[2]

    # nation, language, release date, length
    info_str = str(info_item)
    details_dict['nation'] = re.search(r'制片国家/地区:</span>(.*?)<br/>', info_str).group(1).strip()
    details_dict['language'] = re.search(r'语言:</span>(.*?)<br/>', info_str).group(1).strip()
    details_dict['release_date'] = re.search(r'上映日期:.*?">(.*?)</span>', info_str).group(1).strip()
    details_dict['length'] = re.search(r'片长:.*?">(.*?)</span>', info_str).group(1).strip()

    # return details_dict to json
    return details_dict


def douban_movie_top250_spider():
    movie_lists = []
    page_num = 0
    try_times = 0

    while True:
        url = "http://movie.douban.com/top250?start={}".format(str(page_num * 25))
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
        list_soup = soup.find('ol', {'class': 'grid_view'})

        try_times += 1;
        if list_soup == None and try_times < 200:
            continue
        elif list_soup == None or len(list_soup) <= 1:
            break # Break when no informatoin got after 200 times requesting

        for index, movie_item in enumerate(list_soup.findAll('li')):
            movie_dict = {}
            # picture item
            pic_item = movie_item.find('div', {'class': 'pic'})
            movie_dict['img_url'] = pic_item.find('img').get('src')

            # head item
            hd_item = movie_item.find('div', {'class': 'hd'})
            movie_dict['title'] = hd_item.find('span').get_text()
            movie_dict['movie_url'] = hd_item.find('a').get('href')
            movie_dict['movie_id'] = re.search(r'subject/(.*)/', movie_dict['movie_url']).group(1)

            # movie details
            try:
                movie_dict['movie_details'] = get_movie_details(movie_dict['movie_url'])
            except:
                movie_dict['movie_details'] = {}
            movie_lists.append(movie_dict)
            print('    Downloading Information From block %d' % index + 1)
        try_times = 0 # set 0 when got valid information
        page_num += 1
        print('Downloading Information From Page %d' % page_num)
    return movie_lists


if __name__=='__main__':
    movie_lists = douban_movie_top250_spider()
    print_dict_to_json(movie_lists, '{}/movie_top250.json'.format(path))
    # j = get_movie_details('https://movie.douban.com/subject/1292052/')