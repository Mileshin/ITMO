#!/usr/bin/python36 

# 1. 
# Вх: строка. Если длина > 3, добавить в конец "ing", 
# если в конце нет уже "ing", иначе добавить "ly".
def v(s):
# Test
############################################
	"""
	test v 1
	>>> v( 'test' )
	'testing'

	test v 2
	>>> v( 'skiing' )
	'skiingly'

	test v 3
	>>> v( 'no' )
	'no'

	test v 4
	>>> v( '123' )
	'123'
	
	test v 5
	>>> v( 'testly' )
	'testlying'
	"""
############################################
  
	if len(s) > 3:
		if s[-3:] == 'ing':
			s += 'ly'
		else:
			s += 'ing'
  
	return s


# 2. 
# Вх: строка. Заменить подстроку от 'not' до 'bad'. ('bad' после 'not')
# на 'good'.
# Пример: So 'This music is not so bad!' -> This music is good!

def nb(s):
# Test
############################################
	"""
	test nb 1
	>>> nb( 'This music is not so bad!' )
	'This music is good!'

	test nb 2
	>>> nb( 'This music is not bad!' )
	'This music is good!'
	
	test nb 3
	>>> nb( 'This music is good!' )
	'This music is good!'
	
	test nb 4
	>>> nb( 'This music is bad!' )
	'This music is bad!'
	
	test nb 5
	>>> nb( 'This music is no good!' )
	'This music is no good!'
	"""
############################################

	s = re.sub( r'not.*bad', 'good', s )
	return s

if __name__ == '__main__':
	import doctest
	import re
	doctest.testmod( verbose = True )
