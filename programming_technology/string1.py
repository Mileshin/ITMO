#!/usr/bin/python3 

# 1. 
# Входящие параметры: int <count> , 
# Результат: string в форме
# "Number of: <count>", где <count> число из вход.парам.
#  Если число равно 10 или более, напечатать "many"
#  вместо <count>
#  Пример: (5) -> "Number of: 5"
#  (23) -> 'Number of: many'

def num_of_items(count):
# Test num_of_items
############################################
	"""
	test1 num_of_items 
	>>> num_of_items( 5 )
	'Number of: 5'
	
	test2 num_of_items 
	>>> num_of_items( 23 )
	'Number of: many'
	"""
############################################
	return "Number of: " + ("many" if count > 10 else str(count))

# 2. 
# Входящие параметры: string s, 
# Результат: string из 2х первых и 2х последних символов s
# Пример 'welcome' -> 'weme'.
def start_end_symbols(s):
# Test start_end_symbols
############################################
	"""
	test1 start_end_symbols 
	>>> start_end_symbols( 'welcome' )
	'weme'
	"""
############################################
	if len(s) < 2:
		raise Exception("string length must be bigger than 1 symbol")
	return (s[:2]+s[len(s)-2:])


# 3. 
# Входящие параметры: string s,
# Результат: string где все вхождения 1го символа заменяются на '*'
# (кроме самого 1го символа)
# Пример: 'bibble' -> 'bi**le'
# s.replace(stra, strb) 
def replace_char(s):
# Test replace_char
############################################
	"""
	test1 replace_char 
	>>> replace_char( 'bibble' )
	'bi**le'
	"""
############################################
	return s[0] + s.replace(s[0], "*")[1:]


# 4
# Входящие параметры: string a и b, 
# Результат: string где <a> и <b> разделены пробелом 
# а превые 2 симв обоих строк заменены друг на друга
# Т.е. 'max', pid' -> 'pix mad'
# 'dog', 'dinner' -> 'dig donner'
def str_mix(a, b):
# Test str_mix
############################################
	"""
	test1 str_mix 
	>>> str_mix( 'max', 'pid' )
	'pix mad'
	"""
	
	"""
	test2 str_mix 
	>>> str_mix( 'dog', 'dinner' )
	'dig donner'
	"""
############################################
	return b[:2] + a[2:] + ' ' + a[:2] + b[2:]


# Provided simple test() function used in main() to print
# what each function returns vs. what it's supposed to return.


if __name__ == '__main__':
	import doctest
	import re
	doctest.testmod( verbose = True )
