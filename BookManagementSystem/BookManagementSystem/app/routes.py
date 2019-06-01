# -*- coding: utf-8 -*-

import os, sys
import json, datetime
from app import app
from flask import Response, redirect, make_response, request
from flask_login import current_user, login_user, logout_user
from app.models import User, Role, Book, BorrowRecord
from app.forms import LoginForm

from sqlalchemy import or_


def get_root_dir():
    return os.getcwd()


def get_static_dir():
    return os.path.join(get_root_dir(), 'static')


def read_file(file_path):
    try:
        return open(file_path, 'rb').read()
    except IOError as exc:
        return str(exc)


def get_response(**kw):
    return Response(json.dumps(kw), mimetype='application/json')


@app.route('/', defaults={'path': 'html/index.html'})
@app.route('/<path:path>')
def static_file(path):
    mimetypes = {
        '.png' : 'image/png',
        '.css' : 'text/css',
        '.html': 'text/html',
        '.js'  : 'application/javascript',
    }
    mimetype = mimetypes.get(os.path.splitext(path)[1], "text/html")
    content = read_file(os.path.join(get_static_dir(), path))
    # if error
    return Response(content, mimetype=mimetype)


'''
    find_book_anyone
'''
def get_index(data, value):
    for i, v in enumerate(data):
        if value == v['isbn_code']:
            return i
    return -1


@app.route('/find-book-anyone', methods=['POST'])
def find_book_anyone():
    rdata = { key: value[0] for key, value in request.form.to_dict(flat=False).items() if key in ['find_value']}
    if 'find_value' not in rdata.keys():
        return get_response(status='fail: find_value is must.')
    find_value = '%{}%'.format(rdata['find_value'])
    book_list = Book.query.filter(or_(
        Book.name.like(find_value),
        Book.author.like(find_value),
        Book.translator.like(find_value),
    )).all()
    data = []
    for book in book_list:
        index = get_index(data, book.isbn_code)
        if index == -1:
            data.append({
                'name': book.name,
                'author': book.author,
                'publish_company': book.publish_company,
                'isbn_code': book.isbn_code,
                'number': 1,
            })
        else:
            data[index]['number'] += 1
    return get_response(status='success', data=data)


'''
    login, logout
'''
@app.route('/login-wx', methods=['POST'])
def login_wx():

    def __login(username, password, usertype):
        user = User.find(id=username).first()
        if user is None or not user.check_password(password):
            return get_response(status='fail: user name or password wrong.')
        print('success', file=sys.stderr)
        login_user(user)
        return get_response(status='success', type=user.type)

    if current_user.is_authenticated:
        logout_user()
    form = LoginForm()
    return __login(form.username.data, form.password.data, form.usertype.data)


@app.route('/login', methods=['POST'])
def login():

    def __login(username, password, usertype):

        def __get_user_redirect_url(usertype):
            urldict = {
                'user': '/html/readerPersonal.html',
                'book_admin': '/html/adBorrowBook.html',
                'system_admin': '/html/adminAddReader.html',
            }
            return urldict[usertype] if usertype in urldict.keys() else '/html/login.html'

        user = User.find(id=username).first()
        if user is None or not user.check_password(password):
            return '/html/login.html'
        login_user(user)
        return __get_user_redirect_url(user.type)

    if current_user.is_authenticated:
        logout_user()
    form = LoginForm()
    return redirect(__login(form.username.data, form.password.data, form.usertype.data))


@app.route('/logout', methods=['POST'])
def logout():
    logout_user()
    return redirect('/html/index.html')


'''
    set_self, set_self_password, get_self_wx, get_self
'''
@app.route('/set-self', methods=['POST'])
def set_self():
    if not current_user.is_authenticated:
        return get_response(status='fail: have no permission.')
    keys = current_user.get_keys()
    keys.remove('password')
    rdata = { key: value[0] for key, value in request.form.to_dict(flat=False).items() if key in keys }
    current_user.alter(**rdata)
    return get_response(status='success')


