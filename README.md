# Rastreador de "System Calls"

Este rastreador solo funciona en linux de 64 bits y no muestra los argumentos de cada system call.

Para ejecutar el programa se deben ejecutar los siguientes pasos:


1) Abrir una consola de Linux y descomprimir el archivo llamado manfred-calvo.tgz.

2) Una vez que el archivo este descomprimido, se ejecuta el comando "make" que genera el ejecutable "rastreador".

3) Para ejecutar una corrida se debe ejecutar el siguiente comando:

	rastreador [opciones rastreador] Prog [opciones de Prog]

   donde:

   [opciones rastreador] son: 

			-v Desplegará un mensaje cada vez que detecte un System Call de Prog.
			-V Idéntico a la opción -v, pero hará una pausa hasta que el usuario presione cualquier tecla para continuar la ejecución.
			-S Muestra la tabla acumulativa ordenada de mayor a menor por la cantidad de veces que se utiliza cada System Call.

   Prog: El programa al cuál se le quieren rastrear los system calls.
  	
 
   [opciones de Prog]: Cualquier combinación de opciones permitidas por Prog.

4) Una vez ejecutado el programa si se eligió la opción "-V" el programa se detendrá luego de imprimir cada System Call, para continuar se debe presionar cualquier tecla.

5) Al final de la ejecución se mostrará una tabla con un resumen de cuántas veces se ejecutó cada system call. Si se eligió la opción "-S" esta tabla de mostrará ordenada de mayor a menor.
