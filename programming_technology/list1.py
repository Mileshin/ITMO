#!/usr/bin/python36 

# 1.
# Вх: список строк, Возвр: кол-во строк
# где строка > 2 символов и первый символ == последнему

def me(words):
	
# Test
############################################
    """
	# test me 1
    >>> me(['abc', 'xxy', 'abcA', 'abca', 'aa', '1', '', '*6*'])
    2
    
	# test me 2
    >>> me([])
    0
    """
############################################

    count = 0
    for string in words:
        if len(string) > 2 and string[-1] == string[0]:
            count += 1
    return count


# 2.
# Вх: список строк, Возвр: список со строками (упорядочено)
# за искл всех строк начинающихся с 'x', которые попадают в начало списка.
# ['tix', 'xyz', 'apple', 'xacadu', 'aabbbccc'] -> ['xacadu', 'xyz', 'aabbbccc', 'apple', 'tix']

def fx(words):
	
# Test
############################################
    """
	# Test fx 1
    >>> fx( ['tix', 'xyz', 'apple', 'xacadu', 'aabbbccc'] )
    ['xacadu', 'xyz', 'aabbbccc', 'apple', 'tix']
   
	# Test fx 2
    >>> fx( ['a', 'x', 'y', ''] )
    ['x', '', 'a', 'y']
    """
############################################

    return sorted( words, key=lambda word: word if word and word[0] == 'x' else 'z' + word )

# 3.
# Вх: список непустых кортежей, 
# Возвр: список сортир по возрастанию последнего элемента в каждом корт.
# [(1, 7), (1, 3), (3, 4, 5), (2, 2)] -> [(2, 2), (1, 3), (3, 4, 5), (1, 7)]

def tuplesSort(tuples):
	
# Test
############################################
    """
	# Test tuplesSort 1
    >>> tuplesSort( [(1, 7), (1, 3), (3, 4, 5), (2, 2) ] )
    [(2, 2), (1, 3), (3, 4, 5), (1, 7)]
	"""
############################################
    return sorted( tuples, key = lambda tuple: tuple[-1] )

if __name__ == '__main__':
    import doctest
    doctest.testmod(verbose=True)
