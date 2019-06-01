# -*- coding: utf-8 -*-

import os

class Config(object):
    
    SECRET_KEY                     = 'you-will-never-guess' # os.environ.get('SECRET_KEY') or
    SQLALCHEMY_DATABASE_URI        = 'mysql+pymysql://root:kankan123@localhost:3306/BOOK_MANAGEMENT_SYSTEM?charset=utf8'
    SQLALCHEMY_TRACK_MODIFICATIONS = False
    