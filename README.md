Para que el control PS4 pueda conectarse correctamente, es necesario configurar la dirección MAC del ESP32.
El procedimiento es:
1.	Ejecutar el programa que muestra la MAC del ESP32 en el monitor serial.
2.	Copiar esa dirección.
3.	En cualquier aplicación o herramienta utilizada para emparejar el control, modificar la MAC del control para que coincida con la del ESP32.
4.	Presionar botón Share + PS para entrar en modo emparejamiento.
Una vez hecho esto, la conexión se realiza automáticamente en futuras ocasiones.

Para cambiar la MAC del control se usó: sixasispairtool: https://sixaxispairtool.software.informer.com/Descargar-gratis/

Para ver la MAC del esp32,usar el código verMAC.ino.
Ojo ambas MAC deben coincidir, para que se puedan emparejar.




