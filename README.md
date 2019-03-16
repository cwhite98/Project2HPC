# PROJECT 2 – HPC

## Team Members:

* Alex Montoya Franco - amonto69@eafit.edu.co

* Ronald Cardona Martínez - rcardo11@eafit.edu.co

* Camila White Romero - cwhiter@eafit.edu.co


##  OPTION 2 – TEXT MINING

### Multi-thread Design with OpenMP
* [OpenMP](openmp.md)

### PCAM Methodology
* [PCAM](pcam.md)

### README
Compilation process, execution and analysis of results of serial version of the algorithm and parallel version

#### Steps

A preprocessing of the data is done with python, where a new file is created with the columns:

| ID            | Title         | Content  |
| ------------- |:-------------:| --------:|
|               |               |          |
|               |               |          |

In addition, stopwords and punctuation are eliminated.

The serial C ++ code was made, composed of:

* A *batch* step where the word count is made with every word in each file and save it in a map
 
* An *online* step where the user enter a word and it is searched in the map created in the *batch* step.
Results are printed.

* The parallel algorithm was made using OpenMP (OpenMP folder).

#### Configuration

##### Install python dependencies
* Python 3 must be installed.
* Must have acces to the dataset (articles#.csv)

```
$ pip3 install pandas nltk
```

#### Serial version of the algorithm

##### Pre-processing

Before beginning with the implementation of the serial algorithm it was decided to make a data cleaning process, for ease, making use of the Python programming language. In this pre-processing stage, the following is modified to the dataset:

* Columns that were not used were removed, leaving only (ID, Title, Content).
* All stopwords and punctuation marks were removed from the content field of all the news in the dataset.
* A new file was written with the data clean and separated by tabulator (\ t), since the fields of *content* per have commas (,), making it impossible to read the data correctly.

| File          | Without clean | Clean    |
| ------------- |:-------------:| --------:|
| Articles1.csv | 130           | 195      |
| Articles2.csv | 140           | 216      |
| Articles3.csv | 151           | 230      |
* This table shows the weights in Megabytes of the files before and after this first stage.

[Pre-processing](serial/etl.py)

##### Serial Algorithm
[Serial Algorithm](serial/serial.cpp)

For the implementation of the serial algorithm we followed the suggested strategy of putting together an inverted index with all the news in which each word appears. This part of the processing is developed in batch and it is where the majority of resources and processing time are spent.

### Compile commands and connect to DCA

#### Process Datasets

