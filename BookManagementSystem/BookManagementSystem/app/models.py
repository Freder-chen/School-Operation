# -*- coding: utf-8 -*-

from werkzeug.security import generate_password_hash, check_password_hash
from flask_login import UserMixin
from app import db, login
import sys


@login.user_loader
def load_user(id):
    return User.query.get(id)


class Role(db.Model):
    __tablename__ = 'role'
    
    id            = db.Column(db.String(16), unique=True, primary_key=True)
    departments   = db.Column(db.String(32)) # , nullable=False
    major         = db.Column(db.String(32)) # , nullable=False
    borrow_limit  = db.Column(db.Integer   ) # , nullable=False
    borrow_period = db.Column(db.Integer   ) # , nullable=False
    borrow_amount = db.Column(db.Integer   ) # , nullable=False

    def alter(self, **kw):
        kw = { key: kw[key] for key in kw.keys() if key in Role.get_role_keys() }
        if 'departments' in kw: self.departments = kw['departments']
        if 'major' in kw: self.major = kw['major']
        if 'borrow_limit' in kw: self.borrow_limit = kw['borrow_limit']
        if 'borrow_period' in kw: self.borrow_period = kw['borrow_period']
        if 'borrow_amount' in kw: self.borrow_amount = kw['borrow_amount']
        db.session.commit()

    def commit(self):
        db.session.add(self)
        db.session.commit()

    def drop(self):
        db.session.delete(self)
        db.session.commit()

    def to_dict(self):
        return {
            'id': self.id, 'major': self.major,
            'departments': self.departments,
            'borrow_limit': self.borrow_limit,
            'borrow_period': self.borrow_period,
            'borrow_amount': self.borrow_amount,
        }

    @staticmethod
    def get(id):
        return Role.query.get(id)

    @staticmethod
    def find(**kw):
        return Role.query.filter_by(**kw)

    @staticmethod
    def get_role_keys():
        return ['id', 'departments', 'major', 'borrow_limit', 'borrow_period', 'borrow_amount']


class User(UserMixin, db.Model):
    __tablename__ = 'user'

    id            = db.Column(db.String(16), unique=True, primary_key=True)
    name          = db.Column(db.String(64), nullable=False)
    password_hash = db.Column(db.String(128), nullable=False)

    phone         = db.Column(db.String(16))
    email         = db.Column(db.String(32))
    type          = db.Column(db.String(32)) # user, book_admin, system_admin

    role_id       = db.Column(db.String(16), db.ForeignKey("role.id"))
    role          = db.relationship('Role', backref=db.backref('user'))

    def __init__(self, **kw):
        self.id = kw.pop('id')
        self.name = kw.pop('name', None)
        self.phone = kw.pop('phone', None)
        self.email = kw.pop('email', None)
        self.type = kw.pop('type') if 'type' in kw.keys() else 'user'
        self.password_hash = generate_password_hash(kw['password'] if 'password' in kw.keys() and type(kw['password']) is str else '123456')
        if self.type == 'user':
            kw['id'] = self.role_id = self.id
            kw = { key: kw[key] for key in kw.keys() if key in self.get_role_keys()}
            role = Role.get(kw['id'])
            if role: role.alter(**kw)
            else: Role(**kw).commit()

    def __repr__(self):
        return '<{} {}>'.format(self.type, self.name)

    def get_role_keys(self):
        return Role.get_role_keys() if self.type == 'user' else []

    def get_keys(self):
        keys = ['id', 'name', 'password', 'phone', 'email']
        return keys + [key for key in self.get_role_keys() if key not in keys]

    def check_password(self, password):
        if not password:
            return False
        return check_password_hash(self.password_hash, password)

    def to_dict(self):
        ret = self.role.to_dict() if self.role else {}
        ret.update({
            'id': self.id,
            'name': self.name,
            'phone': self.phone,
            'email': self.email,
        })
        return ret

    def alter(self, **kw):
        kw = { key: kw[key] for key in kw.keys() if key in self.get_keys() }
        if 'name'  in kw: self.name  = kw.pop('name')
        if 'phone' in kw: self.phone = kw.pop('phone')
        if 'email' in kw: self.email = kw.pop('email')
        if 'password' in kw:
            self.password_hash = generate_password_hash(kw.pop('password'))
        if self.type == 'user':
            self.role.alter(**kw)
        db.session.commit()

    def drop(self):
        role = self.role
        db.session.delete(self)
        db.session.commit()
        if self.type == 'user':
            role.drop()

    def commit(self):
        db.session.add(self)
        db.session.commit()

    @staticmethod
    def get(id):
        return User.query.get(id)

    @staticmethod
    def get_all():
        return User.query.all()

    @staticmethod
    def find(**kw):
        return User.query.filter_by(**kw)

    @staticmethod
    def get_user_keys():
        return User.get_admin_keys() + ['departments', 'major', 'borrow_limit', 'borrow_period', 'borrow_amount']

    @staticmethod
    def get_admin_keys():
        return ['id', 'name', 'password', 'phone', 'email']


