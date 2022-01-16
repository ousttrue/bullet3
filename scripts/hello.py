from typing import Tuple
import pybullet
import logging
from enum import Enum

logger = logging.getLogger(__name__)


class Mode(Enum):
    DIRECT = pybullet.DIRECT
    GUI = pybullet.GUI
    SHARED_MEMORY = pybullet.SHARED_MEMORY
    SHARED_MEMORY_GUI = pybullet.SHARED_MEMORY_GUI
    UDP = pybullet.UDP


class Body:
    def __init__(self, id: int) -> None:
        self.id = id

    def get(self) -> Tuple[Tuple[float, float, float], Tuple[float, float, float, float]]:
        return pybullet.getBasePositionAndOrientation(self.id)  # type: ignore


class Simulation:
    def __init__(self, mode: Mode = Mode.GUI, *, key: int = 11347, udp: str = "127.0.0.1", port: int = 1234):
        self.id = pybullet.connect(mode.value, key)
        if self.id == -1:
            raise RuntimeError("connection error")
        pybullet.setGravity(0, 0, -10)

    def __del__(self):
        pybullet.disconnect()

    def load(self, urdf: str, pos=(0, 0, 0), rot=(0, 0, 0, 1)) -> Body:
        id = pybullet.loadURDF(urdf, pos, rot)
        return Body(id)  # type: ignore

    def step(self):
        pybullet.stepSimulation()


def main():
    s = Simulation(Mode.SHARED_MEMORY_GUI)

    plane = s.load("plane.urdf")

    cubeStartPos = [0, 0, 1]
    cubeStartOrientation = pybullet.getQuaternionFromEuler([0, 0, 0])
    box = s.load("r2d2.urdf", cubeStartPos, cubeStartOrientation)

    # for i in range(10):
    #     s.step()
    #     cubePos, cubeOrn = box.get()
    #     logger.debug(f'[{i}] {cubePos}: {cubeOrn}')


if __name__ == '__main__':
    logging.basicConfig(level=logging.DEBUG)
    main()
