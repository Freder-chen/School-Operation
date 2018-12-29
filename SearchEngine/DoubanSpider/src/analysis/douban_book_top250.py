#-*- coding: UTF-8 -*-

import re
import heapq
from collections import Counter

from common import read_json_to_dict
from common import print_dict_to_json


path = '../../data'


'''
    tags rank
'''

def get_ave_rating_of_tag(tag_name, dicts):
    rating_list = []
    for dic in dicts:
        try:
            if tag_name in dic['book_details']['tags'].split(','):
                rating_list.append(float(dic['book_details']['rating']))
        except:
            continue
    return round(sum(rating_list) / len(rating_list), 2)


def get_ave_people_num_of_tag(tag_name, dicts):
    people_num_list = []
    for dic in dicts:
        try:
            if tag_name in dic['book_details']['tags'].split(','):
                people_num_list.append(int(dic['book_details']['people_num']))
        except:
            continue
    return round(sum(people_num_list) / len(people_num_list), 2)


def get_rep_book_of_tag(tag_name, dicts):
    rep_book_name_list = []
    for dic in dicts:
        try:
            if len(rep_book_name_list) > 3: break

            if tag_name in dic['book_details']['tags'].split(','):
                book = {}
                book['title'] = dic['title']
                book['img_url'] = dic['img_url']
                rep_book_name_list.append(book)
        except:
            continue
    return rep_book_name_list


def tags_rank(dicts, number):
    tag_list = []
    for dic in dicts:
        try:
            tag_list += dic['book_details']['tags'].split(',')
        except:
            continue
    tag_list = Counter(tag_list).most_common(number)

    tag_details_list = []
    for tname, tcount in tag_list:
        tag_details = {}
        tag_details['name'] = tname
        tag_details['count'] = tcount
        tag_details['ave_rating'] = get_ave_rating_of_tag(tname, dicts)
        tag_details['ave_people_num'] = get_ave_people_num_of_tag(tname, dicts)
        tag_details['rep_books'] = get_rep_book_of_tag(tname, dicts)
        tag_details_list.append(tag_details)
    
    return tag_details_list


'''
    people number hot rank
'''

def people_num_rank(dicts, number):
    for dic in dicts:
        if dic['book_details'] == {}:
            dic['book_details']['people_num'] = 0
    dicts = heapq.nlargest(number, dicts, key=lambda s: int(s['book_details']['people_num']))

    people_num_details_list = []
    for dic in dicts:
        people_num_details = {}
        people_num_details['title'] = dic['title']
        people_num_details['people_num'] = dic['book_details']['people_num']
        people_num_details['author'] = dic['book_details']['author']
        people_num_details['rating'] = dic['book_details']['rating']
        people_num_details['price'] = dic['book_details']['price']
        people_num_details['img_url'] = dic['img_url']
        people_num_details_list.append(people_num_details)

    return people_num_details_list


'''
    publisher hot rank
'''

def get_ave_rating_of_publisher(publisher_name, dicts):
    rating_list = []
    for dic in dicts:
        try:
            if publisher_name == dic['book_details']['publisher']:
                rating_list.append(float(dic['book_details']['rating']))
        except:
            continue
    return round(sum(rating_list) / len(rating_list), 2)


def get_ave_people_num_of_publisher(publisher_name, dicts):
    people_num_list = []
    for dic in dicts:
        try:
            if publisher_name == dic['book_details']['publisher']:
                people_num_list.append(int(dic['book_details']['people_num']))
        except:
            continue
    return round(sum(people_num_list) / len(people_num_list), 2)


def get_rep_book_of_publisher(publisher_name, dicts):
    rep_book_name_list = []
    for dic in dicts:
        try:
            if len(rep_book_name_list) > 3: break

            if publisher_name == dic['book_details']['publisher']:
                book = {}
                book['title'] = dic['title']
                book['img_url'] = dic['img_url']
                rep_book_name_list.append(book)
        except:
            continue
    return rep_book_name_list


def publisher_rank(dicts, number):
    for dic in dicts:
        try:
            publisher_list += [dic['book_details']['publisher']]
        except:
            continue
    publisher_list = Counter(publisher_list).most_common(number)

    publisher_details_list = []
    for pname, pcount in publisher_list:
        publisher_details = {}
        publisher_details['name'] = pname
        publisher_details['count'] = pcount
        publisher_details['ave_rating'] = get_ave_rating_of_publisher(pname, dicts)
        publisher_details['ave_people_num'] = get_ave_people_num_of_publisher(pname, dicts)
        publisher_details['rep_books'] = get_rep_book_of_publisher(pname, dicts)
        publisher_details_list.append(publisher_details)

    return publisher_details_list


'''
    published year rank
'''

def published_year_rank(dicts):
    import numpy as np
    import matplotlib.pyplot as plt

    published_year_list = []
    for dic in dicts:
        try:
            year = re.search(r'\d+', dic['book_details']['published_year']).group(0)
            published_year_list += [int(year)]
        except:
            continue

    published_year_list = sorted(Counter(published_year_list).items(), key=lambda x: x[0])
    year_list = [x[0] for x in published_year_list]
    count_list = [x[1] for x in published_year_list]

    plt.bar(range(len(published_year_list)), count_list, tick_label=year_list)
    plt.xlabel('Year')
    plt.ylabel('Count')
    plt.title('The Count of Great Book by Year')
    # plt.text(60, .025, r'$mu=100, sigma=15$'))
    plt.show()


'''
    main func
'''

if __name__ == '__main__':
    jdatas = read_json_to_dict('{}/book_top250.json'.format(path))

    tag_details_list = tags_rank(jdatas, 10)
    print_dict_to_json(tag_details_list, '{}/tags_rank.json'.format(path))

    people_num_details_list = people_num_rank(jdatas, 10)
    print_dict_to_json(people_num_details_list, '{}/people_num_rank.json'.format(path))

    publisher_details_list = publisher_rank(jdatas, 10)
    print_dict_to_json(publisher_details_list, '{}/publisher_rank.json'.format(path))
    
    published_year_rank(jdatas)
