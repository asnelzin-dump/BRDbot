import time
import serial
import pygame
from pygame.locals import *
# import Tkinter as tk


ARDUINO_PORT = '/dev/tty.RobotServer-DevB'

# class Application(tk.Frame):
#     def __init__(self, master, **kwargs):
#         #self.robot = kwargs.pop('robot')
#
#         tk.Frame.__init__(self, master, width=300, height=300)
#         self.master = master
#         self.pack()
#
#         self.create_widgets()
#
#     def create_widgets(self):
#         front_sensor = tk.Label(self, text='000')
#         front_sensor.grid(sticky=tk.N+tk.W+tk.E)
#         # self.left = tk.Button(self.frame)
#         # self.left['text'] = 'Left'
#         # self.left['command'] = self.robot.left()


class RobotClient(object):

    def __init__(self):
        self.board = serial.Serial(ARDUINO_PORT)

    def read_sensor(self, sensor):
        self.board.write('S{}\n'.format(sensor))
        return int(self.board.readline().strip())

    def get_sensors_data(self):
        return {s:self.read_sensor(s) for s in ['F', 'L', 'R']}

    def forward(self):
        self.board.write('MF\n')

    def back(self):
        self.board.write('MB\n')

    def left(self):
        self.board.write('ML\n')

    def right(self):
        self.board.write('MR\n')

    def stop(self):
        self.board.write('0\n')


def main():
    robot = RobotClient()

    pygame.init()
    screen = pygame.display.set_mode((300, 300))
    pygame.display.set_caption("RobotClient")
    background = pygame.Surface(screen.get_size())
    background.fill((255, 255, 255))
    screen.blit(background, (0, 0))

    keep_going = True
    while keep_going:
        print robot.board.readline().strip()
        for event in pygame.event.get():
            if (event.type == KEYDOWN) and (event.key == pygame.K_UP):
                robot.forward()
            elif (event.type == KEYUP) and (event.key == pygame.K_UP):
                robot.stop()

            elif (event.type == KEYDOWN) and (event.key == pygame.K_DOWN):
                robot.back()
            elif (event.type == KEYUP) and (event.key == pygame.K_DOWN):
                robot.stop()

            elif (event.type == KEYDOWN) and (event.key == pygame.K_RIGHT):
                robot.right()
            elif (event.type == KEYUP) and (event.key == pygame.K_RIGHT):
                robot.stop()

            elif (event.type == KEYDOWN) and (event.key == pygame.K_LEFT):
                robot.left()
            elif (event.type == KEYUP) and (event.key == pygame.K_LEFT):
                robot.stop()

            elif event.type == pygame.QUIT:
                keep_going = False

    # robot.forward()
    #
    # data = robot.read_sensor('F')
    # while data > 600:
    #     data = robot.read_sensor('F')
    #     print data
    #
    # robot.stop()
    # time.sleep(0.5)
    # root = tk.Tk()
    # app = Application(master=root)
    # app.mainloop()


if __name__ == '__main__':
    main()