#!/usr/bin/env python
import cgi
import os
import shutil

# Directorio donde se guardarán las imágenes
directorio_destino = "carpeta_destino"

# Crear una instancia de FieldStorage para procesar la solicitud POST
form = cgi.FieldStorage()

# Obtener el archivo cargado desde el formulario
archivo = form['temp.txt']

# Verificar si se ha proporcionado un archivo
if archivo:
    # Obtener el nombre del archivo
    nombre_archivo = os.path.basename(archivo.filename)

    # Crear la ruta completa de destino
    ruta_destino = os.path.join(directorio_destino, nombre_archivo)

    # Guardar el archivo en el directorio de destino
    with open(ruta_destino, 'wb') as destino:
        shutil.copyfileobj(archivo.file, destino)

    print("Content-type: text/html\n")
    print("La imagen se ha guardado con éxito en {}".format(ruta_destino))
else:
    print("Content-type: text/html\n")
    print("No se proporcionó ningún archivo.")
