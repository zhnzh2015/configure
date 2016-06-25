import os
import sys
from argparse import ArgumentParser

sys.path.append("../../..")
from jpyutils.utils import opensources
from jpyutils.runner import task_runner


def parse_args():
    arg_parser = ArgumentParser()
    arg_parser.add_argument("-b", "--build", action="store_true", help="Build depends package")
    arg_parser.add_argument("-p", "--publish", action="store_true", help="Publish library")
    arg_parser.add_argument("-m", "--make", action="store_true", help="Make source code")
    arg_parser.add_argument("-c", "--clean", action="store_true", help="Clean")
    arg_parser.add_argument("-l", "--list", action="store_true", help="List all jobs")
    return arg_parser.parse_args()


if __name__ == "__main__":
    args = parse_args()

    if args.build is True:
        opensources.OpenSources("../../third", "glog", "0.3.4").build()
        opensources.OpenSources("../../third", "gtest", "1.7.0").build()
        opensources.OpenSources("../../third", "gflags", "2.1.2").build()
        opensources.OpenSources("../../third", "protobuf", "2.6.1").build()
        opensources.OpenSources("../../third", "boost", "1.61.0").build()

    runner = task_runner.MultiTaskRunner(log = None)
    runner.add(command = "cd make && make publish", name = "publish")
    runner.add(command = "cd make && make", name = "make")
    runner.add(command = "cd make && make clean-all", name = "clean")

    tasks_list = list()
    args.publish and tasks_list.append("1")
    args.make and tasks_list.append("2")
    args.clean and tasks_list.append("3")

    runner.run(",".join(tasks_list))
