#-*- coding: UTF-8 -*-

import re
import os
import jieba
import numpy as np
from snownlp import SnowNLP
from collections import Counter

from common import read_json_to_dict
from common import print_dict_to_json

path = '../../data'


def movie_contry_rank(dicts, number):
    country_list = []
    for dic in dicts:
        country_list += dic['movie_details']['nation'].replace(' ', '').split('/')
    country_list = Count(country_list).most_common(number)
    
    country_details_list = []
    for cname, cnumber in country_list:
        country_details = {}
        country_details['name'] = cname
        country_details['count'] = cnumber
        country_details_list.append(country_details)

    return country_details_list


def movie_length_rank(dicts, number):
    length_list = []
    for dic in dicts:
        length_list.append(int(re.sub(r'\D', '', dic['movie_details']['length'])))
    length_list = Count(length_list).most_common(number)

    length_details_list = []
    for lname, lnumber in length_list:
        length_details = {}
        length_details['name'] = lname
        length_details['count'] = lnumber
        length_details_list.append(length_details)
    
    return length_details_list


def movie_snownlp(comment_path, snownlp_path):
    sentiments_list = []

    for filename in os.listdir(comment_path):
        movie_id = re.search(r'movie_(.*)_comment', filename).group[1]
        movie_comments = read_json_to_dict('{}/{}'.format(comment_path, filename))

        sentiments = []
        for s in movie_comments:
            if len(s) < 5: continue
            sentiments.append(SnowNLP(s).sentiments)
        if sentiments == []: continue

        sentiments_dict = {}
        sentiments_dict['movie_id'] = movie_id
        sentiments_dict['min'] = min(sentiments)
        sentiments_dict['max'] = max(sentiments)
        sentiments_dict['ave'] = np.mean(sentiments)
        sentiments_dict['var'] = np.var(sentiments)
        sentiments_list.append(sentiments_dict)

    return sentiments_list


def cut_comment_list(comment_list):
    segment = []
    for line in comment_list:
        try:
            segs = jieba.lcut(line)
            for seg in segs:
                if len(seg) > 1 and seg not in ['\r\n']:
                    segment.append(seg)
        except:
            print(line)
            continue
    return dict(Counter(segment))


def movie_word_ratio(full_comment_path, comment_ratio_path):
    for filename in os.listdir(comment_path):
        movie_comments = read_json_to_dict('{}/{}'.format(full_comment_path, filename))
        seg_counter = cut_comment_list(movie_comments)
        print_dict_to_json(seg_counter, '{}/{}'.format(comment_ratio_path, filename))


if __name__ == '__main__':
    comment_path = '{}/movie_full_comment'.format(path)

    # movie comment
    movie_path = '{}/movie_comment'.format(path)
    if not os.path.exists(movie_path):
        os.makedirs(movie_path)

    movie_word_ratio(comment_path, movie_path)

    # movie rank
    movie_path = '{}/movie_rank'.format(path)
    if not os.path.exists(movie_path):
        os.makedirs(movie_path)

    jdatas = read_json_to_dict('{}/movie_top250.json'.format(path))
    
    country_details_list = movie_contry_rank(jdatas, 10)
    print_dict_to_json(country_details_list, '{}/country_rank.json'.format(movie_path))

    length_details_list = movie_length_rank(jdatas, 10)
    print_dict_to_json(length_details_list, '{}/length_rank.json'.format(movie_path))

    sentiments_list = movie_snownlp(comment_path, '{}/snownlp'.format(path))
    print_dict_to_json(sentiments_list, '{}/sentiments_rank.json'.format(movie_path))