@app.route('/set-self-password', methods=['POST'])
def set_self_password():
    if not current_user.is_authenticated:
        return get_response(status='fail: have no permission.')
    rdata = { key: value[0] for key, value in request.form.to_dict(flat=False).items() \
        if key in ['new_password', 'old_password'] }
    if not (rdata and 'old_password' in rdata.keys() and current_user.check_password(rdata['old_password'])):
        return get_response(status='fail: current password is wrong.')
    current_user.alter(password=rdata['new_password'])
    return get_response(status='success')


@app.route('/get-self-wx', methods=['POST'])
def get_self_wx():
    if not current_user.is_authenticated:
        return get_response(status='fail: have no permission.')
    return get_response(status='success', data=current_user.to_dict())


@app.route('/get-self', methods=['POST'])
def get_self():
    # to-do: return status
    if not current_user.is_authenticated:
        return None
    return get_response(**current_user.to_dict())


'''
    set_user, get_user, find_user, add_user, del_user
'''
@app.route('/set-user', methods=['POST'])
def set_user():
    if not current_user.is_authenticated or current_user.type != 'system_admin':
        return get_response(status='fail: have no permission.')
    rdata = { key: value[0] for key, value in request.form.to_dict(flat=False).items() if key in User.get_user_keys()}
    if 'id' not in rdata.keys():
        return get_response(status='fail: id is must.')
    user = User.get(rdata['id'])
    if not user:
        return get_response(status='fail: the user({}) not exists.'.format(rdata['id']))
    if user.type != 'user':
        return get_response(status='fail: it\'s not a user.')
    user.alter(**rdata)
    return get_response(status='success')


@app.route('/get-user', methods=['POST'])
def get_user():
    if not current_user.is_authenticated or current_user.type != 'system_admin':
        return get_response(status='fail: have no permission.')
    rdata = { key: value[0] for key, value in request.form.to_dict(flat=False).items() if key in ['id']}
    if 'id' not in rdata.keys():
        return get_response(status='fail: id is must.')
    user = User.get(rdata['id'])
    if not user:
        return get_response(status='fail: the user({}) not exists.'.format(rdata['id']))
    if user.type != 'user':
        return get_response(status='fail: it\'s not a user.')
    return get_response(status='success', data=user.to_dict())


@app.route('/find-user', methods=['POST'])
def find_user():
    if not current_user.is_authenticated or current_user.type != 'system_admin':
        return get_response(status='fail: have no permission.')
    rdata = { key: value[0] for key, value in request.form.to_dict(flat=False).items() if key in ['find_value']}
    if 'find_value' not in rdata.keys():
        return get_response(status='fail: you have no the param which is \'find_value\'.')
    find_value = '%{}%'.format(rdata['find_value'])
    user_list = User.query.filter(
        User.type == 'user',
        or_(
            User.id.like(find_value),
            User.name.like(find_value),
            User.phone.like(find_value),
            User.email.like(find_value),
            Role.departments.like(find_value),
            Role.major.like(find_value)
        )
    ).all()
    return get_response(status='success', data=[user.to_dict() for user in user_list])


@app.route('/add-user', methods=['POST'])
def add_user():
    if not current_user.is_authenticated or current_user.type != 'system_admin':
        return get_response(status='fail: have no permission.')
    rdata = { key: value[0] for key, value in request.form.to_dict(flat=False).items() if key in User.get_user_keys() }
    if 'id' not in rdata.keys():
        return get_response(status='fail: id is must.')
    if User.get(rdata['id']):
        return get_response(status='fail: the user({}) exists.'.format(rdata['id']))
    if 'borrow_amount' not in rdata.keys():
        rdata['borrow_amount'] = 0
    if 'password' not in rdata.keys():
        rdata['password'] = '123456'
    rdata.update({
        'type': 'user',
    })
    User(**rdata).commit()
    return get_response(status='success')


