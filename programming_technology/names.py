#!/usr/bin/python3 

import re
import sys
import os.path

def filter_name(list : []) -> ([], []):
	return [name + ' - ' + rank for (rank,name) in list]

def extr_name(filename):
	"""
	Вход: nameYYYY.html, Выход: список начинается с года, продолжается имя-ранг в алфавитном порядке.
	'2006', 'Aaliyah 91', Aaron 57', 'Abagail 895', ' и т.д.
	"""
	regex_names = re.compile('<tr\ align="right"><td>(?P<rank>\d+)<\/td><td>(?P<male>\w+)<\/td><td>(?P<female>\w+)<\/td>')
	names = re.findall(regex_names, open(filename, 'r').read())
	names = names[:10]
	male = [(rank, male) for (rank, _, male) in names]
	male = sorted(male, key=lambda x: x[1])
	female = [(rank, female) for (rank, female, _) in names]
	female = sorted(female, key=lambda x: x[1])
	return (male, female)
	
def main():
	args = sys.argv[1:]
	if not args:
		print ("use: [--file] file [file ...]")
		sys.exit(1)
	files = [file for file in args if os.path.isfile(file)]
	print("files found: ", files)
	for file in files:
		print("[" + file + "]")
		regex_year = re.compile(r'Popularity in (?P<year>\d+)');
		year =  re.search(regex_year, open(file, 'r').read())
		print(year.group('year'))
		(male, female) = extr_name(file)
		if male :   print("    male: ", filter_name(male))
		else :      print("    male: data not found")
		if female : print("    female: ", filter_name(female))
		else :      print("    female: data not found")



  
  # для каждого переданного аргументом имени файла, вывести имена  extr_name

  # напечатать ТОП-10 муж и жен имен из всех переданных файлов
 

if __name__ == '__main__':
	main()
