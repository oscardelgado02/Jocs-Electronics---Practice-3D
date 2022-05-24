Proyecto hecho por:

- Óscar Delgado Rueda   2299   oscar.delgado

- Víctor Alcaide Rodríguez   231602   victor.alcaide02@estudiant.upf.edu


De momento en nuestro juego tenemos implementado una función muy importante, la cual nos lee un archivo txt con la información de todas las entidades y las carga en el mapa. Diferenciamos entre tres tipos de entidades (según un parámetro que se lee en el archivo .txt): el player (el personaje que controlamos), enemy (que serán otros personajes que con una futura implementación de la IA serán los que te persigan y de los que tendrás que esconderte y escapar) y wall (que son simplemente todos los objetos y/o paredes con las cuales colisionaremos, para marcar el mapa. Las colisiones no las hemos implementado todavía).

Además tenemos una función que pone césped en el suelo, sobre el que caminará nuestro jugador.

Todavía tenemos que acabar el mapa, pero gracias a la aplicación web de Javi Agenjo es simplemente acabar de poner todas las entidades que queremos y retocar unos detalles para conseguir un mapa bonito, exportándolo como un scene el cual pasaremos a .txt y meteremos como parámetro en nuestro código.