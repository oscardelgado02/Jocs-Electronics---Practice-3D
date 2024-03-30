Proyecto hecho por:

- Óscar Delgado Rueda   1oqxqzr8@duck.com

- Víctor Alcaide Rodríguez   victor.alcaide02@estudiant.upf.edu

[VIDEO OF THE GAME](youtu.be/XAfQ3jLxYSo)

De momento en nuestro juego tenemos implementado una función muy importante, la cual nos lee un archivo txt
con la información de todas las entidades y las carga en el mapa. Diferenciamos entre tres tipos de entidades
(según un parámetro que se lee en el archivo .txt): el player (el personaje que controlamos), enemy
(que serán otros personajes que con una futura implementación de la IA serán los que te persigan y de los que
tendrás que esconderte y escapar) y wall (que son simplemente todos los objetos y/o paredes con las cuales
colisionaremos, para marcar el mapa. Las colisiones no las hemos implementado todavía).

Además tenemos una función que pone césped en el suelo, sobre el que caminará nuestro jugador.
