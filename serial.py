# Load the Pandas libraries with alias 'pd' 
import pandas as pd 
import nltk
from nltk.tokenize import word_tokenize
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
"[", "]", "{", "}", "(", ")", "..."]

# Read data from file
data_frame = pd.read_csv("input.csv", skipinitialspace=True, usecols=fields)


#Clean the content field
def clean_data(str):
    word_tokens = word_tokenize(str)
    filtered_str = []
    for word in word_tokens:
        if word not in stop_words:
            filtered_str.append(word)
    
    return filtered_str

data_frame['content'].dropna(inplace=True)
data_frame['content'].apply(clean_data) ##No funciona :(
print("FINISHED CLEAN")


def word_count(str):
    counts = dict()
    words = str.split(' ')
    for word in words: 
        if word in counts: 
            counts[word] += 1
        else:
            counts[word] = 1
    
    return counts

word_occs = []
for index, row in data_frame.iterrows():
    word_occs.append(word_count(row["content"]))

print(word_occs)
for occ in word_occs:
    print(occ, '\n')