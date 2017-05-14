import serial


def main():
    sp = serial.Serial('/dev/tty.RobotServer-DevB')
    msg = chr(0xC0 + 1)
    sp.write(msg)


if __name__ == '__main__':
    main()