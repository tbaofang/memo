import rospy

from nav_msgs.msg import OccupancyGrid
from follow.msg import Leg, LegArray

import detected_cluster

class KalmanMutilTracter:
    def __init__(self):
        self.objects_tracted = []
        self.local_map = None
        self.new_local_map_received = False
        self.in_free_space_threshold = 0.06

        self.local_map_sub = rospy.Subscriber('local_map',OccupancyGrid, self.localMapCb)
        self.detected_clusters_sub = rospy.Subscriber('detected_leg_clusters', LegArray, self.detectedClustersCb);

        self.non_leg_clusters_pub = rospy.Publisher('non_leg_clusters', LegArray, queue_size = 300)

        rospy.spin()

    def localMapCb(self, map):
        self.local_map = map
        self.new_local_map_received = True

    def detectedClustersCb(self, detected_clusters_msg):
        wait_count = 0
        while self.new_local_map_received == False and wait_count < 10:
            rospy.sleep(0.1)
            wait_count += 1
        if wait_count >= 10:
            rospy.loginfo("no new local map received !!")
        else:
            self.new_local_map_received = False



        now = detected_clusters_msg.header.stamp
        detected_clusters = []
        detected_clusters_set = set()
        for leg in detected_clusters_msg.legs:
            new_detected_cluster = detected_cluster.DetectedCluster(
                leg.position.x,
                leg.position.y,
                leg.confidence,
                in_free_space=self.howMuchInFreeSpace(leg.position.x, leg.position.y)
            )
            if new_detected_cluster.in_free_space < self.in_free_space_threshold:
                new_detected_cluster.in_free_space_bool = True
            else:
                new_detected_cluster.in_free_space_bool = False

            detected_clusters.append(new_detected_cluster)
            detected_clusters_set.add(new_detected_cluster)

        to_duplicate = set()
        propogated = copy.deepcopy(self.objects_tracted)
            print new_detected_cluster.in_free_space_bool

    def howMuchInFreeSpace(self, x, y):
        if(self.local_map == None):
            return self.in_free_space_threshold * 2

        map_x = int(round((x - self.local_map.info.origin.position.x) / self.local_map.info.resolution))
        map_y = int(round((y - self.local_map.info.origin.position.y) / self.local_map.info.resolution))

        sum = 0
        kernel_size = 2
        for i in xrange(map_x - kernel_size, map_x + kernel_size):
            for j in xrange(map_y - kernel_size, map_y + kernel_size):
                if i + j * self.local_map.info.height < len(self.local_map.data):
                    sum += self.local_map.data[i + j*self.local_map.info.height]
                else:
                    return self.in_free_space_threshold*2
        percent = sum / (((2. * kernel_size + 1)**2.) * 100.)
        return percent