@app.route('/del-user', methods=['POST'])
def del_user():
    if not current_user.is_authenticated or current_user.type != 'system_admin':
        return get_response(status='fail: have no permission.')
    rdata = { key: value[0] for key, value in request.form.to_dict(flat=False).items() if key in ['id']}
    if 'id' not in rdata.keys():
        return get_response(status='fail: id is must.')
    user = User.get(rdata['id'])
    if not user:
        return get_response(status='fail: the user({}) not exists.'.format(rdata['id']))
    if user.type != 'user':
        return get_response(status='fail: it\'s not a user.')
    user.drop()
    return get_response(status='success')


'''
    set_book_admin, get_book_admin, find_book_admin,
    add_book_admin, del_book_admin, set_system_admin
'''
@app.route('/set-book-admin', methods=['POST'])
def set_book_admin():
    if not current_user.is_authenticated or current_user.type != 'system_admin':
        return get_response(status='fail: have no permission.')
    rdata = { key: value[0] for key, value in request.form.to_dict(flat=False).items() if key in User.get_admin_keys()}
    if 'id' not in rdata.keys():
        return get_response(status='fail: id is must.')
    user = User.get(rdata['id'])
    if not user:
        return get_response(status='fail: the user({}) not exists.'.format(rdata['id']))
    if user.type != 'book_admin':
        return get_response(status='fail: it\'s not a book admin.')
    user.alter(**rdata)
    return get_response(status='success')


@app.route('/get-book-admin', methods=['POST'])
def get_book_admin():
    if not current_user.is_authenticated or current_user.type != 'system_admin':
        return get_response(status='fail: have no permission.')
    rdata = { key: value[0] for key, value in request.form.to_dict(flat=False).items() if key in ['id']}
    if 'id' not in rdata.keys():
        return get_response(status='fail: id is must.')
    user = User.get(rdata['id'])
    if not user:
        return get_response(status='fail: the user({}) not exists.'.format(rdata['id']))
    if user.type != 'book_admin':
        return get_response(status='fail: it\'s not a book admin.')
    return get_response(status='success', data=user.to_dict())


@app.route('/find-book-admin', methods=['POST'])
def find_book_admin():
    if not current_user.is_authenticated or current_user.type != 'system_admin':
        return get_response(status='fail: have no permission.')
    rdata = { key: value[0] for key, value in request.form.to_dict(flat=False).items() if key in ['find_value']}
    if 'find_value' not in rdata.keys():
        return get_response(status='fail: you have no the param which is \'find_value\'.')
    find_value = '%{}%'.format(rdata['find_value'])
    user_list = User.query.filter(
        User.type == 'book_admin',
        or_(
            User.id.like(find_value),
            User.name.like(find_value),
            User.phone.like(find_value),
            User.email.like(find_value),
        )
    ).all()
    return get_response(status='success', data=[user.to_dict() for user in user_list])


@app.route('/add-book-admin', methods=['POST'])
def add_book_admin():
    if not current_user.is_authenticated or current_user.type != 'system_admin':
        return get_response(status='fail: have no permission.')
    rdata = { key: value[0] for key, value in request.form.to_dict(flat=False).items() if key in User.get_admin_keys() }
    if 'id' not in rdata.keys():
        return get_response(status='fail: id is must.')
    if User.get(rdata['id']):
        return get_response(status='fail: the user({}) exists.'.format(rdata['id']))
    if 'password' not in rdata.keys():
        rdata['password'] = '123456'
    rdata.update({
        'type': 'book_admin',
    })
    User(**rdata).commit()
    return get_response(status='success')


@app.route('/del-book-admin', methods=['POST'])
def del_book_admin():
    if not current_user.is_authenticated or current_user.type != 'system_admin':
        return get_response(status='fail: have no permission.')
    rdata = { key: value[0] for key, value in request.form.to_dict(flat=False).items() if key in ['id']}
    if 'id' not in rdata.keys():
        return get_response(status='fail: id is must.')
    user = User.get(rdata['id'])
    if not user:
        return get_response(status='fail: the user({}) not exists.'.format(rdata['id']))
    if user.type != 'book_admin':
        return get_response(status='fail: it\'s not a book admin.')
    user.drop()
    return get_response(status='success')


