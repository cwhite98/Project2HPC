# Load the Pandas libraries with alias 'pd' 
import pandas as pd 
import nltk
from nltk.corpus import stopwords
from nltk.tokenize import word_tokenize
nltk.download('stopwords')
from tabulate import tabulate
import re

fields = ['id', 'title', 'content']

stop_words = ["i", "me", "my", "myself", "we", "our", "ours", "ourselves", "you", "your", "yours", "yourself", 
"yourselves", "he", "him", "his", "himself", "she", "her", "hers", "herself", "it", "its", "itself", "they", "them", 
"their", "theirs", "themselves", "what", "which", "who", "whom", "this", "that", "these", "those", "am", "is", "are", 
"was", "were", "be", "been", "being", "have", "has", "had", "having", "do", "does", "did", "doing", "a", "an", "the", 
"and", "but", "if", "or", "because", "as", "until", "while", "of", "at", "by", "for", "with", "about", "against", 
"between", "into", "through", "during", "before", "after", "above", "below", "to", "from", "up", "down", "in", "out", 
"on", "off", "over", "under", "again", "further", "then", "once", "here", "there", "when", "where", "why", "how", "all", 
"any", "both", "each", "few", "more", "most", "other", "some", "such", "no", "nor", "not", "only", "own", "same", "so", 
"than", "too", "very", "s", "t", "can", "will", "just", "don", "should", "now", ".", "?", "!", ",", ";", ":", "-", "_",
"[", "]", "{", "}", "(", ")", "...", "\'", "\"", '“', '’', '”', "$", "%", "^", "&", "*", "-", "\\", "/", "@", "!", "—"]

stop_words = set(stopwords.words('english'))

quotation = [".", "?", "!", ",", ";", ":", "-", "_", "[", "]", "{", "}", "(", ")", "...", "\'", "\"", '“', '’', '”', "$", "%", "^", "&", "*", "-", "\\", "/", "@", "!", "—"]
for i in quotation: stop_words.add(i)

# Read data from file
data_frame = pd.read_csv("/Users/ronald/Desktop/all-the-news/articles3.csv", skipinitialspace=True, usecols=fields)

#Clean the content field
def clean_data(s):
    word_tokens = word_tokenize(s)
    filtered_str = []
    for word in word_tokens:
        word = word.lower()
        if word not in stop_words:
            filtered_str.append(word)
    
    return ' '.join(filtered_str)

data_frame['content'].dropna(inplace=True)
data_frame['content'] = data_frame['content'].apply(clean_data)
data_frame.to_csv("salida2.csv", sep='\t')