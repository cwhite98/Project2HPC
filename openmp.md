# OpenMP Algorithm
What the serial algorithm does is consume line by line the input file and then index every word and its frecuency in the current article.
To take advantage of multi processing on a single node we decided to parallelize the process of building up the inverted index by means of processing multiple input lines at a time. 
That is the basic idea of the parallel algorithm implemented using OpenMP.
