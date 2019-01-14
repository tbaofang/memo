
class DetectedCluster:
    def __init__(self, pos_x, pos_y, confidence, in_free_space):
        self.pos_x = pos_x
        self.pos_y = pos_y
        self.confidence = confidence
        self.in_free_space = in_free_space
        self.in_free_space_bool = None