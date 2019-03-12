
# Avance

Se realiza un preprocesamiento de los datos con python, en donde se crea un nuevo archivo con las columnas:

| ID            | Title         | Content  |
| ------------- |:-------------:| --------:|
|               |               |          |
|               |               |          |

Además, se eliminan las stopwords y signos de puntuación. 

Se realizó el código serial de C++, compuesto por:
* Una parte *batch* la cual consiste en un word count de todas las palabras de los archivos y las almacena en un mapa.
* Una parte *online* donde el usuario ingresa una palabra y se busca esta palabra en el mapa creado en la parte *batch* y se imprimen los resultados. 
* Se trabajó en el algoritmo paralelo usando OpenMP. (carpeta OpenMP)


Metodología PCAM

