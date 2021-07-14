#!/usr/bin/python3

'''
Draw picture of a problem into PNG file
'''

import json
import argparse
import PIL
from PIL import Image, ImageDraw


class Wall:
    X_OFFSET = 5
    Y_OFFSET = 5

    def __init__(self, problem, solution=None):
        hole = problem['hole']
        figure = problem['figure']
        if solution:
            figure.update(solution)

        vertices = figure['vertices']
        xs, ys = zip(*(hole + vertices))
        self.offset_x_ = Wall.X_OFFSET - min(xs)
        self.offset_y_ = Wall.Y_OFFSET - min(ys)
        width = max(xs) - min(xs) + Wall.X_OFFSET * 2
        height = max(ys) - min(ys) + Wall.Y_OFFSET * 2
        
        self.image_ = Image.new('RGB', (width, height), (128, 128, 128))
        self.draw_ = ImageDraw.Draw(self.image_)

        # Draw hole and noted points
        self.polygon(hole, outline=(0, 0, 0), fill=(255, 255, 255))
        # Draw figure
        if solution is None:
            colors = [(255, 0, 0), (255, 0, 0), (255, 0 ,0)]
        else:
            colors = [(0, 0, 255), (0, 255, 0), (255, 0, 0)]
        for i, j in figure['edges']:
            color = colors[1]
            self.line([vertices[i], vertices[j]], fill=color)

    def polygon(self, vertices, **kwargs):
        self.draw_.polygon(list(map(self.convert_, vertices)), **kwargs)

    def line(self, vertices, **kwargs):
        self.draw_.line(list(map(self.convert_, vertices)), **kwargs)

    def save(self, filepath):
        self.image_.save(filepath)

    def show(self):
        self.image_.show()

    def convert_(self, xy):
        x, y = xy
        return (x + self.offset_x_, y + self.offset_y_)


def visualize(problem, solution=None, output=None):
    wall = Wall(problem, solution)
    if output:
        wall.save(output)
    else:
        wall.show()


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('problem', type=str, help='Path to a problem json file')
    parser.add_argument('-s', '--solution', type=str, help='Path to a solution json file')
    parser.add_argument('-o', '--output', help='Path to output image file')
    args = parser.parse_args()

    with open(args.problem) as f:
        problem = json.load(f)

    solution = None
    if args.solution:
        with open(args.solution) as f:
            solution = json.load(f)

    visualize(problem, solution, args.output)


if __name__ == '__main__':
    main()
