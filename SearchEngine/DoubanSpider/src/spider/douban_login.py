#-*- coding: UTF-8 -*-

import re
import ssl
import urllib
import requests
import numpy as np
from bs4 import BeautifulSoup

from common import hds


path = '../../data' # data save path
ssl._create_default_https_context = ssl._create_unverified_context # MacOS need


def login_session(dest_url):
	url = 'https://accounts.douban.com/login'
	logindata = {
		'redir': dest_url,
		'form_email': '17726100788',
		'form_password': 'p.16m.13',
		'login': u'登录',
	}

	source_code = requests.post(url, logindata, headers=hds[0]).text
	soup = BeautifulSoup(source_code, "html.parser")
	
	# fill captcha-id
	pattern = re.compile('<input type="hidden" name="captcha-id" value="(.*?)"/>')
	if pattern:
		logindata['captcha-id'] = re.findall(pattern, source_code)
	
	# fill captcha-solution
	captcha_image = soup.find('img',id='captcha_image')
	if captcha_image:
		urllib.request.urlretrieve(captcha_image['src'], '{}/captcha.jpg'.format(path))
		logindata['captcha-solution'] = input('please input the captcha:')

	session = requests.Session()
	resp = session.post(dest_url, data = logindata, headers = hds[0])

	return resp, session


	# req = requests.post(url, data=logindata, headers=hds[0])
	# return req

def get_page(url, session):
	resp = session.get(url)
	if resp.status_code == requests.codes.ok:
		return resp.text
	else:
		return '{}\nstatus {}'.format(url, resp.status_code)


if __name__ == '__main__':
	url = 'https://book.douban.com/'
	
	resp, session = login_session(url)

	url = 'https://www.douban.com/people/149796054/'
	page_text = get_page(url, session)
	print(page_text)
