"""
    This script will run all benchmarks across the entire workspace.
"""

import subprocess
import os

def main():
    # Move to the root folder of the project
    os.chdir(os.environ["BUILD_WORKSPACE_DIRECTORY"])

    # Query bazel for all build targets
    p = subprocess.run(["bazel", "query", "//..."], capture_output=True)
    if p.returncode != 0:
        print("Error encountered!")
        print(p.stderr)
        exit()

    # Pick all build targets of the form //[my_package]:benchmark
    target_str = ":benchmark"
    lines = [x.decode('UTF-8') for x in p.stdout.split()]
    bench_targets = [x for x in lines if len(x) > len(target_str) and x[-len(target_str):] == target_str]

    # Run those
    for target in bench_targets:
        p = subprocess.run(["bazel", "run", target], capture_output=False)

if __name__ == "__main__":
    main()