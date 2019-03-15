# Proyecto 2 -- HPC -- Míneria de texto

### Pre-procesamiento
Antes de empezar con la implementacion del algoritmo serial se decidio hacer un proceso de limpieza a los datos, por facilidad, haciendo uso del lenguaje de programacion Python. En esta etapa de pre procesamiento se modifico lo siguiente al dataset:
* Se removieron las columnas que no se usaban, dejando unicamente(ID, Title, Content).
* Se eliminaron todas las stop words y signos de puntuacion del campo de content de todas las noticias en el dataset.
* Se escribio un nuevo archivo con la data limpia y separada por tabulador (\t), ya que debido a que los campos de *content* per se tienen comas(,) imposibilitando la correcta lectura de los datos.
* [Ver Código](serial/etl.py)

| Archivo       | Sin limpiar   | Limpio   |
| ------------- |:-------------:| --------:|
| Articles1.csv | 130           | 195      |
| Articles2.csv | 140           | 216      |
| Articles3.csv | 151           | 230      |
* En esta tabla se muestran los pesos en Megas de los archivos antes y despues de esta primera etapa.

### Algoritmo Serial
* [Video algoritmo serial](serial/etl.py)
Para la implementacion del algoritmo serial se siguio la estrategia sugerida de armar un indice invertido con todas las noticias en las que aparece cada palabra. Esta parte del procesamiento se desarrolla en batch y es donde se gasta la mayoria de recursos y tiempo de procesamiento.