@app.route('/set-system-admin-self', methods=['POST'])
def set_system_admin_self():
    if not current_user.is_authenticated or current_user.type != 'system_admin':
        return get_response(status='fail: have no permission.')
    keys = User.get_admin_keys() + ['new_password', 'old_password']
    keys.remove('password')
    rdata = { key: value[0] for key, value in request.form.to_dict(flat=False).items() if key in keys }
    if not (rdata and 'old_password' in rdata.keys() and current_user.check_password(rdata.pop('old_password'))):
        return get_response(status='fail: current password is wrong.')
    if 'new_password' in rdata.keys():
        rdata['password'] = rdata.pop('new_password')
    current_user.alter(**rdata)
    return get_response(status='success')


'''
book:
    generate_book_id, set_book, get_book, find_book,
    add_book, del_book, set_book_admin
'''
@app.route('/generate-book-id', methods=['POST'])
def generate_book_id():
    if not current_user.is_authenticated or current_user.type != 'book_admin':
        return get_response(status='fail: have no permission.')
    return get_response(status='success', data=Book.generate_book_id())


@app.route('/set-book', methods=['POST'])
def set_book():
    if not current_user.is_authenticated or current_user.type != 'book_admin':
        return get_response(status='fail: have no permission.')
    rdata = { key: value[0] for key, value in request.form.to_dict(flat=False).items() if key in Book.get_book_keys() }
    if 'id' not in rdata.keys():
        return get_response(status='fail: id is must.')
    book = Book.get(rdata['id'])
    if not book:
        return get_response(status='fail: the book({}) not exists.'.format(rdata['id']))
    book.alter(**rdata)
    return get_response(status='success')


@app.route('/get-book', methods=['POST'])
def get_book():
    if not current_user.is_authenticated or current_user.type != 'book_admin':
        return get_response(status='fail: have no permission.')
    rdata = { key: value[0] for key, value in request.form.to_dict(flat=False).items() if key in ['id']}
    if 'id' not in rdata.keys():
        return get_response(status='fail: id is must.')
    book = Book.get(rdata['id'])
    if not book:
        return get_response(status='fail: the book({}) not exists.'.format(rdata['id']))
    return get_response(status='success', data=book.to_dict())


@app.route('/find-book', methods=['POST'])
def find_book():
    if not current_user.is_authenticated or current_user.type != 'book_admin':
        return get_response(status='fail: have no permission.')
    rdata = { key: value[0] for key, value in request.form.to_dict(flat=False).items() if key in ['find_value']}
    if 'find_value' not in rdata.keys():
        return get_response(status='fail: find_value is must.')
    find_value = '%{}%'.format(rdata['find_value'])
    book_list = Book.query.filter(or_(
        Book.id.like(find_value),
        Book.name.like(find_value),
        Book.author.like(find_value),
        Book.translator.like(find_value),
        Book.price.like(find_value),
        Book.isbn_code.like(find_value),
        Book.publish_company.like(find_value),
    )).all()
    return get_response(status='success', data=[book.to_dict() for book in book_list])


@app.route('/add-book', methods=['POST'])
def add_book():
    if not current_user.is_authenticated or current_user.type != 'book_admin':
        return get_response(status='fail: have no permission.')
    rdata = { key: value[0] for key, value in request.form.to_dict(flat=False).items() if key in Book.get_book_keys() }
    if 'id' not in rdata.keys():
        return get_response(status='fail: id is must.')
    if 'name' not in rdata.keys():
        return get_response(status='fail: name is must.')
    if Book.get(rdata['id']):
        return get_response(status='fail: the book({}) already exists.'.format(rdata['id']))
    if 'entering_date' not in rdata.keys() or rdata['entering_date'] == '':
        rdata.update({ 'entering_date': datetime.datetime.now()})
    rdata['entering_men'] = current_user.name
    Book(**rdata).commit()
    return get_response(status='success')


