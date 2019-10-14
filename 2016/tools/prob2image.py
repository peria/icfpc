#!/usr/bin/python3

# Convert problem file to its image.
#  $ prob2image.py --problem-dir ./problems --png-dir ./png
#
# Need to install:
# $ sudo apt install python3-pip
# $ pip3 install pillow

import argparse
import os

from PIL import Image, ImageDraw

IMAGE_SCALE = 512  # Pixcels for length 1
IMAGE_OFFSET = IMAGE_SCALE // 4
IMAGE_SIZE = IMAGE_SCALE + IMAGE_OFFSET * 4

def get_options():
    parser = argparse.ArgumentParser(description='Convert problem to its image.')
    parser.add_argument('--problem-dir', dest='problem_dir', help='directory for problems')
    parser.add_argument('--png-dir', dest='png_dir', help='directory for output images')
    parser.add_argument('--problem', dest='problem', help='specify a problem file')
    return parser.parse_args()


def parse_problem(f):
    def parse_fraction(s):
        s += '/1'
        ns = list(map(int, s.split('/')))
        return ns[:2]

    def parse_coordinate(s):
        x, y = map(parse_fraction, s.split(','))
        return (x, y)

    num_polygon = int(f.readline())
    polygons = []
    for _ in range(num_polygon):
        num_vertexs = int(f.readline())
        vertex = [parse_coordinate(f.readline()) for _ in range(num_vertexs)]
        polygons.append(vertex)
    skeltons = []
    num_lines = int(f.readline())
    for _ in range(num_lines):
        line = f.readline()
        skeltons.append(tuple(map(parse_coordinate, line.split())))

    # Offset for big numbers
    offset_x = min(x[0][0] // x[0][1] for x in polygons[0])
    offset_y = min(y[1][0] // y[1][1] for y in polygons[0])

    polygons = [
        [
            ((x[0] - offset_x * x[1]) / x[1],
             (y[0] - offset_y * y[1]) / y[1]) for x, y in polygon
        ] for polygon in polygons
    ]
    skeltons = [
        [
            ((x[0] - offset_x * x[1]) / x[1],
             (y[0] - offset_y * y[1]) / y[1]) for x, y in line
        ] for line in skeltons
    ]

    xmin = min(xy[0] for xy in polygons[0])
    xmax = max(xy[0] for xy in polygons[0])
    ymin = min(xy[1] for xy in polygons[0])
    ymax = max(xy[1] for xy in polygons[0])

    offset = ((xmin + xmax) / 2 - 0.5, (ymin + ymax) / 2 - 0.5);

    return {
        'polygons': polygons,
        'skeltons': skeltons,
        'offset': offset,
        'size': (xmax - xmin, ymax - ymin),
    }


def draw_problem(problem):
    # Colors
    FORE_SHEET = (0, 255, 255)
    FORE_LINE = (0, 128, 255)
    FORE_SKELTON = (0, 128, 128)

    def coord_in_image(xy, offset):
        x, y = xy
        x = IMAGE_OFFSET * 2 + (x - offset[0]) * IMAGE_SCALE
        y = IMAGE_SIZE - IMAGE_OFFSET * 2 - (y - offset[1]) * IMAGE_SCALE
        return int(x), int(y)

    image = Image.new("RGB", (IMAGE_SIZE, IMAGE_SIZE), (255, 255, 255))
    draw = ImageDraw.Draw(image)
    offset = problem['offset']
    for polygon in problem['polygons']:
        xy = [coord_in_image(c, offset) for c in polygon]
        draw.polygon(xy, fill=FORE_SHEET, outline=FORE_LINE)
    for line in problem['skeltons']:
        xy = [coord_in_image(c, offset) for c in line]
        draw.line(xy, fill=FORE_SKELTON)
    for polygon in problem['polygons']:
        xy = [coord_in_image(c, offset) for c in polygon]
        xy.append(xy[0])
        draw.line(xy, fill=FORE_LINE, width=2)

    return image


def convert_file(problem_filepath, png_filepath):
    if png_filepath and os.path.exists(png_filepath):
        return

    with open(problem_filepath) as f:
        try:
            problem = parse_problem(f)
        except Exception as e:
            print('Invalid input:\n\t{}\n{}'.format(problem_filepath, e))
            exit(0)
        image = draw_problem(problem)
        if png_filepath:
            image.save(png_filepath)
            print('Saved {}'.format(png_filepath))
        else:
            image.show()


def main():
    args = get_options()

    if args.problem:
        convert_file(args.problem, None)

    problem_dir = args.problem_dir
    png_dir = args.png_dir
    for filename in os.listdir(problem_dir):
        if filename.find('.') < 0:
            continue
        basename, ext = filename.split('.')
        if ext != 'txt':
            continue
        problem_filepath = os.path.join(problem_dir, filename)
        png_filepath = os.path.join(png_dir, '{}.png'.format(basename))
        convert_file(problem_filepath, png_filepath)

        
if __name__ == '__main__':
    main()

