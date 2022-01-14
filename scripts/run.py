

def main():
    import pybullet
    # pybullet.TCP、pybullet.UDP、pybullet.SHARED_MEMORY, pybullet.GUI
    # pybullet.connect(pybullet.DIRECT)
    # pybullet.connect(pybullet.GUI, options="--opengl2")
    # pybullet.connect(pybullet.SHARED_MEMORY,1234)
    # pybullet.connect(pybullet.UDP,"192.168.0.1")
    # pybullet.connect(pybullet.UDP,"localhost", 1234)
    # pybullet.connect(pybullet.TCP,"localhost", 6667)
    pybullet.connect(pybullet.GUI)
    while pybullet:
        pybullet.stepSimulation()


if __name__ == '__main__':
    main()