@app.route('/del-book', methods=['POST'])
def del_book():
    if not current_user.is_authenticated or current_user.type != 'book_admin':
        return get_response(status='fail: have no permission.')
    rdata = { key: value[0] for key, value in request.form.to_dict(flat=False).items() if key in ['id']}
    if 'id' not in rdata.keys():
        return get_response(status='fail: id is must.')
    book = Book.get(rdata['id'])
    if not book:
        return get_response(status='fail: the book({}) not exists.'.format(rdata['id']))
    book.drop()
    return get_response(status='success')


@app.route('/set-book-admin-self', methods=['POST'])
def set_book_admin_self():
    if not current_user.is_authenticated or current_user.type != 'book_admin':
        return get_response(status='fail: have no permission.')
    keys = User.get_admin_keys() + ['new_password', 'old_password']
    keys.remove('password')
    rdata = { key: value[0] for key, value in request.form.to_dict(flat=False).items() if key in keys }
    if not (rdata and 'old_password' in rdata.keys() and current_user.check_password(rdata.pop('old_password'))):
        return get_response(status='fail: current password is wrong.')
    if 'new_password' in rdata.keys():
        rdata['password'] = rdata.pop('new_password')
    current_user.alter(**rdata)
    return get_response(status='success')


'''
borrorw_record:
    find_borrowing, find_user_borrowing, find_book_borrowing,
    borrow_book, return_book
'''
@app.route('/find-borrowing', methods=['POST'])
def find_borrowing():
    if not current_user.is_authenticated or current_user.type != 'book_admin':
        return get_response(status='fail: have no permission.')
    rdata = { key: value[0] for key, value in request.form.to_dict(flat=False).items() if key in ['find_value']}
    if 'find_value' not in rdata.keys():
        return get_response(status='fail: find_value is must.')
    find_value = '%{}%'.format(rdata['find_value'])
    borrow_list = BorrowRecord.query.filter(or_(
        BorrowRecord.status == 1,
        BorrowRecord.book_id.like(find_value),
        BorrowRecord.user_id.like(find_value),
        Book.name.like(find_value),
    )).all()
    return get_response(status='success', data=[record.to_dict() for record in borrow_list])


@app.route('/find-user-borrowing', methods=['POST'])
def find_user_borrowing():
    if not current_user.is_authenticated or current_user.type != 'book_admin':
        return get_response(status='fail: have no permission.')
    rdata = { key: value[0] for key, value in request.form.to_dict(flat=False).items() if key in ['user_id'] }
    if 'user_id' not in rdata.keys():
        return get_response(status='fail: you have no the param which is \'user_id\'.')
    user = User.get(rdata['user_id'])
    if not user or user.type != 'user':
        return get_response(status='fail: user is not valuable.')
    borrow_list = BorrowRecord.query.filter(
        BorrowRecord.user_id == rdata['user_id'],
        BorrowRecord.status  == 0
    ).all()
    return get_response(status='success', data=[record.to_dict() for record in borrow_list])


@app.route('/find-book-borrowing', methods=['POST'])
def find_book_borrowing():
    if not current_user.is_authenticated or current_user.type != 'book_admin':
        return get_response(status='fail: have no permission.')
    rdata = { key: value[0] for key, value in request.form.to_dict(flat=False).items() if key in ['find_value']}
    if 'find_value' not in rdata.keys():
        return get_response(status='fail: find_value is must.')
    find_value = '%{}%'.format(rdata['find_value'])
    borrow_list = BorrowRecord.query.filter(or_(
        BorrowRecord.book_id.like(find_value),
        Book.name.like(find_value),
        Book.author.like(find_value),
        Book.translator.like(find_value),
        Book.price.like(find_value),
        Book.publish_company.like(find_value),
        Book.isbn_code.like(find_value),
        Book.entering_men.like(find_value),
    )).all()
    return get_response(status='success', data=[record.to_dict() for record in borrow_list])


