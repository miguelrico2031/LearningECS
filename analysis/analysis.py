import os
import json

def read_jsons_from_directory(ruta_carpeta):
    datos = []

    if not os.path.isdir(ruta_carpeta):
        print(f"La ruta '{ruta_carpeta}' no es válida.")
        return datos

    for nombre_archivo in os.listdir(ruta_carpeta):
        if nombre_archivo.endswith(".json"):
            ruta_archivo = os.path.join(ruta_carpeta, nombre_archivo)
            try:
                with open(ruta_archivo, 'r', encoding='utf-8') as archivo:
                    contenido = json.load(archivo)
                    datos.append(contenido)
            except Exception as e:
                print(f"No se pudo leer '{nombre_archivo}': {e}")

    return datos

script_dir = os.path.dirname(os.path.abspath(__file__))
ecspath = os.path.join(script_dir, "analysis_ecs")
ocpath = os.path.join(script_dir, "analysis_oc")
ecsdata = read_jsons_from_directory(ecspath)
ocdata = read_jsons_from_directory(ocpath)

count = len(ecsdata)
ecs_total_avg = 0
oc_total_avg = 0
for i in range(count):
    ecs = ecsdata[i]
    oc = ocdata[i]
    ecs_total_avg += ecs["avg_fps"]
    oc_total_avg += oc["avg_fps"]

ecs_total_avg /= count
oc_total_avg /= count
ecs_improvement = (ecs_total_avg * 100 / oc_total_avg) - 100

print(f"Total average FPS in {count} simulations using an Object-Component architecture: {oc_total_avg:.2f}")
print(f"Total average FPS in {count} simulations using an ECS architecture: {ecs_total_avg:.2f}")
print(f"ECS improvement over Object-Component: {ecs_improvement:.2f}%")