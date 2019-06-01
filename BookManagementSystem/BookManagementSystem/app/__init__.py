# -*- coding: utf-8 -*-

from flask import Flask
from flask_sqlalchemy import SQLAlchemy
from flask_login import LoginManager
from flask_migrate import Migrate

from app.config import Config

app = Flask(__name__)
app.config.from_object(Config)

db = SQLAlchemy(app)
login = LoginManager(app)
migrate = Migrate(app, db)

from app import routes, models

db.create_all()

from app.models import User, Book, BorrowRecord
import sys

# User(id='001', name='susan', email='susan@example.com', password="kankan123").commit()
# User(id='002', type='system_admin', name='freder', email='freder@example.com', password="kankan123").commit()
# User(id='003', type='book_admin', name='black', email='black@example.com', password="kankan123").commit()

# Book(id='000001', name='深入浅出Vue.js', author='刘博文', price='55.70', isbn_code='9787115509055').commit()
# Book(id='000002', name='深入浅出Vue.js', author='刘博文', price='55.70', isbn_code='9787115509055').commit()
# Book.generate_book_id()

# BorrowRecord(user_id='001', book_id='000001').commit()
# print(BorrowRecord.get(1).user.id)