@app.route('/borrow-book', methods=['POST'])
def borrow_book():
    if not current_user.is_authenticated or current_user.type != 'book_admin':
        return get_response(status='fail: have no permission.')
    rdata = { key: value[0] for key, value in request.form.to_dict(flat=False).items() if key in ['user_id', 'book_id'] }
    if 'user_id' not in rdata.keys():
        return get_response(status='fail: user_id is must.')
    if 'book_id' not in rdata.keys():
        return get_response(status='fail: book_id is must.')
    user = User.get(rdata['user_id'])
    book = Book.get(rdata['book_id'])
    if not user or user.type != 'user':
        return get_response(status='fail: user is not valuable.')
    if user.role is None or user.role.borrow_amount is None or user.role.borrow_limit is None or user.role.borrow_amount >= user.role.borrow_limit:
        return get_response(status='fail: the user\'s borrow number exceed the limit.')
    print(rdata['book_id'], book, file=sys.stderr)
    if not book:
        return get_response(status='fail: the book is not valuable.')
    if book.status == 0:
        return get_response(status='fail: the book was borrowed.')
    user.alter(borrow_amount=user.role.borrow_amount + 1)
    book.alter(status=0)
    now = datetime.datetime.now()
    BorrowRecord(
        user_id = rdata['user_id'],
        book_id = rdata['book_id'],
        borrow_time = now,
        should_time = now + datetime.timedelta(days=user.role.borrow_period),
        status = 0,
    ).commit()
    return get_response(status='success')


@app.route('/return-book', methods=['POST'])
def return_book():
    if not current_user.is_authenticated or current_user.type != 'book_admin':
        return get_response(status='fail: have no permission.')
    rdata = { key: value[0] for key, value in request.form.to_dict(flat=False).items() if key in ['book_id'] }
    if 'book_id' not in rdata.keys():
        return get_response(status='fail: book_id is must.')
    borrow_list = BorrowRecord.query.filter(
        BorrowRecord.book_id == rdata['book_id'],
        BorrowRecord.status  == 0
    ).all()
    if len(borrow_list) == 0:
        return get_response(status='fail: the book is not out.')
    borrow_record = borrow_list[0]
    user = User.get(borrow_record.user_id)
    book = Book.get(borrow_record.book_id)
    user.alter(borrow_amount=user.role.borrow_amount - 1)
    book.alter(status=1)
    now = datetime.datetime.now()
    borrow_record.alter(status=1, return_time=now)
    return get_response(status='success')


'''
    find_borrowing_self, find_borrowed_self
'''
@app.route('/find-borrowing-self', methods=['POST'])
def find_borrowing_self():
    if not current_user.is_authenticated or current_user.type != 'user':
        return get_response(status='fail: have no permission.')
    borrow_list = BorrowRecord.query.filter(
        BorrowRecord.status  == 0,
        BorrowRecord.user_id == current_user.id,
    ).all()
    amount = current_user.role.borrow_amount
    remain = current_user.role.borrow_limit - amount
    return get_response(
        status='success',
        amount = amount,
        remain = remain,
        data = [record.to_dict() for record in borrow_list]
    )


@app.route('/find-borrowed-self', methods=['POST'])
def find_borrowed_self():
    if not current_user.is_authenticated or current_user.type != 'user':
        return get_response(status='fail: have no permission.')
    borrow_list = BorrowRecord.query.filter(
        BorrowRecord.status  == 1,
        BorrowRecord.user_id == current_user.id,
    ).all()
    return get_response(status='success', data=[record.to_dict() for record in borrow_list])


# @app.errorhandler(404)
# def not_found(error):
#     return '404:\n{}'.format(error), 404
