#-*- coding: UTF-8 -*-

from douban_book_top250 import *
from douban_book_top250 import *

from common import read_json_to_dict, print_dict_to_json


def book_top250_analysis(path):
	jdatas = read_json_to_dict('{}/book_top250.json'.format(path))

    tag_details_list = tags_rank(jdatas, 10)
    print_dict_to_json(tag_details_list, '{}/tags_rank.json'.format(path))

    people_num_details_list = people_num_rank(jdatas, 10)
    print_dict_to_json(people_num_details_list, '{}/people_num_rank.json'.format(path))

    publisher_details_list = publisher_rank(jdatas, 10)
    print_dict_to_json(publisher_details_list, '{}/publisher_rank.json'.format(path))
    
    published_year_rank(jdatas)


def movie_top250_analysis(path):
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


if __name__ == '__main__':
	path = '../data'
	book_top250_analysis(path)
	movie_top250_analysis(path)
