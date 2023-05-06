import json
import numpy as np

def convert_spheres_to_geogebra(json_filename, output_filename):
    with open(json_filename, 'r') as f:
        spheres = json.load(f)

    with open(output_filename, 'w') as f:
        for i, sphere in enumerate(spheres):
            center = sphere['center']
            radius = sphere['radius']

            f.write(f'Center{i} = ({center["x"]}, {center["y"]}, {center["z"]})\n')
            f.write(f'Sphere{i} = Sphere[Center{i}, {radius}]\n')
            f.write(f'SetColor[Sphere{i}, {np.random.random()}, {np.random.random()}, {np.random.random()}]\n')
            f.write('\n')

if __name__ == '__main__':
    convert_spheres_to_geogebra('assets/spheres.json', 'spheres_geogebra.ggb')
