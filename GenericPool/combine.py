import os

# Nombre del archivo de salida
archivo_salida = "combinado.txt"

# Obtener la lista de archivos .c y .h en el directorio actual
archivos = [f for f in os.listdir('.') if f.endswith('.cpp') or f.endswith('.h')]
archivos.sort()  # Opcional: ordena alfabéticamente

# Abrir archivo de salida para escribir
with open(archivo_salida, 'w', encoding='utf-8') as salida:
    for nombre_archivo in archivos:
        separador = f"\n/*******************/\n// {nombre_archivo}\n"
        salida.write(separador)
        
        with open(nombre_archivo, 'r', encoding='utf-8') as entrada:
            contenido = entrada.read()
            salida.write(contenido)
            salida.write('\n')  # Asegura salto de línea al final

print(f"Archivos combinados en '{archivo_salida}'")
