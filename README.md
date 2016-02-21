# Rastreador de "System Calls"

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