class Book(db.Model):
    __tablename__ = 'book'

    id              = db.Column(db.String(16), unique=True, primary_key=True)
    name            = db.Column(db.String(64), nullable=False)
    author          = db.Column(db.String(64))
    translator      = db.Column(db.String(64))
    price           = db.Column(db.Float)
    isbn_code       = db.Column(db.String(64))
    come_up_time    = db.Column(db.String(64))
    publish_company = db.Column(db.String(64))
    status          = db.Column(db.Integer) # 0借出, 1在库
    entering_men    = db.Column(db.String(64))
    entering_date   = db.Column(db.DateTime)

    def __init__(self, **kw):
        self.id = kw.pop('id')
        self.name = kw.pop('name')
        self.author = kw.pop('author', None)
        self.translator = kw.pop('translator', None)
        self.price = kw.pop('price', None)
        self.isbn_code = kw.pop('isbn_code', None)
        self.come_up_time = kw.pop('come_up_time', None)
        self.publish_company = kw.pop('publish_company', None)
        self.status = kw.pop('status', 1)
        self.entering_men = kw.pop('entering_men', None)
        self.entering_date = kw.pop('entering_date', None)

    def __repr__(self):
        return '<Book {}>'.format(self.name)

    def to_dict(self):
        return {
            'id': self.id,
            'name': self.name,
            'author': self.author,
            'translator': self.translator,
            'price': self.price,
            'isbn_code': self.isbn_code,
            'come_up_time': self.come_up_time,
            'publish_company': self.publish_company,
            'status': self.status,
            'entering_men': self.entering_men,
            'entering_date': self.entering_date.strftime('%Y-%m-%d') if self.entering_date else None
        }

    def alter(self, **kw):
        kw = { key: kw[key] for key in kw.keys() if key in Book.get_book_keys() }
        if 'name' in kw: self.name = kw['name']
        if 'author' in kw: self.author = kw['author']
        if 'translator' in kw: self.translator = kw['translator']
        if 'price' in kw: self.price = kw['price']
        if 'isbn_code' in kw: self.isbn_code = kw['isbn_code']
        if 'come_up_time' in kw: self.come_up_time = kw['come_up_time']
        if 'publish_company' in kw: self.publish_company = kw['publish_company']
        if 'status' in kw: self.status = kw['status']
        if 'entering_men' in kw: self.entering_men = kw['entering_men']
        db.session.commit()

    def commit(self):
        db.session.add(self)
        db.session.commit()

    def drop(self):
        db.session.delete(self)
        db.session.commit()

    @staticmethod
    def get(id):
        return Book.query.get(id)

    @staticmethod
    def get_max_id():
        return db.session.query(db.func.max(Book.id)).scalar()

    @staticmethod
    def generate_book_id():
        old_id = Book.get_max_id()
        if not old_id:
            return '{:06}'.format(1)
        return '{:06}'.format(int(old_id) + 1)

    @staticmethod
    def get_book_keys():
        return [
            'id', 'name', 'author', 'translator', 'price', 'isbn_code', 'come_up_time',
            'publish_company', 'status', 'entering_men', 'entering_date'
        ]


class BorrowRecord(db.Model):
    __tablename__ = 'borrow_record'
    
    id          = db.Column(db.Integer, unique=True, primary_key=True)
    user_id     = db.Column(db.String(16), db.ForeignKey("user.id"))
    book_id     = db.Column(db.String(16), db.ForeignKey("book.id"))
    borrow_time = db.Column(db.DateTime)
    should_time = db.Column(db.DateTime)
    return_time = db.Column(db.DateTime)
    status       = db.Column(db.Integer) # 0借出, 1已还
    user        = db.relationship('User', backref=db.backref('borrow_record'))
    book        = db.relationship('Book', backref=db.backref('borrow_record'))

    def __init__(self, **kw):
        self.user_id = kw.pop('user_id')
        self.book_id = kw.pop('book_id')
        self.borrow_time = kw.pop('borrow_time', None)
        self.should_time = kw.pop('should_time', None)
        self.return_time = kw.pop('return_time', None)
        self.status = kw.pop('status', 0)

    def __repr__(self):
        return '<BorrowRecord {}>'.format(self.id)

    def to_dict(self):
        return {
            'user_id': self.user_id,
            'user_name': self.user.name if self.user else None,

            'book_id': self.book_id,
            'book_name': self.book.name if self.book else None,
            'book_author': self.book.author if self.book else None,
            'book_translator': self.book.translator if self.book else None,
            'book_price': self.book.price if self.book else None,
            'book_publish_company': self.book.publish_company if self.book else None,
            'book_come_up_time': self.book.come_up_time if self.book else None,
            'book_isbn_code': self.book.isbn_code if self.book else None,
            'book_entering_men': self.book.entering_men if self.book else None,
            'book_entering_date': self.book.entering_date.strftime('%Y-%m-%d') if self.book and self.book.entering_date else None,
            'book_status': '借出' if self.book and self.book.status == 1 else '未借',

            'borrow_time': self.borrow_time.strftime('%Y-%m-%d') if self.borrow_time else None,
            'should_time': self.should_time.strftime('%Y-%m-%d') if self.should_time else None,
            'return_time': self.return_time.strftime('%Y-%m-%d') if self.return_time else None,
            'status': self.status,
        }

    def alter(self, **kw):
        kw = { key: kw[key] for key in kw.keys() if key in BorrowRecord.get_borrow_record_keys() }
        if 'user_id' in kw: self.user_id = kw['user_id']
        if 'book_id' in kw: self.book_id = kw['book_id']
        if 'borrow_time' in kw: self.borrow_time = kw['borrow_time']
        if 'should_time' in kw: self.should_time = kw['should_time']
        if 'return_time' in kw: self.return_time = kw['return_time']
        if 'status' in kw: self.status = kw['status']
        db.session.commit()

    def commit(self):
        db.session.add(self)
        db.session.commit()

    def drop(self):
        db.session.delete(self)
        db.session.commit()

    @staticmethod
    def get(id):
        return BorrowRecord.query.get(id)

    @staticmethod
    def get_borrow_record_keys():
        return ['user_id', 'book_id', 'borrow_time', 'should_time', 'return_time', 'status']

