#!/usr/bin/python3


import argparse
import random
import sys

def read_words(filename):
	with open(filename) as f:
		text = f.read()
	return text.split()

def make_dict(words):
	next_words ={}
	for i in range(0, len(words) - 1):
		word_list = next_words.get(words[i],[])
		word_list.append(words[i+1])
		next_words[words[i]] = word_list
	print (next_words)
	return next_words

def pick_next_word(word, next_words):
	words = next_words.get(word)
	if words is not None:
		return random.choice(words)

def gen_text(filename, count, start=None):
	words = read_words(filename)
	next_words = make_dict(words)
	if start is None:
		word = random.choice(words)
	else:
		word = start
	for i in range(0, count):
		if word is not None:
			print (word, end=' ')
			word = pick_next_word(word, next_words)
	print()

if __name__ == '__main__':
	parser = argparse.ArgumentParser(description="generating new text based on transmitted text")
	parser.add_argument("filename", 
						help="populate dictionary from given file")
	parser.add_argument("count",
						help="generate text with given number of words",
						type=int)
	parser.add_argument("-s", "--start",
                      help="generate text starting with given word")
	args = parser.parse_args()
	gen_text(args.filename, args.count, args.start)
