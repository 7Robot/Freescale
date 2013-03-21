# -*- coding: utf-8 -*-
import pygame
from collections import deque, defaultdict
pygame.init()

class Stdout():
    def __init__(self):
        pass

    def update(self, d):
        print('-------------------')
        for name, value in d.items():
            print("%s: %s" % (name, value))


class MovingGraph():
    def __init__(self):
        self.width = 1000
        self.height = 600
        self.max_points = self.width / 2
        self.max = None
        self.min = None
        self.allow_scale_update = True
        self.colors = (pygame.Color(255, 0, 0, 0), pygame.Color(0, 0, 255, 0), pygame.Color(0, 255, 0, 0), pygame.Color(249, 132, 229, 0), pygame.Color(0, 255, 255, 0))
        self.font = pygame.font.SysFont('arial', 16, True)

        self.screen = pygame.display.set_mode((self.width, self.height))

        self.reset()

    def reset(self):
        self.points = defaultdict(deque)

    def convert_y(self, value):
        return self.height - ((value - self.min) * self.height / (self.max - self.min))

    def redraw(self):
        self.screen.fill(pygame.Color(0,0,0,0))

        # échelle
        i = self.min
        while i <= self.max:
            surface = self.font.render(str(i), True, pygame.Color(100,100,100,0))
            self.screen.blit(surface, (0, self.convert_y(i)))
            pygame.draw.line(self.screen, pygame.Color(60,60,60,0), (0, self.convert_y(i)), (self.width, self.convert_y(i)))
            i += (self.max - self.min) / 25.

        # points
        for i, (name, points) in enumerate(self.points.items()):
            color = self.colors[i]

            surface = self.font.render(name, True, color)
            self.screen.blit(surface, ((i+1)*100, 0))

            generator = [(j * self.width / self.max_points, self.convert_y(point)) for j, point in enumerate(points)]
            if len(generator) > 1:
                pygame.draw.aalines(self.screen, color, False, generator)

        pygame.display.flip()


    def add_points(self, d):
        for name, point in d.items():
            self.points[name].append(point)
            if self.allow_scale_update:
                self.max = point if self.max is None else max(self.max, point)
                self.min = point if self.min is None else min(self.min, point)
            if len(self.points[name]) > self.max_points:
                self.points[name].popleft()

    def update(self, d):
        self.add_points(d)
        self.redraw()


class Graph(MovingGraph):

    def update(self, d):
        self.reset()
        for i in d.values():
            self.add_points({'': i})
        self.max_points = len(d)
        self.redraw()
